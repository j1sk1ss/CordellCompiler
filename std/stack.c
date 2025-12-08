#include <std/stack.h>

int stack_init(sstack_t* s) {
    if (!s) return 0;
    s->data = (sstack_scope_t*)mm_malloc(STACK_INIT_CAPACITY * sizeof(sstack_scope_t));
    if (!s->data) return 0;
    s->top = -1;
    s->capacity = STACK_INIT_CAPACITY;
    return 1;
}

int stack_push(sstack_t* s, void* data) {
    if (!s) return 0;
    if ((unsigned int)(s->top + 1) >= s->capacity) {
        s->capacity += STACK_INIT_CAPACITY;
        sstack_scope_t* ncap = (sstack_scope_t*)mm_realloc(
            s->data, 
            s->capacity * sizeof(sstack_scope_t)
        );

        if (!ncap) return 0;
        s->data = ncap;
    }

    s->data[++s->top].d = data;
    return 1;
}

int stack_pop(sstack_t* s, void** d) {
    if (!s || s->top < 0) return 0;
    if (d) *d = s->data[s->top].d;
    s->top--;
    return 1;
}

int stack_top(sstack_t* s, void** d) {
    if (!s || s->top < 0) return 0;
    if (d) *d = s->data[s->top].d;
    return 1;
}

int stack_unload(sstack_t* s) {
    if (!s) return 0;
    s->top = -1;
    s->capacity = 0;
    mm_free(s->data);
    s->data = NULL;
    return 1;
}