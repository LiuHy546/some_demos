#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"
#include "moduleB/moduleB1/moduleB1b/moduleB1b.h"
#include "moduleC/moduleC2/moduleC2a/moduleC2a.h"

int moduleB1a_func(int x) {
    log_util("B1a start");
    int arr[2] = {x, x*2};
    quick_sort(arr, 2);
    int y = mul(x, 3) + moduleB1b_func(x);
    y += read_file("baz.txt");
    y += open_socket("192.168.1.1");
    y += moduleC2a_func(x);
    return y;
} 