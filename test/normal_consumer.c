#include <stdio.h>
#include "main.h"

int main() {
    printf("正常调用.\n");
    
    // 调用动态库中的函数
    print_variable();
    
    printf("成功.\n");
    return 0;
} 