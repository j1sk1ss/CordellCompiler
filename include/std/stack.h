#ifndef STACK_H_
#define STACK_H_

#include <std/str.h>

#define MAX_SCOPE_DEPTH 1024

typedef struct {
    short id;
    int   offset;
} scope_elem_t;

typedef struct {
    scope_elem_t data[MAX_SCOPE_DEPTH];
    int          top;
} scope_stack_t;

static inline void scope_reset(scope_stack_t* st) {
    st->top = -1;
}

static inline void scope_push(scope_stack_t* st, short id, int offset) {
    st->data[++st->top].id = id;
    st->data[st->top].offset = offset;
}

static inline void scope_pop(scope_stack_t* st) {
    if (st->top >= 0) st->top--;
}

static inline void scope_top(scope_stack_t* st, scope_elem_t* e) {
    if (st->top < 0) e->id = -1;
    else str_memcpy(e, &st->data[st->top], sizeof(scope_elem_t));
}

static inline void scope_pop_top(scope_stack_t* st, scope_elem_t* e) {
    scope_top(st, e);
    if (st->top >= 0) st->top--;
}

static inline short scope_id_top(scope_stack_t* st) {
    return (st->top >= 0) ? st->data[st->top].id : -1;
}

#endif