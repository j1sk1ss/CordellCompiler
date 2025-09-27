#include <symtab/vartb.h>

int VRTB_update_offset(long id, long offset, vartab_ctx_t* ctx) {
    print_debug("VRTB_update_offset(id=%i, offset=%i)", id, offset);
    variable_info_t* h = ctx->h;
    while (h) {
        if (h->v_id == id) {
            h->offset = offset;
            return 1;
        }

        h = h->next;
    }
    
    return 0;    
}

int VRTB_get_info_id(long id, variable_info_t* info, vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        if (h->v_id == id) {
            if (info) str_memcpy(info, h, sizeof(variable_info_t));
            return 1;
        }

        h = h->next;
    }
    
    return 0;
}

int VRTB_get_info(const char* varname, short s_id, variable_info_t* info, vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        if (((s_id < 0) || s_id == h->s_id) && !str_strcmp(varname, h->name)) {
            if (info) str_memcpy(info, h, sizeof(variable_info_t));
            return 1;
        }

        h = h->next;
    }
    
    return 0;
}

static variable_info_t* _create_variable_info(const char* name, token_type_t type, short s_id, token_flags_t* flags) {
    variable_info_t* var = (variable_info_t*)mm_malloc(sizeof(variable_info_t));
    if (!var) return NULL;
    str_memset(var, 0, sizeof(variable_info_t));

    var->s_id = s_id;
    if (name) {
        str_strncpy(var->name, name, TOKEN_MAX_SIZE);
    }

    if (flags) {
        var->heap = flags->heap;
        var->ptr  = flags->ptr;
        var->glob = flags->glob;
    }

    var->p_id = -1;
    var->type = type;
    var->next = NULL;
    return var;
}

int VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx) {
    variable_info_t* nnd = _create_variable_info(src->name, src->type, src->s_id, NULL);
    if (!nnd) return 0;
    nnd->heap = src->heap;
    nnd->ptr  = src->ptr;
    nnd->glob = src->glob;
    nnd->p_id = src->v_id;

    nnd->v_id = ctx->curr_id++;
    if (!ctx->h) {
        ctx->h = nnd;
        return nnd->v_id;
    }

    variable_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return nnd->v_id;
}

int VRTB_add_info(const char* name, token_type_t type, short s_id, token_flags_t* flags, vartab_ctx_t* ctx) {
    print_debug("VRTB_add_info(name=%s, type=%i, s_id=%i)", name, type, s_id);
    variable_info_t* nnd = _create_variable_info(name, type, s_id, flags);
    if (!nnd) return 0;

    nnd->v_id = ctx->curr_id++;
    if (!name) {
        snprintf(nnd->name, TOKEN_MAX_SIZE, "tmp");
    }
    
    if (!ctx->h) {
        ctx->h = nnd;
        return nnd->v_id;
    }

    variable_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return nnd->v_id;
}

int VRTB_unload(vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        variable_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}
