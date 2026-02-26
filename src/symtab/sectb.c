#include <symtab/sectb.h>

static section_info_t* _create_section(string_t* name) {
    section_info_t* s = (section_info_t*)mm_malloc(sizeof(section_info_t));
    if (!s) return NULL;
    s->name = name->copy(name);
    list_init(&s->elems);
    return s;
}

static section_elem_t* _create_element(symbol_id_t id, section_elem_type_t t) {
    section_elem_t* el = (section_elem_t*)mm_malloc(sizeof(section_elem_t));
    if (!el) return NULL;
    el->id = id;
    el->t  = t;
    return el;
}

static int _unload_secinfo(section_info_t* info) {
    destroy_string(info->name);
    list_free_force(&info->elems);
    return mm_free(info);
}

int SCTB_add_to_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx) {
    print_log("SCTB_add_to_section(section=%s, id=%li, t=%i)", section->body, id, t);
    section_elem_t* el = _create_element(id, t);
    if (!el) return 0;

    section_info_t* info;
    if (!map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        info = _create_section(section);
        if (!map_put(&ctx->sectb, (long)section->hash, info)) {
            _unload_secinfo(info);
            return 0;
        }
    }

    return list_add(&info->elems, el);
}

int SCTB_get_section(list_t* out, string_t* section, sectb_ctx_t* ctx) {
    print_log("SCTB_get_section(section=%s)", section->body);
    section_info_t* info;
    if (!map_get(&ctx->sectb, (long)section->hash, (void**)&info)) {
        return 0;
    }

    foreach (section_elem_t* el, &info->elems) {
        list_add(out, el);
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
    map_free_force_op(&ctx->sectb, (int (*)(void*))_unload_secinfo);
    return 1;
}
