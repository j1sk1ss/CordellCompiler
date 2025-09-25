#include <std/set.h>

int set_init(set_t* s) {
    s->head = NULL;
    return 1;
}

int set_has_addr(set_t* s, void* data) {
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.addrdata == data) return 1;
        n = n->next;
    }

    return 0;
}

int set_add_addr(set_t* s, void* data) {
    if (set_has(s, data)) return 0;
    set_node_t* n = (set_node_t*)mm_malloc(sizeof(set_node_t));
    if (!n) return 0;

    n->stg.addrdata = data;
    n->next = s->head;
    s->head = n;
    return 1;
}

void* set_iter_next_addr(set_iter_t* it) {
    if (!it->current) return NULL;
    void* data = it->current->stg.addrdata;
    it->current = it->current->next;
    return data;
}

int set_has_int(set_t* s, long data) {
    set_node_t* n = s->head;
    while (n) {
        if (n->stg.intdata == data) return 1;
        n = n->next;
    }

    return 0;
}

int set_add_int(set_t* s, long data) {
    if (set_has(s, data)) return 0;
    set_node_t* n = (set_node_t*)mm_malloc(sizeof(set_node_t));
    if (!n) return 0;

    n->stg.intdata = data;
    n->next = s->head;
    s->head = n;
    return 1;
}

long set_iter_next_int(set_iter_t* it) {
    if (!it->current) return NULL;
    void* data = it->current->stg.intdata;
    it->current = it->current->next;
    return data;
}

int set_iter_init(set_t* s, set_iter_t* it) {
    it->current = s->head;
    return 1;
}

int set_free(set_t* s) {
    set_node_t* n = s->head;
    while (n) {
        set_node_t* next = n->next;
        free(n);
        n = next;
    }

    s->head = NULL;
    return 1;
}
