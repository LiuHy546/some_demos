#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// 插件系统版本
#define PLUGIN_SYSTEM_VERSION_MAJOR 1
#define PLUGIN_SYSTEM_VERSION_MINOR 0
#define PLUGIN_SYSTEM_VERSION_PATCH 0

// 错误码定义
typedef enum {
    PLUGIN_SUCCESS = 0,
    PLUGIN_ERROR_INVALID_HANDLE = -1,
    PLUGIN_ERROR_VERSION_MISMATCH = -2,
    PLUGIN_ERROR_INIT_FAILED = -3,
    PLUGIN_ERROR_INVALID_CONFIG = -4,
    PLUGIN_ERROR_MEMORY_ALLOCATION = -5,
    PLUGIN_ERROR_NOT_IMPLEMENTED = -6
} plugin_error_t;

// 插件信息结构
typedef struct {
    const char* name;
    const char* version;
    const char* description;
    const char* author;
    const char* license;
    int version_major;
    int version_minor;
    int version_patch;
} plugin_info_t;

// 插件配置结构
typedef struct {
    int default_value;
    const char* config_file;
    int enable_logging;
    int timeout_ms;
} plugin_config_t;

// 插件上下文结构
typedef struct plugin_context* plugin_context_t;

// 插件导出函数类型定义
typedef plugin_context_t (*plugin_create_func)(const plugin_config_t* config);
typedef int (*plugin_destroy_func)(plugin_context_t ctx);
typedef int (*plugin_get_value_func)(plugin_context_t ctx);
typedef const plugin_info_t* (*plugin_get_info_func)(void);
typedef int (*plugin_validate_config_func)(const plugin_config_t* config);

// 插件导出函数声明
#ifdef _WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT __attribute__((visibility("default")))
#endif

// 必需的插件导出函数
PLUGIN_EXPORT plugin_context_t plugin_create(const plugin_config_t* config);
PLUGIN_EXPORT int plugin_destroy(plugin_context_t ctx);
PLUGIN_EXPORT int plugin_get_value(plugin_context_t ctx);.
PLUGIN_EXPORT const plugin_info_t* plugin_get_info(void);
PLUGIN_EXPORT int plugin_validate_config(const plugin_config_t* config);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_INTERFACE_H 