#ifndef MAP_H_
#define MAP_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/logg.h>

#define MAP_INITIAL_CAPACITY 16
#define MAP_LOAD_FACTOR      0.5

typedef struct {
    long  key;
    void* value;
    int   used;
} map_entry_t;

typedef struct {
    long         capacity;
    long         index;
    map_entry_t* entries;
} map_iter_t;

typedef struct {
    /* information flags */
    char          cmp;      /* cmp ability           */
    unsigned long hash;     /* map hash for cmp      */
    char          compr;    /* map compressed state  */

    /* map information   */
    long          size;     /* count of used entries */
    long          capacity; /* entier entries size   */
    map_entry_t*  entries;  /* map body              */
} map_t;

#define MAP_NO_CMP 0
#define MAP_CMP    1
/*
map_init function allocate memory for map memory and setup all necessary flags.
This map uses linearprob approuch as hashing strategy. 
Params:
    - m - Map object.
    - cmp - Support map_equals function flag.
            Note: map_equals works only with two m.cmp maps.
            Note 2: Use MAP_NO_CMP and MAP_CMP flags for this field.

Return 1 if init routine success, otherwise this function will return 0.
*/
int map_init(map_t* m, int cmp);

/*
map_put function save void* data to map with k-key.
Params:
    - m - Map object. 
    - k - Long key.
    - v - Data for store.
          Note: Data can be not only an allocated data. 
                For instance, to store integeres, just provide them with (void*).

Return 1 if put routine success, otherwise this function will return 0.
*/
int map_put(map_t* m, long k, void* v);

/*
map_get function will store pointer (or value) in v from m map.
Params:
    - m - Map object.
    - k - Object key for loading from map.
    - v - Target location for loading.
          Note: This function will return POINTER to object, not a copy.
          Note 2: To retrive a value, use (void**)&something structure.

Return 1 if get routine success, otherwise this function will return 0.
*/
int map_get(map_t* m, long k, void** v);

/*
map_copy function copy all content and information from src to dst map.
Note: ! Don't map_init(dst, ...) before map_copy, due to background allocation in map_copy !
Params:
    - dst - Destination map for copy process.
    - src - Source map, where data is taken.

Return 1 if copy routine success, otherwise this function will return 0.
*/
int map_copy(map_t* dst, map_t* src);

/*
map_remove function will remove data from map with provided key.
Note: This function will not free data, that placed by this key.
      You should do it by youreself. 
Params:
    - m - Map object.
    - k - Object key for remove.

Return 1 if remove routine success, otherwise this function will return 0.
*/
int map_remove(map_t* m, long k);

/*
map_iter_init init iterator on map's head.
Params:
    - m - Map object for iteration.
    - it - Iterator.

Return 1 if init routine success, otherwise this function will return 0.
*/
int map_iter_init(map_t* m, map_iter_t* it);

/*
map_iter_next moves iterator till next used entry in map and return it.
Params:
    - it - Iterator.
    - d - Target location for loading.
          Note: This function will return POINTER to object, not a copy.
          Note 2: To retrive a value, use (void**)&something structure.

Return 1 if this is not an end, otherwise will return 0.
*/
int map_iter_next(map_iter_t* it, void** d);

int map_isempty(map_t* m);
int map_equals(map_t* f, map_t* s);

int map_compress(map_t* m);
int map_decompress(map_t* m);

int map_free(map_t* m);
int map_free_force(map_t* m);

#endif