#include <std/mm.h>
#include "../../misc/testing.h"

int main() {
    assert(mm_init(), "Memory manager init failed!");
    assert(mm_get_allocated() == 0, "Initial allocation counter isn't zero!");
    assert(mm_malloc(0) == NULL, "Zero allocation succeeded!");

    char* a = (char*)mm_malloc(5);
    char* b = (char*)mm_malloc(32);
    assert(a && b, "Allocation failed!");
    assert(mm_get_allocated() > 0, "Allocation counter didn't grow!");
    for (int i = 0; i < 5; i++) a[i] = (char)('a' + i);

    char* same = (char*)mm_realloc(a, 3);
    assert(same == a, "Shrink realloc moved pointer!");
    assert(same[0] == 'a' && same[2] == 'c', "Shrink realloc lost data!");

    assert(mm_free(b), "Free failed!");
    char* grown = (char*)mm_realloc(same, 40);
    assert(grown, "Grow realloc failed!");
    assert(grown[0] == 'a' && grown[2] == 'c', "Grow realloc lost data!");
    assert(mm_realloc(NULL, 8) != NULL, "NULL realloc allocation failed!");
    assert(mm_realloc((void*)0x1234, 8) == NULL, "Foreign pointer realloc succeeded!");
    assert(mm_realloc(grown + 1, 8) == NULL, "Interior pointer realloc succeeded!");
    assert(mm_malloc(ALLOC_BUFFER_SIZE) == NULL, "Oversized allocation succeeded!");
    assert(mm_realloc(grown, ALLOC_BUFFER_SIZE) == NULL, "Oversized realloc succeeded!");
    assert(mm_realloc(grown, 0) == NULL, "Zero realloc didn't free!");
    assert(!mm_free(grown), "Double free succeeded!");
    assert(!mm_free((void*)0x1234), "Foreign pointer free succeeded!");
    assert(mm_get_allocated() >= 0, "Allocation counter is invalid!");
    return 0;
}
