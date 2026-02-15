#include <hir/hirgens/hirgens.h>

static hir_subject_t* _get_final_head(
    hir_subject_t* base, hir_subject_t* offt, hir_ctx_t* ctx, sym_table_t* smt, hir_subject_type_t* indexed_type
) {
    hir_subject_t* head = HIR_reference_subject(base, smt, 0);
    hir_operation_t base_op = HIR_STORE;

    array_info_t ai;
    *indexed_type = base->t;
    if (
        HIR_is_arrtype(base->t) &&                              /* If this is an array type                    */
        ARTB_get_info(base->storage.var.v_id, &ai, &smt->a) &&  /* and it is registered in smt as an array     */
        !ai.heap                                                /* and this array isn't a heap array           */
    ) {                                                         /* We 'lea' the base to a referenced variable  */
        if (!base->ptr) {
            head->ptr++;
            base_op = HIR_REF;
        }
        
        token_t tmp = { .t_type = ai.elements_info.el_type };
        *indexed_type = HIR_get_tmptype_tkn(&tmp, 0);
    }

    HIR_BLOCK2(ctx, base_op, head, base);

    /* The final offset for the base address is the result of the
        expression 'real_offset = offset * element_size' */
    hir_subject_t* real_offset = HIR_SUBJ_TMPVAR(offt->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(offt->t), 0, NULL, &smt->v));
    HIR_BLOCK3(
        ctx, HIR_iMUL, real_offset, offt, 
        HIR_generate_implconv(ctx, offt->ptr, offt->t, HIR_SUBJ_CONST(HIR_get_type_size(*indexed_type)), smt)
    );

    /* No we move the address (base) by the offser (addr):
        - final_head = head + real_offset */
    hir_subject_t* final_head = HIR_SUBJ_CPVAR(head, smt);
    final_head->ptr = head->ptr;
    HIR_BLOCK3(ctx, HIR_iADD, final_head, head, HIR_generate_implconv(ctx, head->ptr, head->t, real_offset, smt));
    return final_head;
} 

hir_subject_t* HIR_generate_load_indexation(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* base = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* offt = HIR_generate_elem(node->c->siblings.n, ctx, smt);
    
    hir_subject_type_t indexed_type;
    hir_subject_t* final_head = _get_final_head(base, offt, ctx, smt, &indexed_type);
    
    hir_subject_t* res = HIR_SUBJ_TMPVAR(indexed_type, VRTB_add_info(NULL, HIR_get_tmptkn_type(indexed_type), 0, NULL, &smt->v));;
    res->ptr = MAX(final_head->ptr - 1, 0);
    
    HIR_BLOCK2(ctx, HIR_GDREF, res, final_head);
    return res;
}

int HIR_generate_store_indexation(ast_node_t* node, hir_subject_t* data, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* base = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* offt = HIR_generate_elem(node->c->siblings.n, ctx, smt);
    
    hir_subject_type_t indexed_type;
    hir_subject_t* final_head = _get_final_head(base, offt, ctx, smt, &indexed_type);

    HIR_BLOCK2(ctx, HIR_LDREF, final_head, HIR_generate_implconv(ctx, final_head->ptr - 1, indexed_type, data, smt));
    return 1;
}
