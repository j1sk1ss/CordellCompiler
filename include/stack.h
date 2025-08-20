#ifndef STACK_H_
#define STACK_H_

#define MAX_SCOPE_DEPTH 1024

typedef struct {
    int data[MAX_SCOPE_DEPTH];
    int top;
} scope_stack_t;

static inline void scope_push(scope_stack_t* st, int id) {
    st->data[++st->top] = id;
}

static inline void scope_pop(scope_stack_t* st) {
    if (st->top >= 0) st->top--;
}

static inline int scope_top(scope_stack_t* st) {
    return (st->top >= 0) ? st->data[st->top] : -1;
}

#endif