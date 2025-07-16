#include <stdio.h>

// 定义我们要覆盖的变量
// 注意：变量名和类型必须与原系统完全一致
int system_default_value = 999;  // 你的插件值

// 构造函数，在库加载时执行
__attribute__((constructor))
void init_variable_override() {
    printf("=== 变量覆盖库已加载 ===\n");
    printf("系统默认值已被覆盖为: %d\n", system_default_value);
}

// 析构函数，在库卸载时执行
__attribute__((destructor))
void cleanup_variable_override() {
    printf("=== 变量覆盖库已卸载 ===\n");
} 