#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_return(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create the base for the '%s' statement!", RETURN_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    forward_token(it, 1);
    if (TKN_isclose(CURRENT_TOKEN)) {
        return node;
    }

    ast_node_t* exp_node = cpl_parse_expression(it, ctx, smt, 1);
    if (!exp_node) {
        PARSE_ERROR("Error during the return statement! return <stmt>!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, exp_node);
    return node;
}
