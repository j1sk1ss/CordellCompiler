#include <hir/hirgens/hirgens.h>

int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx) {
    HIR_BLOCK0(ctx, STRT);
    return HIR_generate_block(node->child, ctx);
}

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx) {
    HIR_BLOCK1(ctx, EXITOP, HIR_generate_elem(node->child, ctx));    
    return 1;
}

hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx) {
    int args_count = 0;
    for (ast_node_t* e = node->child; e; e = e->sibling) {
        HIR_BLOCK1(ctx, PARAM, HIR_generate_elem(e, ctx));
        args_count++;
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(TMPVARI64);
    HIR_BLOCK2(ctx, SYSC, res, HIR_SUBJ_CONST(args_count));
    return res;
}
