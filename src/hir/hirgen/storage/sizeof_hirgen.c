#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_sizeof(hir_subject_t* s, sym_table_t* smt) {
    int size = s->ptr > 0 ? CONF_get_full_bytness() : HIR_get_type_size(s->t);
    if (HIR_is_vartype(s->t)) {
        if (HIR_is_arrtype(s->t)) {
            array_info_t ai;
            if (ARTB_get_info(s->storage.var.v_id, &ai, &smt->a)) {
                token_t tmp = { .t_type = ai.elements_info.el_type, .flags.ptr = ai.elements_info.el_flags.ptr };
                size = ai.size * HIR_get_type_size(HIR_get_tmptype_tkn(&tmp, 1));
            }
        }

        variable_info_t vi;
        if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) && vi.vmi.align > CONF_get_full_bytness()) {
            size = ALIGN(size, vi.vmi.align);
        }
    }

    return HIR_SUBJ_CONST(size);
}
