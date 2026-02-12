#include <std/queue.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    queue_t a;
    queue_init(&a);

    for (int i = 0; i < 250; i++) {
        queue_push(&a, (void*)1);
        queue_push(&a, (void*)2);
        queue_push(&a, (void*)3);
    }

    for (int i = 0; i < 250; i++) {
        long val = 0;
        assert(queue_pop(&a, (void**)&val) && val == 1, "Incorrect first popped value!");
        assert(queue_pop(&a, (void**)&val) && val == 2, "Incorrect second popped value!");
        assert(queue_pop(&a, (void**)&val) && val == 3, "Incorrect third popped value!");
    }

    queue_unload(&a);
    return 0;
}
