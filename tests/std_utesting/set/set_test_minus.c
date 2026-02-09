#include <std/set.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    set_t a, b;
    set_init(&a, SET_NO_CMP);
    set_init(&b, SET_NO_CMP);

    set_add(&a, (void*)1);
    set_add(&a, (void*)2);
    set_add(&b, (void*)2);

    set_minus_set(&a, &b);
    assert(!set_has(&a, (void*)2), "Minus didn't remove the element!");

    set_free(&a);
    set_free(&b);
    return 0;
}
