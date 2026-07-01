#include <std/mm.h>

#ifndef PROD_MALLOC
#define FREE_LIST_COUNT 32
#define MIN_SPLIT_SIZE  ALIGNMENT

typedef union {
    unsigned long long __align;
    unsigned char      buffer[ALLOC_BUFFER_SIZE];
} mm_heap_storage_t;

static mm_heap_storage_t _mm_storage;
static mm_block_t* _mm_head = (mm_block_t*)_mm_storage.buffer;
static mm_block_t* _free_lists[FREE_LIST_COUNT];
static int _allocated = 0;

static size_t _align_up(size_t size) {
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

static int _free_list_index(size_t size) {
    int idx = 0;
    size_t limit = ALIGNMENT;

    while (idx < FREE_LIST_COUNT - 1 && size > limit) {
        limit <<= 1;
        idx++;
    }

    return idx;
}

static void _remove_free_block(mm_block_t* block) {
    int idx = _free_list_index(block->size);

    if (block->prev_free) {
        block->prev_free->next_free = block->next_free;
    } 
    else {
        _free_lists[idx] = block->next_free;
    }

    if (block->next_free) {
        block->next_free->prev_free = block->prev_free;
    }

    block->next_free = NULL;
    block->prev_free = NULL;
}

static void _insert_free_block(mm_block_t* block) {
    int idx = _free_list_index(block->size);

    block->free = 1;
    block->next_free = _free_lists[idx];
    block->prev_free = NULL;

    if (_free_lists[idx]) {
        _free_lists[idx]->prev_free = block;
    }

    _free_lists[idx] = block;
}

static void _merge_with_next(mm_block_t* block) {
    mm_block_t* next = block->next;

    block->size += sizeof(mm_block_t) + next->size;
    block->next = next->next;

    if (block->next) {
        block->next->prev = block;
    }
}

static void _split_block(mm_block_t* block, size_t size) {
    if (block->size < size + sizeof(mm_block_t) + MIN_SPLIT_SIZE) {
        return;
    }

    mm_block_t* new_block = (mm_block_t*)((unsigned char*)block + sizeof(mm_block_t) + size);

    new_block->magic = MM_BLOCK_MAGIC;
    new_block->size = block->size - size - sizeof(mm_block_t);
    new_block->free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    new_block->next_free = NULL;
    new_block->prev_free = NULL;

    if (new_block->next) {
        new_block->next->prev = new_block;
    }

    block->next = new_block;
    block->size = size;

    _insert_free_block(new_block);
}

static mm_block_t* _find_free_block(size_t size) {
    int idx = _free_list_index(size);
    for (int i = idx; i < FREE_LIST_COUNT; i++) {
        mm_block_t* current = _free_lists[i];
        while (current) {
            if (current->size >= size) return current;
            current = current->next_free;
        }
    }

    return NULL;
}

static int _ptr_in_heap(void* ptr) {
    return ptr &&
           ptr >= (void*)(_mm_storage.buffer + sizeof(mm_block_t)) &&
           ptr <  (void*)(_mm_storage.buffer + ALLOC_BUFFER_SIZE);
}
#endif

int mm_init() {
#ifndef PROD_MALLOC
    for (int i = 0; i < FREE_LIST_COUNT; i++) {
        _free_lists[i] = NULL;
    }

    _allocated = 0;

    _mm_head            = (mm_block_t*)_mm_storage.buffer;
    _mm_head->magic     = MM_BLOCK_MAGIC;
    _mm_head->size      = ALLOC_BUFFER_SIZE - sizeof(mm_block_t);
    _mm_head->free      = 1;
    _mm_head->next      = NULL;
    _mm_head->prev      = NULL;
    _mm_head->next_free = NULL;
    _mm_head->prev_free = NULL;

    _insert_free_block(_mm_head);
#endif
    return 1;
}

void* mm_base_malloc(__attribute__ ((unused)) const char* f, __attribute__ ((unused)) int l, size_t size) {
#ifndef PROD_MALLOC
    if (!size) {
        return NULL;
    }

    size = _align_up(size);

    mm_block_t* block = _find_free_block(size);
    if (!block) {
        print_mm("Allocation error! I can't allocate [%i]!", (int)size);
        return NULL;
    }

    _remove_free_block(block);
    block->free = 0;
    _split_block(block, size);

    _allocated += block->size + sizeof(mm_block_t);

    print_mm("Allocation in %s on line=%i, size=%i, ptr=%p", f, l, (int)size, (unsigned char*)block + sizeof(mm_block_t));
    return (unsigned char*)block + sizeof(mm_block_t);
#else
    return malloc(size);
#endif
}

void* mm_realloc(void* ptr, size_t elem) {
#ifndef PROD_MALLOC
    if (!ptr) {
        return elem ? mm_malloc(elem) : NULL;
    }

    if (!elem) {
        mm_free(ptr);
        return NULL;
    }

    if (!_ptr_in_heap(ptr)) {
        return NULL;
    }

    mm_block_t* block = (mm_block_t*)((unsigned char*)ptr - sizeof(mm_block_t));
    if (block->magic != MM_BLOCK_MAGIC || block->free) {
        return NULL;
    }

    size_t old_size = block->size;
    size_t new_size = _align_up(elem);

    if (new_size <= old_size) {
        _split_block(block, new_size);
        _allocated -= old_size - block->size;
        return ptr;
    }

    if (block->next && block->next->free &&
        old_size + sizeof(mm_block_t) + block->next->size >= new_size) {
        int old_allocated = (int)(block->size + sizeof(mm_block_t));

        _remove_free_block(block->next);
        _merge_with_next(block);
        _split_block(block, new_size);

        _allocated += (int)(block->size + sizeof(mm_block_t)) - old_allocated;
        return ptr;
    }

    void* new_data = mm_malloc(elem);
    if (!new_data) {
        return NULL;
    }

    str_memcpy(new_data, ptr, old_size < elem ? old_size : elem);
    mm_free(ptr);
    return new_data;
#else
    return realloc(ptr, elem);
#endif
}

int mm_base_free(__attribute__ ((unused)) const char* f, __attribute__ ((unused)) int l, void* ptr) {
#ifndef PROD_MALLOC
    print_mm("Trying to free ptr=%p from file=%s, line=%i", ptr, f, l);

    if (!_ptr_in_heap(ptr)) {
        return 0;
    }

    mm_block_t* block = (mm_block_t*)((unsigned char*)ptr - sizeof(mm_block_t));
    if (block->magic != MM_BLOCK_MAGIC || block->free) {
        return 0;
    }

    str_memset(ptr, 0xDE, block->size);

    _allocated -= block->size + sizeof(mm_block_t);
    block->free = 1;

    if (block->prev && block->prev->free) {
        block = block->prev;
        _remove_free_block(block);
        _merge_with_next(block);
    }

    if (block->next && block->next->free) {
        _remove_free_block(block->next);
        _merge_with_next(block);
    }

    _insert_free_block(block);

    print_mm("Free [%p] / allocated [%i]", ptr, _allocated);
    return 1;
#else
    free(ptr);
    return 1;
#endif
}

int mm_get_allocated() {
#ifndef PROD_MALLOC
    return _allocated;
#else
    return -1;
#endif
}