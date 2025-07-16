#include "plugin_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#define dlopen(path, flags) LoadLibraryA(path)
#define dlsym(handle, symbol) GetProcAddress(handle, symbol)
#define dlclose(handle) FreeLibrary(handle)
#define dlerror() "Windows DLL error"
#else
#include <dlfcn.h>
#endif

// 如果strdup不存在，定义它
#ifndef _GNU_SOURCE
char* strdup(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* dup = malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}
#endif

// 插件节点结构
typedef struct plugin_node {
    char* name;
    char* path;
    void* handle;
    plugin_context_t context;
    const plugin_info_t* info;
    plugin_state_t state;
    plugin_config_t config;
    time_t load_time;
    int enabled;
    struct plugin_node* next;
} plugin_node_t;

// 插件管理器结构
struct plugin_manager {
    plugin_node_t* plugins;
    int total_plugins;
    int loaded_plugins;
    int active_plugins;
    int failed_plugins;
    int default_value;
};

// 内部函数声明
static plugin_node_t* find_plugin(plugin_manager_t manager, const char* name);
static void update_stats(plugin_manager_t manager);
static int validate_plugin_version(const plugin_info_t* info);
static void log_message(const char* format, ...);

// 创建插件管理器
plugin_manager_t plugin_manager_create(void) {
    plugin_manager_t manager = (plugin_manager_t)malloc(sizeof(struct plugin_manager));
    if (manager) {
        manager->plugins = NULL;
        manager->total_plugins = 0;
        manager->loaded_plugins = 0;
        manager->active_plugins = 0;
        manager->failed_plugins = 0;
        manager->default_value = 42; // 默认值
        log_message("Plugin manager created successfully");
    }
    return manager;
}

// 销毁插件管理器
void plugin_manager_destroy(plugin_manager_t manager) {
    if (!manager) return;
    
    log_message("Destroying plugin manager...");
    
    // 卸载所有插件
    plugin_node_t* current = manager->plugins;
    while (current) {
        plugin_node_t* next = current->next;
        
        if (current->context && current->state == PLUGIN_STATE_INITIALIZED) {
            plugin_destroy_func destroy_func = (plugin_destroy_func)dlsym(current->handle, "plugin_destroy");
            if (destroy_func) {
                destroy_func(current->context);
            }
        }
        
        if (current->handle) {
            dlclose(current->handle);
        }
        
        free(current->name);
        free(current->path);
        free(current);
        current = next;
    }
    
    free(manager);
    log_message("Plugin manager destroyed");
}

