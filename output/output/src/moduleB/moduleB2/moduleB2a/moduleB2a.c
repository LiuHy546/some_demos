#include "moduleA/moduleA2/moduleA2a/moduleA2a.h"
#include "moduleC/moduleC2/moduleC2a/moduleC2a.h"

int moduleB2a_func(int x) {
    return moduleA2a_func(x) * moduleC2a_func(x);
} 