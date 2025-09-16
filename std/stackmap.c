#include <stackmap.h>

int stack_map_init(int offset, stack_map_t* smap) {
    int size = (ALIGN(offset) / STACK_CELL_SIZE) + 1;
    smap->offset      = size;
    smap->last_offset = size;
    str_memset(smap->bitmap, 0, sizeof(smap->bitmap));
    return 1;
}

int stack_map_move(int offset, stack_map_t* smap) {
    int size = (ALIGN(offset) / STACK_CELL_SIZE) + 1;
    smap->offset      = size;
    smap->last_offset = size;
    return 1;
}

int stack_map_alloc(int n, stack_map_t* smap) {
    int size  = ALIGN(n) / STACK_CELL_SIZE;
    for (int i = 0; i < STACK_MAP_MAX; ++i) {
        int free = 1;
        for (int j = 0; j < size; ++j) {
            int idx = i + j;
            if (idx >= STACK_MAP_MAX) return -1;
            if (smap->bitmap[idx / 64] & (1ULL << (idx % 64))) {
                free = 0;
                i = idx;
                break;
            }
        }

        if (free) {
            for (int j = 0; j < size; ++j) {
                int idx = i + j;
                smap->bitmap[idx / 64] |= (1ULL << (idx % 64));
            }

            return (i + size) * STACK_CELL_SIZE;
        }
    }

    return -1;
}

int stack_map_free(int offset, int n, stack_map_t* smap) {
    int size  = ALIGN(n) / STACK_CELL_SIZE;
    int start = (ALIGN(offset) - ALIGN(n)) / STACK_CELL_SIZE;
    for (int i = 0; i < size; ++i) {
        int idx = start + i;
        if (idx >= STACK_MAP_MAX) break;
        smap->bitmap[idx / 64] &= ~(1ULL << (idx % 64));
    }

    if (start < smap->offset / STACK_CELL_SIZE) {
        smap->offset = start * STACK_CELL_SIZE;
    }

    return 1;
}

int stack_map_deinit(stack_map_t* smap) {
    for (int i = smap->last_offset; i < smap->offset; ++i) {
        smap->bitmap[i / 64] &= ~(1ULL << (i % 64));
    }

    smap->offset = smap->last_offset;
    return 1;
}
