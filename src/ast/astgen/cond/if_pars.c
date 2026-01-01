#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_if(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the '%s' statement!", IF_COMMAND);
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt);
    if (!cond) {
        print_error("Error during the condition parsing in the '%s' structure! %s <stmt>", IF_COMMAND, IF_COMMAND);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(it, 1);

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
        if (!branch) {
            print_error("Error during the true branch parsing in the '%s' statement!", IF_COMMAND);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == ELSE_TOKEN) {
        forward_token(it, 1);
        ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
        if (!branch) {
            print_error("Error during the false branch parsing in the '%s' statement!", IF_COMMAND);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}
