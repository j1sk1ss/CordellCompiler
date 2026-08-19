#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_loop, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the 'loop' statement!");
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    DUMP_ANNOTATION_TO_NODE(ctx, base);

    ast_node_t* body = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
    else                                      body = cpl_parse_scope(it, ctx, smt, 1);
    
    PARSER_DO_OR_THROW(!body, base, "Error during parsing in the 'loop' statement body!");
    AST_add_node(base, body);
    return base;
})
