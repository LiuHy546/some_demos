#include "math/arith.h"
#include "util/log.h"
void quick_sort(int* arr, int n) { log_util("quick_sort called"); if(n>1) add(arr[0], arr[n-1]); }
void merge_sort(int* arr, int n) { log_util("merge_sort called"); if(n>1) sub(arr[0], arr[n-1]); } 