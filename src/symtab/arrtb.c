#include <symtab/arrtb.h>

int ARTB_get_info(const char* name, short scope, array_info_t* info, arrtab_ctx_t* ctx) {
    array_info_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name) && ((scope < 0) || scope == h->s_id)) {
            if (info) str_memcpy(info, h, sizeof(array_info_t));
            return 1;
        }

        h = h->next;
    }

    return 0;
}

static array_info_t* _create_info_array_entry(const char* name, short scope, token_type_t el_type) {
    array_info_t* entry = (array_info_t*)mm_malloc(sizeof(array_info_t));
    if (!entry) return NULL;
    str_memset(entry, 0, sizeof(array_info_t));

    entry->s_id = scope;
    if (name) str_strncpy(entry->name, name, TOKEN_MAX_SIZE);
    
    entry->el_type = el_type;
    entry->next = NULL;
    return entry;
}

int ARTB_add_info(const char* name, short scope, token_type_t el_type, arrtab_ctx_t* ctx) {
    print_debug("ARTB_add_info(name=%s, scope=%i, el_type=%i)", name, scope, el_type);
    array_info_t* nnd = _create_info_array_entry(name, scope, el_type);
    if (!nnd) return 0;

    nnd->v_id = ctx->curr_id++;
    if (!ctx->h) {
        ctx->h = nnd;
        return nnd->v_id;
    }

    array_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return nnd->v_id;
}

int ARTB_unload(arrtab_ctx_t* ctx) {
    array_info_t* h = ctx->h;
    while (h) {
        array_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}