// 加载插件（带选项）
int plugin_manager_load_plugin_ex(plugin_manager_t manager, const plugin_load_options_t* options) {
    if (!manager || !options || !options->plugin_path) {
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    // 检查插件是否已存在
    if (find_plugin(manager, options->plugin_name)) {
        log_message("Plugin %s already loaded", options->plugin_name);
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    // 打开动态库
    void* handle = dlopen(options->plugin_path, RTLD_LAZY);
    if (!handle) {
        log_message("Failed to load plugin: %s - %s", options->plugin_path, dlerror());
        manager->failed_plugins++;
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    // 获取插件信息
    plugin_get_info_func get_info_func = (plugin_get_info_func)dlsym(handle, "plugin_get_info");
    if (!get_info_func) {
        log_message("Invalid plugin: missing plugin_get_info function");
        dlclose(handle);
        manager->failed_plugins++;
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    const plugin_info_t* info = get_info_func();
    if (!info) {
        log_message("Failed to get plugin info");
        dlclose(handle);
        manager->failed_plugins++;
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    // 版本验证
    if (options->validate_version && !validate_plugin_version(info)) {
        log_message("Plugin version mismatch: %s", info->version);
        dlclose(handle);
        manager->failed_plugins++;
        return PLUGIN_ERROR_VERSION_MISMATCH;
    }
    
    // 配置验证
    plugin_validate_config_func validate_config_func = (plugin_validate_config_func)dlsym(handle, "plugin_validate_config");
    if (validate_config_func && validate_config_func(&options->config) != PLUGIN_SUCCESS) {
        log_message("Invalid plugin configuration");
        dlclose(handle);
        manager->failed_plugins++;
        return PLUGIN_ERROR_INVALID_CONFIG;
    }
    
    // 创建插件节点
    plugin_node_t* node = (plugin_node_t*)malloc(sizeof(plugin_node_t));
    if (!node) {
        log_message("Memory allocation failed");
        dlclose(handle);
        manager->failed_plugins++;
        return PLUGIN_ERROR_MEMORY_ALLOCATION;
    }
    
    node->name = strdup(options->plugin_name ? options->plugin_name : info->name);
    node->path = strdup(options->plugin_path);
    node->handle = handle;
    node->info = info;
    node->config = options->config;
    node->state = PLUGIN_STATE_LOADED;
    node->load_time = time(NULL);
    node->enabled = 1;
    node->next = manager->plugins;
    manager->plugins = node;
    manager->total_plugins++;
    
    // 初始化插件
    plugin_create_func create_func = (plugin_create_func)dlsym(handle, "plugin_create");
    if (create_func) {
        node->context = create_func(&options->config);
        if (node->context) {
            node->state = PLUGIN_STATE_INITIALIZED;
            manager->active_plugins++;
            log_message("Plugin %s loaded and initialized successfully", node->name);
        } else {
            node->state = PLUGIN_STATE_ERROR;
            manager->failed_plugins++;
            log_message("Failed to initialize plugin %s", node->name);
        }
    } else {
        node->state = PLUGIN_STATE_ERROR;
        manager->failed_plugins++;
        log_message("Plugin %s missing plugin_create function", node->name);
    }
    
    update_stats(manager);
    return PLUGIN_SUCCESS;
}

// 加载插件（简化版本）
int plugin_manager_load_plugin(plugin_manager_t manager, const char* plugin_path) {
    plugin_load_options_t options = {0};
    options.plugin_path = plugin_path;
    options.plugin_name = NULL;
    options.config.default_value = 42;
    options.config.enable_logging = 1;
    options.config.timeout_ms = 5000;
    options.auto_reload = 0;
    options.validate_version = 1;
    
    return plugin_manager_load_plugin_ex(manager, &options);
}

// 卸载插件
int plugin_manager_unload_plugin(plugin_manager_t manager, const char* plugin_name) {
    if (!manager || !plugin_name) return PLUGIN_ERROR_INVALID_HANDLE;
    
    plugin_node_t* current = manager->plugins;
    plugin_node_t* prev = NULL;
    
    while (current) {
        if (strcmp(current->name, plugin_name) == 0) {
            log_message("Unloading plugin %s", plugin_name);
            
            // 销毁插件上下文
            if (current->context && current->state == PLUGIN_STATE_INITIALIZED) {
                plugin_destroy_func destroy_func = (plugin_destroy_func)dlsym(current->handle, "plugin_destroy");
                if (destroy_func) {
                    destroy_func(current->context);
                }
            }
            
            // 关闭动态库
            if (current->handle) {
                dlclose(current->handle);
            }
            
            // 从链表中移除
            if (prev) {
                prev->next = current->next;
            } else {
                manager->plugins = current->next;
            }
            
            // 更新统计信息
            if (current->state == PLUGIN_STATE_INITIALIZED) {
                manager->active_plugins--;
            } else if (current->state == PLUGIN_STATE_ERROR) {
                manager->failed_plugins--;
            }
            manager->total_plugins--;
            
            free(current->name);
            free(current->path);
            free(current);
            
            update_stats(manager);
            log_message("Plugin %s unloaded successfully", plugin_name);
            return PLUGIN_SUCCESS;
        }
        prev = current;
        current = current->next;
    }
    
    log_message("Plugin %s not found", plugin_name);
    return PLUGIN_ERROR_INVALID_HANDLE;
}

// 获取插件值
int plugin_manager_get_value(plugin_manager_t manager, const char* plugin_name, int* value) {
    if (!manager || !plugin_name || !value) return PLUGIN_ERROR_INVALID_HANDLE;
    
    plugin_node_t* node = find_plugin(manager, plugin_name);
    if (!node || node->state != PLUGIN_STATE_INITIALIZED || !node->enabled) {
        *value = manager->default_value;
        return PLUGIN_SUCCESS;
    }
    
    plugin_get_value_func get_value_func = (plugin_get_value_func)dlsym(node->handle, "plugin_get_value");
    if (!get_value_func) {
        *value = manager->default_value;
        return PLUGIN_ERROR_NOT_IMPLEMENTED;
    }
    
    int result = get_value_func(node->context);
    if (result < 0) {
        *value = manager->default_value;
        return result;
    }
    
    *value = result;
    return PLUGIN_SUCCESS;
}

// 获取插件信息
const plugin_info_t* plugin_manager_get_plugin_info(plugin_manager_t manager, const char* plugin_name) {
    if (!manager || !plugin_name) return NULL;
    
    plugin_node_t* node = find_plugin(manager, plugin_name);
    return node ? node->info : NULL;
}

// 获取插件状态
plugin_state_t plugin_manager_get_plugin_state(plugin_manager_t manager, const char* plugin_name) {
    if (!manager || !plugin_name) return PLUGIN_STATE_UNLOADED;
    
    plugin_node_t* node = find_plugin(manager, plugin_name);
    return node ? node->state : PLUGIN_STATE_UNLOADED;
}

// 列出所有插件
int plugin_manager_list_plugins(plugin_manager_t manager) {
    if (!manager) return PLUGIN_ERROR_INVALID_HANDLE;
    
    printf("\n=== Plugin Manager Status ===\n");
    printf("Total plugins: %d\n", manager->total_plugins);
    printf("Active plugins: %d\n", manager->active_plugins);
    printf("Failed plugins: %d\n", manager->failed_plugins);
    printf("Default value: %d\n", manager->default_value);
    printf("\n=== Loaded Plugins ===\n");
    
    plugin_node_t* current = manager->plugins;
    while (current) {
        printf("Name: %s\n", current->name);
        printf("Path: %s\n", current->path);
        printf("Version: %s\n", current->info->version);
        printf("State: %d\n", current->state);
        printf("Enabled: %s\n", current->enabled ? "Yes" : "No");
        printf("Load time: %s", ctime(&current->load_time));
        printf("---\n");
        current = current->next;
    }
    
    return PLUGIN_SUCCESS;
}

// 获取插件管理器统计信息
int plugin_manager_get_stats(plugin_manager_t manager, plugin_stats_t* stats) {
    if (!manager || !stats) return PLUGIN_ERROR_INVALID_HANDLE;
    
    stats->total_plugins = manager->total_plugins;
    stats->loaded_plugins = manager->loaded_plugins;
    stats->active_plugins = manager->active_plugins;
    stats->failed_plugins = manager->failed_plugins;
    
    return PLUGIN_SUCCESS;
}

// 重新加载插件
int plugin_manager_reload_plugin(plugin_manager_t manager, const char* plugin_name) {
    if (!manager || !plugin_name) return PLUGIN_ERROR_INVALID_HANDLE;
    
    plugin_node_t* node = find_plugin(manager, plugin_name);
    if (!node) return PLUGIN_ERROR_INVALID_HANDLE;
    
    // 保存配置
    plugin_config_t config = node->config;
    char* path = strdup(node->path);
    
    // 卸载插件
    int result = plugin_manager_unload_plugin(manager, plugin_name);
    if (result != PLUGIN_SUCCESS) {
        free(path);
        return result;
    }
    
    // 重新加载插件
    plugin_load_options_t options = {0};
    options.plugin_path = path;
    options.plugin_name = plugin_name;
    options.config = config;
    options.auto_reload = 0;
    options.validate_version = 1;
    
    result = plugin_manager_load_plugin_ex(manager, &options);
    free(path);
    return result;
}

// 启用/禁用插件
int plugin_manager_enable_plugin(plugin_manager_t manager, const char* plugin_name, int enable) {
    if (!manager || !plugin_name) return PLUGIN_ERROR_INVALID_HANDLE;
    
    plugin_node_t* node = find_plugin(manager, plugin_name);
    if (!node) return PLUGIN_ERROR_INVALID_HANDLE;
    
    node->enabled = enable ? 1 : 0;
    log_message("Plugin %s %s", plugin_name, enable ? "enabled" : "disabled");
    
    return PLUGIN_SUCCESS;
}

// 验证插件兼容性
int plugin_manager_validate_plugin(plugin_manager_t manager, const char* plugin_path) {
    if (!manager || !plugin_path) return PLUGIN_ERROR_INVALID_HANDLE;
    
    void* handle = dlopen(plugin_path, RTLD_LAZY);
    if (!handle) {
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    plugin_get_info_func get_info_func = (plugin_get_info_func)dlsym(handle, "plugin_get_info");
    if (!get_info_func) {
        dlclose(handle);
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    const plugin_info_t* info = get_info_func();
    if (!info) {
        dlclose(handle);
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    int result = validate_plugin_version(info) ? PLUGIN_SUCCESS : PLUGIN_ERROR_VERSION_MISMATCH;
    dlclose(handle);
    
    return result;
}

// 内部函数实现
static plugin_node_t* find_plugin(plugin_manager_t manager, const char* name) {
    if (!manager || !name) return NULL;
    
    plugin_node_t* current = manager->plugins;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static void update_stats(plugin_manager_t manager) {
    if (!manager) return;
    
    manager->loaded_plugins = 0;
    manager->active_plugins = 0;
    manager->failed_plugins = 0;
    
    plugin_node_t* current = manager->plugins;
    while (current) {
        manager->loaded_plugins++;
        if (current->state == PLUGIN_STATE_INITIALIZED) {
            manager->active_plugins++;
        } else if (current->state == PLUGIN_STATE_ERROR) {
            manager->failed_plugins++;
        }
        current = current->next;
    }
}

static int validate_plugin_version(const plugin_info_t* info) {
    if (!info) return 0;
    
    // 检查主版本号兼容性
    if (info->version_major != PLUGIN_SYSTEM_VERSION_MAJOR) {
        return 0;
    }
    
    // 次版本号应该小于等于系统版本
    if (info->version_minor > PLUGIN_SYSTEM_VERSION_MINOR) {
        return 0;
    }
    
    return 1;
}

static void log_message(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("[PLUGIN_MANAGER] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
} 