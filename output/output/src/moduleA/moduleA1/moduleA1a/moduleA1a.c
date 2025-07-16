#include "util/log.h"
#include "math/arith.h"
#include "io/fileio.h"
#include "algo/sort.h"
#include "net/socket.h"
#include "moduleA/moduleA1b/moduleA1b.h"
#include "moduleB/moduleB1/moduleB1a/moduleB1a.h"
#include "moduleC/moduleC1/moduleC1a/moduleC1a.h"

int moduleA1a_func(int x) {
    log_util("A1a start");
    int arr[2] = {x, x+1};
    quick_sort(arr, 2);
    int y = add(x, 2) + moduleA1b_func(x);
    y += read_file("foo.txt");
    y += open_socket("127.0.0.1");
    y += moduleB1a_func(x) + moduleC1a_func(x);
    return y;
} 