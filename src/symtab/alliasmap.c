#include <symtab/alliasmap.h>

static allias_t* _create_allias(long v_id) {
    allias_t* a = (allias_t*)mm_malloc(sizeof(allias_t));
    if (!a) return NULL;
    str_memset(a, 0, sizeof(allias_t));
    set_init(&a->owners);
    a->v_id = v_id;
    return a;
}

static allias_t* _add_allias(long v_id, allias_map_t* ctx) {
    allias_t* a = _create_allias(v_id);
    if (!a) return NULL;
    if (!ctx->h) {
        ctx->h = a;
        return a;
    }

    allias_t* h = ctx->h;
    while (h->next) h = h->next;
    h->next = a;
    return a;
}

int ALLIAS_add_owner(long v_id, long owner_id, allias_map_t* ctx) {
    allias_t* h = ctx->h;
    while (h) {
        if (h->v_id == v_id) return set_add_int(&h->owners, owner_id);
        h = h->next;
    }

    allias_t* a = _add_allias(v_id, ctx);
    if (!a) return 0;
    return set_add_int(&a->owners, owner_id);
}

int ALLIAS_get_owners(long v_id, set_t* out, allias_map_t* ctx) {
    allias_t* h = ctx->h;
    while (h) {
        if (h->v_id == v_id) {
            set_copy_int(out, &h->owners);
            return 1;
        }
        h = h->next;
    }

    set_init(out);
    return 0;
}

int ALLIAS_unload(allias_map_t* ctx) {
    allias_t* h = ctx->h;
    while (h) {
        allias_t* n = h->next;
        set_free(&h->owners);
        mm_free(h);
        h = n;
    }
    
    ctx->h = NULL;
    return 1;
}
