#include <symtab/vartb.h>

int VRTB_update_memory(long id, long offset, long size, char reg, vartab_ctx_t* ctx) {
    print_log("VRTB_update_memory(id=%i, offset=%i, size=%i, reg=%i)", id, offset, size, reg);
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        vi->vmi.offset    = offset;
        vi->vmi.size      = size;
        vi->vmi.reg       = reg;
        vi->vmi.allocated = 1;
        return 1;
    }

    return 0;    
}

int VRTB_update_definition(long id, long definition, vartab_ctx_t* ctx) {
    print_log("VRTB_update_definition(id=%i, definition=%i)", id, definition);
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (vi->vdi.defined) return 0;
        vi->vdi.definition = definition;
        vi->vdi.defined    = 1;
        return 1;
    }

    return 0;    
}

int VRTB_get_info_id(long id, variable_info_t* info, vartab_ctx_t* ctx) {
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (info) str_memcpy(info, vi, sizeof(variable_info_t));
        return 1;
    }

    return 0;
}

int VRTB_get_info(const char* varname, short s_id, variable_info_t* info, vartab_ctx_t* ctx) {
    map_iter_t it;
    map_iter_init(&ctx->vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
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

    var->vmi.reg    = -1;
    var->vmi.offset = -1;

    var->p_id = -1;
    var->type = type;
    return var;
}

int VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx) {
    print_log("VRTB_add_copy(v_id=%i)", src->v_id);
    variable_info_t* nnd = _create_variable_info(src->name, src->type, src->s_id, NULL);
    if (!nnd) return 0;
    str_memcpy(nnd, src, sizeof(variable_info_t));
    nnd->v_id = ctx->curr_id++;
    nnd->p_id = src->v_id;
    map_put(&ctx->vartb, nnd->v_id, nnd);
    return nnd->v_id;
}

int VRTB_add_info(const char* name, token_type_t type, short s_id, token_flags_t* flags, vartab_ctx_t* ctx) {
    print_log("VRTB_add_info(name=%s, type=%i, s_id=%i)", name, type, s_id);
    variable_info_t* nnd = _create_variable_info(name, type, s_id, flags);
    if (!nnd) return 0;

    nnd->v_id = ctx->curr_id++;
    if (!name) snprintf(nnd->name, TOKEN_MAX_SIZE, "tmp");
    map_put(&ctx->vartb, nnd->v_id, nnd);
    return nnd->v_id;
}

int VRTB_unload(vartab_ctx_t* ctx) {
    return map_free_force(&ctx->vartb);
}
