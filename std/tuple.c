#include <std/tuple.h>

int_tuple_t* inttuple_create(long x, long y) {
    int_tuple_t* t = (int_tuple_t*)mm_malloc(sizeof(int_tuple_t));
    if (!t) return 0;
    t->x = x;
    t->y = y;
    return t;
}

int inttuple_free(int_tuple_t* t) {
    return mm_free(t);
}
