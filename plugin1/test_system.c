#include <stdio.h>
#include <stdlib.h>
#include "mock_system_library.h"

int main() {
    printf("=== 测试模拟系统库 ===\n\n");
    
    // 直接使用系统库
    printf("1. 直接调用系统库函数:\n");
    const char* value = get_system_value();
    printf("   返回值: %s\n\n", value);
    
    int number = get_system_number();
    printf("   返回数字: %d\n\n", number);
    
    // 测试配置和初始化
    printf("2. 测试系统库的其他功能:\n");
    configure_system("test_config");
    init_system();
    cleanup_system();
    
    printf("\n=== 测试完成 ===\n");
    return 0;
} 