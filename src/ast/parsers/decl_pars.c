#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_array_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    forward_token(it, 1);

    token_type_t eltype = I64_TYPE_TOKEN;
    ast_node_t* name_node = AST_create_node((token_t*)list_iter_current(it));
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    forward_token(it, 1);

    int el_size    = 1;
    int array_size = 1;

    if (((token_t*)list_iter_current(it))->t_type == OPEN_INDEX_TOKEN) {
        forward_token(it, 1);

        ast_node_t* size_node = AST_create_node((token_t*)list_iter_current(it));
        if (!size_node) {
            AST_unload(node);
            return NULL;
        }
        
        if (size_node->token->t_type != UNKNOWN_NUMERIC_TOKEN) name_node->token->flags.heap = 1;
        else array_size = str_atoi(size_node->token->value);
        
        AST_add_node(node, size_node);
        forward_token(it, 2);

        ast_node_t* elem_size_node = AST_create_node((token_t*)list_iter_current(it));
        if (!elem_size_node) {
            AST_unload(node);
            return NULL;
        }

        eltype = elem_size_node->token->t_type;
        AST_add_node(node, elem_size_node);
        forward_token(it, 2);
    }

    if (((token_t*)list_iter_current(it))->t_type == ASSIGN_TOKEN) {
        forward_token(it, 1);
        int act_size = 0;
        if ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type == OPEN_BLOCK_TOKEN) {
            forward_token(it, 1);
            while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BLOCK_TOKEN) {
                if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
                    forward_token(it, 1);
                    continue;
                }

                ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
                if (arg) AST_add_node(node, arg);
                array_size = MAX(array_size, act_size++);
            }

            forward_token(it, 1);
        }
    }

    name_node->sinfo.v_id = VRTB_add_info(
        name_node->token->value, ARRAY_TYPE_TOKEN, scope_id_top(&ctx->scopes.stack), &name_node->token->flags, &smt->v
    );

    ARTB_add_info(name_node->sinfo.v_id, array_size, name_node->token->flags.heap, eltype, &smt->a);
    var_lookup(name_node, ctx, smt);
    return node;
}

ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;

    forward_token(it, 1);
    ast_node_t* name_node = AST_create_node((token_t*)list_iter_current(it));
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    AST_add_node(node, name_node);
    forward_token(it, 1);

    name_node->sinfo.v_id = VRTB_add_info(
        name_node->token->value, node->token->t_type, scope_id_top(&ctx->scopes.stack), &name_node->token->flags, &smt->v
    );

    if (!(token_t*)list_iter_current(it) || ((token_t*)list_iter_current(it))->t_type == ASSIGN_TOKEN) {
        forward_token(it, 1);
        ast_node_t* value_node = cpl_parse_expression(it, ctx, smt);
        if (!value_node) {
            AST_unload(node);
            return NULL;
        }

        if (node->token->t_type == STR_TYPE_TOKEN) {
            ARTB_add_info(name_node->sinfo.v_id, str_strlen(value_node->token->value) + 1, 0, I8_TYPE_TOKEN, &smt->a);
        }

        AST_add_node(node, value_node);
    }

    var_lookup(name_node, ctx, smt);
    return node;
}
