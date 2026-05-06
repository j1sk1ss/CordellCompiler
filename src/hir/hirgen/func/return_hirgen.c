#include <hir/hirgens/hirgens.h>

int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* rval = HIR_generate_elem(node->c, ctx, smt);
    if (ctx->carry.ptr2) {
        token_t* trg = (token_t*)ctx->carry.ptr2;
        rval = HIR_generate_implconv(ctx, trg->flags.ptr, HIR_get_tmptype_tkn(trg, 0), rval, smt);
    }

    return HIR_BLOCK1(ctx, HIR_FRET, rval);
}
