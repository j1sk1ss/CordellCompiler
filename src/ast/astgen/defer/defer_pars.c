#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_defer, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for a defer!");
    
    forward_token(it, 1);
    ast_node_t* buffer = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_ASSERT(!buffer, base, "Error during the defer body parse! defer call()!");
    AST_add_node(base, buffer);

    PARSER_ASSERT(buffer->t->t_type != CALLING_TOKEN, base, "Defer works only with function calls!");
    return base;
})