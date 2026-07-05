#include <std/map.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    map_t m;
    void* value = NULL;

    assert(!map_init(NULL, MAP_NO_CMP), "NULL map init accepted!");
    assert(map_init(&m, MAP_CMP), "Map init failed!");
    assert(map_isempty(&m), "New map isn't empty!");
    assert(!map_get(NULL, 1, &value), "NULL map get accepted!");
    assert(!map_remove(NULL, 1), "NULL map remove accepted!");

    for (long i = 0; i < 40; i++) {
        assert(map_put(&m, i, (void*)(i + 100)), "Map put failed!");
    }
    assert(m.size == 40 && m.capacity > MAP_INITIAL_CAPACITY, "Map resize didn't happen!");
    assert(map_get(&m, 7, &value) && value == (void*)107, "Map get failed!");
    assert(map_put(&m, 7, (void*)777), "Map update failed!");
    assert(map_get(&m, 7, &value) && value == (void*)777, "Updated map value is incorrect!");
    assert(!map_get(&m, 1000, &value), "Missing key lookup succeeded!");
    assert(map_remove(&m, 7), "Map remove failed!");
    assert(!map_get(&m, 7, &value), "Removed key still exists!");
    assert(!map_remove(&m, 7), "Second remove succeeded!");

    map_iter_t it;
    long iter_count = 0;
    assert(map_iter_init(&m, &it), "Iterator init failed!");
    while (map_iter_next(&it, &value)) iter_count++;
    assert(iter_count == m.size, "Iterator count failed!");
    assert(!map_iter_next(NULL, &value), "NULL iterator next accepted!");
    assert(!map_iter_init(NULL, &it), "NULL map iterator init accepted!");

    map_t copy;
    assert(map_copy(&copy, &m), "Map copy failed!");
    assert(map_equals(&m, &copy), "Copied maps aren't equal!");
    assert(map_put(&copy, 999, (void*)999), "Copy put failed!");
    assert(!map_equals(&m, &copy), "Different maps are equal!");
    assert(map_free(&copy), "Copy free failed!");

    map_t same_hash;
    assert(map_init(&same_hash, MAP_CMP), "Same-hash map init failed!");
    assert(map_put(&same_hash, 0, (void*)1), "Same-hash first put failed!");
    assert(map_put(&same_hash, 1, (void*)2), "Same-hash second put failed!");
    assert(map_free(&m), "Map free before same-hash check failed!");
    assert(map_init(&m, MAP_CMP), "Map re-init failed!");
    assert(map_put(&m, 0, (void*)1), "Map re-put first failed!");
    assert(map_put(&m, 1, (void*)3), "Map re-put second failed!");
    m.hash = same_hash.hash;
    assert(!map_equals(&same_hash, &m), "Map equals accepted same hash with different values!");
    assert(map_free(&same_hash), "Same-hash map free failed!");

    map_t empty_src = { 0 };
    map_t empty_copy;
    assert(map_copy(&empty_copy, &empty_src), "Empty map copy failed!");
    assert(map_free(&empty_copy), "Empty copy free failed!");
    assert(!map_copy(NULL, &m), "NULL destination copy accepted!");
    assert(!map_copy(&copy, NULL), "NULL source copy accepted!");

    assert(map_free(&m), "Map free failed!");
    assert(!map_get(&m, 1, &value), "Get after free succeeded!");
    return 0;
}
