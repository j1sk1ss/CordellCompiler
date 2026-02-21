#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_loop(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", LOOP_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);

    ast_node_t* body = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, carry);
    else body = cpl_parse_scope(it, ctx, smt, carry);
    if (!body) {
        PARSE_ERROR("Error during parsing in the '%s' statement body!", LOOP_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, body);
    return node;
}
