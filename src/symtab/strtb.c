#include <symtab/strtb.h>

int STTB_get_info_id(long id, str_info_t* info, strtb_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    str_info_t* si;
    while ((si = (str_info_t*)list_iter_next(&it))) {
        if (si->id == id) {
            if (info) str_memcpy(info, si, sizeof(str_info_t));
            return 1;
        }
    }

    return 0;
}

int STTB_get_info(const char* name, str_info_t* info, strtb_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    str_info_t* si;
    while ((si = (str_info_t*)list_iter_next(&it))) {
        if (!str_strncmp(si->value, name, TOKEN_MAX_SIZE)) {
            if (info) str_memcpy(info, si, sizeof(str_info_t));
            return 1;
        }
    }

    return 0;
}

int STTB_add_info(const char* name, strtb_ctx_t* ctx) {
    print_debug("STTB_add_info(name=%s)", name);
    str_info_t* nnd = (str_info_t*)mm_malloc(sizeof(str_info_t));
    if (!nnd) return 0;
    nnd->id = ctx->curr_id++;
    if (name) str_strncpy(nnd->value, name, TOKEN_MAX_SIZE);
    list_add(&ctx->lst, nnd);
    return nnd->id;
}

int STTB_unload(strtb_ctx_t* ctx) {
    return list_free_force(&ctx->lst);
}
