#include <cpl_parser.h>

ast_node_t* cpl_parse_switch(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    ast_node_t* stmt = ctx->expr(curr, ctx);
    if (!stmt) {
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
            ast_node_t* case_stmt = NULL;
            if ((*curr)->t_type != CASE_TOKEN) {
                case_stmt = AST_create_node(
                    TKN_create_token(DEFAULT_TOKEN, DEFAULT_COMMAND, str_strlen(DEFAULT_COMMAND), 0)
                );
            }
            else {
                forward_token(curr, 1);
                case_stmt = ctx->expr(curr, ctx);
                case_stmt->token->t_type = CASE_TOKEN;
            }
            
            if (!case_stmt) {
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            forward_token(curr, 1);
            ast_node_t* case_body = ctx->scope(curr, ctx);
            if (!case_body) {
                AST_unload(case_stmt);
                AST_unload(cases_scope);
                AST_unload(node);
                return NULL;
            }

            case_stmt->token->vinfo.glob = 0;
            case_stmt->token->vinfo.ro   = 0;

            AST_add_node(case_stmt, case_body);
            AST_add_node(cases_scope, case_stmt);
        }
    }

    AST_add_node(node, cases_scope);
    forward_token(curr, 1);
    return node;
}

ast_node_t* cpl_parse_condop(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    forward_token(curr, 1);
    ast_node_t* cond = ctx->expr(curr, ctx);
    if (!cond) {
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, cond);
    forward_token(curr, 1);

    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        ast_node_t* branch = ctx->scope(curr, ctx);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }

    if (*curr && (*curr)->t_type == ELSE_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* branch = ctx->scope(curr, ctx);
        if (!branch) {
            AST_unload(node);
            return NULL;
        }
        
        AST_add_node(node, branch);
    }
    
    return node;
}