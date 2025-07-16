#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 模拟系统库的默认值
static const char* DEFAULT_SYSTEM_VALUE = "System Default Value";

// 模拟系统库的主要函数
const char* get_system_value() {
    printf("System library: Returning default value\n");
    return DEFAULT_SYSTEM_VALUE;
}

// 模拟系统库的另一个函数
int get_system_number() {
    printf("System library: Returning default number\n");
    return 42;
}

// 模拟系统库的配置函数
void configure_system(const char* config) {
    printf("System library: Configuring with '%s'\n", config);
}

// 模拟系统库的初始化函数
int init_system() {
    printf("System library: Initializing system\n");
    return 0;
}

// 模拟系统库的清理函数
void cleanup_system() {
    printf("System library: Cleaning up system\n");
} 