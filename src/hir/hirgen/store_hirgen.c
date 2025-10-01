#include <hir/hirgens/hirgens.h>

int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
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
                HIR_BLOCK3(ctx, HIR_LINDEX, HIR_SUBJ_ASTVAR(node), HIR_generate_elem(off, ctx, smt), src);
            }

            break;
        }
        default: HIR_BLOCK2(ctx, HIR_STORE, HIR_SUBJ_ASTVAR(node), src); break;
    }

    return 1;
}