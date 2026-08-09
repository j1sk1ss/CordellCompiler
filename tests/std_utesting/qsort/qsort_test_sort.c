#include <std/qsort.h>
#include "../../misc/testing.h"

static int _cmp_int(const void* a, const void* b) {
    int av = *(const int*)a;
    int bv = *(const int*)b;
    return av - bv;
}

int main() {
    mm_init();
    int data[] = { 5, -1, 7, 7, 0, 3 };
    int expected[] = { -1, 0, 3, 5, 7, 7 };

    assert(!sort_qsort(NULL, 1, sizeof(int), _cmp_int), "NULL array accepted!");
    assert(!sort_qsort(data, 0, sizeof(int), _cmp_int), "Zero size accepted!");
    assert(!sort_qsort(data, 1, 0, _cmp_int), "Zero element size accepted!");
    assert(!sort_qsort(data, 1, sizeof(int), NULL), "NULL comparator accepted!");
    assert(sort_qsort(data, 6, sizeof(int), _cmp_int), "Sort failed!");
    for (int i = 0; i < 6; i++) {
        assert(data[i] == expected[i], "Sort order is incorrect!");
    }
    return 0;
}
