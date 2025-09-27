#include <symtab/arrtb.h>

int ARTB_get_info_id(long id, array_info_t* info, arrtab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    array_info_t* ai;
    while ((ai = (array_info_t*)list_iter_next(&it))) {
        if (ai->v_id == id) {
            if (info) str_memcpy(info, ai, sizeof(array_info_t));
            return 1;
        }
    }

    return 0;
}

int ARTB_get_info(const char* name, short scope, array_info_t* info, arrtab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    array_info_t* ai;
    while ((ai = (array_info_t*)list_iter_next(&it))) {
        if (!str_strcmp(ai->name, name) && ((scope < 0) || scope == ai->s_id)) {
            if (info) str_memcpy(info, ai, sizeof(array_info_t));
            return 1;
        }
    }

    return 0;
}

static array_info_t* _create_info_array_entry(const char* name, short scope, int heap, token_type_t el_type) {
    array_info_t* entry = (array_info_t*)mm_malloc(sizeof(array_info_t));
    if (!entry) return NULL;
    str_memset(entry, 0, sizeof(array_info_t));
    entry->s_id = scope;
    if (name) str_strncpy(entry->name, name, TOKEN_MAX_SIZE);
    entry->el_type = el_type;
    entry->heap = heap;
    return entry;
}

int ARTB_add_info(const char* name, short scope, int heap, token_type_t el_type, arrtab_ctx_t* ctx) {
    print_debug("ARTB_add_info(name=%s, scope=%i, heap=%i, el_type=%i)", name, scope, heap, el_type);
    array_info_t* nnd = _create_info_array_entry(name, scope, heap, el_type);
    if (!nnd) return 0;
    nnd->v_id = ctx->curr_id++;
    list_add(&ctx->lst, nnd);
    return nnd->v_id;
}

int ARTB_unload(arrtab_ctx_t* ctx) {
    return list_free_force(&ctx->lst);
}
