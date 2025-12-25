#include <hir/hirgens/hirgens.h>

int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx) {
    for (ast_node_t* func = node->c->c; func; func = func->siblings.n) {
        HIR_BLOCK1(ctx, HIR_IMPORT, HIR_SUBJ_FUNCNAME(func));
    }

    return 1;
}

int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx) {
    return HIR_BLOCK1(ctx, HIR_OEXT, HIR_SUBJ_STRING(node));
}

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    return HIR_BLOCK1(ctx, HIR_EXITOP, HIR_generate_elem(node->c, ctx, smt));
}

hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    hir_subject_t* args = HIR_SUBJ_LIST();
    for (ast_node_t* e = node->c; e; e = e->siblings.n) {
        hir_subject_t* el = HIR_generate_elem(e, ctx, smt);
        HIR_BLOCK1(ctx, HIR_VRUSE, el);
        list_add(&args->storage.list.h, HIR_copy_subject(el));
    }

    if (!ret) {
        HIR_BLOCK3(ctx, HIR_SYSC, NULL, NULL, args);
        return NULL;    
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARI64, VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, NULL, &smt->v));
    HIR_BLOCK3(ctx, HIR_STORE_SYSC, res, NULL, args);
    return res;
}

int HIR_generate_breakpoint_block(hir_ctx_t* ctx) {
    return HIR_BLOCK0(ctx, HIR_BREAKPOINT);
}
