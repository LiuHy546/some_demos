#include "moduleA/moduleA1/moduleA1a/moduleA1a.h"
#include "moduleA/moduleA1b/moduleA1b.h"
#include "moduleB/moduleB1/moduleB1a/moduleB1a.h"
#include "moduleC/moduleC1/moduleC1a/moduleC1a.h"
#include "math/arith.h"
#include "util/log.h"
#include <stdio.h>

int main() {
    printf("moduleA1a_func(5) = %d\n", moduleA1a_func(5));
    printf("moduleA1b_func(3) = %d\n", moduleA1b_func(3));
    printf("moduleB1a_func(2) = %d\n", moduleB1a_func(2));
    printf("moduleC1a_func(1) = %d\n", moduleC1a_func(1));
    printf("add(7, 8) = %d\n", add(7, 8));
    log_util("test log");
    return 0;
} 