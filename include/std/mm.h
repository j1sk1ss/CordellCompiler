#ifndef MM_H_
#define MM_H_

#include <stddef.h>
#include <std/mem.h>
#include <std/math.h>
#include <std/logg.h>

#define ALIGNMENT         8
#define ALLOC_BUFFER_SIZE 16777216
#define MM_BLOCK_MAGIC    0xC07DEL

typedef struct mm_block {
    unsigned int     magic;
    size_t           size;
    unsigned char    free;
    struct mm_block* next;
} mm_block_t;

/*
Init first memory block in memory manager.

Return -1 if something goes wrong.
Return 1 if success init.
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
#define mm_malloc(size) mm_base_malloc(__FILE__, __LINE__, size);

/*
Realloc pointer to new location with new size.
Realloc took from https://github.com/j1sk1ss/CordellOS.PETPRJ/blob/Userland/src/kernel/memory/allocator.c#L138

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
Return 1 if free success.
*/
int mm_base_free(const char* f, int l, void* ptr);
#define mm_free(ptr) mm_base_free(__FILE__, __LINE__, ptr);

/*
Return current allocated size.
Return allocated size.
*/
int mm_get_allocated();

#endif