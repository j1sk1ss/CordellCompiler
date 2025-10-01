#include <std/list.h>

int list_init(list_t* l) {
    l->h = NULL;
    return 1;
}

int list_iter_hinit(list_t* l, list_iter_t* it) {
    it->curr = l->h;
    return 1;
}

int list_iter_tinit(list_t* l, list_iter_t* it) {
    it->curr = l->t;
    return 1;
}

int list_size(list_t* l) {
    return l->s;
}

int list_isempty(list_t* l) {
    return l->s == 0;
}

int list_add(list_t* l, void* data) {
    list_node_t* h = (list_node_t*)mm_malloc(sizeof(list_node_t));
    if (!h) return 0;
    str_memset(h, 0, sizeof(list_node_t));
    h->data = data;

    if (!l->h) {
        l->h = h;
        l->t = h;
        l->s = 1;
        return 1;
    }

    h->p = l->t;
    l->t->n = h;
    l->t = h;
    l->s++;
    return 1;
}

int list_push_back(list_t* l, void* data) {
    list_node_t* node = mm_malloc(sizeof(list_node_t));
    if (!node) return -1;

    node->data = data;
    node->n = NULL;
    node->p = l->t;

    if (l->t) l->t->n = node;
    else l->h = node;
    l->t = node;
    l->s++;

    return 0;
}

int list_push_front(list_t* l, void* data) {
    list_node_t* node = mm_malloc(sizeof(list_node_t));
    if (!node) return -1;

    node->data = data;
    node->p = NULL;
    node->n = l->h;

    if (l->h) l->h->p = node;
    else l->t = node;
    l->h = node;
    l->s++;
    return 0;
}

int list_copy(list_t* src, list_t* dst) {
    list_iter_t it;
    list_iter_hinit(src, &it);
    void* d;
    while ((d = list_iter_next(&it))) {
        list_add(dst, d);
    }

    return 1;
}

void* list_iter_current(list_iter_t* it) {
    if (!it->curr) return NULL;
    return it->curr->data;
}

void* list_iter_next(list_iter_t* it) {
    if (!it->curr) return NULL;
    void* data = it->curr->data;
    it->curr = it->curr->n;
    return data;
}

void* list_iter_next_top(list_iter_t* it) {
    if (!it->curr || !it->curr->n) return NULL;
    return it->curr->n->data;
}

void* list_iter_prev(list_iter_t* it) {
    if (!it->curr) return NULL;
    void* data = it->curr->data;
    it->curr = it->curr->p;
    return data;
}

void* list_iter_prev_top(list_iter_t* it) {
    if (!it->curr || !it->curr->p) return NULL;
    return it->curr->p->data;
}

void* list_get_head(list_t* l) {
    if (!l->h) return NULL;
    return l->h->data;
}

void* list_get_tail(list_t* l) {
    if (!l->t) return NULL;
    return l->t->data;
}

static int _list_free(list_t* l, int force) {
    list_node_t* h = l->h;
    while (h) {
        list_node_t* n = h->n;
        if (force) mm_free(h->data);
        mm_free(h);
        h = n;
    }

    l->s = 0;
    l->h = NULL;
    l->t = NULL;

    return 1;    
}

int list_free(list_t* l) {
    return _list_free(l, 0);
}

int list_free_force(list_t* l) {
    return _list_free(l, 1);
}
