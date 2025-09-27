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
    list_add(&ctx->lst, a);
    return a;
}

int ALLIAS_add_owner(long v_id, long owner_id, allias_map_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    allias_t* ai;
    while ((ai = (allias_t*)list_iter_next(&it))) {
        if (ai->v_id == v_id) return set_add_int(&ai->owners, owner_id);
    }

    allias_t* a = _add_allias(v_id, ctx);
    if (!a) return 0;
    return set_add_int(&a->owners, owner_id);
}

int ALLIAS_get_owners(long v_id, set_t* out, allias_map_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    allias_t* ai;
    while ((ai = (allias_t*)list_iter_next(&it))) {
        if (ai->v_id == v_id) {
            set_copy_int(out, &ai->owners);
            return 1;
        }
    }

    set_init(out);
    return 0;
}

int ALLIAS_unload(allias_map_t* ctx) {
    return list_free_force(&ctx->lst);
}
