#include <stdio.h>
#include <stdlib.h>

// 模拟原系统中的全局变量
// 如果没有LD_PRELOAD，使用默认值
#ifdef _WIN32
// Windows下使用默认值
static int system_default_value = 42;
#else
// Linux下使用extern，依赖LD_PRELOAD
// 如果没有LD_PRELOAD，提供一个弱符号定义
extern int system_default_value __attribute__((weak));
// 提供默认值
int system_default_value = 42;
#endif

int main() {
    printf("=== 测试变量覆盖 ===\n\n");
    
    printf("当前系统默认值: %d\n", system_default_value);
    
    // 模拟使用这个值的操作
    int result = system_default_value * 2;
    printf("使用该值的计算结果: %d\n", result);
    
    printf("\n=== 测试完成 ===\n");
    printf("\n使用方法:\n");
    printf("LD_PRELOAD=./variable_override.so ./test_variable_override\n");
    
    return 0;
} 