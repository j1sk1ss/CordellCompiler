#include <symtab/typetb.h>

static type_entry_info_t* _create_entry(symbol_id_t lv) {
    type_entry_info_t* entry = (type_entry_info_t*)mm_malloc(sizeof(type_entry_info_t));
    if (!entry) return NULL;
    entry->vid = lv;
    return entry;
}

static type_info_t* _create_type_info(string_t* name) {
    type_info_t* info = (type_info_t*)mm_malloc(sizeof(type_info_t));
    if (!info) return NULL;
    list_init(&info->entries);
    list_init(&info->link.c);
    info->link.name = NULL;
    info->link.p = NO_SYMBOL_ID;
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

symbol_id_t TPTB_add_copy(symbol_id_t id, token_t* t, typetab_ctx_t* ctx) {
    type_info_t* ti;
    if (!map_get(&ctx->typetb, id, (void**)&ti)) return NO_SYMBOL_ID;

    type_info_t* info = _create_type_info(ti->name);
    if (!info) return NO_SYMBOL_ID;

    info->t            = ti->t;
    info->memory.size  = ti->memory.size;
    info->link.p       = ti->link.p;
    info->memory.align = ti->memory.align;
    if (ti->link.name) info->link.name = ti->link.name->copy(ti->link.name);
    foreach (symbol_id_t c_id, &ti->link.c) {
        list_add(&info->link.c, (void*)c_id);
    }

    info->memory.tt  = t->t_type;
    info->memory.ptr = t->flags.ptr;
    info->id         = ctx->curr_id++;
    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

symbol_id_t TPTB_add_info_from_token(symbol_id_t s_id, token_t* t, symbol_id_t v_id, typetab_ctx_t* ctx) {
    type_info_t* info = _create_type_info(NULL);
    if (!info) return NO_SYMBOL_ID;

    info->link.v_id  = v_id;
    info->id         = ctx->curr_id++;
    info->s_id       = s_id;
    info->memory.tt  = t->t_type;
    info->memory.ptr = t->flags.ptr;

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
    else info->t = TYPE_PRIMITIVE;

    if (info->t == TYPE_PRIMITIVE) {
        switch (TKN_variable_bitness(t, 1)) {
            case TYPE_FULL_SIZE:    info->memory.size = CONF_get_full_bytness();  break;
            case TYPE_HALF_SIZE:    info->memory.size = CONF_get_half_bytness();  break;
            case TYPE_QUARTER_SIZE: info->memory.size = CONF_get_quart_bytness(); break;
            default:                info->memory.size = CONF_get_eight_bytness(); break;
        }
    }

    map_put(&ctx->typetb, info->id, info);
    return info->id;
}

int TPTB_add_as_child(symbol_id_t p_id, symbol_id_t c_id, string_t* name, long size, typetab_ctx_t* ctx) {
    type_info_t *p_ti, *c_ti;
    if (
        map_get(&ctx->typetb, p_id, (void**)&p_ti) &&
        map_get(&ctx->typetb, c_id, (void**)&c_ti)
    ) {
        list_add(&p_ti->link.c, (void*)c_id);
        c_ti->link.p = p_id;
        if (name) {
            if (c_ti->link.name) destroy_string(c_ti->link.name);
            c_ti->link.name = name->copy(name);
        }

        if (size == FIELD_NO_CHANGE) p_ti->memory.size += ALIGN(c_ti->memory.size, p_ti->memory.align);
        else                         p_ti->memory.size += ALIGN(size, p_ti->memory.align);
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

int TPTB_info_add_entry(symbol_id_t id, symbol_id_t vid, typetab_ctx_t* ctx) {
    if (id == NO_SYMBOL_ID) return 1;
    type_info_t* info;
    if (!map_get(&ctx->typetb, id, (void**)&info)) return 0;

    type_entry_info_t* entry = _create_entry(vid);
    if (!entry) return 0;

    list_add(&info->entries, entry);
    return 1;
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
    list_free_force(&info->entries);
    list_free(&info->link.c);
    if (info->name)      destroy_string(info->name);
    if (info->link.name) destroy_string(info->link.name);
    mm_free(info);
    return 1;
}

int TPTB_unload(typetab_ctx_t* ctx) {
    return map_free_force_op(&ctx->typetb, (int (*)(void*))_unload_info);
}