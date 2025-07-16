#include "moduleA/moduleA1/moduleA1a/moduleA1a.h"
#include "moduleB/moduleB1/moduleB1a/moduleB1a.h"
#include "moduleC/moduleC2/moduleC2a/moduleC2a.h"

int moduleA2a_func(int x) {
    return moduleA1a_func(x) + moduleB1a_func(x) + moduleC2a_func(x);
} 