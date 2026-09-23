#include <hir/hirgens/hirgens.h>

static symbol_id_t _intern_vtable(type_info_t* ti, sym_table_t* smt) {
    long slots = 0, fallback_index = 0;
    foreach (symbol_id_t c, &ti->body.custom.layout.children) {
        type_info_t c_ti;
        if (
            TPTB_get_info_id(c, &c_ti, &smt->t) && c_ti.t == TYPE_METHOD && 
            c_ti.body.method.in_vtable
        ) {
            long vtable_index = c_ti.body.method.vtable_index;
            if (vtable_index == SMT_NULL) vtable_index = fallback_index;
            if (vtable_index >= slots) slots = vtable_index + 1;
            fallback_index++;
        }
    }

    if (!slots) return NO_SYMBOL_ID;

    vtable_info_t vti;
    if (VTTB_get_info_type(ti->id, &vti, &smt->vt)) return vti.id;

    symbol_id_t vt_id = VTTB_add_info(ti->id, &smt->vt);
    if (vt_id == NO_SYMBOL_ID) return NO_SYMBOL_ID;
    for (long slot = 0; slot < slots; slot++) {
        symbol_id_t f_id = NO_SYMBOL_ID;
        fallback_index = 0;
        foreach (symbol_id_t c, &ti->body.custom.layout.children) {
            type_info_t c_ti;
            func_info_t c_fi;
            if (
                !TPTB_get_info_id(c, &c_ti, &smt->t) ||
                c_ti.t != TYPE_METHOD                ||
                !c_ti.body.method.in_vtable
            ) continue;

            long vtable_index = c_ti.body.method.vtable_index;
            if (vtable_index == SMT_NULL) vtable_index = fallback_index;
            fallback_index++;
            if (vtable_index != slot) continue;
            if (!FNTB_get_info_id(c_ti.body.method.f_id, &c_fi, &smt->f) || c_fi.flags.abstract) break;

            f_id = c_fi.id;
            break;
        }

        VTTB_add_func(vt_id, f_id, &smt->vt);
    }

    string_t* section = create_string(CONF_get_ro_section());
    SCTB_add_to_section(section, CONF_get_full_bytness(), vt_id, SECTION_ELEMENT_VTABLE, &smt->c);
    destroy_string(section);
    return vt_id;
}

hir_subject_t* HIR_load_vtable(type_info_t* ti, hir_ctx_t* ctx, variable_info_t* vi, sym_table_t* smt) {
    symbol_id_t vt_id = _intern_vtable(ti, smt);
    if (vt_id == NO_SYMBOL_ID) return NULL;
    if (vi && vi->vfs.glob) {
        ARTB_add_elems(vi->v_id, (array_elem_info_t){ .s.vt_id = vt_id, .t = ARRAY_ELEM_VTABLE_TYPE }, &smt->a);
        return NULL;
    }

    hir_subject_t* vtable_init = HIR_SUBJ_TMPVAR(HIR_STKVARI0, VRTB_add_info(NULL, TMP_I0_TYPE_TOKEN, NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v));
    vtable_init->ptr = 1;
    HIR_BLOCK2(ctx, HIR_REF, vtable_init, HIR_SUBJ_VTABLE(vt_id));
    return vtable_init;
}