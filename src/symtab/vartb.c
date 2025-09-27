#include <symtab/vartb.h>

int VRTB_update_offset(long id, long offset, vartab_ctx_t* ctx) {
    print_debug("VRTB_update_offset(id=%i, offset=%i)", id, offset);
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        if (vi->v_id == id) {
            vi->offset = offset;
            return 1;
        }
    }
    
    return 0;    
}

int VRTB_get_info_id(long id, variable_info_t* info, vartab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        if (vi->v_id == id) {
            if (info) str_memcpy(info, vi, sizeof(variable_info_t));
            return 1;
        }
    }
    
    return 0;
}

int VRTB_get_info(const char* varname, short s_id, variable_info_t* info, vartab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        if (((s_id < 0) || s_id == vi->s_id) && !str_strcmp(varname, vi->name)) {
            if (info) str_memcpy(info, vi, sizeof(variable_info_t));
            return 1;
        }
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
    return var;
}

int VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx) {
    print_debug("VRTB_add_copy(src=%s)", src->name);
    variable_info_t* nnd = _create_variable_info(src->name, src->type, src->s_id, NULL);
    if (!nnd) return 0;
    str_memcpy(nnd, src, sizeof(variable_info_t));
    nnd->v_id = ctx->curr_id++;
    list_add(&ctx->lst, nnd);
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
    
    list_add(&ctx->lst, nnd);
    return nnd->v_id;
}

int VRTB_unload(vartab_ctx_t* ctx) {
    return list_free_force(&ctx->lst);
}
