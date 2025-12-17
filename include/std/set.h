#ifndef SET_H_
#define SET_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/tuple.h>

typedef struct {
    map_t body;
} set_t;

typedef struct {
    map_iter_t it;
} set_iter_t;

#define SET_NO_CMP 0
#define SET_CMP    1
int set_init(set_t* s, int cmp);
int set_has_inttuple(set_t* s, int_tuple_t* t);
int set_has(set_t* s, void* data);

int set_add(set_t* s, void* data);
int set_remove(set_t* s, void* data);

int set_iter_init(set_t* s, set_iter_t* it);
int set_iter_next(set_iter_t* it, void** d);

int set_union(set_t* dst, set_t* a, set_t* b);
int set_minus_set(set_t* trg, set_t* s);
int set_intersect(set_t* dst, set_t* a, set_t* b);

int set_copy(set_t* dst, set_t* src);
int set_equal(set_t* a, set_t* b);
int set_size(set_t* s);

int set_free(set_t* s);
int set_free_force(set_t* s);

#define set_foreach(v, lst)                                                              \
    set_iter_t CONCAT(__it_, __LINE__);                                                  \
    set_iter_init(lst, &CONCAT(__it_, __LINE__));                                        \
    void* CONCAT(__val_, __LINE__);                                                      \
    while ((set_iter_next(&CONCAT(__it_, __LINE__), (void**)&CONCAT(__val_, __LINE__)))) \
        for (v = CONCAT(__val_, __LINE__); CONCAT(__val_, __LINE__); CONCAT(__val_, __LINE__) = NULL)

#endif