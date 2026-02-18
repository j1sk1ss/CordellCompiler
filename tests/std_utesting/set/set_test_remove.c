#include <std/set.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    set_t a;
    set_init(&a, SET_NO_CMP);

    set_add(&a, (void*)1);
    assert(set_has(&a, (void*)1), "Set doesn't have the added element!");

    set_remove(&a, (void*)1);
    assert(!set_has(&a, (void*)1), "Set has the element after remove!");

    set_free(&a);
    return 0;
}
