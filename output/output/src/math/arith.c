#include "util/log.h"
int add(int a, int b) { log_util("add called"); return a + b; }
int sub(int a, int b) { log_util("sub called"); return a - b; }
int mul(int a, int b) { log_util("mul called"); return a * b; }
int div_int(int a, int b) { log_util("div called"); return b ? a / b : 0; } 