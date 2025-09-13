#include <cpl_parser.h>

ast_node_t* cpl_parse_switch(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    node->info.s_id = scope_id_top(&ctx->scopes.stack);

    forward_token(curr, 1);
    ast_node_t* stmt = p->expr(curr, ctx, p);
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
                ast_node_t* case_stmt = p->expr(curr, ctx, p);
                if (case_stmt->token->t_type == CHAR_VALUE_TOKEN) {
                    snprintf(case_stmt->token->value, TOKEN_MAX_SIZE, "%i", case_stmt->token->value[0]);
                    case_stmt->token->t_type = UNKNOWN_NUMERIC_TOKEN;
                }

                AST_add_node(case_node, case_stmt);
            }

            forward_token(curr, 1);
            ast_node_t* case_body = p->scope(curr, ctx, p);
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

ast_node_t* cpl_parse_condop(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    node->info.s_id = scope_id_top(&ctx->scopes.stack);
    
    forward_token(curr, 1);
    ast_node_t* cond = p->expr(curr, ctx, p);
    if (!cond) {
        print_error("AST error during cond stmt parsing! line=%i", (*curr)->lnum);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(curr, 1);

    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = p->scope(curr, ctx, p);
        if (!branch) {
            print_error("AST error during if lbranch parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* branch = p->scope(curr, ctx, p);
        if (!branch) {
            print_error("AST error during if rbranch parsing! line=%i", (*curr)->lnum);
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}
