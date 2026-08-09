#include <hir/hirgens/hirgens.h>

hir_subject_t* _get_size_as_constant(hir_subject_t* s, sym_table_t* smt) {
    int size = s->ptr > 0 ? CONF_get_full_bytness() : HIR_get_type_size(s->t);
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        int has_vi = VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v);
        if (HIR_is_arrtype(s->t)) {
            array_info_t ai;
            if (ARTB_get_info(s->storage.var.v_id, &ai, &smt->a)) {
                long type_size = has_vi ? TPTB_get_memory_size_id(vi.t_id, &smt->t) : FIELD_NO_CHANGE;
                if (type_size == FIELD_NO_CHANGE && has_vi) {
                    symbol_id_t elem_type = TPTB_get_first_child(vi.t_id, &smt->t);
                    type_size = TPTB_get_memory_size_id(elem_type, &smt->t);
                    if (type_size != FIELD_NO_CHANGE) type_size *= ai.size;
                }

                if (type_size != FIELD_NO_CHANGE) size = type_size;
            }
        }

        if (
            has_vi && 
            vi.vmi.align > CONF_get_full_bytness()
        ) size = ALIGN(size, vi.vmi.align);
    }

    return HIR_SUBJ_CONST(size);
}

hir_subject_t* HIR_generate_sizeof(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    if (
        TKN_is_builtin_type(node->c->t) ||
        node->c->sinfo.t_id != NO_SYMBOL_ID
    ) {
        if (node->c->sinfo.t_id != NO_SYMBOL_ID) {
            type_info_t ti;
            if (TPTB_get_info_id(node->c->sinfo.t_id, &ti, &smt->t)) {
                return HIR_SUBJ_CONST(ti.memory.size);
            }
        }

        return HIR_SUBJ_CONST(HIR_get_type_size(HIR_get_tmptype_tkn(node->c->t, 1)));
    }

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
