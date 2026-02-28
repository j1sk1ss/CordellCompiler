#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_if(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", IF_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt, 1);
    if (cond) AST_add_node(base, cond);
    else {
        PARSE_ERROR("Error during the condition parsing in the '%s' structure! %s <stmt>", IF_COMMAND, IF_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* tbranch = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) tbranch = cpl_parse_line_scope(it, ctx, smt, 1);
    else tbranch = cpl_parse_scope(it, ctx, smt, 1);
    if (tbranch) AST_add_node(base, tbranch);
    else {
        PARSE_ERROR("Error during the true branch parsing in the '%s' statement!", IF_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == ELSE_TOKEN) {
        ast_node_t* fbranch = NULL;
        forward_token(it, 1);
        switch (CURRENT_TOKEN->t_type) {
            case OPEN_BLOCK_TOKEN: fbranch = cpl_parse_scope(it, ctx, smt, 1);          break;
            case IF_TOKEN:         fbranch = cpl_parse_if(it, ctx, smt, carry);         break;
            default:               fbranch = cpl_parse_line_scope(it, ctx, smt, carry); break;
        }
        
        if (fbranch) AST_add_node(base, fbranch);
        else {
            PARSE_ERROR("Error during the false branch parsing in the '%s' statement!", IF_COMMAND);
            AST_unload(base);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }
    
    return base;
}
