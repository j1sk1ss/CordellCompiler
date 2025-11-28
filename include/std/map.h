#ifndef MAP_H_
#define MAP_H_

#include <std/mm.h>
#include <std/str.h>

#define MAP_INITIAL_CAPACITY 16
#define MAP_LOAD_FACTOR      0.5

typedef struct {
    long   key;
    void*  value;
    int    used;
} map_entry_t;

typedef struct {
    long         capacity;
    long         index;
    map_entry_t* entries;
} map_iter_t;

typedef struct {
    char          cmp;      /* cmp ability      */
    unsigned long hash;     /* map hash for cmp */
    long          capacity;
    long          size;
    map_entry_t*  entries;
} map_t;

int map_init(map_t* m);
int map_enable_cmp(map_t* m);
int map_put(map_t* m, long k, void* v);
int map_copy(map_t* dst, map_t* src);
int map_remove(map_t* m, long k);
int map_get(map_t* m, long k, void** v);

int map_iter_init(map_t* m, map_iter_t* it);
int map_iter_next(map_iter_t* it, void** d);

int map_isempty(map_t* m);
int map_equals(map_t* f, map_t* s);

int map_free(map_t* m);
int map_free_force(map_t* m);

#endif