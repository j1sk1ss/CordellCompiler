#include <hir/hirgens/hirgens.h>

int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, FRET, HIR_generate_elem(node->child, ctx, smt));
    return 1;
}

hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    int variables_size = 0;
    ast_node_t* name = node;

    int arg_count = 0;
    for (ast_node_t* arg = name->child; arg; arg = arg->sibling) {
        HIR_BLOCK1(ctx, FARGST, HIR_generate_elem(arg, ctx, smt));
        arg_count++;
    }

    func_info_t fi;
    FNTB_get_info(name->token->value, &fi, &smt->f);

    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(fi.rtype ? fi.rtype->token : NULL));
    HIR_BLOCK3(ctx, FCLL, res, HIR_SUBJ_FUNCNAME(name), HIR_SUBJ_CONST(arg_count));
    return res;
}

int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node = node->child;
    ast_node_t* body_node = name_node->sibling;

    HIR_BLOCK1(ctx, FDCL, HIR_SUBJ_FUNCNAME(name_node));
    ast_node_t* t = NULL;
    for (t = body_node->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
        HIR_BLOCK1(ctx, FARGLD, HIR_SUBJ_VAR(t));
    }

    HIR_generate_block(t->child, ctx, smt);
    HIR_BLOCK0(ctx, FEND);
    return 1;
}
