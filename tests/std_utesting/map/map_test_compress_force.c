#include <std/map.h>
#include "../../misc/testing.h"

static int freed = 0;

static int _free_count(void* p) {
    freed++;
    return mm_free(p);
}

int main() {
    mm_init();
    map_t m;
    void* value = NULL;

    assert(map_init(&m, MAP_CMP), "Map init failed!");
    assert(map_compress(&m), "Empty map compress failed!");
    assert(m.compr && m.size == 0, "Empty compressed map state failed!");
    assert(!map_put(&m, -1, (void*)1), "Negative compressed key accepted!");
    assert(!map_get(&m, -1, &value), "Negative compressed get accepted!");
    assert(!map_remove(&m, -1), "Negative compressed remove accepted!");
    assert(map_decompress(&m), "Decompress empty map failed!");
    assert(map_free(&m), "Free empty compressed map failed!");

    assert(map_init(&m, MAP_CMP), "Second map init failed!");
    int* a = (int*)mm_malloc(sizeof(int));
    int* b = (int*)mm_malloc(sizeof(int));
    assert(a && b, "Allocation failed!");
    *a = 10;
    *b = 20;
    assert(map_put(&m, 0, a), "Put key 0 failed!");
    assert(map_put(&m, 1, b), "Put key 1 failed!");
    assert(map_compress(&m), "Map compress failed!");
    assert(map_compress(&m), "Repeated map compress failed!");
    assert(map_get(&m, 0, &value) && value == a, "Compressed get key 0 failed!");
    assert(map_put(&m, 1, (void*)123), "Compressed update failed!");
    assert(map_get(&m, 1, &value) && value == (void*)123, "Compressed updated value failed!");
    assert(!map_put(&m, m.capacity, (void*)1), "Out-of-range compressed put accepted!");
    assert(!map_get(&m, m.capacity, &value), "Out-of-range compressed get accepted!");
    assert(map_remove(&m, 1), "Compressed remove failed!");
    assert(!map_remove(&m, 1), "Compressed second remove succeeded!");
    assert(map_decompress(&m), "Map decompress failed!");
    assert(map_decompress(&m), "Repeated map decompress failed!");
    assert(map_free_force_op(&m, _free_count), "Map force free op failed!");
    assert(freed == 1, "Map force free op didn't free live value!");

    assert(map_init(&m, MAP_NO_CMP), "Default force map init failed!");
    int* p = (int*)mm_malloc(sizeof(int));
    assert(p, "Allocation failed!");
    assert(map_put(&m, 1, p), "Default force map put failed!");
    assert(map_free_force(&m), "Map default force free failed!");

    assert(!map_free(NULL), "NULL map free accepted!");
    assert(!map_free_force(NULL), "NULL map force free accepted!");
    return 0;
}
