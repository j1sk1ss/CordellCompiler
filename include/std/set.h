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

/*
Check if the 'b' is a subset of the 'a'.
Params:
    - `a` - The basic set.
    - `b` - The subset in the basic set.

Returns 1 if the 'b' is a subset of the 'a'.
*/
int set_subset(set_t* a, set_t* b);

int set_add(set_t* s, void* data);
int set_remove(set_t* s, void* data);

int set_iter_init(set_t* s, set_iter_t* it);
int set_iter_next(set_iter_t* it, void** d);

/*
Get a union set of the provided sets.
Params:
    - `dst` - Destination union set.
              Note: can be provided without initialization.
    - `a` - A set.
    - `b` - B set.

Returns 1 if succeeds.
*/
int set_union(set_t* dst, set_t* a, set_t* b);

/*
Removes from the `trg` set all values from the `s` set.
Params:
    - `trg` - Source set.
    - `s` - Values to delete.

Returns 1 if succeeds.
*/
int set_minus_set(set_t* trg, set_t* s);

/*
Get an intersection set of the provided sets.
Params:
    - `dst` - Destination intersection set.
              Note: must be provided with initialization.
    - `a` - A set.
    - `b` - B set.
    
Returns 1 if succeeds.
*/
int set_intersect(set_t* dst, set_t* a, set_t* b);

int set_copy(set_t* dst, set_t* src);
int set_equal(set_t* a, set_t* b);
int set_size(set_t* s);

int set_free(set_t* s);
int set_free_force(set_t* s);

#define set_foreach(v, lst)                                                              \
    set_iter_t CONCAT(__it_, __LINE__);                                                  \
    set_iter_init((lst), &CONCAT(__it_, __LINE__));                                      \
    void* CONCAT(__val_, __LINE__);                                                      \
    int CONCAT(__ok_, __LINE__);                                                         \
    while ((CONCAT(__ok_, __LINE__) = set_iter_next(&CONCAT(__it_, __LINE__),            \
                                                    (void**)&CONCAT(__val_, __LINE__)))) \
        for (v = CONCAT(__val_, __LINE__); CONCAT(__ok_, __LINE__);                      \
             CONCAT(__ok_, __LINE__) = 0)

#endif