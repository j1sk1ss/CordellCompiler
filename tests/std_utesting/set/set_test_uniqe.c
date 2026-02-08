#include <std/set.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    set_t a;
    set_init(&a, SET_NO_CMP);

    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)1);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    set_add(&a, (void*)2);
    
    assert(set_size(&a) == 2, "Set size isn't correct!");

    set_free(&a);
    return 0;
}
