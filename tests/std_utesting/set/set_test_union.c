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
    set_add(&b, (void*)3);

    set_t c;
    set_union(&c, &a, &b);

    assert(set_size(&c) == 3, "Union result isn't correct!");

    set_free(&a);
    set_free(&b);
    set_free(&c);
    return 0;
}
