#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 错误码定义
typedef enum {
    PLUGIN_SUCCESS = 0,
    PLUGIN_ERROR_INVALID_HANDLE = -1,
    PLUGIN_ERROR_INIT_FAILED = -2,
    PLUGIN_ERROR_INVALID_CONFIG = -3,
    PLUGIN_ERROR_MEMORY_ALLOCATION = -4
} plugin_error_t;

// 插件信息结构
typedef struct {
    const char* name;
    const char* version;
    const char* description;
    const char* author;
} plugin_info_t;

// 插件配置结构
typedef struct {
    int default_value;
    int enable_logging;
} plugin_config_t;

// 插件上下文结构
typedef struct plugin_context* plugin_context_t;

// 插件导出函数声明
#ifdef _WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

// 必需的插件导出函数
PLUGIN_EXPORT plugin_context_t plugin_create(const plugin_config_t* config);
PLUGIN_EXPORT int plugin_destroy(plugin_context_t ctx);
PLUGIN_EXPORT int plugin_get_value(plugin_context_t ctx);
PLUGIN_EXPORT const plugin_info_t* plugin_get_info(void);
PLUGIN_EXPORT int plugin_validate_config(const plugin_config_t* config);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_INTERFACE_H 