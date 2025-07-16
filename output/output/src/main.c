#include "moduleA/moduleA1/moduleA1a/moduleA1a.h"
#include "moduleA/moduleA1b/moduleA1b.h"
#include "moduleA/moduleA2/moduleA2a/moduleA2a.h"
#include "moduleA/moduleA2/moduleA2b/moduleA2b.h"
#include "moduleB/moduleB1/moduleB1a/moduleB1a.h"
#include "moduleB/moduleB1/moduleB1b/moduleB1b.h"
#include "moduleB/moduleB2/moduleB2a/moduleB2a.h"
#include "moduleB/moduleB2/moduleB2b/moduleB2b.h"
#include "moduleC/moduleC1/moduleC1a/moduleC1a.h"
#include "moduleC/moduleC1/moduleC1b/moduleC1b.h"
#include "moduleC/moduleC2/moduleC2a/moduleC2a.h"
#include "moduleC/moduleC2/moduleC2b/moduleC2b.h"

int main() {
    int x = 5;
    moduleA1a_func(x);
    moduleA1b_func(x);
    moduleA2a_func(x);
    moduleA2b_func(x);
    moduleB1a_func(x);
    moduleB1b_func(x);
    moduleB2a_func(x);
    moduleB2b_func(x);
    moduleC1a_func(x);
    moduleC1b_func(x);
    moduleC2a_func(x);
    moduleC2b_func(x);
    return 0;
} 