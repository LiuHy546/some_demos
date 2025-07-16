#ifndef MAIN_H
#define MAIN_H

// 这是一个从动态库导出的函数，我们的主逻辑和插件都会用到它
int get_base_value();

// 这是我们主要的业务函数，它会打印出 special_variable 的值
void print_variable();

#endif // MAIN_H 