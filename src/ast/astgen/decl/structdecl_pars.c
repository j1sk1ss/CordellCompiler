#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_structdecl(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the struct define command!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name = AST_create_node(CURRENT_TOKEN); /* TODO: Register name as a structure + register as a variable + register as an array */
    if (!name) {
        PARSE_ERROR("Error during struct name creation!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, name);
    var_lookup(name, ctx, smt);
    forward_token(it, 1);
    return node;
}
