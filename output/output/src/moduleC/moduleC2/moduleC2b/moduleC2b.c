#include "moduleA/moduleA2/moduleA2b/moduleA2b.h"
#include "moduleB/moduleB2/moduleB2b/moduleB2b.h"

int moduleC2b_func(int x) {
    return moduleA2b_func(x) - moduleB2b_func(x);
} 