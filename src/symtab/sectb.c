#include <symtab/sectb.h>

static section_info_t* _create_section(string_t* name) {
    section_info_t* s = (section_info_t*)mm_malloc(sizeof(section_info_t));
    if (!s) return NULL;
    s->name = name->copy(name);
    set_init(&s->vars, SET_NO_CMP);
    set_init(&s->func, SET_NO_CMP);
    return s;
}

static int _unload_secinfo(section_info_t* info) {
    destroy_string(info->name);
    set_free(&info->vars);
    set_free(&info->func);
    return mm_free(info);
}

string_t* SCTB_get_section_id(symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    map_foreach (section_info_t* i, &ctx->sectb) {
        if (set_has(t == SECTION_ELEMENT_VARIABLE ? &i->vars : &i->func, (void*)id)) {
            return i->name;
        }
    }

    return NULL;
}

int SCTB_remove_from_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_remove_from_section(section=%s, id=%li, t=%i)", section->body, id, t);
    section_info_t* info;
    if (map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        return set_remove(t == SECTION_ELEMENT_VARIABLE ? &info->vars : &info->func, (void*)id);
    }

    return 0;
}

int SCTB_add_to_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_add_to_section(section=%s, id=%li, t=%i)", section->body, id, t);
    section_info_t* info;
    if (!map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        info = _create_section(section);
        if (!map_put(&ctx->sectb, (long)section->hash, info)) {
            _unload_secinfo(info);
            return 0;
        }
    }

    return set_add(t == SECTION_ELEMENT_VARIABLE ? &info->vars : &info->func, (void*)id);
}

int SCTB_move_to_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_move_to_section(section=%s, id=%li, t=%i)", section->body, id, t);
    string_t* ps = SCTB_get_section_id(id, t, ctx);
    if (ps) SCTB_remove_from_section(ps, id, t, ctx);
    return SCTB_add_to_section(section, id, t, ctx);
}

int SCTB_get_section(list_t* out, string_t* section, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_get_section(section=%s)", section->body);
    section_info_t* info;
    if (!map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        return 0;
    }

    set_foreach (symbol_id_t id, t == SECTION_ELEMENT_VARIABLE ? &info->vars : &info->func) {
        list_add(out, id);
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
    return map_free_force_op(&ctx->sectb, (int (*)(void*))_unload_secinfo);
}
