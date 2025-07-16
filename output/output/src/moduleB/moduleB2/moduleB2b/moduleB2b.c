#include "moduleB/moduleB2/moduleB2a/moduleB2a.h"
#include "moduleC/moduleC1/moduleC1b/moduleC1b.h"

int moduleB2b_func(int x) {
    return moduleB2a_func(x) + moduleC1b_func(x);
} 