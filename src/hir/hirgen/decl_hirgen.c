#include <hir/hirgens/hirgens.h>

static int _strdeclaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;
    HIR_BLOCK2(ctx, HIR_STRDECL, HIR_SUBJ_VAR(name_node), HIR_SUBJ_STRING(val_node));
    return 1;
}

static int _arrdeclaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_type_node = size_node->sibling;
    ast_node_t* elems_node   = el_type_node->sibling;

    for (ast_node_t* e = elems_node; e; e = e->sibling) {
        HIR_BLOCK1(ctx, HIR_PRMST, HIR_generate_elem(e, ctx, smt));
    }

    HIR_BLOCK2(ctx, HIR_ARRDECL, HIR_SUBJ_VAR(name_node), HIR_generate_elem(size_node, ctx, smt));
    return 1;
}

static int _starr_declaration(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, ctx, smt);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, ctx, smt);
    return 1;
}

int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* name_node = node->child;
    if (!VRS_one_slot(name_node->token)) {
        return _starr_declaration(node, ctx, smt);
    }

    ast_node_t* val_node = name_node->sibling;
    hir_subject_t* var = HIR_SUBJ_VAR(name_node);
    HIR_BLOCK1(ctx, HIR_VARDECL, var);
    HIR_BLOCK2(ctx, HIR_STORE, var, HIR_generate_elem(val_node, ctx, smt));
    return 1;
}
