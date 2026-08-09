#ifndef MM_H_
#define MM_H_

#include <stddef.h>
#include <std/mem.h>
#include <std/math.h>
#include <std/logg.h>

#ifdef PROD_MALLOC
    #include <stdlib.h>
    #undef MEM_OPERATION_LOGS
#endif
#define ALIGNMENT             8
#ifndef ALLOC_BUFFER_SIZE
    #define ALLOC_BUFFER_SIZE 16777216
#endif
#define MM_BLOCK_MAGIC        0xC07DEL

typedef struct mm_block {
    unsigned int     magic;
    size_t           size;
    unsigned char    free : 1;
    struct mm_block* next;
    struct mm_block* prev;
    struct mm_block* next_free;
    struct mm_block* prev_free;
} mm_block_t;

/*
Init first memory block in memory manager.

Return -1 if something goes wrong.
Returns 1 if initialization succeeds.
*/
int mm_init();

/*
Allocate memory block.

Params:
    - f - __FILE__ macro.
    - l - __LINE__ macro.
    - size - Memory block size.

Return NULL if can't allocate memory.
Return pointer to allocated memory.
*/
void* mm_base_malloc(const char* f, int l, size_t size);
#define mm_malloc(size) mm_base_malloc(__FILE__, __LINE__, size)

/*
Realloc pointer to new location with new size.

Params:
    - ptr - Pointer to old place.
    - elem - Size of new allocated area.

Return NULL if can't allocate data.
Return pointer to new allocated area.
*/
void* mm_realloc(void* ptr, size_t elem);

/*
Free allocated memory.

Params:
    - ptr - Pointer to allocated data.

Return -1 if something goes wrong.
Returns 1 if freeing succeeds.
*/
int mm_base_free(const char* f, int l, void* ptr);
#define mm_free(ptr) mm_base_free(__FILE__, __LINE__, ptr)

/*
Return current allocated size.
Return allocated size.
*/
int mm_get_allocated();

#endif