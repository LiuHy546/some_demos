#include "util/log.h"
#include <stdio.h>
int read_file(const char* path) { log_util("read_file called"); return 42; }
int write_file(const char* path, const char* msg) { log_util("write_file called"); return 0; } 