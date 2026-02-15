#include <hir/hirgens/hirgens.h>

int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx, sym_table_t* smt) {
    switch (node->t->t_type) {
        case DREF_TYPE_TOKEN:  HIR_generate_dref(node, ctx, smt, src);             break;
        case INDEXATION_TOKEN: HIR_generate_store_indexation(node, src, ctx, smt); break;
        default: {
            hir_subject_t* trg = HIR_SUBJ_ASTVAR(node);
            HIR_BLOCK2(ctx, HIR_STORE, trg, HIR_generate_implconv(ctx, trg->ptr, trg->t, src, smt)); 
            break;
        }
    }

    return 1;
}
