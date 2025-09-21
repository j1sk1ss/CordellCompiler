#include <hir/hirgens/hirgens.h>

int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    hir_subject_t* flb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();
    
    HIR_BLOCK2(ctx, IFOP, condtmp, flb);
    if (lbranch) {
        HIR_BLOCK0(ctx, MKSCOPE);
        HIR_generate_block(lbranch->child, ctx, smt);
        HIR_BLOCK0(ctx, ENDSCOPE);
    }

    HIR_BLOCK1(ctx, JMP, elb);
    HIR_BLOCK1(ctx, MKLB, flb);
    if (rbranch) {
        HIR_BLOCK0(ctx, MKSCOPE);
        HIR_generate_block(rbranch->child, ctx, smt);
        HIR_BLOCK0(ctx, ENDSCOPE);
    }

    HIR_BLOCK1(ctx, MKLB, elb);
    return 1;
}

int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    
    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    hir_subject_t* entrylb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();

    HIR_BLOCK1(ctx, MKLB, entrylb);
    HIR_BLOCK2(ctx, IFOP, condtmp, elb);
    if (lbranch) {
        HIR_BLOCK0(ctx, MKSCOPE);
        HIR_generate_block(lbranch->child, ctx, smt);
        HIR_BLOCK0(ctx, ENDSCOPE);
    }

    HIR_BLOCK1(ctx, JMP, entrylb);
    HIR_BLOCK1(ctx, MKLB, elb);
    if (rbranch) {
        HIR_BLOCK0(ctx, MKSCOPE);
        HIR_generate_block(rbranch->child, ctx, smt);
        HIR_BLOCK0(ctx, ENDSCOPE);
    }
    
    return 1;
}

int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;

    int case_count = 0;
    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) case_count++;

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    HIR_BLOCK2(ctx, SWITCHOP, condtmp, HIR_SUBJ_CONST(case_count));

    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN) {
            HIR_BLOCK0(ctx, MKDEFCASE);
            HIR_generate_block(curr_case->child, ctx, smt);
            HIR_BLOCK0(ctx, MKENDCASE);
        } 
        else {
            HIR_BLOCK1(ctx, MKCASE, HIR_generate_elem(curr_case->child, ctx, smt));
            HIR_generate_block(curr_case->child->sibling, ctx, smt);
            HIR_BLOCK0(ctx, MKENDCASE);
        }
    }

    return 1;
}
