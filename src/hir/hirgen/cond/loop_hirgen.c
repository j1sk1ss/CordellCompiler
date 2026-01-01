#include <hir/hirgens/hirgens.h>

int HIR_generate_loop_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* lbranch = node->c;
    
    hir_subject_t* entry_lb = HIR_SUBJ_LABEL();
    hir_subject_t* end_lb   = HIR_SUBJ_LABEL();

    HIR_BLOCK1(ctx, HIR_MKLB, entry_lb);
    if (lbranch) {
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        
        void* backup = ctx->carry;
        ctx->carry = end_lb; /* Save the backup label for the `break` statement */
        HIR_generate_block(lbranch->c, ctx, smt);
        ctx->carry = backup; /* Restore the backup label                        */

        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_BLOCK1(ctx, HIR_JMP, entry_lb);
    }

    HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    return 1;
}
