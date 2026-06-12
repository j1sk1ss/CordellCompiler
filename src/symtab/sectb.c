#include <symtab/sectb.h>

static inline set_t* _get_set_target(section_elem_type_t t, section_info_t* section) {
    switch (t) {
        case SECTION_ELEMENT_VARIABLE: return &section->vars;
        case SECTION_ELEMENT_FUNCTION: return &section->func;
        case SECTION_ELEMENT_STRING:   return &section->strs;
        default: return NULL;
    }
}

static inline list_t* _get_list_target(section_elem_type_t t, section_info_t* section) {
    switch (t) {
        case SECTION_ELEMENT_VARIABLE: return &section->sorted.vars;
        case SECTION_ELEMENT_FUNCTION: return &section->sorted.func;
        case SECTION_ELEMENT_STRING:   return &section->sorted.strs;
        default: return NULL;
    }
}

static section_info_t* _create_section(string_t* name, int align) {
    section_info_t* s = (section_info_t*)mm_malloc(sizeof(section_info_t));
    if (!s) return NULL;
    if (name) s->name = name->copy(name);
    set_init(&s->vars, SET_NO_CMP);
    set_init(&s->func, SET_NO_CMP);
    set_init(&s->strs, SET_NO_CMP);
    s->align = align;
    list_init(&s->sorted.vars);
    list_init(&s->sorted.func);
    list_init(&s->sorted.strs);
    return s;
}

static int _unload_secinfo(section_info_t* info) {
    destroy_string(info->name);
    set_free(&info->vars);
    set_free(&info->func);
    set_free(&info->strs);
    list_free(&info->sorted.vars);
    list_free(&info->sorted.func);
    list_free(&info->sorted.strs);
    return mm_free(info);
}

int SCTB_remove_from_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_remove_from_section(section=%s, id=%li, t=%i)", section->body, id, t);
    section_info_t* info;
    if (map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        return set_remove(_get_set_target(t, info), (void*)id) && 
               list_remove(_get_list_target(t, info), (void*)id);
    }

    return 0;
}

int SCTB_add_to_section(string_t* section, int align, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_add_to_section(section=%s, id=%li, t=%i)", section ? section->body : "(null)", id, t);
    long section_address = section ? (long)section->hash : 0;
    
    section_info_t* info;
    if (!map_get(&ctx->sectb, section_address, (void**)&info)) {
        info = _create_section(section, align);
        if (
            !map_put(&ctx->sectb, section_address, info) || 
            !list_add(&ctx->sorted.sectb, info)
        ) {
            _unload_secinfo(info);
            return 0;
        }
    }

    info->align = MAX(info->align, align);
    return set_add(_get_set_target(t, info), (void*)id) && 
           list_add(_get_list_target(t, info), (void*)id);
}

string_t* SCTB_get_section_name(symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    map_foreach (section_info_t* i, &ctx->sectb) {
        if (set_has(_get_set_target(t, i), (void*)id)) {
            return i->name;
        }
    }

    return NULL;
}

int SCTB_move_to_section(string_t* section, int align, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_move_to_section(section=%s, id=%li, t=%i)", section ? section->body : "(null)", id, t);
    string_t* ps = SCTB_get_section_name(id, t, ctx);
    if (ps) SCTB_remove_from_section(ps, id, t, ctx);
    return SCTB_add_to_section(section, align, id, t, ctx);
}

int SCTB_get_section(list_t* out, string_t* section, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_get_section(section=%s)", section->body);
    section_info_t* info;
    if (!map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        return 0;
    }

    set_foreach (symbol_id_t id, _get_set_target(t, info)) {
        list_add(out, (void*)id);
    }

    return 1;
}

int SCTB_get_sections(list_t* out, sectb_ctx_t* ctx) {
    print_log("SCTB_get_sections()");
    map_foreach (section_info_t* i, &ctx->sectb) {
        list_add(out, i->name);
    }

    return 1;
}

int SCTB_unload(sectb_ctx_t* ctx) {
    list_free(&ctx->sorted.sectb);
    return map_free_force_op(&ctx->sectb, (int (*)(void*))_unload_secinfo);
}
