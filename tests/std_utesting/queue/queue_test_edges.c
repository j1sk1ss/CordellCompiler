#include <std/queue.h>
#include "../../misc/testing.h"

static int freed = 0;

static int _free_count(void* p) {
    freed++;
    return mm_free(p);
}

int main() {
    mm_init();
    queue_t q;
    long value = 0;

    assert(!queue_init(NULL), "NULL queue init accepted!");
    assert(queue_init(&q), "Queue init failed!");
    assert(queue_isempty(&q), "New queue isn't empty!");
    assert(!queue_pop(&q, (void**)&value), "Empty pop succeeded!");
    assert(!queue_pop(&q, NULL), "NULL output pop succeeded!");

    for (long i = 0; i < QUEUE_INITIAL_CAPACITY; i++) {
        assert(queue_push(&q, (void*)i), "Initial queue push failed!");
    }
    for (long i = 0; i < 5; i++) {
        assert(queue_pop(&q, (void**)&value) && value == i, "Wrapped queue pop failed!");
    }
    for (long i = 100; i < 120; i++) {
        assert(queue_push(&q, (void*)i), "Wrapped queue push failed!");
    }
    for (long i = 5; i < QUEUE_INITIAL_CAPACITY; i++) {
        assert(queue_pop(&q, (void**)&value) && value == i, "Queue order after wrap failed!");
    }
    for (long i = 100; i < 120; i++) {
        assert(queue_pop(&q, (void**)&value) && value == i, "Queue order after resize failed!");
    }
    assert(queue_isempty(&q), "Queue isn't empty after pops!");
    assert(queue_free(&q), "Queue free failed!");
    assert(queue_free(&q), "Repeated queue free failed!");

    assert(queue_init(&q), "Resize queue init failed!");
    for (long i = 0; i < QUEUE_INITIAL_CAPACITY + 1; i++) {
        assert(queue_push(&q, (void*)i), "Linear resize push failed!");
    }
    for (long i = 0; i < QUEUE_INITIAL_CAPACITY + 1; i++) {
        assert(queue_pop(&q, (void**)&value) && value == i, "Linear resize order failed!");
    }
    assert(queue_free(&q), "Linear resize queue free failed!");

    assert(queue_init(&q), "Second queue init failed!");
    for (int i = 0; i < 3; i++) {
        int* p = (int*)mm_malloc(sizeof(int));
        assert(p, "Allocation failed!");
        *p = i;
        assert(queue_push(&q, p), "Pointer queue push failed!");
    }
    assert(queue_free_force_op(&q, _free_count), "Queue force free op failed!");
    assert(freed == 3, "Queue force free op didn't visit all elements!");
    assert(!queue_free_force(&q), "Force free on empty queue succeeded!");

    assert(queue_init(&q), "Default force queue init failed!");
    int* p = (int*)mm_malloc(sizeof(int));
    assert(p, "Allocation failed!");
    assert(queue_push(&q, p), "Default force queue push failed!");
    assert(queue_free_force(&q), "Queue default force free failed!");
    assert(!queue_push(NULL, (void*)1), "NULL queue push accepted!");
    return 0;
}
