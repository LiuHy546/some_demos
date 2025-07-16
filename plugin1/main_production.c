#include "plugin_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("=== Production Plugin System Demo ===\n\n");
    
    // 创建插件管理器
    plugin_manager_t manager = plugin_manager_create();
    if (!manager) {
        printf("Failed to create plugin manager!\n");
        return 1;
    }
    
    // 演示1: 没有插件时使用默认值
    printf("Demo 1: No plugin loaded\n");
    int value = 0;
    if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
        printf("Value: %d (using default)\n", value);
    }
    printf("\n");
    
    // 演示2: 加载插件
    printf("Demo 2: Loading production plugin\n");
    plugin_load_options_t options = {0};
    options.plugin_path = "./production_plugin.so";
    options.plugin_name = "ProductionValuePlugin";
    options.config.default_value = 123;
    options.config.enable_logging = 1;
    options.config.timeout_ms = 5000;
    options.auto_reload = 0;
    options.validate_version = 1;
    
    int result = plugin_manager_load_plugin_ex(manager, &options);
    if (result == PLUGIN_SUCCESS) {
        printf("Plugin loaded successfully!\n");
        
        // 获取插件信息
        const plugin_info_t* info = plugin_manager_get_plugin_info(manager, "ProductionValuePlugin");
        if (info) {
            printf("Plugin Info:\n");
            printf("  Name: %s\n", info->name);
            printf("  Version: %s\n", info->version);
            printf("  Description: %s\n", info->description);
            printf("  Author: %s\n", info->author);
            printf("  License: %s\n", info->license);
        }
        
        // 获取插件值
        if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
            printf("Plugin value: %d\n", value);
        }
        
        // 获取插件状态
        plugin_state_t state = plugin_manager_get_plugin_state(manager, "ProductionValuePlugin");
        printf("Plugin state: %d\n", state);
        
    } else {
        printf("Failed to load plugin: %d\n", result);
    }
    printf("\n");
    
    // 演示3: 列出所有插件
    printf("Demo 3: List all plugins\n");
    plugin_manager_list_plugins(manager);
    printf("\n");
    
    // 演示4: 获取统计信息
    printf("Demo 4: Plugin manager statistics\n");
    plugin_stats_t stats;
    if (plugin_manager_get_stats(manager, &stats) == PLUGIN_SUCCESS) {
        printf("Total plugins: %d\n", stats.total_plugins);
        printf("Loaded plugins: %d\n", stats.loaded_plugins);
        printf("Active plugins: %d\n", stats.active_plugins);
        printf("Failed plugins: %d\n", stats.failed_plugins);
    }
    printf("\n");
    
    // 演示5: 禁用插件
    printf("Demo 5: Disable plugin\n");
    plugin_manager_enable_plugin(manager, "ProductionValuePlugin", 0);
    if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
        printf("Value after disable: %d (using default)\n", value);
    }
    printf("\n");
    
    // 演示6: 重新启用插件
    printf("Demo 6: Re-enable plugin\n");
    plugin_manager_enable_plugin(manager, "ProductionValuePlugin", 1);
    if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
        printf("Value after re-enable: %d\n", value);
    }
    printf("\n");
    
    // 演示7: 重新加载插件
    printf("Demo 7: Reload plugin\n");
    if (plugin_manager_reload_plugin(manager, "ProductionValuePlugin") == PLUGIN_SUCCESS) {
        printf("Plugin reloaded successfully!\n");
        if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
            printf("Value after reload: %d\n", value);
        }
    }
    printf("\n");
    
    // 演示8: 卸载插件
    printf("Demo 8: Unload plugin\n");
    if (plugin_manager_unload_plugin(manager, "ProductionValuePlugin") == PLUGIN_SUCCESS) {
        printf("Plugin unloaded successfully!\n");
        if (plugin_manager_get_value(manager, "ProductionValuePlugin", &value) == PLUGIN_SUCCESS) {
            printf("Value after unload: %d (using default)\n", value);
        }
    }
    printf("\n");
    
    // 演示9: 验证插件兼容性
    printf("Demo 9: Validate plugin compatibility\n");
    result = plugin_manager_validate_plugin(manager, "./production_plugin.so");
    if (result == PLUGIN_SUCCESS) {
        printf("Plugin is compatible with current system\n");
    } else {
        printf("Plugin compatibility check failed: %d\n", result);
    }
    printf("\n");
    
    // 最终统计
    printf("Final statistics:\n");
    plugin_manager_list_plugins(manager);
    
    // 销毁插件管理器
    plugin_manager_destroy(manager);
    
    printf("\n=== Demo completed ===\n");
    return 0;
} 