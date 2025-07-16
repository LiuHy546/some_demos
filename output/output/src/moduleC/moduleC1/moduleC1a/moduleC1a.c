#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"
#include "moduleC/moduleC1/moduleC1b/moduleC1b.h"

int moduleC1a_func(int x) {
    log_util("C1a start");
    int arr[2] = {x, x+3};
    quick_sort(arr, 2);
    int y = add(x, 5) + moduleC1b_func(x);
    y += read_file("c1a.txt");
    y += open_socket("10.0.0.1");
    return y;
} 