#include <hir/hirgens/hirgens.h>

hir_subject_t* _get_size_as_constant(hir_subject_t* s, sym_table_t* smt) {
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

hir_subject_t* HIR_generate_sizeof(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, HIR_SETPOS, HIR_SUBJ_LOCATION(&node->t->finfo));
    hir_block_t* entry  = ctx->hot.t;
    hir_subject_t* src  = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* size = _get_size_as_constant(src, smt);
    hir_block_t* hidden = entry->next;
    
    if (hidden) {
        while (hidden) {
            hir_block_t* next = hidden->next;
            HIR_unload_block(hidden);
            hidden = next;
        }

        entry->next = NULL;
        ctx->hot.t = entry;
    }

    HIR_unload_subject(src);
    return size;
}
