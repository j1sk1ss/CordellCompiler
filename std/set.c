#include <std/set.h>

int set_init(set_t* s) {
    return map_init(&s->body);
}

int set_has_inttuple(set_t* s, int_tuple_t* t) {
    map_iter_t it;
    map_iter_init(&s->body, &it);
    int_tuple_t* tuple;
    while (map_iter_next(&it, (void**)&tuple)) {
        if (tuple->x == t->x && tuple->y == t->y) return 1;
    }

    return 0;    
}

int set_has(set_t* s, void* data) {
    void* d;
    if (map_get(&s->body, (long)data, (void**)&d)) return 1;
    return 0;
}

int set_add(set_t* s, void* data) {
    if (set_has(s, data)) return 0;
    return map_put(&s->body, (long)data, data);
}

int set_remove(set_t* s, void* data) {
    return map_remove(&s->body, (long)data);
}

int set_iter_init(set_t* s, set_iter_t* it) {
    return map_iter_init(&s->body, &it->it);
}

int set_iter_next(set_iter_t* it, void** d) {
    return map_iter_next(&it->it, d);
}

int set_minus_set(set_t* trg, set_t* s) {
    set_iter_t it;
    set_iter_init(s, &it);
    void* data;
    while (set_iter_next(&it, (void**)&data)) {
        set_remove(trg, data);
    }

    return 1;
}

int set_copy(set_t* dst, set_t* src) {
    return map_copy(&dst->body, &src->body);
}

int set_intersect(set_t* dst, set_t* a, set_t* b) {
    set_iter_t it;
    set_iter_init(a, &it);
    void* data;
    while (set_iter_next(&it, (void**)&data)) {
        if (set_has(b, data)) set_add(dst, data);
    }
    
    return 1;
}

int set_equal(set_t* a, set_t* b) {
    return map_equals(&a->body, &b->body);
}

int set_union(set_t* dst, set_t* a, set_t* b) {
    map_copy(&dst->body, &a->body);

    set_iter_t it;
    set_iter_init(b, &it);
    void* data;
    while (set_iter_next(&it, (void**)&data)) {
        set_add(dst, data);
    }

    return 1;
}

int set_size(set_t* s) {
    return s->body.size;
}

int set_free(set_t* s) {
    return map_free(&s->body);
}

int set_free_force(set_t* s) {
    return map_free_force(&s->body);
}
