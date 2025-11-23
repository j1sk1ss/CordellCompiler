#include <std/map.h>

static unsigned long __hash(long val) {
    unsigned long x = (unsigned long)val;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9UL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebUL;
    x = x ^ (x >> 31);
    return x;
}

static long _get_index(long key, long capacity) {
    return __hash(key) % capacity;
}

static int _map_update_hash(map_t* m) {
    unsigned long h = 0;
    for (long i = 0; i < m->capacity; i++) {
        if (m->entries[i].used) {
            h ^= __hash(m->entries[i].key);
            h ^= __hash((long)m->entries[i].value);
        }
    }

    h ^= m->size;
    m->hash = h;
    return 1;
}

int map_init(map_t* m) {
    if (!m) return 0;
    m->capacity = MAP_INITIAL_CAPACITY;
    m->size     = 0;
    m->hash     = 0;
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
        if (old_entries[i].used) {
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

    long idx = _get_index(k, m->capacity);
    for (;;) {
        if (!m->entries[idx].used) {
            m->entries[idx].key   = k;
            m->entries[idx].value = v;
            m->entries[idx].used  = 1;
            m->size++;
            _map_update_hash(m);
            return 1;
        }

        if (m->entries[idx].used && m->entries[idx].key == k) {
            m->entries[idx].value = v;
            _map_update_hash(m);
            return 1;
        }

        idx = (idx + 1) % m->capacity;
    }
}

int map_copy(map_t* dst, map_t* src) {
    if (!dst || !src) return 0;
    dst->capacity = src->capacity;
    dst->size     = src->size;
    dst->hash     = src->hash;
    dst->entries  = (map_entry_t*)mm_malloc(src->capacity * sizeof(map_entry_t));
    str_memcpy(dst->entries, src->entries, src->capacity * sizeof(map_entry_t));
    return 1;
}

int map_get(map_t* m, long k, void** v) {
    if (!m) return 0;
    long idx = _get_index(k, m->capacity);

    for (;;) {
        if (!m->entries[idx].used) return 0;
        if (m->entries[idx].used && m->entries[idx].key == k) {
            if (v) *v = m->entries[idx].value;
            return 1;
        }

        idx = (idx + 1) % m->capacity;
    }

    return 0;
}

int map_remove(map_t* m, long k) {
    if (!m) return 0;
    long idx = _get_index(k, m->capacity);

    for (;;) {
        if (!m->entries[idx].used) return 0;
        if (m->entries[idx].used && m->entries[idx].key == k) {
            m->entries[idx].used  = 0;
            m->entries[idx].value = NULL;
            m->size--;
            _map_update_hash(m);
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

int map_iter_next(map_iter_t* it, void** d) {
    if (!it || it->index >= it->capacity) return 0;
    while (!it->entries[it->index].used && it->index < it->capacity) it->index++;
    if (it->index >= it->capacity) return 0;
    if (d) *d = it->entries[it->index++].value;
    return 1;
}

int map_isempty(map_t* m) {
    return !m->size;
}

int map_equals(map_t* a, map_t* b) {
    if (!a || !b) return 0;
    return a->hash == b->hash;
}

int map_free(map_t* m) {
    if (!m) return 0;
    mm_free(m->entries);
    m->entries  = NULL;
    m->capacity = 0;
    m->size     = 0;
    m->hash     = 0;
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
