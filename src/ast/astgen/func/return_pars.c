#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_return, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for a 'return' statement!");
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    forward_token(it, 1);
    if (TKN_is_close(CURRENT_TOKEN)) {
        return base;
    }

    ast_node_t* value = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_ASSERT(!value, base, "Error during the return statement parsing!");
    AST_add_node(base, value);
    forward_token(it, 1);
    return base;
})
