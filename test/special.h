#ifndef SPECIAL_H
#define SPECIAL_H

// 'extern' 关键字告诉编译器：
// "这个变量是真实存在的，但是在别的.c文件中定义的，
// 链接器（linker）在链接阶段会找到它"。
extern int special_variable;

#endif // SPECIAL_H 