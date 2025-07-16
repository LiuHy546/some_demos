#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"
#include "moduleB/moduleB1/moduleB1b/moduleB1b.h"
#include "moduleC/moduleC1/moduleC1b/moduleC1b.h"

int moduleA1b_func(int x) {
    debug_util("A1b start");
    int arr[2] = {x, x-1};
    merge_sort(arr, 2);
    int y = sub(x, 1) + moduleB1b_func(x);
    y += write_file("bar.txt", "data");
    y += close_socket(x);
    y += moduleC1b_func(x);
    return y;
} 