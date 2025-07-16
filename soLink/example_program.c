#include <stdio.h>
#include <unistd.h>

int main() {
    printf("\n--- 程序正常运行 ---\n");
    for (int i = 1; i <= 3; i++) {
        printf("执行第 %d 次操作...\n", i);
        sleep(1);
    }
    printf("\n=== 示例程序结束 ===\n");
    return 0;
} 