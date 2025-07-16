#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"

int moduleC1b_func(int x) {
    debug_util("C1b start");
    int arr[2] = {x, x-3};
    merge_sort(arr, 2);
    int y = sub(x, 3);
    y += write_file("c1b.txt", "data");
    y += close_socket(x);
    return y;
} 