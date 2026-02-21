#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_dref(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the dereferense command!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 1);
    if (!body) {
        PARSE_ERROR("Error during the dereferense body parse! dref <exp>!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, body);
    return node;
}
