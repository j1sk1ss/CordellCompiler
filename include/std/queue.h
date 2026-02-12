#ifndef QUEUE_H_
#define QUEUE_H_

#include <std/mm.h>

#define QUEUE_INITIAL_CAPACITY 16

typedef struct {
    void* data;
} queue_entry_t;

typedef struct {
    int            size;
    struct {
        int        head;
        int        tail;
        int        count;
    } meta;
    queue_entry_t* body;
} queue_t;

int queue_init(queue_t* q);
int queue_push(queue_t* q, void* d);
int queue_pop(queue_t* q, void** d);
int queue_unload(queue_t* q);

#endif
