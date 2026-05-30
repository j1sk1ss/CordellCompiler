#include <symtab/typetb.h>

static type_info_t* _create_type_info(string_t* name) {
    type_info_t* info = (type_info_t*)mm_malloc(sizeof(type_info_t));
    if (!info) return NULL;
    list_init(&info->link.c);

    info->p         = NO_SYMBOL_ID;
    info->link.name = NULL;
    info->link.p    = NO_SYMBOL_ID;
    if (name) info->name = name->copy(name);
    else info->name = NULL;
    str_memset(&info->memory, 0, sizeof(info->memory));
    info->memory.align = CONF_get_eight_bytness();
    
    return info;
}

symbol_id_t TPTB_add_info(string_t* name, symbol_id_t s_id, type_type_t t, int align, typetab_ctx_t* ctx) {
    if (TPTB_get_info(name, s_id, NULL, ctx)) return NO_SYMBOL_ID;
    type_info_t* info = _create_type_info(name);
    if (!info) return NO_SYMBOL_ID;
    info->id           = ctx->curr_id++;
    info->s_id         = s_id;
    info->t            = t;
    info->memory.align = align;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

symbol_id_t TPTB_add_copy(symbol_id_t id, symbol_id_t nv_id, int ptr, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return NO_SYMBOL_ID;

    type_info_t* info = _create_type_info(ti->name);
    if (!info) return NO_SYMBOL_ID;

    info->p            = id;
    info->t            = ti->t;
    info->memory.size  = ti->memory.size;
    info->memory.ptr   = ptr;
    info->link.p       = ti->link.p;
    info->link.v_id    = nv_id;
    info->memory.align = ti->memory.align;
    if (ti->link.name) info->link.name = ti->link.name->copy(ti->link.name);
    foreach (symbol_id_t c_id, &ti->link.c) {
        list_add(&info->link.c, (void*)c_id);
    }

    info->id         = ctx->curr_id++;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

static inline symbol_id_t _get_type_by_token(token_t* t, typetab_ctx_t* ctx) {
    if (!t) return NO_SYMBOL_ID;
    map_foreach (type_info_t* ti, &ctx->typetb) {
        if (ti->t == t->t_type && ti->memory.ptr == t->flags.ptr) return ti->id;
    }

    return NO_SYMBOL_ID;
}

symbol_id_t TPTB_add_info_from_token(symbol_id_t s_id, token_t* t, symbol_id_t v_id, typetab_ctx_t* ctx) {
    symbol_id_t existed = _get_type_by_token(t, ctx);
    if (existed != NO_SYMBOL_ID) return existed;
    
    type_info_t* info = _create_type_info(NULL);
    if (!info) return NO_SYMBOL_ID;

    info->link.v_id  = v_id;
    info->id         = ctx->curr_id++;
    info->s_id       = s_id;
    info->memory.ptr = t->flags.ptr;

    switch (t->t_type) {
        case GENERIC_TYPE_TOKEN:
        case GENERIC_VARIABLE_TOKEN: info->t = TYPE_GENERICS;  break;
        case CUSTOM_TYPE_TOKEN:
        case CUSTOM_VARIABLE_TOKEN:  info->t = TYPE_CUSTOM;    break;
        case FUNC_TOKEN:
        case FUNC_PROT_TOKEN:        info->t = TYPE_METHOD;    break;
        case ARRAY_TYPE_TOKEN:       info->t = TYPE_ARRAY;     break;
        default:                     info->t = TYPE_PRIMITIVE; break;
    }

    if (
        t->t_type == GENERIC_TYPE_TOKEN || 
        t->t_type == GENERIC_VARIABLE_TOKEN
    ) info->t = TYPE_GENERICS;
    else if (
        t->t_type == CUSTOM_TYPE_TOKEN ||
        t->t_type == CUSTOM_VARIABLE_TOKEN
    ) info->t = TYPE_CUSTOM;
    else if (
        t->t_type == FUNC_PROT_TOKEN ||
        t->t_type == FUNC_TOKEN
    ) info->t = TYPE_METHOD;
    else if (
        t->t_type == ARRAY_TYPE_TOKEN
    ) info->t = TYPE_ARRAY;
    else info->t = TYPE_PRIMITIVE;

    info->memory.size = 0;
    if (info->t == TYPE_PRIMITIVE) {
        info->memory.size = TKN_convert_type_size(TKN_variable_bitness(t, 1));
    }

    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

int TPTB_add_as_child(symbol_id_t p_id, symbol_id_t c_id, string_t* name, long overrite_size, typetab_ctx_t* ctx) {
    type_info_t *p_ti, *c_ti;
    if (
        p_id != c_id &&
        map_get(&ctx->typetb, p_id, (void**)&p_ti) &&
        map_get(&ctx->typetb, c_id, (void**)&c_ti)
    ) {
        map_foreach (type_info_t* another, &ctx->typetb) {
            if (another->p != p_id || another->id == p_id) continue;
            TPTB_add_as_child(another->id, c_id, name, overrite_size, ctx);
        }

        list_add(&p_ti->link.c, (void*)c_id);
        c_ti->link.p = p_id;
        if (name) {
            if (c_ti->link.name) destroy_string(c_ti->link.name);
            c_ti->link.name = name->copy(name);
        }
        
        if (overrite_size != FIELD_NO_CHANGE) c_ti->memory.size = overrite_size;
        p_ti->memory.size += ALIGN(c_ti->memory.size, p_ti->memory.align);
        return 1;
    }

    return 0;
}

symbol_id_t TPTB_resolve_child(symbol_id_t p_id, string_t* name, typetab_ctx_t* ctx) {
    type_info_t *p_ti, *c_ti;
    if (!map_get(&ctx->typetb, p_id, (void**)&p_ti)) return NO_SYMBOL_ID;
    
    foreach (symbol_id_t c_id, &p_ti->link.c) {
        if (!map_get(&ctx->typetb, c_id, (void**)&c_ti)) continue;
        if (c_ti->link.name->equals(c_ti->link.name, name)) return c_id;
    }

    return NO_SYMBOL_ID;
}

long TPTB_get_child_offset(symbol_id_t p_id, symbol_id_t tc_id, typetab_ctx_t* ctx) {
    type_info_t *p_ti, *c_ti;
    if (!map_get(&ctx->typetb, p_id, (void**)&p_ti)) return -1;

    long offset = 0;
    foreach (symbol_id_t c_id, &p_ti->link.c) {
        if (!map_get(&ctx->typetb, c_id, (void**)&c_ti)) continue;
        if (tc_id == c_id) return offset;
        offset += ALIGN(c_ti->memory.ptr ? CONF_get_full_bytness() : c_ti->memory.size, p_ti->memory.align);
    }

    return -1;
}

int TPTB_get_info_id(symbol_id_t id, type_info_t* info, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (map_get(&ctx->typetb, id, (void**)&ti)) {
        if (info) str_memcpy(info, ti, sizeof(type_info_t));
        return 1;
    }

    return 0;
}

type_type_t TPTB_get_type_type_id(symbol_id_t id, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (map_get(&ctx->typetb, id, (void**)&ti)) return ti->t;
    return 0;
}

int TPTB_get_info(string_t* name, symbol_id_t s_id, type_info_t* info, typetab_ctx_t* ctx) {
    map_foreach (type_info_t* ti, &ctx->typetb) {
        if (name && ti->name && name->equals(name, ti->name) && s_id == ti->s_id) {
            if (info) str_memcpy(info, ti, sizeof(type_info_t));
            return 1;
        }
    }

    return 0;
}

static int _unload_info(type_info_t* info) {
    list_free(&info->link.c);
    if (info->name)      destroy_string(info->name);
    if (info->link.name) destroy_string(info->link.name);
    mm_free(info);
    return 1;
}

int TPTB_unload(typetab_ctx_t* ctx) {
    return map_free_force_op(&ctx->typetb, (int (*)(void*))_unload_info);
}
