#include <std/map.h>

static long _hash(long key, long capacity) {
    return ((unsigned long)key * 11400714819323198485llu) % capacity;
}

int map_init(map_t* m) {
    if (!m) return 0;
    m->capacity = MAP_INITIAL_CAPACITY;
    m->size     = 0;
    m->entries  = (map_entry_t*)mm_malloc(m->capacity * sizeof(map_entry_t));
    str_memset(m->entries, 0, m->capacity * sizeof(map_entry_t));
    return m->entries ? 1 : 0;
}

static int _map_resize(map_t* m, long newcap) {
    map_entry_t* old_entries = m->entries;
    long oldcap = m->capacity;

    m->entries = (map_entry_t*)mm_malloc(newcap * sizeof(map_entry_t));
    str_memset(m->entries, 0, newcap * sizeof(map_entry_t));
    if (!m->entries) {
        m->entries = old_entries;
        return 0;
    }

    m->capacity = newcap;
    m->size = 0;

    for (long i = 0; i < oldcap; i++) {
        if (old_entries[i].used == 1) {
            map_put(m, old_entries[i].key, old_entries[i].value);
        }
    }

    mm_free(old_entries);
    return 1;
}

int map_put(map_t* m, long k, void* v) {
    if (!m) return -1;
    if ((double)m->size / m->capacity > MAP_LOAD_FACTOR) {
        if (!_map_resize(m, m->capacity * 2)) return 0;
    }

    long idx = _hash(k, m->capacity);
    for (;;) {
        if (!m->entries[idx].used) {
            m->entries[idx].key   = k;
            m->entries[idx].value = v;
            m->entries[idx].used  = 1;
            m->size++;
            return 1;
        }

        if (m->entries[idx].used && m->entries[idx].key == k) {
            m->entries[idx].value = v;
            return 1;
        }

        idx = (idx + 1) % m->capacity;
    }
}

int map_get(map_t* m, long k, void** v) {
    if (!m) return 0;
    long idx = _hash(k, m->capacity);

    for (;;) {
        if (!m->entries[idx].used) return 0;
        if (m->entries[idx].used && m->entries[idx].key == k) {
            *v = m->entries[idx].value;
            return 1;
        }

        idx = (idx + 1) % m->capacity;
    }

    return 0;
}

int map_remove(map_t* m, long k) {
    if (!m) return 0;
    long idx = _hash(k, m->capacity);

    for (;;) {
        if (!m->entries[idx].used) return 0;
        if (m->entries[idx].used && m->entries[idx].key == k) {
            m->entries[idx].used  = -1;
            m->entries[idx].value = NULL;
            m->size--;
            return 1;
        }

        idx = (idx + 1) % m->capacity;
    }

    return 0;
}

int map_iter_init(map_t* m, map_iter_t* it) {
    if (!m || !it) return 0;
    it->entries  = m->entries;
    it->capacity = m->capacity;
    it->index    = 0;
    return 1;
}

void* map_iter_next(map_iter_t* it) {
    if (!it || it->index >= it->capacity) return NULL;
    while (!it->entries[it->index].used && it->index < it->capacity) it->index++;
    if (it->index >= it->capacity) return NULL;
    return it->entries[it->index++].value;
}

int map_isempty(map_t* m) {
    return !m->size;
}

int map_free(map_t* m) {
    if (!m) return 0;
    mm_free(m->entries);
    m->entries = NULL;
    m->capacity = 0;
    m->size = 0;
    return 1;
}

int map_free_force(map_t* m) {
    if (!m) return 0;
    for (long i = 0; i < m->capacity; i++) {
        if (m->entries[i].used && m->entries[i].value) {
            mm_free(m->entries[i].value);
        }
    }

    return map_free(m);
}
