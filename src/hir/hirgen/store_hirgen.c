#include <hir/hirgens/hirgens.h>

int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node->token) return 0;
    if (TKN_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) HIR_BLOCK2(ctx, HIR_STORE, HIR_SUBJ_ASTVAR(node), src);
            else HIR_BLOCK2(ctx, HIR_LDREF, HIR_SUBJ_ASTVAR(node), src);
        }

        return 1;
    }

    switch (node->token->t_type) {
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing: {}
            ast_node_t* off = node->child;
            if (!off) HIR_BLOCK2(ctx, HIR_STORE, HIR_SUBJ_ASTVAR(node), src);
            else {
                hir_subject_t* offval = HIR_generate_elem(off, ctx, smt);
                hir_subject_t* base   = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));

                array_info_t ai;
                token_t tmp = { .t_type = node->token->t_type };
                if (!ARTB_get_info(node->sinfo.v_id, &ai, &smt->a)) HIR_BLOCK2(ctx, HIR_STORE, base, HIR_SUBJ_ASTVAR(node));
                else {
                    HIR_BLOCK2(ctx, HIR_REF, base, HIR_SUBJ_ASTVAR(node));
                    tmp.t_type = ai.el_type;
                }

                hir_subject_t* addr = HIR_SUBJ_TMPVAR(offval->t, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
                HIR_BLOCK3(ctx, HIR_iMUL, addr, offval, HIR_SUBJ_CONST(HIR_get_type_size(HIR_get_tmptype_tkn(&tmp, 1))));

                hir_subject_t* head = HIR_SUBJ_TMPVAR(base->t, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
                HIR_BLOCK3(ctx, HIR_iADD, head, base, addr);
                HIR_BLOCK2(ctx, HIR_LDREF, head, src);
                // HIR_BLOCK3(ctx, HIR_LINDEX, HIR_SUBJ_ASTVAR(node), HIR_generate_elem(off, ctx, smt), src);
            }

            break;
        }
        default: HIR_BLOCK2(ctx, HIR_STORE, HIR_SUBJ_ASTVAR(node), src); break;
    }

    return 1;
}