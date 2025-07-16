#include "io/fileio.h"
#include <stdio.h>
void log_util(const char* msg) { printf("LOG: %s\n", msg); write_file("log.txt", msg); }
void debug_util(const char* msg) { printf("DEBUG: %s\n", msg); write_file("debug.txt", msg); } 