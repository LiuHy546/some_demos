#include "moduleA/moduleA2/moduleA2a/moduleA2a.h"
#include "moduleB/moduleB1/moduleB1b/moduleB1b.h"

int moduleA2b_func(int x) {
    return moduleA2a_func(x) - moduleB1b_func(x);
} 