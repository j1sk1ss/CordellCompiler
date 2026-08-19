#include <hir/hirgens/hirgens.h>

/* Extract the final element size from the pointed data.
Params:
    - `index_type` - Type id of the whole indexation expression.
    - `base` - The subject which represents an array or a pointer.
    - `smt` - Symtable.

Returns the element size of the pointed data in bytes. */
static int _get_pointed_element_size(symbol_id_t index_type, hir_subject_t* base, sym_table_t* smt) {
    long type_size = TPTB_get_memory_size_id(index_type, &smt->t);
    if (type_size != SMT_NULL) return type_size;

    array_info_t ai;
    int el_size = (base->ptr - 1) > 0 ? CONF_get_full_bytness() : HIR_get_type_size(base->t);
    if (
        HIR_is_arrtype(base->t) &&
        ARTB_get_info(base->storage.var.v_id, &ai, &smt->a) && 
        !ai.vla
    ) {
        token_t tmp = { .t_type = ai.elements_info.el_type };
        el_size = ai.elements_info.el_flags.ptr ? CONF_get_full_bytness() : HIR_get_type_size(HIR_get_tmptype_tkn(&tmp, 0));
    }

    variable_info_t vi;
    if (
        HIR_is_arrtype(base->t) &&
        VRTB_get_info_id(base->storage.var.v_id, &vi, &smt->v) &&
        vi.t_id != NO_SYMBOL_ID
    ) {
        symbol_id_t elem_type = TPTB_get_first_child(vi.t_id, &smt->t);
        long type_size = TPTB_get_memory_size_id(elem_type, &smt->t);
        if (type_size != SMT_NULL) el_size = type_size;
    }

    return el_size;
}

/* Setup all essential instructions and return a head to the input data.
Params:
    - `node` - Indexation AST node.
    - `smt` - Symtable.
    - `indexed_type` - Output final dereferenced type (The type of the element).

Returns the subject which represents a pointer to a head of the data 
(In correct reference level and type). */
static hir_subject_t* _get_final_head(
    ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, hir_subject_type_t* indexed_type
) {
    ast_node_t* base_node = node->c;
    ast_node_t* offt_node = node->c->siblings.n;
    hir_subject_t* base = HIR_generate_elem(base_node, ctx, smt);
    hir_subject_t* offt = HIR_generate_elem(offt_node, ctx, smt);
    hir_subject_t* head = HIR_reference_subject(base, smt, 0);
    hir_operation_t base_op = HIR_STORE;
    hir_subject_type_t _dummy;
    if (!indexed_type) indexed_type = &_dummy;

    array_info_t ai;
    *indexed_type = base->t;
    if (
        HIR_is_arrtype(base->t) &&                              /* If this is an array type                    */
        ARTB_get_info(base->storage.var.v_id, &ai, &smt->a) &&  /* and it is registered in smt as an array     */
        !ai.vla                                                 /* and this array isn't a vla array           */
    ) {                                                         /* We 'lea' the base to a referenced variable  */
        if (!base->ptr) {
            head->ptr = MAX(ai.elements_info.el_flags.ptr + 1, head->ptr + 1);
            base_op   = HIR_REF;
        }
        
        token_t tmp = { .t_type = ai.elements_info.el_type };
        *indexed_type = HIR_get_tmptype_tkn(&tmp, 0);
    }

    HIR_BLOCK2(ctx, base_op, head, base);

    /* The final offset for the base address is the result of the
        expression 'real_offset = offset * element_size' */
    hir_subject_t* real_offset = HIR_SUBJ_TMPVAR(
        HIR_TMPVARU64, 
        VRTB_add_info(NULL, HIR_get_tmptkn_type(HIR_promote_types(offt->t, HIR_I8CONSTVAL)), NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v)
    );
    real_offset->ptr = offt->ptr;

    HIR_BLOCK3(
        ctx, HIR_iMUL, real_offset, 
        HIR_generate_implconv(ctx, real_offset->ptr, real_offset->t, offt, smt), 
        HIR_SUBJ_CONST(_get_pointed_element_size(node->sinfo.t_id, base, smt))
    );

    /* No we move the address (base) by the offser (addr):
        - final_head = head + real_offset */
    hir_subject_t* final_head = HIR_SUBJ_CPVAR(head, smt);
    final_head->ptr = head->ptr;
    HIR_BLOCK3(ctx, HIR_iADD, final_head, head, HIR_generate_implconv(ctx, head->ptr, head->t, real_offset, smt));
    return final_head;
} 

hir_subject_t* HIR_generate_load_indexation(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_type_t indexed_type;
    hir_subject_t* final_head = _get_final_head(node, ctx, smt, &indexed_type);

    /* There is no need here to dereference an stack element,
       that's why if this is an array or a container, return it without
       dereference operation */
    type_info_t p_ti, c_ti;
    if (
        TPTB_get_info_id(node->c->sinfo.t_id, &p_ti, &smt->t) && p_ti.t == TYPE_ARRAY &&
        TPTB_get_info_id(TPTB_get_first_child(node->c->sinfo.t_id, &smt->t), &c_ti, &smt->t) &&
        (c_ti.t == TYPE_ARRAY || c_ti.t == TYPE_CUSTOM) && !c_ti.memory.ptr
    ) return final_head;

    hir_subject_t* res = HIR_SUBJ_TMPVAR(indexed_type, VRTB_add_info(NULL, HIR_get_tmptkn_type(indexed_type), NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v));
    res->ptr = MAX(final_head->ptr - 1, 0);
    
    HIR_BLOCK2(ctx, HIR_GDREF, res, final_head);
    return res;
}

hir_subject_t* HIR_generate_ref_indexation(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    return _get_final_head(node, ctx, smt, NULL);
}

int HIR_generate_store_indexation(ast_node_t* node, hir_subject_t* data, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_type_t indexed_type;
    hir_subject_t* final_head = _get_final_head(node, ctx, smt, &indexed_type);
    HIR_BLOCK2(ctx, HIR_LDREF, final_head, HIR_generate_implconv(ctx, final_head->ptr - 1, indexed_type, data, smt));
    return 1;
}
