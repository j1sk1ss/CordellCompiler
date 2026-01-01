#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_while(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the '%s' statement!", WHILE_COMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt);
    if (!cond) {
        print_error("Error during the parsing process of the condition in the '%s' statement!", WHILE_COMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(it, 1);

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
        if (!branch) {
            print_error("Error during parsing in the '%s' statement body!", WHILE_COMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}
