#include <hir/hirgens/hirgens.h>

static hir_subject_t* _point_to_field(ast_node_t* root, hir_ctx_t* ctx, type_info_t* field_info, sym_table_t* smt) {
    hir_subject_t* base = NULL;
    if (
        !root->c || !root->c->t || 
        root->c->t->t_type != MEMBER_ACCESS_TOKEN
    ) base = HIR_generate_elem(root->c, ctx, smt);
    else {
        type_info_t parent_field;
        base = _point_to_field(root->c, ctx, &parent_field, smt);
        variable_info_t parent_var;
        if (
            parent_field.t != TYPE_ARRAY &&
            VRTB_get_info_id(parent_field.link.v_id, &parent_var, &smt->v) &&
            parent_var.vfs.ptr
        ) {
            token_t tmp = { .t_type = parent_var.type, .flags.ptr = parent_var.vfs.ptr };
            hir_subject_t* value = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, NULL, &smt->v));
            value->ptr = tmp.flags.ptr;
            HIR_BLOCK2(ctx, HIR_GDREF, value, base);
            base = value;
        }
    }

    if (!base->ptr) {
        hir_subject_t* ref_base = HIR_reference_subject(base, smt, 1);
        HIR_BLOCK2(ctx, HIR_REF, ref_base, base);
        base = ref_base;
    }

    long offset = TPTB_get_child_offset(root->c->sinfo.t_id, root->sinfo.t_id, &smt->t);
    hir_subject_t* real_offset = HIR_SUBJ_TMPVAR(HIR_STKVARU8, VRTB_add_info(NULL, TMP_U8_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v));
    real_offset->ptr = base->ptr;

    TPTB_get_info_id(root->sinfo.t_id, field_info, &smt->t);
    HIR_BLOCK3(ctx, HIR_iADD, real_offset, base, HIR_SUBJ_CONST(offset));
    return real_offset;
}

static hir_subject_t* _load_array_field_head(hir_subject_t* head, array_info_t* ai, hir_ctx_t* ctx, sym_table_t* smt) {
    token_t tmp = { .t_type = ai->elements_info.el_type };
    token_flags_t flags = ai->elements_info.el_flags;
    flags.ptr++;

    hir_subject_t* value = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, &flags, &smt->v));
    value->ptr = flags.ptr;

    HIR_BLOCK2(ctx, HIR_STORE, value, head);
    return value;
}

hir_subject_t* HIR_generate_load_member_access(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    type_info_t ti;
    hir_subject_t* head = _point_to_field(node, ctx, &ti, smt);

    array_info_t ai;
    if (ti.t == TYPE_ARRAY && ARTB_get_info(ti.link.v_id, &ai, &smt->a)) {
        return _load_array_field_head(head, &ai, ctx, smt);
    }

    variable_info_t vi;
    VRTB_get_info_id(ti.link.v_id, &vi, &smt->v);
    token_t tmp = { .t_type = vi.type, .flags.ptr = vi.vfs.ptr };

    hir_subject_t* value = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, NULL, &smt->v));
    value->ptr = tmp.flags.ptr;

    HIR_BLOCK2(ctx, HIR_GDREF, value, head);
    return value;
}

hir_subject_t* HIR_generate_ref_member_access(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    type_info_t ti;
    return _point_to_field(node, ctx, &ti, smt);
}

int HIR_generate_store_member_access(ast_node_t* node, hir_subject_t* data, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    type_info_t ti;
    hir_subject_t* head = _point_to_field(node, ctx, &ti, smt);

    variable_info_t vi;
    VRTB_get_info_id(ti.link.v_id, &vi, &smt->v);
    token_t tmp = { .t_type = vi.type, .flags.ptr = vi.vfs.ptr };

    /* If we're dealing with a pointer, we add one level of reference,
       given the nature of IR and because we need to preserve the pointer's
       level after container's dereference */
    if (tmp.flags.ptr) {
        head->ptr += tmp.flags.ptr;
    }

    hir_subject_t* target = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, NULL, &smt->v));
    target->ptr = head->ptr;
    
    HIR_BLOCK2(ctx, HIR_TPTR, target, head);
    HIR_BLOCK2(ctx, HIR_LDREF, target, HIR_generate_implconv(ctx, tmp.flags.ptr, HIR_get_tmptype_tkn(&tmp, 0), data, smt));
    return 1;
}
