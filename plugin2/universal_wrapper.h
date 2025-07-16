#ifndef UNIVERSAL_WRAPPER_H
#define UNIVERSAL_WRAPPER_H

#include "plugin_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

// 通用包装器配置
typedef struct {
    const char* original_library_path;     // 原始动态库路径
    const char* wrapper_name;              // 包装器名称
    int plugin_value;                      // 插件要返回的值
    int enable_logging;                    // 是否启用日志
    const char* description;               // 插件描述
} universal_wrapper_config_t;

// 通用包装器函数
int create_universal_wrapper(const universal_wrapper_config_t* config,
                            const char* output_plugin_path);

// 验证库是否可以加载
int validate_library_loadable(const char* library_path);

#ifdef __cplusplus
}
#endif

#endif // UNIVERSAL_WRAPPER_H 