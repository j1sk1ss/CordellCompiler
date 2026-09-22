#include <symtab/vttb.h>

int VTTB_get_info_id(symbol_id_t id, vtable_info_t* info, vttab_ctx_t* ctx) {
    print_log("VTTB_get_info_id(id=%li)", id);
    vtable_info_t* vi;
    if (map_get(&ctx->vttb, id, (void**)&vi)) {
        if (info) str_memcpy(info, vi, sizeof(vtable_info_t));
        return 1;
    }

    return 0;
}

int VTTB_get_info_type(symbol_id_t t_id, vtable_info_t* info, vttab_ctx_t* ctx) {
    print_log("VTTB_get_info_type(t_id=%li)", t_id);
    map_foreach (vtable_info_t* vi, &ctx->vttb) {
        if (vi->t_id == t_id) {
            if (info) str_memcpy(info, vi, sizeof(vtable_info_t));
            return 1;
        }
    }

    return 0;
}

static vtable_info_t* _create_vtable_info(symbol_id_t t_id) {
    vtable_info_t* info = (vtable_info_t*)mm_malloc(sizeof(vtable_info_t));
    if (!info) return NULL;
    str_memset(info, 0, sizeof(vtable_info_t));
    info->t_id = t_id;
    list_init(&info->funcs);
    return info;
}

symbol_id_t VTTB_add_info(symbol_id_t t_id, vttab_ctx_t* ctx) {
    print_log("VTTB_add_info(t_id=%li)", t_id,);
    vtable_info_t other;
    if (VTTB_get_info_type(t_id, &other, ctx)) return other.id;

    vtable_info_t* nnd = _create_vtable_info(t_id);
    if (!nnd) return NO_SYMBOL_ID;
    nnd->id = ctx->curr_id++;

    map_put(&ctx->vttb, nnd->id, nnd);
    return nnd->id;
}

int VTTB_add_func(symbol_id_t id, symbol_id_t f_id, vttab_ctx_t* ctx) {
    print_log("VTTB_add_func(id=%li, f_id=%li)", id, f_id);
    vtable_info_t* vi;
    if (map_get(&ctx->vttb, id, (void**)&vi)) {
        return list_add(&vi->funcs, (void*)f_id);
    }

    return 0;
}

static int _vtable_info_unload(vtable_info_t* info) {
    list_free(&info->funcs);
    return mm_free(info);
}

int VTTB_unload(vttab_ctx_t* ctx) {
    return map_free_force_op(&ctx->vttb, (int (*)(void*))_vtable_info_unload);
}