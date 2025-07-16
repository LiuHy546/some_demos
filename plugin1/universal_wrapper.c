#include "universal_wrapper.h"
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

// 验证库是否可以加载
int validate_library_loadable(const char* library_path) {
    if (!library_path) {
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    void* handle = dlopen(library_path, RTLD_LAZY);
    if (!handle) {
        printf("Failed to load library: %s - %s\n", library_path, dlerror());
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    dlclose(handle);
    return PLUGIN_SUCCESS;
}

// 创建通用包装器
int create_universal_wrapper(const universal_wrapper_config_t* config,
                            const char* output_plugin_path) {
    if (!config || !output_plugin_path) {
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    // 验证原始库是否可以加载
    int validation_result = validate_library_loadable(config->original_library_path);
    if (validation_result != PLUGIN_SUCCESS) {
        return validation_result;
    }
    
    // 生成插件源代码
    FILE* fp = fopen(output_plugin_path, "w");
    if (!fp) {
        printf("Failed to create plugin file: %s\n", output_plugin_path);
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    // 生成插件源代码
    fprintf(fp, "#include \"plugin_interface.h\"\n");
    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "#include <stdlib.h>\n");
    fprintf(fp, "#include <string.h>\n");
    fprintf(fp, "#include <time.h>\n");
    fprintf(fp, "#include <stdarg.h>\n");
    fprintf(fp, "\n");
    
    // 添加平台相关的头文件
    fprintf(fp, "#ifdef _WIN32\n");
    fprintf(fp, "#include <windows.h>\n");
    fprintf(fp, "#define dlopen(path, flags) LoadLibraryA(path)\n");
    fprintf(fp, "#define dlsym(handle, symbol) GetProcAddress(handle, symbol)\n");
    fprintf(fp, "#define dlclose(handle) FreeLibrary(handle)\n");
    fprintf(fp, "#define dlerror() \"Windows DLL error\"\n");
    fprintf(fp, "#else\n");
    fprintf(fp, "#include <dlfcn.h>\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\n");
    
    // 添加strdup定义
    fprintf(fp, "#ifndef _GNU_SOURCE\n");
    fprintf(fp, "char* strdup(const char* str) {\n");
    fprintf(fp, "    if (!str) return NULL;\n");
    fprintf(fp, "    size_t len = strlen(str) + 1;\n");
    fprintf(fp, "    char* dup = malloc(len);\n");
    fprintf(fp, "    if (dup) {\n");
    fprintf(fp, "        memcpy(dup, str, len);\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    return dup;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "#endif\n");
    fprintf(fp, "\n");
    
    // 包装器上下文结构
    fprintf(fp, "typedef struct {\n");
    fprintf(fp, "    void* original_library_handle;\n");
    fprintf(fp, "    int plugin_value;\n");
    fprintf(fp, "    int initialized;\n");
    fprintf(fp, "    int enable_logging;\n");
    fprintf(fp, "    char* library_path;\n");
    fprintf(fp, "} universal_wrapper_context_t;\n");
    fprintf(fp, "\n");
    
    // 日志函数
    fprintf(fp, "static void log_message(universal_wrapper_context_t* ctx, const char* format, ...) {\n");
    fprintf(fp, "    if (!ctx || !ctx->enable_logging) return;\n");
    fprintf(fp, "    time_t now = time(NULL);\n");
    fprintf(fp, "    char time_str[64];\n");
    fprintf(fp, "    strftime(time_str, sizeof(time_str), \"%%Y-%%m-%%d %%H:%%M:%%S\", localtime(&now));\n");
    fprintf(fp, "    printf(\"[%%s] [%s] \", time_str);\n", config->wrapper_name);
    fprintf(fp, "    va_list args;\n");
    fprintf(fp, "    va_start(args, format);\n");
    fprintf(fp, "    vprintf(format, args);\n");
    fprintf(fp, "    va_end(args);\n");
    fprintf(fp, "    printf(\"\\n\");\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 插件信息
    fprintf(fp, "static const plugin_info_t plugin_info = {\n");
    fprintf(fp, "    .name = \"%s\",\n", config->wrapper_name);
    fprintf(fp, "    .version = \"1.0.0\",\n");
    fprintf(fp, "    .description = \"%s\",\n", config->description ? config->description : "Universal wrapper plugin");
    fprintf(fp, "    .author = \"Universal Wrapper\"\n");
    fprintf(fp, "};\n");
    fprintf(fp, "\n");
    
    // 配置验证函数
    fprintf(fp, "static int validate_config(const plugin_config_t* config) {\n");
    fprintf(fp, "    if (!config) return PLUGIN_ERROR_INVALID_CONFIG;\n");
    fprintf(fp, "    return PLUGIN_SUCCESS;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 插件创建函数
    fprintf(fp, "PLUGIN_EXPORT plugin_context_t plugin_create(const plugin_config_t* config) {\n");
    fprintf(fp, "    printf(\"[%s] Creating universal wrapper plugin...\\n\");\n", config->wrapper_name);
    fprintf(fp, "    \n");
    fprintf(fp, "    if (validate_config(config) != PLUGIN_SUCCESS) {\n");
    fprintf(fp, "        return NULL;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    universal_wrapper_context_t* ctx = malloc(sizeof(universal_wrapper_context_t));\n");
    fprintf(fp, "    if (!ctx) return NULL;\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    memset(ctx, 0, sizeof(universal_wrapper_context_t));\n");
    fprintf(fp, "    ctx->plugin_value = %d;\n", config->plugin_value);
    fprintf(fp, "    ctx->enable_logging = %d;\n", config->enable_logging);
    fprintf(fp, "    ctx->library_path = strdup(\"%s\");\n", config->original_library_path);
    fprintf(fp, "    \n");
    fprintf(fp, "    // 加载原始库（但不关心其内部结构）\n");
    fprintf(fp, "    ctx->original_library_handle = dlopen(ctx->library_path, RTLD_LAZY);\n");
    fprintf(fp, "    if (!ctx->original_library_handle) {\n");
    fprintf(fp, "        printf(\"Warning: Failed to load original library: %%s\\n\", dlerror());\n");
    fprintf(fp, "        printf(\"Plugin will still work, but original library is not available\\n\");\n");
    fprintf(fp, "        // 不返回NULL，插件仍然可以工作\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    ctx->initialized = 1;\n");
    fprintf(fp, "    log_message(ctx, \"Universal wrapper plugin created successfully\");\n");
    fprintf(fp, "    log_message(ctx, \"Plugin value: %%d\", ctx->plugin_value);\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    return (plugin_context_t)ctx;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 插件销毁函数
    fprintf(fp, "PLUGIN_EXPORT int plugin_destroy(plugin_context_t context) {\n");
    fprintf(fp, "    if (!context) return PLUGIN_ERROR_INVALID_HANDLE;\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    universal_wrapper_context_t* ctx = (universal_wrapper_context_t*)context;\n");
    fprintf(fp, "    log_message(ctx, \"Destroying universal wrapper plugin...\");\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    if (ctx->original_library_handle) {\n");
    fprintf(fp, "        dlclose(ctx->original_library_handle);\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    if (ctx->library_path) free(ctx->library_path);\n");
    fprintf(fp, "    free(ctx);\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    return PLUGIN_SUCCESS;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 获取值函数
    fprintf(fp, "PLUGIN_EXPORT int plugin_get_value(plugin_context_t context) {\n");
    fprintf(fp, "    if (!context) return PLUGIN_ERROR_INVALID_HANDLE;\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    universal_wrapper_context_t* ctx = (universal_wrapper_context_t*)context;\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    if (!ctx->initialized) {\n");
    fprintf(fp, "        return PLUGIN_ERROR_INIT_FAILED;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    log_message(ctx, \"Returning plugin value: %%d\", ctx->plugin_value);\n");
    fprintf(fp, "    \n");
    fprintf(fp, "    return ctx->plugin_value;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 获取插件信息函数
    fprintf(fp, "PLUGIN_EXPORT const plugin_info_t* plugin_get_info(void) {\n");
    fprintf(fp, "    return &plugin_info;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    
    // 验证配置函数
    fprintf(fp, "PLUGIN_EXPORT int plugin_validate_config(const plugin_config_t* config) {\n");
    fprintf(fp, "    return validate_config(config);\n");
    fprintf(fp, "}\n");
    
    fclose(fp);
    
    printf("Universal wrapper plugin source generated: %s\n", output_plugin_path);
    return PLUGIN_SUCCESS;
} 