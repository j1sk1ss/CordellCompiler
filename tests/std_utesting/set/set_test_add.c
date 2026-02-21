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
    set_t a;
    set_init(&a, SET_NO_CMP);

    _fill_the_set(&a);
    assert(set_size(&a) == 11, "Set size isn't correct!");

    set_free(&a);
    return 0;
}
