#include <symtab/strtb.h>

int STTB_update_info(long id, string_t* value, str_type_t t, strtb_ctx_t* ctx) {
    str_info_t* si;
    if (map_get(&ctx->strtb, id, (void**)&si)) {
        si->t = t;
        if (value) {
            if (si->value) destroy_string(si->value);
            si->value = value->copy(value);
        }

        return 1;
    }

    return 0;    
}

int STTB_get_info_id(long id, str_info_t* info, strtb_ctx_t* ctx) {
    str_info_t* si;
    if (map_get(&ctx->strtb, id, (void**)&si)) {
        if (info) str_memcpy(info, si, sizeof(str_info_t));
        return 1;
    }

    return 0;
}

int STTB_get_info(string_t* value, str_info_t* info, strtb_ctx_t* ctx) {
    map_iter_t it;
    map_iter_init(&ctx->strtb, &it);
    str_info_t* si;
    while (map_iter_next(&it, (void**)&si)) {
        if (si->value->equals(si->value, value)) {
            if (info) str_memcpy(info, si, sizeof(str_info_t));
            return 1;
        }
    }

    return 0;
}

int STTB_add_info(string_t* value, str_type_t t, strtb_ctx_t* ctx) {
    print_log("STTB_add_info(value=%s, t=%i)", value->body, t);
    str_info_t* nnd = (str_info_t*)mm_malloc(sizeof(str_info_t));
    if (!nnd) return 0;
    nnd->id = ctx->curr_id++;
    nnd->t  = t;

    nnd->value = value->copy(value);
    map_put(&ctx->strtb, nnd->id, nnd);
    return nnd->id;
}

static int _string_info_unload(str_info_t* info) {
    destroy_string(info->value);
    return mm_free(info);
}

int STTB_unload(strtb_ctx_t* ctx) {
    return map_free_force_op(&ctx->strtb, (int (*)(void *))_string_info_unload);
}
