#include "util/log.h"
#include "io/fileio.h"
int open_socket(const char* addr) { log_util("open_socket called"); return read_file(addr); }
int close_socket(int fd) { log_util("close_socket called"); return write_file("sock", "close"); } 