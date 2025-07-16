#include <stdio.h>
#include <stdlib.h>

// 析构函数 - 在库卸载时自动执行
__attribute__((destructor))
void library_destructor(void) {
    printf("666\n");
    fflush(stdout);
}
