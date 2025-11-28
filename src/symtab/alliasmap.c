#include <symtab/alliasmap.h>

static allias_t* _create_allias(long v_id) {
    allias_t* a = (allias_t*)mm_malloc(sizeof(allias_t));
    if (!a) return NULL;
    str_memset(a, 0, sizeof(allias_t));
    
    set_init(&a->owners);
    set_enable_cmp(&a->owners);
    
    set_init(&a->delown);
    set_enable_cmp(&a->delown);
    
    a->v_id = v_id;
    return a;
}

static allias_t* _add_allias(long v_id, allias_map_t* ctx) {
    allias_t* a = _create_allias(v_id);
    if (!a) return NULL;
    map_put(&ctx->allias, v_id, a);
    return a;
}

int ALLIAS_get_owners(long v_id, set_t* out, allias_map_t* ctx) {
    allias_t* ai;
    if (map_get(&ctx->allias, v_id, (void**)&ai)) {
        if (out) set_copy(out, &ai->owners);
        return 1;
    }

    set_init(out);
    return 0;
}

int ALLIAS_add_owner(long v_id, long owner_id, allias_map_t* ctx) {
    print_log("ALLIAS_add_owner(v_id=%i, owner=%i)", v_id, owner_id);
    allias_t* ai;
    if (map_get(&ctx->allias, v_id, (void**)&ai)) {
        return set_add(&ai->owners, (void*)owner_id);
    }

    allias_t* a = _add_allias(v_id, ctx);
    if (!a) return 0;
    return set_add(&a->owners, (void*)owner_id);
}

int ALLIAS_mark_owner(long v_id, long owner_id, allias_map_t* ctx) {
    allias_t* ai;
    if (map_get(&ctx->allias, v_id, (void**)&ai)) {
        set_add(&ai->delown, (void*)owner_id);
        return set_equal(&ai->owners, &ai->delown);
    }

    return 0;
}

int ALLIAS_unload(allias_map_t* ctx) {
    map_iter_t it;
    map_iter_init(&ctx->allias, &it);
    allias_t* ai;
    while (map_iter_next(&it, (void**)&ai)) {
        set_free_force(&ai->delown);
        set_free_force(&ai->owners);
    }

    return map_free_force(&ctx->allias);
}
