#include <std/queue.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    queue_t a;
    queue_init(&a);

    queue_push(&a, (void*)1);
    queue_push(&a, (void*)2);
    queue_push(&a, (void*)3);

    long val;
    assert(queue_pop(&a, (void**)&val) && val == 1, "Incorrect popped value!");
    assert(queue_pop(&a, (void**)&val) && val == 2, "Incorrect popped value!");
    assert(queue_pop(&a, (void**)&val) && val == 3, "Incorrect popped value!");

    queue_unload(&a);
    return 0;
}
