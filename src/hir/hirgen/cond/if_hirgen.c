#include <hir/hirgens/hirgens.h>

int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond    = node->c;
    ast_node_t* lbranch = cond->siblings.n;
    ast_node_t* rbranch = lbranch->siblings.n;

    hir_subject_t* true_lb  = HIR_SUBJ_LABEL();
    hir_subject_t* false_lb = HIR_SUBJ_LABEL();
    hir_subject_t* end_lb   = HIR_SUBJ_LABEL();

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    HIR_BLOCK3(ctx, HIR_IFOP2, condtmp, true_lb, rbranch ? false_lb : end_lb);

    if (lbranch) {
        HIR_BLOCK1(ctx, HIR_MKLB, true_lb);
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_generate_block(lbranch->c, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_BLOCK1(ctx, HIR_JMP, end_lb);
    }

    if (!rbranch) HIR_unload_subject(false_lb);
    else {
        HIR_BLOCK1(ctx, HIR_MKLB, false_lb);
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
        HIR_generate_block(rbranch->c, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
        HIR_BLOCK1(ctx, HIR_JMP, end_lb);
    }

    HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    return 1;
}
