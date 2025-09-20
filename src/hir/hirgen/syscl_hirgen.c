#include <hir/hirgens/hirgens.h>

int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* func = node->child->child; func; func = func->sibling) {
        HIR_BLOCK1(ctx, IMPORT, HIR_SUBJ_STRING(func->token->value));
    }

    return 1;
}

int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    HIR_BLOCK1(ctx, EXTERN, HIR_SUBJ_STRING(node->child->token->value));
    return 1;
}

int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    HIR_BLOCK0(ctx, STRT);
    ast_node_t* st = node->child;
    for (; st && st->token; st = st->sibling) {
        HIR_BLOCK1(ctx, STARGLD, HIR_SUBJ_VAR(st));
    }

    return HIR_generate_block(st, ctx);
}

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    HIR_BLOCK1(ctx, EXITOP, HIR_generate_elem(node->child, ctx));    
    return 1;
}

hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    int args_count = 0;
    for (ast_node_t* e = node->child; e; e = e->sibling) {
        HIR_BLOCK1(ctx, PRMST, HIR_generate_elem(e, ctx));
        args_count++;
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(TMPVARI64);
    HIR_BLOCK2(ctx, SYSC, res, HIR_SUBJ_CONST(args_count));
    return res;
}
