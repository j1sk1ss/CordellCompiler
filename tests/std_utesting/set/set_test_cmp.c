#include <std/set.h>
#include "../../misc/testing.h"

static void _fill_the_set(set_t* s) {
    long data[] = { 1, 2, 3, 432, 1253, 928838, 1, 1, 532, 54, 123, 653, 7 };
    for (int i = 0; i < (int)(sizeof(data) / sizeof(long)); i++) {
        set_add(s, (void*)data[i]);
    }
}

int main() {
    mm_init();
    set_t a, b;
    set_init(&a, SET_CMP);
    set_init(&b, SET_CMP);

    _fill_the_set(&a);
    _fill_the_set(&b);
    assert(set_equal(&a, &b), "Sets aren't equal to each other!");

    set_free(&a);
    set_free(&b);
    return 0;
}
