#include <symtab/vartb.h>
#define DEFINE_SETTER(name, field)                   \
    int name(symbol_id_t id, vartab_ctx_t* ctx) {    \
        print_log("%s(id=%li)", #name, id);          \
        variable_info_t* vi;                         \
        if (map_get(&ctx->vartb, id, (void**)&vi)) { \
            vi->field = 1;                           \
            return 1;                                \
        }                                            \
        return 0;                                    \
    }
DEFINE_SETTER(VRTB_set_not_null, csa.not_null);
DEFINE_SETTER(VRTB_set_volatile, vmi.vlatile);
DEFINE_SETTER(VRTB_set_used, vmi.used);
#undef DEFINE_SETTER

int VRTB_update_memory(symbol_id_t id, long offset, long size, char reg, short align, vartab_ctx_t* ctx) {
    print_log("VRTB_update_memory(id=%li, offset=%li, size=%li, reg=%c, align=%i)", id, offset, size, reg, align);
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (offset != FIELD_NO_CHANGE) vi->vmi.offset = offset;
        if (size   != FIELD_NO_CHANGE) vi->vmi.size   = size;
        if (reg    != FIELD_NO_CHANGE) vi->vmi.reg    = reg;
        if (align  != FIELD_NO_CHANGE) vi->vmi.align  = align;
        if (
            reg    >= 0 || 
            offset >= 0
        ) vi->vmi.allocated = 1;
        return 1;
    }

    return 0;  
}

int VRTB_update_definition(symbol_id_t id, long definition, symbol_id_t overdefined, vartab_ctx_t* ctx, int rewrite) {
    print_log("VRTB_update_definition(id=%li, definition=%li, overdef=%li, rewrite=%i)\n", id, definition, overdefined, rewrite);
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        switch (vi->vdi.defined) {
            case OVERDEFINED_VARIABLE: {
                if (!rewrite || vi->vdi.definition == overdefined) return 0;
                break;
            }
            case DEFINED_VARIABLE: {
                if (
                    !rewrite ||
                    (vi->vdi.definition == definition && overdefined == NO_SYMBOL_ID)
                ) return 0;
                break;
            }
            case UNDEFINED_VARIABLE:
            default: break;
        }

        if (overdefined != NO_SYMBOL_ID) {
            vi->vdi.definition = overdefined;
            vi->vdi.defined    = OVERDEFINED_VARIABLE;
        }
        else {
            vi->vdi.definition = definition;
            vi->vdi.defined    = DEFINED_VARIABLE;
        }

        return 1;
    }

    return 0;    
}

int VRTB_update_type(symbol_id_t id, int t, symbol_id_t t_id, vartab_ctx_t* ctx) {
    print_log("VRTB_update_type(id=%li, t=%i, t_id=%li)", id, t, t_id);
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (t != FIELD_NO_CHANGE)    vi->type = t;
        if (t_id != FIELD_NO_CHANGE) vi->t_id = t_id;
        return 1;
    }

    return 0;    
}

symbol_id_t VRTB_resolve_parent(symbol_id_t id, vartab_ctx_t* ctx) {
    variable_info_t* vi;
    while (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (vi->p_id != NO_SYMBOL_ID) id = vi->p_id;
        else break;
    }

    return id;
}

int VRTB_get_info_id(symbol_id_t id, variable_info_t* info, vartab_ctx_t* ctx) {
    variable_info_t* vi;
    if (map_get(&ctx->vartb, id, (void**)&vi)) {
        if (info) str_memcpy(info, vi, sizeof(variable_info_t));
        return 1;
    }

    return 0;
}

int VRTB_find_by_type_id(symbol_id_t t_id, variable_info_t* info, vartab_ctx_t* ctx) {
    map_foreach (variable_info_t* vi, &ctx->vartb) {
        if (vi->t_id == t_id) {
            if (info) str_memcpy(info, vi, sizeof(variable_info_t));
            return 1;
        }
    }

    return 0;
}

int VRTB_find_by_type_id_name(symbol_id_t t_id, string_t* name, symbol_id_t s_id, variable_info_t* info, vartab_ctx_t* ctx) {
    if (!name) return 0;
    map_foreach (variable_info_t* vi, &ctx->vartb) {
        if (vi->t_id != t_id) continue;
        if (s_id != NO_SYMBOL_ID && vi->s_id != s_id) continue;
        if (!vi->name || !name->equals(name, vi->name)) continue;

        if (info) str_memcpy(info, vi, sizeof(variable_info_t));
        return 1;
    }

    return 0;
}

int VRTB_get_info(string_t* varname, symbol_id_t s_id, variable_info_t* info, vartab_ctx_t* ctx) {
    map_foreach (variable_info_t* vi, &ctx->vartb) {
        if (((s_id == NO_SYMBOL_ID) || s_id == vi->s_id) && varname->equals(varname, vi->name)) {
            if (info) str_memcpy(info, vi, sizeof(variable_info_t));
            return 1;
        }
    }
    
    return 0;
}

static variable_info_t* _create_variable_info(string_t* name, token_type_t type, symbol_id_t s_id, basic_object_info_t flags) {
    variable_info_t* var = (variable_info_t*)mm_malloc(sizeof(variable_info_t));
    if (!var) return NULL;
    str_memset(var, 0, sizeof(variable_info_t));
    var->s_id        = s_id;
    if (name) var->name = name->copy(name);
    var->vfs         = flags;
    var->vmi.reg     = SMT_NULL;
    var->vmi.offset  = SMT_NULL;
    var->vmi.align   = CONF_get_full_bytness();
    var->vdi.defined = UNDEFINED_VARIABLE;
    var->p_id        = NO_SYMBOL_ID;
    var->type        = type;
    var->t_id        = NO_SYMBOL_ID;
    return var;
}

symbol_id_t VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx) {
    print_log("VRTB_add_copy(v_id=%i)", src->v_id);
    variable_info_t* nnd = _create_variable_info(NULL, src->type, src->s_id, (basic_object_info_t){ 0 });
    if (!nnd) return NO_SYMBOL_ID;
    
    str_memcpy(nnd, src, sizeof(variable_info_t));
    // nnd->vmi.allocated = 0;
    nnd->vdi.defined   = 0;

    nnd->v_id = ctx->curr_id++;
    nnd->p_id = src->v_id;
    nnd->s_id = src->s_id;
    nnd->name = src->name->copy(src->name);

    map_put(&ctx->vartb, nnd->v_id, nnd);
    return nnd->v_id;
}

symbol_id_t VRTB_add_info(string_t* name, token_type_t type, symbol_id_t s_id, basic_object_info_t flags, vartab_ctx_t* ctx) {
    print_log("VRTB_add_info(name=%s, type=%i, s_id=%i)", name ? name->body : "(null)", type, s_id);
    if (STRICT_ACTION(name && VRTB_get_info(name, s_id, NULL, ctx))) CONF_set_symtab_error(); // silent error btw
    variable_info_t* nnd = _create_variable_info(name, type, s_id, flags);
    if (!nnd) return NO_SYMBOL_ID;
    nnd->v_id = ctx->curr_id++;
    if (!name) nnd->name = create_string("tmp");
    map_put(&ctx->vartb, nnd->v_id, nnd);
    return nnd->v_id;
}

static int _variable_info_unload(variable_info_t* info) {
    destroy_string(info->name);
    return mm_free(info);
}

int VRTB_unload(vartab_ctx_t* ctx) {
    return map_free_force_op(&ctx->vartb, (int (*)(void*))_variable_info_unload);
}
