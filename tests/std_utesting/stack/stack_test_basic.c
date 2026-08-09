#include <std/stack.h>
#include "../../misc/testing.h"

static int freed = 0;

static int _free_count(void* p) {
    freed++;
    return mm_free(p);
}

int main() {
    mm_init();
    sstack_t s;
    long v = 0;

    assert(!stack_init(NULL), "NULL stack init accepted!");
    assert(stack_init(&s), "Stack init failed!");
    assert(!stack_pop(&s, (void**)&v), "Pop from empty stack succeeded!");
    assert(!stack_top(&s, (void**)&v), "Top from empty stack succeeded!");

    for (long i = 0; i < 40; i++) {
        assert(stack_push(&s, (void*)i), "Stack push failed!");
    }
    assert(stack_top(&s, (void**)&v) && v == 39, "Stack top failed!");
    assert(stack_pop(&s, (void**)&v) && v == 39, "Stack pop failed!");
    assert(stack_free(&s), "Stack free failed!");

    assert(stack_init(&s), "Second stack init failed!");
    for (int i = 0; i < 3; i++) {
        int* p = (int*)mm_malloc(sizeof(int));
        assert(p, "Allocation failed!");
        *p = i;
        assert(stack_push(&s, p), "Pointer push failed!");
    }
    assert(stack_free_force_op(&s, _free_count), "Forced stack free failed!");
    assert(freed == 3, "Forced stack free didn't visit all elements!");

    assert(stack_init(&s), "Default force stack init failed!");
    int* q = (int*)mm_malloc(sizeof(int));
    assert(q, "Allocation failed!");
    assert(stack_push(&s, q), "Default force stack push failed!");
    assert(stack_free_force(&s), "Stack default force free failed!");
    assert(!stack_free(NULL), "NULL stack free accepted!");
    return 0;
}
