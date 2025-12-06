#ifndef STACK_H_
#define STACK_H_

#include <std/mm.h>
#include <std/mem.h>

#define STACK_INIT_CAPACITY 16

typedef struct {
    void* d;
} sstack_scope_t;

typedef struct {
    sstack_scope_t* data;
    int             top;
    unsigned int    capacity;
} sstack_t;

int stack_init(sstack_t* s);
int stack_push(sstack_t* s, void* data);
int stack_pop(sstack_t* s, void** d);
int stack_top(sstack_t* s, void** d);
int stack_unload(sstack_t* s);

#endif