#ifndef STACKMAP_H_
#define STACKMAP_H_

#include <std/str.h>

#define STACK_CELL_SIZE 8
#define STACK_MAP_MAX   4096
#define CELLS_PER_BLOCK sizeof(unsigned long) * 8
#define ALIGN(x) ((x + 7) & ~(7))

typedef struct {
    unsigned long bitmap[(STACK_MAP_MAX + (CELLS_PER_BLOCK - 1)) / CELLS_PER_BLOCK];
    long          offset;
    long          last_offset;
    long          base_offset;
} stack_map_t;

int stack_map_init(int offset, stack_map_t* smap);
int stack_map_set_base(int offset, stack_map_t* smap);
int stack_map_alloc(int n, stack_map_t* smap);
int stack_map_free(int offset, int n, stack_map_t* smap);
int stack_map_free_range(int from, int to, stack_map_t* smap);
int stack_map_deinit(stack_map_t* smap);

#endif