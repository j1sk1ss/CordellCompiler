#include <ir/hir/hir.h>

static ir_subject_t* _strdeclaration(ast_node_t* node, ir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;
    return 1;
}

static ir_subject_t* _arrdeclaration(ast_node_t* node, ir_ctx_t* ctx) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_size_node = size_node->sibling;
    ast_node_t* elems_node   = el_size_node->sibling;
    return 1;
}

static ir_subject_t* _starr_declaration(ast_node_t* node, ir_ctx_t* ctx) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, ctx);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, ctx);
    return 1;
}

ir_subject_t* HIR_generate_declaration_block(ast_node_t* node, ir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    if (!VRS_instack(name_node->token)) return 0;
    if (!VRS_one_slot(name_node->token)) {
        return _starr_declaration(node, ctx);
    }

    ast_node_t* val_node = name_node->sibling;
    ir_subject_t* vt1 = HIR_generate_elem_block(val_node, ctx);
    ir_subject_t* res = IR_SUBJ_VAR(0, 0, 0);
    return res;
}
