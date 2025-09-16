#include <cpl_parser.h>

ast_node_t* cpl_parse_array_declaration(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
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
        
        if (size_node->token->t_type != UNKNOWN_NUMERIC_TOKEN) name_node->token->vinfo.heap = 1; 
        else array_size = str_atoi(size_node->token->value);
        
        AST_add_node(node, size_node);
        forward_token(curr, 2);

        ast_node_t* elem_size_node = AST_create_node(*curr);
        if (!elem_size_node) {
            AST_unload(node);
            return NULL;
        }

        el_size = VRS_variable_bitness(elem_size_node->token, 1) / 8;
        AST_add_node(node, elem_size_node);
        forward_token(curr, 2);
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

                ast_node_t* arg = p->expr(curr, ctx, p);
                if (arg) AST_add_node(node, arg);
                array_size = MAX(array_size, ++act_size);
            }

            forward_token(curr, 1);
        }
    }
    
    ART_add_info(name_node->token->value, scope_id_top(&ctx->scopes.stack), el_size, array_size, ctx->symtb.arrs);
    STX_var_update(node, ctx, name_node->token->value, ALIGN(array_size * el_size), name_node->token->vinfo.ro, name_node->token->vinfo.glob);
    STX_var_lookup(name_node, ctx);
    return node;
}

ast_node_t* cpl_parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    ast_node_t* name_node = AST_create_node(*curr);
    if (!name_node) {
        AST_unload(node);
        return NULL;
    }
    
    if (
        VRS_one_slot(name_node->token) &&  /* Pointer or variable (String and array occupie several stack slots) */
        VRS_instack(node->token)           /* Global and RO variables placed not in stack */
    ) {
        int var_size = VRS_variable_bitness(name_node->token, 1) / 8;
        STX_var_update(node, ctx, name_node->token->value, var_size, name_node->token->vinfo.ro, name_node->token->vinfo.glob);
        STX_var_lookup(name_node, ctx);
    }

    forward_token(curr, 1);
    AST_add_node(node, name_node);
    if (!*curr || (*curr)->t_type != ASSIGN_TOKEN) { /* If this is uninitilized variable */
        return node;
    }

    forward_token(curr, 1);
    ast_node_t* value_node = p->expr(curr, ctx, p);
    if (!value_node) {
        AST_unload(node);
        return NULL;
    }
    
    if (node->token->t_type == STR_TYPE_TOKEN) {
        ART_add_info(name_node->token->value, scope_id_top(&ctx->scopes.stack), 1, node->info.size, ctx->symtb.arrs);
        STX_var_update(node, ctx, name_node->token->value, ALIGN(str_strlen(value_node->token->value)), name_node->token->vinfo.ro, name_node->token->vinfo.glob);
        STX_var_lookup(name_node, ctx);
    }

    AST_add_node(node, value_node);
    return node;
}
