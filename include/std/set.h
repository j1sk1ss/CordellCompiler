#ifndef SET_H_
#define SET_H_

#include <std/mm.h>
#include <std/tuple.h>

typedef struct set_node {
    union {
        void*        addrdata;
        long         intdata;
    } stg;
    struct set_node* next;
} set_node_t;

typedef struct {
    set_node_t* head;
} set_t;

typedef struct {
    set_node_t* current;
} set_iter_t;

int set_init(set_t* s);
int set_iter_init(set_t* s, set_iter_t* it);
int set_size(set_t* s);
int set_free(set_t* s);
int set_free_force(set_t* s);

int set_add_int(set_t* s, long data);
int set_remove_int(set_t* s, long data);
int set_has_int(set_t* s, long data);
long set_iter_next_int(set_iter_t* it);
int set_union_int(set_t* dst, set_t* a, set_t* b);
int set_copy_int(set_t* dst, set_t* src);
int set_minus_int_set(set_t* trg, set_t* s);
int set_equal_int(set_t* a, set_t* b);

int set_add_addr(set_t* s, void* data);
int set_remove_addr(set_t* s, void* data);
int set_has_inttuple(set_t* s, int_tuple_t* t);
int set_has_addr(set_t* s, void* data);
void* set_iter_next_addr(set_iter_t* it);
int set_intersect_addr(set_t* dst, set_t* a, set_t* b);
int set_copy_addr(set_t* dst, set_t* src);
int set_union_addr(set_t* dst, set_t* a, set_t* b);
int set_equal_addr(set_t* a, set_t* b);

#endif