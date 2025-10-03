#ifndef MAP_H_
#define MAP_H_

#include <std/mm.h>
#include <std/str.h>

#define MAP_INITIAL_CAPACITY 16
#define MAP_LOAD_FACTOR      0.75

typedef struct {
    long   key;
    void*  value;
    int    used;
} map_entry_t;

typedef struct {
    long         capacity;
    long         size;
    map_entry_t* entries;
} map_t;

int map_init(map_t* m);
int map_put(map_t* m, long k, void* v);
int map_remove(map_t* m, long k);
int map_get(map_t* m, long k, void** v);
int map_free(map_t* m);
int map_free_force(map_t* m);

#endif