#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_switch(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }
 
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);

    forward_token(it, 1);
    ast_node_t* stmt = cpl_parse_expression(it, ctx, smt);
    if (!stmt) {
        print_error("cpl_parse_expression return NULL!");
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, stmt);
    ast_node_t* cases_scope = AST_create_node(NULL);
    if (!cases_scope) {
        print_error("AST_create_node error!");
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(it, 1);
        while (CURRENT_TOKEN->t_type == CASE_TOKEN || CURRENT_TOKEN->t_type == DEFAULT_TOKEN) {
            ast_node_t* case_node = AST_create_node(CURRENT_TOKEN);
            if (CURRENT_TOKEN->t_type == CASE_TOKEN) {
                forward_token(it, 1);
                ast_node_t* case_stmt = cpl_parse_expression(it, ctx, smt);
                AST_add_node(case_node, case_stmt);
            }

            forward_token(it, 1);
            ast_node_t* case_body = cpl_parse_scope(it, ctx, smt);
            if (!case_body) {
                print_error("cpl_parse_scope return NULL!");
                AST_unload(case_node);
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            AST_add_node(case_node, case_body);
            AST_add_node(cases_scope, case_node);
        }
    }

    AST_add_node(node, cases_scope);
    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_condop(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    
    forward_token(it, 1);
    ast_node_t* cond = cpl_parse_expression(it, ctx, smt);
    if (!cond) {
        print_error("cpl_parse_expression return NULL!");
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(it, 1);

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
        if (!branch) {
            print_error("AST error during if lbranch parsing! line=%i", CURRENT_TOKEN->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == ELSE_TOKEN) {
        forward_token(it, 1);
        ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
        if (!branch) {
            print_error("AST error during if rbranch parsing! line=%i", CURRENT_TOKEN->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}
