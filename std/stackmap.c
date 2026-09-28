#include <std/stackmap.h>

static inline int _bit_test(stack_map_t* smap, int idx) {
    return (smap->bitmap[idx / CELLS_PER_BLOCK] >> (idx % CELLS_PER_BLOCK)) & 1ULL;
}

static inline void _bit_set(stack_map_t* smap, int idx) {
    smap->bitmap[idx / CELLS_PER_BLOCK] |= (1ULL << (idx % CELLS_PER_BLOCK));
}

static inline void _bit_clear(stack_map_t* smap, int idx) {
    smap->bitmap[idx / CELLS_PER_BLOCK] &= ~(1ULL << (idx % CELLS_PER_BLOCK));
}

int stack_map_init(int offset, stack_map_t* smap) {
    if (!smap || offset < 0) return 0;

    smap->base_offset = offset;
    smap->offset      = offset;
    smap->last_offset = offset;
    smap->bsize       = offset + STACK_MAP_DEFAULT;
    
    smap->bitmap = (unsigned long*)mm_malloc(((smap->bsize + (CELLS_PER_BLOCK - 1)) / CELLS_PER_BLOCK) * sizeof(unsigned long));
    str_memset(smap->bitmap, 0, ((smap->bsize + (CELLS_PER_BLOCK - 1)) / CELLS_PER_BLOCK) * sizeof(unsigned long));
    return 1;
}

int stack_map_destroy(stack_map_t* smap) {
    return mm_free(smap->bitmap);
}

int stack_map_alloc(int n, stack_map_t* smap) {
    if (!smap || n <= 0) return -1;
    for (int i = smap->base_offset; i + n <= smap->bsize; ++i) {
        int ok = 1;
        for (int j = 0; j < n; ++j) {
            int idx = i + j;
            if (_bit_test(smap, idx)) {
                ok = 0;
                i = idx;
                break;
            }
        }

        if (!ok) continue;
        for (int j = 0; j < n; ++j) {
            _bit_set(smap, i + j);
        }

        int end = i + n;
        smap->last_offset = MAX(smap->last_offset, end);
        smap->offset = end;
        return end;
    }

    int start      = smap->bsize;
    int old_bsize  = smap->bsize;
    int new_bsize  = smap->bsize + MAX(n, STACK_MAP_DEFAULT);
    int old_blocks = (old_bsize + (CELLS_PER_BLOCK - 1)) / CELLS_PER_BLOCK;
    int new_blocks = (new_bsize + (CELLS_PER_BLOCK - 1)) / CELLS_PER_BLOCK;

    unsigned long* bitmap = mm_realloc(smap->bitmap, new_blocks * sizeof(unsigned long));

    if (!bitmap) return -1;
    smap->bitmap = bitmap;
    if (new_blocks > old_blocks) {
        str_memset(smap->bitmap + old_blocks, 0, (new_blocks - old_blocks) * sizeof(unsigned long));
    }

    smap->bsize = new_bsize;
    for (int i = 0; i < n; ++i) {
        _bit_set(smap, start + i);
    }

    int end = start + n;
    smap->last_offset = MAX(smap->last_offset, end);
    smap->offset = end;
    return end;
}

int stack_map_free(int offset, int n, stack_map_t* smap) {
    if (
        !smap                  ||
        n <= 0                 ||
        offset < 0             || 
        offset >= smap->bsize
    ) return 0;

    int sequence_start = offset - n;
    int is_valid_sequence = (
        sequence_start >= 0 &&
        sequence_start >= smap->base_offset &&
        sequence_start + n <= smap->bsize
    );

    int is_valid_offset = (
        offset >= 0 &&
        offset >= smap->base_offset &&
        offset + n <= smap->bsize
    );

    int start = -1;
    if (is_valid_sequence) {
        int all = 1;
        for (int i = 0; i < n; ++i) {
            if (!_bit_test(smap, sequence_start + i)) {
                all = 0;
                break;
            }
        }
        
        if (all) {
            start = sequence_start;
        }
    }

    if (start < 0 && is_valid_offset) {
        int all = 1;
        for (int i = 0; i < n; ++i) {
            if (!_bit_test(smap, offset + i)) {
                all = 0;
                break;
            }
        }

        if (all) {
            start = offset;
        }
    }

    if (start < 0) return 0;
    for (int i = 0; i < n; ++i) {
        _bit_clear(smap, start + i);
    }

    if (start < smap->offset) {
        smap->offset = start;
        if (smap->offset < smap->base_offset) {
            smap->offset = smap->base_offset;
        }
    }

    return 1;
}

int stack_map_set_base(int offset, stack_map_t* smap) {
    if (
        !smap      ||
        offset < 0 ||
        offset >= smap->bsize
    ) return 0;

    smap->base_offset = offset;
    if (smap->offset < smap->base_offset) {
        smap->offset = smap->base_offset;
    }

    if (smap->last_offset < smap->base_offset) {
        smap->last_offset = smap->base_offset;
    }

    return 1;
}