#include <hir/hirgens/hirgens.h>

hir_subject_t* _point_to_field(ast_node_t* root, hir_ctx_t* ctx, type_info_t* field_info, sym_table_t* smt) {
    hir_subject_t* base = HIR_generate_elem(root->c, ctx, smt);
    long offset = TPTB_get_child_offset(root->c->sinfo.t_id, root->sinfo.t_id, &smt->t);
    hir_subject_t* real_offset = HIR_SUBJ_TMPVAR(HIR_STKVARU8, VRTB_add_info(NULL, TMP_U8_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v));
    TPTB_get_info_id(root->sinfo.t_id, field_info, &smt->t);
    HIR_BLOCK3(ctx, HIR_iADD, real_offset, base, HIR_SUBJ_CONST(offset));
    return real_offset;
}

hir_subject_t* HIR_generate_load_member_access(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    type_info_t ti;
    hir_subject_t* head = _point_to_field(node, ctx, &ti, smt);

    token_t tmp = { .t_type = ti.memory.tt, .flags.ptr = ti.memory.ptr };
    hir_subject_t* value = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, NULL, &smt->v));
    value->ptr = tmp.flags.ptr;

    HIR_BLOCK2(ctx, HIR_GDREF, value, head);
    return value;
}

int HIR_generate_store_member_access(ast_node_t* node, hir_subject_t* data, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    type_info_t ti;
    hir_subject_t* head = _point_to_field(node, ctx, &ti, smt);
    HIR_BLOCK2(ctx, HIR_LDREF, head, data);
    return 1;
}
