#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    func_info_t fi;
    if (!FNTB_get_info_id(node->sinfo.v_id, &fi, &smt->f)) {
        return NULL;
    }

    hir_subject_t* args = HIR_SUBJ_LIST();
    for (ast_node_t* arg = node->c; arg; arg = arg->siblings.n) {
        hir_subject_t* el = HIR_generate_elem(arg, ctx, smt);
        HIR_BLOCK1(ctx, HIR_VRUSE, el);
        list_add(&args->storage.list.h, HIR_copy_subject(el));
    }
    
    if (!ret) {
        HIR_BLOCK3(ctx, fi.flags.external ? HIR_ECLL : HIR_FCLL, NULL, HIR_SUBJ_FUNCNAME(node), args);
        return NULL;
    }
    
    hir_subject_t* res = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(fi.rtype ? fi.rtype->t : NULL, 0),
        VRTB_add_info(NULL, TKN_get_tmp_type(fi.rtype ? fi.rtype->t->t_type : I64_TYPE_TOKEN), 0, NULL, &smt->v)
    );
    
    res->ptr = fi.rtype ? fi.rtype->t->flags.ptr : 0;
    HIR_BLOCK3(ctx, fi.flags.external ? HIR_STORE_ECLL : HIR_STORE_FCLL, res, HIR_SUBJ_FUNCNAME(node), args);
    return res;
}
