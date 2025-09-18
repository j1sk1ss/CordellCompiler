#ifndef STACKMAP_H_
#define STACKMAP_H_

#include <std/str.h>
#include <std/regs.h>

#define STACK_CELL_SIZE 8
#define STACK_MAP_MAX   1024

typedef struct {
    unsigned long bitmap[(STACK_MAP_MAX + 63) / 64];
    int           offset;
    int           last_offset;
} stack_map_t;

int stack_map_init(int offset, stack_map_t* smap);
int stack_map_move(int offset, stack_map_t* smap);
int stack_map_alloc(int n, stack_map_t* smap);
int stack_map_free(int offset, int n, stack_map_t* smap);
int stack_map_deinit(stack_map_t* smap);

#endif