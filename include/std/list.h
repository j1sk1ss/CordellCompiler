#ifndef LIST_H_
#define LIST_H_

#include <std/mm.h>

typedef struct list_node {
    void*             data;
    struct list_node* n;
    struct list_node* p;
} list_node_t;

typedef struct {
    int          s;
    list_node_t* h;
    list_node_t* t;
} list_t;

typedef struct {
    list_node_t* curr;
} list_iter_t;

int list_init(list_t* l);
int list_iter_hinit(list_t* l, list_iter_t* it);
int list_iter_tinit(list_t* l, list_iter_t* it);
int list_size(list_t* l);
int list_isempty(list_t* l);
int list_add(list_t* l, void* data);
void* list_iter_next(list_iter_t* it);
void* list_iter_prev(list_iter_t* it);
void* list_get_head(list_t* l);
void* list_get_tail(list_t* l);
int list_free(list_t* l);
int list_free_force(list_t* l);

#endif