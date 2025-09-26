#include <hir/hirgens/hirgens.h>

int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    for (ast_node_t* func = node->child->child; func; func = func->sibling) {
        HIR_BLOCK1(ctx, HIR_IMPORT, HIR_SUBJ_FUNCNAME(func));
    }

    return 1;
}

int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    HIR_BLOCK1(ctx, HIR_EXTERN, HIR_SUBJ_STRING(node));
    return 1;
}

int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    HIR_BLOCK1(ctx, HIR_STRT, HIR_SUBJ_FUNCNAME(node));

    int agrnum = 0;
    ast_node_t* st = node->child;
    for (; st && st->token; st = st->sibling) {
        HIR_BLOCK2(ctx, HIR_STARGLD, HIR_generate_load(st->child, ctx, smt), HIR_SUBJ_CONST(agrnum++));
    }

    HIR_generate_block(st, ctx, smt);
    HIR_BLOCK0(ctx, HIR_STEND);
    return 1;
}

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    HIR_BLOCK1(ctx, HIR_EXITOP, HIR_generate_elem(node->child, ctx, smt));    
    return 1;
}

hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    int args_count = 0;
    for (ast_node_t* e = node->child; e; e = e->sibling) {
        HIR_BLOCK1(ctx, HIR_PRMST, HIR_generate_elem(e, ctx, smt));
        args_count++;
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARI64, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
    HIR_BLOCK2(ctx, HIR_SYSC, res, HIR_SUBJ_CONST(args_count));
    return res;
}
