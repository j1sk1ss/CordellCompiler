#include <hir/hirgens/hirgens.h>

int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, HIR_FRET, HIR_generate_elem(node->child, ctx, smt));
    return 1;
}

hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    int variables_size = 0;
    ast_node_t* name = node;

    func_info_t fi;
    if (!FNTB_get_info_id(name->sinfo.v_id, &fi, &smt->f)) return NULL;

    int arg_count = 0;
    for (ast_node_t *arg = name->child, *targ = fi.args->child; arg && targ; arg = arg->sibling, targ = targ->sibling) {
        HIR_BLOCK1(ctx, HIR_FARGST, HIR_generate_elem(arg, ctx, smt));
        arg_count++;
    }
    
    if (!ret) {
        HIR_BLOCK2(ctx, fi.external ? HIR_ECLL : HIR_FCLL, HIR_SUBJ_FUNCNAME(name), HIR_SUBJ_CONST(arg_count));
        return NULL;
    }
    
    hir_subject_t* res = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(fi.rtype ? fi.rtype->token : NULL, fi.rtype ? fi.rtype->token->flags.ptr : 1),
        VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v)
    );
    
    HIR_BLOCK3(ctx, fi.external ? HIR_STORE_ECLL : HIR_STORE_FCLL, res, HIR_SUBJ_FUNCNAME(name), HIR_SUBJ_CONST(arg_count));
    return res;
}

int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node = node->child;
    ast_node_t* body_node = name_node->sibling;
    HIR_BLOCK1(ctx, HIR_FDCL, HIR_SUBJ_FUNCNAME(name_node));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(body_node->sinfo.s_id));

    int argnum = 0;
    ast_node_t* t = NULL;
    for (t = body_node->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->child));
        HIR_BLOCK2(ctx, HIR_FARGLD, HIR_SUBJ_ASTVAR(t->child), HIR_SUBJ_CONST(argnum++));
    }

    HIR_generate_block(t, ctx, smt);
    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(body_node->sinfo.s_id));
    HIR_BLOCK0(ctx, HIR_FEND);
    return 1;
}
