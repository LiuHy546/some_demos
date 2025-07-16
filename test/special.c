#include "main.h" // 包含主库的头文件以调用其函数

// 1. 在这里只定义变量，不进行初始化。
//    作为全局变量，它会被自动初始化为 0。
int special_variable;

// 2. 创建一个构造函数。
//    __attribute__((constructor)) 是一个编译器指令，它告诉链接器
//    在 main() 函数执行之前，需要先自动运行这个函数。
__attribute__((constructor))
void initialize_special_variable() {
    // 在这里进行运行时才能完成的初始化
    special_variable = 999 + get_base_value();
}

// 添加一个虚拟函数，确保 special.c 生成自己的 gcov 数据
void dummy_special_func() {
    // 只是一个占位实现
} 