#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "plugin_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

// 插件管理器句柄
typedef struct plugin_manager* plugin_manager_t;

// 插件加载选项
typedef struct {
    const char* plugin_path;
    const char* plugin_name;
    plugin_config_t config;
    int auto_reload;
    int validate_version;
} plugin_load_options_t;

// 插件状态
typedef enum {
    PLUGIN_STATE_UNLOADED = 0,
    PLUGIN_STATE_LOADED,
    PLUGIN_STATE_INITIALIZED,
    PLUGIN_STATE_ERROR,
    PLUGIN_STATE_DISABLED
} plugin_state_t;

// 插件管理器统计信息
typedef struct {
    int total_plugins;
    int loaded_plugins;
    int active_plugins;
    int failed_plugins;
} plugin_stats_t;

// 创建插件管理器
plugin_manager_t plugin_manager_create(void);

// 销毁插件管理器
void plugin_manager_destroy(plugin_manager_t manager);

// 加载插件（带选项）
int plugin_manager_load_plugin_ex(plugin_manager_t manager, const plugin_load_options_t* options);

// 加载插件（简化版本）
int plugin_manager_load_plugin(plugin_manager_t manager, const char* plugin_path);

// 卸载插件
int plugin_manager_unload_plugin(plugin_manager_t manager, const char* plugin_name);

// 获取插件值
int plugin_manager_get_value(plugin_manager_t manager, const char* plugin_name, int* value);

// 获取插件信息
const plugin_info_t* plugin_manager_get_plugin_info(plugin_manager_t manager, const char* plugin_name);

// 获取插件状态
plugin_state_t plugin_manager_get_plugin_state(plugin_manager_t manager, const char* plugin_name);

// 列出所有插件
int plugin_manager_list_plugins(plugin_manager_t manager);

// 获取插件管理器统计信息
int plugin_manager_get_stats(plugin_manager_t manager, plugin_stats_t* stats);

// 重新加载插件
int plugin_manager_reload_plugin(plugin_manager_t manager, const char* plugin_name);

// 启用/禁用插件
int plugin_manager_enable_plugin(plugin_manager_t manager, const char* plugin_name, int enable);

// 验证插件兼容性
int plugin_manager_validate_plugin(plugin_manager_t manager, const char* plugin_path);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_MANAGER_H 