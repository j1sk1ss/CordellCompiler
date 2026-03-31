#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* args = HIR_SUBJ_LIST();
    for (ast_node_t* e = node->c; e; e = e->siblings.n) {
        hir_subject_t* el = HIR_generate_elem(e, ctx, smt);
        if (!HIR_is_defined_type(el->t)) {
            HIR_BLOCK1(ctx, HIR_VRUSE, el);
            el = HIR_copy_subject(el);
        }
        
        list_add(&args->storage.list.h, el);
    }

    if (!ret) {
        HIR_BLOCK3(ctx, HIR_SYSC, NULL, NULL, args);
        return NULL;    
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARI64, VRTB_add_info(NULL, I64_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v));
    HIR_BLOCK3(ctx, HIR_STORE_SYSC, res, NULL, args);
    return res;
}
