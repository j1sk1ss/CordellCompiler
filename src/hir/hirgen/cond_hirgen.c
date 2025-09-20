#include <hir/hirgen/hirgen.h>

int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx);
    hir_subject_t* flb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();
    
    HIR_BLOCK2(ctx, IFOP, condtmp, flb);
    HIR_generate_block(lbranch, ctx);
    HIR_BLOCK1(ctx, JMP, elb);
    HIR_BLOCK1(ctx, MKLB, flb);
    HIR_generate_block(rbranch, ctx);
    HIR_BLOCK1(ctx, MKLB, elb);
    
    return 1;
}

int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    
    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx);
    hir_subject_t* entrylb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();

    HIR_BLOCK1(ctx, MKLB, entrylb);
    HIR_BLOCK2(ctx, IFOP, condtmp, elb);
    HIR_generate_block(lbranch, ctx);
    HIR_BLOCK1(ctx, JMP, entrylb);
    HIR_BLOCK1(ctx, MKLB, elb);
    HIR_generate_block(rbranch, ctx);
    
    return 1;
}

int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;

    int case_count = 0;
    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) case_count++;

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx);
    HIR_BLOCK2(ctx, SWITCHOP, cond, HIR_SUBJ_CONST(case_count));

    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN) {
            HIR_BLOCK0(ctx, MKDEFCASE);
            HIR_generate_block(curr_case->child, ctx);
            HIR_BLOCK0(ctx, MKENDCASE);
        } 
        else {
            HIR_BLOCK1(ctx, MKCASE, HIR_generate_elem(curr_case->child, ctx));
            HIR_generate_block(curr_case->child->sibling, ctx);
            HIR_BLOCK0(ctx, MKENDCASE);
        }
    }

    return 1;
}
