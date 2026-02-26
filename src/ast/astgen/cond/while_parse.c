#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_while(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", WHILE_COMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt, 1);
    if (!cond) {
        PARSE_ERROR("Error during the parsing process of the condition in the '%s' statement!", WHILE_COMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, cond);

    ast_node_t* body = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
    else body = cpl_parse_scope(it, ctx, smt, 1);
    if (!body) {
        PARSE_ERROR("Error during parsing in the '%s' statement body!", WHILE_COMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, body);
    return node;
}
