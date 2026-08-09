#include <std/stackmap.h>
#include "../../misc/testing.h"

int main() {
    stack_map_t smap;
    assert(!stack_map_init(-1, &smap), "Negative offset accepted!");
    assert(!stack_map_init(STACK_MAP_MAX, &smap), "Too large offset accepted!");
    assert(!stack_map_init(0, NULL), "NULL map init accepted!");
    assert(stack_map_init(2, &smap), "Stack map init failed!");

    int first = stack_map_alloc(3, &smap);
    int second = stack_map_alloc(2, &smap);
    assert(first == 5, "First allocation offset failed!");
    assert(second == 7, "Second allocation offset failed!");
    assert(stack_map_free(first, 3, &smap), "Free by end offset failed!");
    assert(stack_map_alloc(1, &smap) == 3, "Reused freed slot failed!");
    assert(stack_map_free(5, 2, &smap), "Free by start offset failed!");
    assert(!stack_map_free(100, 2, &smap), "Free of empty range succeeded!");

    assert(stack_map_set_base(10, &smap), "Set base failed!");
    assert(smap.base_offset == 10 && smap.offset >= 10 && smap.last_offset >= 10, "Base update failed!");
    assert(!stack_map_set_base(-1, &smap), "Negative base accepted!");
    assert(stack_map_alloc(0, &smap) == -1, "Zero allocation accepted!");
    assert(stack_map_alloc(STACK_MAP_MAX + 1, &smap) == -1, "Too large allocation accepted!");
    assert(!stack_map_free(-1, 1, &smap), "Negative free accepted!");

    stack_map_t full;
    assert(stack_map_init(0, &full), "Full map init failed!");
    assert(stack_map_alloc(STACK_MAP_MAX, &full) == STACK_MAP_MAX, "Full map allocation failed!");
    assert(stack_map_alloc(1, &full) == -1, "Full map accepted extra allocation!");
    assert(stack_map_free(0, STACK_MAP_MAX, &full), "Full map free by start failed!");
    assert(stack_map_set_base(20, &full), "Full map set base failed!");
    full.offset = 1;
    full.last_offset = 2;
    assert(stack_map_set_base(10, &full), "Base clamp set failed!");
    assert(full.offset == 10 && full.last_offset == 10, "Base clamp failed!");
    return 0;
}
