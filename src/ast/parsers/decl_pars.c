#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_array_declaration(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    forward_token(curr, 1);

    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    forward_token(curr, 1);

    int el_size    = 1;
    int array_size = 1;

    if ((*curr)->t_type == OPEN_INDEX_TOKEN) {
        forward_token(curr, 1);

        ast_node_t* size_node = AST_create_node(*curr);
        if (!size_node) {
            AST_unload(node);
            return NULL;
        }
        
        if (size_node->token->t_type != UNKNOWN_NUMERIC_TOKEN) {
            name_node->token->flags.heap = 1;
        }
        
        AST_add_node(node, size_node);
        forward_token(curr, 2);

        ast_node_t* elem_size_node = AST_create_node(*curr);
        if (!elem_size_node) {
            AST_unload(node);
            return NULL;
        }

        AST_add_node(node, elem_size_node);
        forward_token(curr, 2);

        ARTB_add_info(
            name_node->token->value, scope_id_top(&ctx->scopes.stack), elem_size_node->token->t_type, &smt->a
        );
    }

    if ((*curr)->t_type == ASSIGN_TOKEN) {
        forward_token(curr, 1);
        int act_size = 0;
        if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
            forward_token(curr, 1);
            while (*curr && (*curr)->t_type != CLOSE_BLOCK_TOKEN) {
                if ((*curr)->t_type == COMMA_TOKEN) {
                    forward_token(curr, 1);
                    continue;
                }

                ast_node_t* arg = cpl_parse_expression(curr, ctx, smt);
                if (arg) AST_add_node(node, arg);
                array_size = MAX(array_size, ++act_size);
            }

            forward_token(curr, 1);
        }
    }

    VRTB_add_info(
        name_node->token->value, ARRAY_TYPE_TOKEN, scope_id_top(&ctx->scopes.stack), &name_node->token->flags, &smt->v
    );

    var_lookup(name_node, ctx, smt);
    return node;
}

ast_node_t* cpl_parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    forward_token(curr, 1);

    if (!*curr || (*curr)->t_type == ASSIGN_TOKEN) {
        forward_token(curr, 1);
        ast_node_t* value_node = cpl_parse_expression(curr, ctx, smt);
        if (!value_node) {
            AST_unload(node);
            return NULL;
        }
        
        if (node->token->t_type == STR_TYPE_TOKEN) {
            ARTB_add_info(
                name_node->token->value, scope_id_top(&ctx->scopes.stack), I8_TYPE_TOKEN, &smt->a
            );
        }

        AST_add_node(node, value_node);
    }

    VRTB_add_info(
        name_node->token->value, node->token->t_type, scope_id_top(&ctx->scopes.stack), &name_node->token->flags, &smt->v
    );

    var_lookup(name_node, ctx, smt);
    return node;
}
