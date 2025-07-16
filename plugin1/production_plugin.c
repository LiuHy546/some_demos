#include "plugin_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

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

// 插件上下文结构
typedef struct {
    int value;
    int initialized;
    int enable_logging;
    int timeout_ms;
    time_t create_time;
    char* config_file;
} production_plugin_context_t;

// 插件信息
static const plugin_info_t plugin_info = {
    .name = "ProductionValuePlugin",
    .version = "1.0.0",
    .description = "A production-ready value plugin with comprehensive error handling and logging",
    .author = "AI Assistant",
    .license = "MIT",
    .version_major = 1,
    .version_minor = 0,
    .version_patch = 0
};

// 内部日志函数
static void log_message(production_plugin_context_t* ctx, const char* format, ...) {
    if (!ctx || !ctx->enable_logging) return;
    
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] [ProductionPlugin] ", time_str);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

// 配置验证函数
static int validate_config(const plugin_config_t* config) {
    if (!config) {
        printf("[ProductionPlugin] Error: Invalid configuration (NULL)\n");
        return PLUGIN_ERROR_INVALID_CONFIG;
    }
    
    if (config->default_value < 0) {
        printf("[ProductionPlugin] Error: Default value cannot be negative\n");
        return PLUGIN_ERROR_INVALID_CONFIG;
    }
    
    if (config->timeout_ms < 0 || config->timeout_ms > 30000) {
        printf("[ProductionPlugin] Error: Timeout must be between 0 and 30000ms\n");
        return PLUGIN_ERROR_INVALID_CONFIG;
    }
    
    return PLUGIN_SUCCESS;
}

// 插件创建函数
PLUGIN_EXPORT plugin_context_t plugin_create(const plugin_config_t* config) {
    printf("[ProductionPlugin] Creating plugin instance...\n");
    
    // 验证配置
    int config_result = validate_config(config);
    if (config_result != PLUGIN_SUCCESS) {
        printf("[ProductionPlugin] Configuration validation failed\n");
        return NULL;
    }
    
    // 分配上下文内存
    production_plugin_context_t* ctx = (production_plugin_context_t*)malloc(sizeof(production_plugin_context_t));
    if (!ctx) {
        printf("[ProductionPlugin] Memory allocation failed\n");
        return NULL;
    }
    
    // 初始化上下文
    memset(ctx, 0, sizeof(production_plugin_context_t));
    ctx->value = config ? config->default_value : 123; // 默认插件值
    ctx->initialized = 0;
    ctx->enable_logging = config ? config->enable_logging : 1;
    ctx->timeout_ms = config ? config->timeout_ms : 5000;
    ctx->create_time = time(NULL);
    
    // 复制配置文件路径
    if (config && config->config_file) {
        ctx->config_file = strdup(config->config_file);
    }
    
    // 模拟初始化过程
    log_message(ctx, "Plugin context created successfully");
    log_message(ctx, "Default value: %d", ctx->value);
    log_message(ctx, "Logging enabled: %s", ctx->enable_logging ? "Yes" : "No");
    log_message(ctx, "Timeout: %dms", ctx->timeout_ms);
    
    // 模拟一些初始化工作
    if (ctx->timeout_ms > 0) {
        log_message(ctx, "Performing initialization tasks...");
        // 这里可以添加实际的初始化代码
    }
    
    ctx->initialized = 1;
    log_message(ctx, "Plugin initialization completed");
    
    return (plugin_context_t)ctx;
}

// 插件销毁函数
PLUGIN_EXPORT int plugin_destroy(plugin_context_t context) {
    if (!context) {
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    production_plugin_context_t* ctx = (production_plugin_context_t*)context;
    
    log_message(ctx, "Destroying plugin instance...");
    
    // 执行清理工作
    if (ctx->initialized) {
        log_message(ctx, "Performing cleanup tasks...");
        // 这里可以添加实际的清理代码
    }
    
    // 释放资源
    if (ctx->config_file) {
        free(ctx->config_file);
    }
    
    log_message(ctx, "Plugin instance destroyed");
    free(ctx);
    
    return PLUGIN_SUCCESS;
}

// 获取值函数
PLUGIN_EXPORT int plugin_get_value(plugin_context_t context) {
    if (!context) {
        return PLUGIN_ERROR_INVALID_HANDLE;
    }
    
    production_plugin_context_t* ctx = (production_plugin_context_t*)context;
    
    if (!ctx->initialized) {
        log_message(ctx, "Error: Plugin not initialized");
        return PLUGIN_ERROR_INIT_FAILED;
    }
    
    log_message(ctx, "Getting plugin value: %d", ctx->value);
    
    // 模拟一些处理逻辑
    time_t now = time(NULL);
    if (ctx->create_time > 0) {
        int uptime = (int)(now - ctx->create_time);
        log_message(ctx, "Plugin uptime: %d seconds", uptime);
    }
    
    return ctx->value;
}

// 获取插件信息函数
PLUGIN_EXPORT const plugin_info_t* plugin_get_info(void) {
    return &plugin_info;
}

// 验证配置函数
PLUGIN_EXPORT int plugin_validate_config(const plugin_config_t* config) {
    return validate_config(config);
} 