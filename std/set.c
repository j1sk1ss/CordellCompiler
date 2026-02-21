#include <std/set.h>

int set_init(set_t* s, int cmp) {
    return map_init(&s->body, cmp);
}

int set_has_inttuple(set_t* s, int_tuple_t* t) {
    map_foreach (int_tuple_t* tuple, &s->body) {
        if (tuple->x == t->x && tuple->y == t->y) return 1;
    }

    return 0;    
}

int set_has(set_t* s, void* data) {
    return map_get(&s->body, (long)data, NULL);
}

int set_subset(set_t* a, set_t* b) {
    int is_subset = 1;
    set_foreach (void* d, b) {
        is_subset = is_subset && set_has(a, d);
        if (!is_subset) break;
    }

    return set_size(a) > set_size(b) && is_subset;
}

int set_add(set_t* s, void* data) {
    return !set_has(s, data) && map_put(&s->body, (long)data, data);
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
    set_foreach (void* data, s) {
        set_remove(trg, data);
    }

    return 1;
}

int set_copy(set_t* dst, set_t* src) {
    return map_copy(&dst->body, &src->body);
}

int set_intersect(set_t* dst, set_t* a, set_t* b) {
    set_foreach (void* data, a) {
        if (set_has(b, data)) {
            set_add(dst, data);
        }
    }

    return 1;
}

int set_equal(set_t* a, set_t* b) {
    return map_equals(&a->body, &b->body);
}

int set_union(set_t* dst, set_t* a, set_t* b) {
    set_t tmp;
    set_copy(&tmp, a);
    set_foreach (void* data, b) {
        set_add(&tmp, data);
    }

    set_free(dst);
    set_copy(dst, &tmp);
    set_free(&tmp);
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
