#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_if(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;
    
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", IF_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt, 1);
    if (!cond) {
        PARSE_ERROR("Error during the condition parsing in the '%s' structure! %s <stmt>", IF_COMMAND, IF_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, cond);

    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type != OPEN_BLOCK_TOKEN) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token during a parse of the '%s' statement!", IF_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    ast_node_t* tbranch = cpl_parse_scope(it, ctx, smt);
    if (!tbranch) {
        PARSE_ERROR("Error during the true branch parsing in the '%s' statement!", IF_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, tbranch);

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == ELSE_TOKEN) {
        forward_token(it, 1);
        ast_node_t* fbranch = cpl_parse_scope(it, ctx, smt);
        if (!fbranch) {
            PARSE_ERROR("Error during the false branch parsing in the '%s' statement!", IF_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
        
        AST_add_node(node, fbranch);
    }
    
    return node;
}
