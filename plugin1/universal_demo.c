#include "universal_wrapper.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Universal Plugin Wrapper Demo ===\n\n");
    
    // 步骤1: 创建通用包装器
    printf("Step 1: Creating universal wrapper...\n");
    
    universal_wrapper_config_t config = {
        .original_library_path = "./your_complex_library.so",  // 你的复杂动态库
        .wrapper_name = "MyUniversalPlugin",                   // 包装器名称
        .plugin_value = 999,                                   // 插件要返回的值
        .enable_logging = 1,                                   // 启用日志
        .description = "Universal wrapper for complex library" // 描述
    };
    
    int result = create_universal_wrapper(&config, "universal_wrapper_plugin.c");
    if (result != PLUGIN_SUCCESS) {
        printf("Failed to create universal wrapper: %d\n", result);
        return 1;
    }
    
    printf("Universal wrapper plugin source generated successfully!\n\n");
    
    // 步骤2: 编译生成的插件
    printf("Step 2: Compiling generated plugin...\n");
    printf("Please compile the generated plugin manually:\n");
    printf("gcc -fPIC -shared -o universal_wrapper_plugin.so universal_wrapper_plugin.c\n\n");
    
    // 步骤3: 说明工作原理
    printf("Step 3: How it works...\n");
    printf("1. The wrapper loads your complex library: %s\n", config.original_library_path);
    printf("2. It doesn't need to know the internal structure of your library\n");
    printf("3. When the system calls plugin_get_value(), it returns: %d\n", config.plugin_value);
    printf("4. Your original library is still loaded and available if needed\n");
    printf("5. The wrapper provides a clean plugin interface\n\n");
    
    printf("=== Demo completed ===\n");
    printf("\nTo use with your complex library:\n");
    printf("1. Replace './your_complex_library.so' with your actual library path\n");
    printf("2. Set the desired plugin_value (currently %d)\n", config.plugin_value);
    printf("3. Compile the generated plugin\n");
    printf("4. Use the generated plugin in your system\n");
    
    return 0;
} 