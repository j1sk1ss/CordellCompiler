#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_while, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for the 'while' statement!");
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* stmt = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_ASSERT(!stmt, base, "Error during parsing process of a condition in the 'while' statement!");
    AST_add_node(base, stmt);

    ast_node_t* body = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
    else                                      body = cpl_parse_scope(it, ctx, smt, 1);
    
    PARSER_ASSERT(!body, base, "Error during parsing in the 'while' statement body!");
    AST_add_node(base, body);
    return base;
})
