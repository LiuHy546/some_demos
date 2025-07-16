#include "main.h"
#include <stdio.h>

// 关键改动：移除 #ifdef，使用 "弱符号" 属性
// 这行代码告诉链接器：
// 1. 我在这里提供一个 special_variable 的定义，默认值是 100。
// 2. 但是这个定义是"弱"的。
// 3. 如果在最终链接可执行文件时，有另一个地方（比如 special.c）
//    提供了一个同名的"强"定义，那么就使用那个强的，把我这个弱的扔掉。
// 4. 如果没有其他人提供定义，那就用我这个默认的。
__attribute__((weak)) int special_variable = 100;

// .so 库中的一个函数实现
int get_base_value() {
    return 50;
}

// .so 库中主要的业务逻辑函数
void print_variable() {
    printf("libmain.so : %d\n", special_variable);
} 