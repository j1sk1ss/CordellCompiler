#include <hir/hirgens/hirgens.h>

int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    return HIR_BLOCK1(ctx, HIR_FRET, HIR_generate_elem(node->child, ctx, smt));
}

hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    func_info_t fi;
    if (!FNTB_get_info_id(node->sinfo.v_id, &fi, &smt->f)) {
        return NULL;
    }

    hir_subject_t* args = HIR_SUBJ_LIST();
    for (ast_node_t* arg = node->child; arg; arg = arg->sibling) {
        hir_subject_t* el = HIR_generate_elem(arg, ctx, smt);
        HIR_BLOCK1(ctx, HIR_VRUSE, el);
        list_add(&args->storage.list.h, HIR_copy_subject(el));
    }
    
    if (!ret) {
        HIR_BLOCK3(ctx, fi.external ? HIR_ECLL : HIR_FCLL, NULL, HIR_SUBJ_FUNCNAME(node), args);
        return NULL;
    }
    
    hir_subject_t* res = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(fi.rtype ? fi.rtype->token : NULL, fi.rtype ? fi.rtype->token->flags.ptr : 1),
        VRTB_add_info(NULL, TKN_get_tmp_type(fi.rtype ? fi.rtype->token->t_type : I64_TYPE_TOKEN), 0, NULL, &smt->v)
    );
    
    HIR_BLOCK3(ctx, fi.external ? HIR_STORE_ECLL : HIR_STORE_FCLL, res, HIR_SUBJ_FUNCNAME(node), args);
    return res;
}

static int _generate_calling_block(
    ast_node_t* name, ast_node_t* body, char entry, hir_ctx_t* ctx, sym_table_t* smt
) {
    HIR_BLOCK1(ctx, entry ? HIR_STRT : HIR_FDCL, HIR_SUBJ_FUNCNAME(name));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(body->sinfo.s_id));

    int argnum = 0;
    ast_node_t* t;
    for (t = body->child; t && t->token && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->child));
        HIR_BLOCK2(ctx, entry ? HIR_STARGLD : HIR_FARGLD, HIR_SUBJ_ASTVAR(t->child), HIR_SUBJ_CONST(argnum++));
    }

    HIR_generate_block(t, ctx, smt);
    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(body->sinfo.s_id));
    HIR_BLOCK0(ctx, entry ? HIR_STEND : HIR_FEND);
    return 1;
}

int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    return _generate_calling_block(node, node, 1, ctx, smt);
}

int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    func_info_t fi;
    if (!FNTB_get_info_id(node->child->sinfo.v_id, &fi, &smt->f)) {
        return 0;
    }
    
    return _generate_calling_block(node->child, node->child->sibling, fi.entry, ctx, smt);
}
