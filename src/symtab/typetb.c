#include <symtab/typetb.h>

static type_info_t* _create_type_info(string_t* name) {
    type_info_t* info = (type_info_t*)mm_malloc(sizeof(type_info_t));
    if (!info) return NULL;
    list_init(&info->link.c);

    info->p         = NO_SYMBOL_ID;
    info->s_id      = NO_SYMBOL_ID;
    info->cs_id     = NO_SYMBOL_ID;
    info->link.name = NULL;
    info->link.p    = NO_SYMBOL_ID;
    info->link.v_id = NO_SYMBOL_ID;
    info->tt        = UNKNOWN_STRING_TOKEN;

    if (name) info->name = name->copy(name);
    else info->name = NULL;
    
    str_memset(&info->memory, 0, sizeof(info->memory));
    info->memory.align = CONF_get_eight_bytness();
    return info;
}

symbol_id_t TPTB_add_info(string_t* name, symbol_id_t s_id, type_type_t t, int align, int multiple, typetab_ctx_t* ctx) {
    if (TPTB_get_info(name, s_id, 0, NULL, ctx)) return NO_SYMBOL_ID;
    type_info_t* info = _create_type_info(name);
    if (!info) return NO_SYMBOL_ID;
    info->id   = ctx->curr_id++;
    info->s_id = s_id;
    info->t    = t;

    switch (t) {
        case TYPE_GENERICS:  info->tt = GENERIC_TYPE_TOKEN;   break;
        case TYPE_CUSTOM:    info->tt = CUSTOM_TYPE_TOKEN;    break;
        case TYPE_METHOD:    info->tt = FUNC_TOKEN;           break;
        case TYPE_ARRAY:     info->tt = ARRAY_TYPE_TOKEN;     break;
        case TYPE_PRIMITIVE:
        default:             info->tt = UNKNOWN_STRING_TOKEN; break;
    }

    info->memory.align    = align;
    info->memory.multiple = multiple;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

symbol_id_t TPTB_add_copy(symbol_id_t id, symbol_id_t nv_id, int ptr, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return NO_SYMBOL_ID;
    if (ti->t == TYPE_GENERICS) return id;

    type_info_t* info = _create_type_info(ti->name);
    if (!info) return NO_SYMBOL_ID;

    info->p               = id;
    info->s_id            = ti->s_id;
    info->t               = ti->t;
    info->tt              = ti->tt;
    info->memory.size     = ti->memory.size;
    info->memory.ptr      = ptr;
    info->memory.multiple = ti->memory.multiple;
    info->link.p          = ti->link.p;
    info->link.v_id       = nv_id;
    info->memory.align    = ti->memory.align;

    info->id = ctx->curr_id++;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

static inline symbol_id_t _get_type_by_token(token_t* t, typetab_ctx_t* ctx) {
    if (!t) return NO_SYMBOL_ID;
    if (t->t_type == ARRAY_TYPE_TOKEN) return NO_SYMBOL_ID;
    map_foreach (type_info_t* ti, &ctx->typetb) {
        if (ti->tt != t->t_type || ti->memory.ptr != t->flags.ptr)     continue;
        if (ti->link.v_id != NO_SYMBOL_ID || ti->link.name)            continue;
        if (!t->body && !ti->name)                                     return ti->id;
        if (t->body && ti->name && t->body->equals(t->body, ti->name)) return ti->id;
    }

    return NO_SYMBOL_ID;
}

symbol_id_t TPTB_add_info_from_token(symbol_id_t s_id, token_t* t, symbol_id_t v_id, typetab_ctx_t* ctx) {
    symbol_id_t existed = _get_type_by_token(t, ctx);
    if (existed != NO_SYMBOL_ID) return existed;
    
    type_info_t* info = _create_type_info(t->body);
    if (!info) return NO_SYMBOL_ID;

    info->link.v_id  = v_id;
    info->id         = ctx->curr_id++;
    info->s_id       = s_id;
    info->memory.ptr = t->flags.ptr;
    info->tt         = t->t_type;

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

    info->memory.size = 0;
    if (info->t == TYPE_PRIMITIVE) {
        info->memory.size = TKN_convert_type_size(TKN_variable_bitness(t, 0));
    }

    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

long TPTB_get_memory_size_id(symbol_id_t id, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return SMT_NULL;
    return ti->memory.ptr ? CONF_get_full_bytness() : ti->memory.size;
}

int TPTB_set_memory_size_id(symbol_id_t id, long size, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return 0;
    ti->memory.size = size;
    return 1;
}

int TPTB_set_child_scope_id(symbol_id_t id, symbol_id_t cs_id, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return 0;
    ti->cs_id = cs_id;
    return 1;
}

symbol_id_t TPTB_resolve_parent(symbol_id_t c, typetab_ctx_t* ctx) {
    type_info_t* c_ti;
    while (
        map_get(&ctx->typetb, c, (void**)&c_ti) &&
        c_ti->p != NO_SYMBOL_ID
    ) c = c_ti->p;
    return c;
}

int TPTB_link_child(symbol_id_t p_id, symbol_id_t c_id, typetab_ctx_t* ctx) {
    p_id = TPTB_resolve_parent(p_id, ctx);

    type_info_t *p_ti, *c_ti;
    if (
        p_id != c_id &&
        map_get(&ctx->typetb, p_id, (void**)&p_ti) &&
        map_get(&ctx->typetb, c_id, (void**)&c_ti)
    ) {
        list_add(&p_ti->link.c, (void*)c_id);
        if (p_ti->cs_id == NO_SYMBOL_ID) p_ti->cs_id = c_ti->s_id;
        return 1;
    }

    return 0;
}

symbol_id_t TPTB_get_first_child(symbol_id_t p_id, typetab_ctx_t* ctx) {
    p_id = TPTB_resolve_parent(p_id, ctx);
    type_info_t* p_ti;
    if (!map_get(&ctx->typetb, p_id, (void**)&p_ti)) return NO_SYMBOL_ID;
    if (!p_ti->link.c.s) return NO_SYMBOL_ID;
    return (symbol_id_t)list_get_head(&p_ti->link.c);
}

symbol_id_t TPTB_get_indexed_type(symbol_id_t id, typetab_ctx_t* ctx) {
    type_info_t ti;
    if (!TPTB_get_info_id(id, &ti, ctx)) return NO_SYMBOL_ID;
    if (ti.memory.ptr) return TPTB_add_copy(id, NO_SYMBOL_ID, ti.memory.ptr - 1, ctx);
    if (ti.t == TYPE_ARRAY) return TPTB_get_first_child(id, ctx);
    return NO_SYMBOL_ID;
}

int TPTB_add_as_child(symbol_id_t p_id, symbol_id_t c_id, string_t* name, long overrite_size, typetab_ctx_t* ctx) {
    if (p_id == NO_SYMBOL_ID) return 0;
    p_id = TPTB_resolve_parent(p_id, ctx);

    type_info_t *p_ti, *c_ti;
    if (
        p_id != c_id &&
        map_get(&ctx->typetb, p_id, (void**)&p_ti) &&
        map_get(&ctx->typetb, c_id, (void**)&c_ti)
    ) {
        list_add(&p_ti->link.c, (void*)c_id);
        c_ti->link.p = p_id;
        if (name) {
            if (c_ti->link.name) destroy_string(c_ti->link.name);
            c_ti->link.name = name->copy(name);
        }
        
        long field_size = c_ti->memory.size;
        if (overrite_size != FIELD_NO_CHANGE) {
            field_size = overrite_size;
            if (!c_ti->memory.ptr) c_ti->memory.size = overrite_size;
        }

        if (!p_ti->memory.multiple)        p_ti->memory.size = MAX(p_ti->memory.size, ALIGN(field_size, p_ti->memory.align));
        else if (p_ti->memory.align != -1) p_ti->memory.size += ALIGN(field_size, p_ti->memory.align);
        else                               p_ti->memory.size += ALIGN(field_size, field_size);
        // if (p_ti->memory.align == -1)      p_ti->memory.size = ALIGN(p_ti->memory.size, c_ti->memory.size);
        return 1;
    }

    return 0;
}

symbol_id_t TPTB_resolve_child(symbol_id_t p_id, string_t* name, typetab_ctx_t* ctx) {
    p_id = TPTB_resolve_parent(p_id, ctx);

    type_info_t *p_ti, *c_ti;
    if (!map_get(&ctx->typetb, p_id, (void**)&p_ti)) return NO_SYMBOL_ID;
    
    foreach (symbol_id_t c_id, &p_ti->link.c) {
        if (!map_get(&ctx->typetb, c_id, (void**)&c_ti)) continue;
        if (!c_ti->link.name) continue;
        if (c_ti->link.name->equals(c_ti->link.name, name)) return c_id;
    }

    return NO_SYMBOL_ID;
}

long TPTB_get_child_offset(symbol_id_t p_id, symbol_id_t tc_id, typetab_ctx_t* ctx) {
    p_id = TPTB_resolve_parent(p_id, ctx);

    type_info_t *p_ti, *c_ti;
    if (!map_get(&ctx->typetb, p_id, (void**)&p_ti)) return -1;
    if (!p_ti->memory.multiple) return 0;

    long offset = 0;
    foreach (symbol_id_t c_id, &p_ti->link.c) {
        if (!map_get(&ctx->typetb, c_id, (void**)&c_ti)) continue;
        long size = c_ti->memory.ptr ? CONF_get_full_bytness() : c_ti->memory.size;
        if (p_ti->memory.align == -1) {
            offset = ALIGN(offset, size);
            if (tc_id == c_id) return offset;
            offset += size;
        }
        else {
            if (tc_id == c_id) return offset;
            offset += ALIGN(size, p_ti->memory.align);
        }
    }

    return -1;
}

int TPTB_find_type_init_slot(symbol_id_t t_id, long target_slot, long base_offset, type_init_info_t* slot_info, typetab_ctx_t* ctx) {
    type_info_t ti;
    if (!TPTB_get_info_id(t_id, &ti, ctx)) return 0;

    long type_size = TPTB_get_memory_size_id(t_id, ctx);
    if (ti.memory.ptr || ti.t == TYPE_PRIMITIVE) {
        if (slot_info->curr_idx++ != target_slot) return 0;
        slot_info->slot_off   = base_offset;
        slot_info->slot_size  = type_size;
        slot_info->slot_type  = t_id;
        slot_info->slot_owner = t_id;
        return type_size > 0;
    }

    symbol_id_t scan_id = ti.p != NO_SYMBOL_ID ? TPTB_resolve_parent(t_id, ctx) : t_id;
    type_info_t scan_ti;
    if (!TPTB_get_info_id(scan_id, &scan_ti, ctx)) return 0;

    foreach (symbol_id_t child_id, &scan_ti.link.c) {
        long child_offset = TPTB_get_child_offset(scan_id, child_id, ctx);
        long child_size   = TPTB_get_memory_size_id(child_id, ctx);
        if (child_offset < 0 || child_size <= 0) continue;
        long repeats = scan_ti.t == TYPE_ARRAY ? type_size / child_size : 1;
        for (long repeat = 0; repeat < repeats; repeat++) {
            long nested_base = base_offset + child_offset + repeat * child_size;
            if (TPTB_find_type_init_slot(child_id, target_slot, nested_base, slot_info, ctx)) {
                if (scan_ti.t == TYPE_ARRAY) slot_info->slot_owner = scan_id;
                return 1;
            }
        }
    }

    return 0;
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

token_type_t TPTB_get_token_type_id(symbol_id_t id, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (map_get(&ctx->typetb, id, (void**)&ti)) return ti->tt;
    return CUSTOM_TYPE_TOKEN;
}

int TPTB_get_info(string_t* name, symbol_id_t s_id, int ptr, type_info_t* info, typetab_ctx_t* ctx) {
    map_foreach (type_info_t* ti, &ctx->typetb) {
        if (ti->link.v_id != NO_SYMBOL_ID || ti->link.name) continue;
        if (ti->memory.ptr != ptr) continue;
        if (
            name && ti->name && name->equals(name, ti->name) &&
            (s_id == ti->s_id || ti->s_id == NO_SYMBOL_ID)
        ) {
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
