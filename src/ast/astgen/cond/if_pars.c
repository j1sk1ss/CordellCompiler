#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_if(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the 'if' statement!");
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    DUMP_ANNOTATION_TO_NODE(ctx, base);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_DO_OR_THROW(!cond, base, "Error during condition parsing in the 'if' structure!");
    AST_add_node(base, cond);

    ast_node_t* tbranch = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) tbranch = cpl_parse_line_scope(it, ctx, smt, 1);
    else                                      tbranch = cpl_parse_scope(it, ctx, smt, 1);
    
    PARSER_DO_OR_THROW(!tbranch, base, "Error during the 'then' branch parsing in the 'if' statement!");
    AST_add_node(base, tbranch);

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == ELSE_TOKEN) {
        ast_node_t* fbranch = NULL;
        forward_token(it, 1);
        switch (CURRENT_TOKEN->t_type) {
            case OPEN_BLOCK_TOKEN: fbranch = cpl_parse_scope(it, ctx, smt, 1);          break;
            case IF_TOKEN:         fbranch = cpl_parse_if(it, ctx, smt, carry);         break;
            default:               fbranch = cpl_parse_line_scope(it, ctx, smt, carry); break;
        }
        
        PARSER_DO_OR_THROW(!fbranch, base, "Error during the 'else' branch parsing in the 'if' statement!");
        AST_add_node(base, fbranch);
    }
    
    return base;
}
