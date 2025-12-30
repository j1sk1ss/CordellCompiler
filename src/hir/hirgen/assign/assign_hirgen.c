#include <hir/hirgens/hirgens.h>

int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* left   = node->c;
    hir_subject_t* src = HIR_generate_elem(left->siblings.n, ctx, smt);
    return HIR_generate_store_block(
        left, HIR_generate_conv(
            ctx, HIR_get_tmptype_tkn(left->t, left->t->flags.ptr), src, smt
        ), ctx, smt
    );
}
