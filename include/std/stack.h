#ifndef STACK_H_
#define STACK_H_

#include <std/str.h>

#define MAX_SCOPE_DEPTH 1024

typedef struct {
    long id;
    long offset;
} scope_elem_t;

typedef struct {
    scope_elem_t data[MAX_SCOPE_DEPTH];
    int          top;
} scope_stack_t;

static inline void scope_reset(scope_stack_t* st) {
    st->top = -1;
}

static inline void scope_push(scope_stack_t* st, long id, long offset) {
    st->data[++st->top].id   = id;
    st->data[st->top].offset = offset;
}

static inline void scope_push_id(scope_stack_t* st, long id) {
    st->data[++st->top].id   = id;
    st->data[st->top].offset = 0;
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

static inline long scope_offset_top(scope_stack_t* st) {
    return (st->top >= 0) ? st->data[st->top].offset : -1;
}

typedef struct {
    union {
        void* addrdata;
        long intdata;
    } data;
} stack_elem_t;

typedef struct {
    stack_elem_t data[MAX_SCOPE_DEPTH];
    int          top;
} sstack_t;

static inline void stack_push_addr(sstack_t* st, void* data) {
    st->data[++st->top].data.addrdata = data;
}

static inline void stack_push_int(sstack_t* st, long data) {
    st->data[++st->top].data.intdata = data;
}

static inline void stack_pop(sstack_t* st) {
    if (st->top >= 0) st->top--;
}

static inline void stack_top_addr(sstack_t* st, stack_elem_t* e) {
    if (st->top < 0) e->data.addrdata = NULL;
    else str_memcpy(e, &st->data[st->top], sizeof(scope_elem_t));
}

static inline int stack_top_int(sstack_t* st, stack_elem_t* e) {
    if (st->top < 0) {
        e->data.intdata = -1;
        return 0;
    }
    
    str_memcpy(e, &st->data[st->top], sizeof(stack_elem_t));
    return 1;
}

#endif