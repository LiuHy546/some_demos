#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"
#include "moduleC/moduleC1/moduleC1a/moduleC1a.h"

int moduleB1b_func(int x) {
    debug_util("B1b start");
    int arr[2] = {x, x-2};
    merge_sort(arr, 2);
    int y = div_int(x, 2) + moduleC1a_func(x);
    y += write_file("qux.txt", "data");
    y += close_socket(x);
    return y;
} 