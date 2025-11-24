#include <std/stackmap.h>

int stack_map_init(int offset, stack_map_t* smap) {
    smap->last_offset = offset;
    smap->base_offset = offset;
    str_memset(smap->bitmap, 0, sizeof(smap->bitmap));
    return 1;
}

int stack_map_alloc(int n, stack_map_t* smap) {
    int size = n;
    for (int i = smap->base_offset; i < STACK_MAP_MAX; ++i) {
        int free = 1;
        for (int j = 0; j < size; ++j) {
            int idx = i + j;
            if (idx >= STACK_MAP_MAX) return -1;
            if (smap->bitmap[idx / CELLS_PER_BLOCK] & (1ULL << (idx % CELLS_PER_BLOCK))) {
                free = 0;
                i = idx;
                break;
            }
        }

        if (free) {
            for (int j = 0; j < size; ++j) {
                int idx = i + j;
                smap->bitmap[idx / CELLS_PER_BLOCK] |= (1ULL << (idx % CELLS_PER_BLOCK));
            }

            return (i + size);
        }
    }
    
    return -1;
}

int stack_map_free(int offset, int n, stack_map_t* smap) {
    int size  = n;
    int start = offset - n;
    for (int i = 0; i < size; ++i) {
        int idx = start + i;
        if (idx >= STACK_MAP_MAX) break;
        smap->bitmap[idx / CELLS_PER_BLOCK] &= ~(1ULL << (idx % CELLS_PER_BLOCK));
    }

    if (start < smap->offset) {
        smap->offset = start;
    }

    return 1;
}

int stack_map_set_base(int offset, stack_map_t* smap) {
    int size = ALIGN(offset) / STACK_CELL_SIZE;
    if (size >= STACK_MAP_MAX) return -1;
    smap->base_offset = size;
    if (smap->offset < smap->base_offset * STACK_CELL_SIZE) {
        smap->offset = smap->base_offset * STACK_CELL_SIZE;
    }
    return 1;
}
