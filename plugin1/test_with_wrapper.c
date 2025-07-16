#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "universal_wrapper.h"

#ifdef _WIN32
#include <windows.h>
#define LIB_HANDLE HMODULE
#define LOAD_LIB(path) LoadLibraryA(path)
#define GET_SYM(lib, name) GetProcAddress(lib, name)
#define CLOSE_LIB(lib) FreeLibrary(lib)
#else
#include <dlfcn.h>
#define LIB_HANDLE void*
#define LOAD_LIB(path) dlopen(path, RTLD_LAZY)
#define GET_SYM(lib, name) dlsym(lib, name)
#define CLOSE_LIB(lib) dlclose(lib)
#endif

int main() {
    printf("=== 测试Universal Wrapper覆盖系统默认值 ===\n\n");
    
    // 1. 首先测试原始系统库
    printf("1. 测试原始系统库:\n");
    LIB_HANDLE original_lib = LOAD_LIB("./mock_system_library.so");
    if (!original_lib) {
        printf("   错误: 无法加载原始系统库\n");
        return 1;
    }
    
    const char* (*get_value_func)();
    *(void**)(&get_value_func) = GET_SYM(original_lib, "get_system_value");
    if (!get_value_func) {
        printf("   错误: 无法找到get_system_value函数\n");
        CLOSE_LIB(original_lib);
        return 1;
    }
    
    const char* original_value = get_value_func();
    printf("   原始系统返回值: %s\n\n", original_value);
    
    CLOSE_LIB(original_lib);
    
    // 2. 创建universal wrapper来覆盖默认值
    printf("2. 创建universal wrapper:\n");
    int plugin_value = 123; // 插件要返回的 int 值
    
    universal_wrapper_config_t config;
    config.original_library_path = "./mock_system_library.so";
    config.wrapper_name = "get_system_value_wrapper"; // 包装器名称
    config.plugin_value = plugin_value;
    config.enable_logging = 1;
    config.description = "Universal wrapper for mock_system_library";

    int result = create_universal_wrapper(&config, "universal_wrapper_plugin.c");
    
    if (result != 0) {
        printf("   错误: 创建universal wrapper失败，错误码: %d\n", result);
        return 1;
    }
    
    printf("   Universal wrapper创建成功!\n\n");
    
    // 3. 测试包装后的库
    printf("3. 测试包装后的库:\n");
    LIB_HANDLE wrapped_lib = LOAD_LIB("./universal_wrapper_plugin.so");
    if (!wrapped_lib) {
        printf("   错误: 无法加载包装库\n");
        return 1;
    }
    
    int (*wrapped_get_value_func)();
    *(void**)(&wrapped_get_value_func) = GET_SYM(wrapped_lib, "get_system_value_wrapper");
    if (!wrapped_get_value_func) {
        printf("   错误: 无法找到包装函数\n");
        CLOSE_LIB(wrapped_lib);
        return 1;
    }
    
    int wrapped_value = wrapped_get_value_func();
    printf("   包装后返回值: %d\n\n", wrapped_value);
    
    if (wrapped_value == plugin_value) {
        printf("   ✓ 成功: 系统默认值已被插件值覆盖!\n");
    } else {
        printf("   ✗ 失败: 值未被正确覆盖\n");
    }
    
    CLOSE_LIB(wrapped_lib);
    
    printf("\n=== 测试完成 ===\n");
    return 0;
} 