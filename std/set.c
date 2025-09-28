#include <std/set.h>

int set_init(set_t* s) {
    s->head = NULL;
    return 1;
}

int set_has_inttuple(set_t* s, int_tuple_t* t) {
    set_node_t* n = s->head;
    while (n) {
        int_tuple_t* tuple = (int_tuple_t*)n->stg.addrdata;
        if (tuple->x == t->x && tuple->y == t->y) return 1;
        n = n->next;
    }

    return 0;    
}

int set_has_addr(set_t* s, void* data) {
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.addrdata == data) return 1;
        n = n->next;
    }

    return 0;
}

int set_has_int(set_t* s, long data) {
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.intdata == data) return 1;
        n = n->next;
    }

    return 0;
}

int set_add_addr(set_t* s, void* data) {
    if (set_has_addr(s, data)) return 0;
    set_node_t* n = (set_node_t*)mm_malloc(sizeof(set_node_t));
    if (!n) return 0;
    n->stg.addrdata = data;
    n->next = s->head;
    s->head = n;
    return 1;
}

int set_remove_addr(set_t* s, void* data) {
    set_node_t* prev = NULL;
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.addrdata == data) {
            if (!prev) s->head = n->next;
            else prev->next = n->next;
            mm_free(n);
            return 1;
        }

        n = n->next;
    }

    return 0;
}

int set_remove_int(set_t* s, long data) {
    set_node_t* prev = NULL;
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.intdata == data) {
            if (!prev) s->head = n->next;
            else prev->next = n->next;
            mm_free(n);
            return 1;
        }

        n = n->next;
    }

    return 0;
}

int set_minus_int_set(set_t* trg, set_t* s) {
    set_iter_t it;
    set_iter_init(s, &it);
    long data;
    while ((data = set_iter_next_int(&it)) >= 0) {
        set_remove_int(trg, data);
    }

    return 1;
}

int set_add_int(set_t* s, long data) {
    if (set_has_int(s, data)) return 0;
    set_node_t* n = (set_node_t*)mm_malloc(sizeof(set_node_t));
    if (!n) return 0;

    n->stg.intdata = data;
    n->next = s->head;
    s->head = n;
    return 1;
}

int set_iter_init(set_t* s, set_iter_t* it) {
    it->current = s->head;
    return 1;
}

void* set_iter_next_addr(set_iter_t* it) {
    if (!it->current) return NULL;
    void* data = it->current->stg.addrdata;
    it->current = it->current->next;
    return data;
}

long set_iter_next_int(set_iter_t* it) {
    if (!it->current) return -1;
    long data = it->current->stg.intdata;
    it->current = it->current->next;
    return data;
}

int set_copy_addr(set_t* dst, set_t* src) {
    set_free(dst);
    set_iter_t it;
    set_iter_init(src, &it);
    void* data;
    while ((data = set_iter_next_addr(&it))) {
        set_add_addr(dst, data);
    }

    return 1;
}

int set_copy_int(set_t* dst, set_t* src) {
    set_free(dst);
    set_iter_t it;
    set_iter_init(src, &it);
    long data;
    while ((data = set_iter_next_int(&it)) >= 0) {
        set_add_int(dst, data);
    }

    return 1;
}

int set_intersect_addr(set_t* dst, set_t* a, set_t* b) {
    set_free(dst);
    set_iter_t it;
    set_iter_init(a, &it);
    void* data;
    while ((data = set_iter_next_addr(&it))) {
        if (set_has_addr(b, data)) {
            set_add_addr(dst, data);
        }
    }
    
    return 1;
}

int set_equal_addr(set_t* a, set_t* b) {
    set_iter_t it;
    set_iter_init(a, &it);
    void* data;
    while ((data = set_iter_next_addr(&it))) {
        if (!set_has_addr(b, data)) {
            return 0;
        }
    }

    set_iter_init(b, &it);
    while ((data = set_iter_next_addr(&it))) {
        if (!set_has_addr(a, data)) {
            return 0;
        }
    }

    return 1;
}

int set_equal_int(set_t* a, set_t* b) {
    set_iter_t it;
    set_iter_init(a, &it);
    long data;
    while ((data = set_iter_next_int(&it)) >= 0) {
        if (!set_has_int(b, data)) {
            return 0;
        }
    }

    set_iter_init(b, &it);
    while ((data = set_iter_next_int(&it)) >= 0) {
        if (!set_has_int(a, data)) {
            return 0;
        }
    }

    return 1;
}

int set_union_addr(set_t* dst, set_t* a, set_t* b) {
    set_free(dst);

    set_iter_t it;
    set_iter_init(a, &it);
    void* data;
    while ((data = set_iter_next_addr(&it))) {
        set_add_addr(dst, data);
    }

    set_iter_init(b, &it);
    while ((data = set_iter_next_addr(&it))) {
        set_add_addr(dst, data);
    }

    return 1;
}

int set_union_int(set_t* dst, set_t* a, set_t* b) {
    set_iter_t it;
    set_iter_init(a, &it);
    long data;
    while ((data = set_iter_next_int(&it)) >= 0) {
        set_add_int(dst, data);
    }

    set_iter_init(b, &it);
    while ((data = set_iter_next_int(&it)) >= 0) {
        set_add_int(dst, data);
    }

    return 1;
}

int set_size(set_t* s) {
    int size = 0;
    set_node_t* n = s->head;
    while (n) {
        n = n->next;
        size++;
    }

    return size;
}
int _set_free(set_t* s, int force) {
    set_node_t* n = s->head;
    while (n) {
        set_node_t* next = n->next;
        if (force) mm_free(n->stg.addrdata);
        mm_free(n);
        n = next;
    }

    s->head = NULL;
    return 1;
}

int set_free(set_t* s) {
    return _set_free(s, 0);
}

int set_free_force(set_t* s) {
    return _set_free(s, 1);
}
