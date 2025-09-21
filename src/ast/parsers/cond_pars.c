#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_switch(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    node->sinfo.s_id = scope_id_top(&ctx->scopes.stack);

    forward_token(curr, 1);
    ast_node_t* stmt = cpl_parse_expression(curr, ctx, smt);
    if (!stmt) {
        print_error("AST error during switch stmt parsing! line=%i", (*curr)->lnum);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, stmt);
    ast_node_t* cases_scope = AST_create_node(NULL);
    if (!cases_scope) {
        AST_unload(node);
        return NULL;
    }

    forward_token(curr, 1);
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(curr, 1);
        while ((*curr)->t_type == CASE_TOKEN || (*curr)->t_type == DEFAULT_TOKEN) {
            ast_node_t* case_node = AST_create_node(*curr);
            if ((*curr)->t_type == CASE_TOKEN) {
                forward_token(curr, 1);
                ast_node_t* case_stmt = cpl_parse_expression(curr, ctx, smt);
                AST_add_node(case_node, case_stmt);
            }

            forward_token(curr, 1);
            ast_node_t* case_body = cpl_parse_scope(curr, ctx, smt);
            if (!case_body) {
                print_error("AST error during switch case body parsing! line=%i", (*curr)->lnum);
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
    forward_token(curr, 1);
    return node;
}

ast_node_t* cpl_parse_condop(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    node->sinfo.s_id = scope_id_top(&ctx->scopes.stack);
    
    forward_token(curr, 1);
    ast_node_t* cond = cpl_parse_expression(curr, ctx, smt);
    if (!cond) {
        print_error("AST error during cond stmt parsing! line=%i", (*curr)->lnum);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(curr, 1);

    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = cpl_parse_scope(curr, ctx, smt);
        if (!branch) {
            print_error("AST error during if lbranch parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* branch = cpl_parse_scope(curr, ctx, smt);
        if (!branch) {
            print_error("AST error during if rbranch parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}
