#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"

__attribute__((destructor)) void cmdmain() {
    printf("我是命令行开始\n");
    InitReadline();
    while (1) {
        char* input = RobustReadLine("> ");
        if (!input) break;
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }
        printf("\n你输入了: %s\n\n", input);
        free(input);
    }   
    printf("\n");
}