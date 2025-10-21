#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_extern(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    
    forward_token(it, 1);
    while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != DELIMITER_TOKEN) {
        if (!TKN_isdecl((token_t*)list_iter_current(it)) && ((token_t*)list_iter_current(it))->t_type != FUNC_NAME_TOKEN) forward_token(it, 1);
        else if (TKN_isdecl((token_t*)list_iter_current(it))) {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("AST error during function arg parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
                AST_unload(node);
                return NULL;
            }

            AST_add_node(node, arg);
        }
        else if (((token_t*)list_iter_current(it))->t_type == FUNC_NAME_TOKEN) {
            ast_node_t* name_node = AST_create_node((token_t*)list_iter_current(it));
            AST_add_node(node, name_node);
            forward_token(it, 1); // TODO: Functab add function
        }
    }

    return node;
}

ast_node_t* cpl_parse_rexit(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    
    node->sinfo.s_id = scope_id_top(&ctx->scopes.stack);
    forward_token(it, 1);
    if (TKN_isclose((token_t*)list_iter_current(it))) {
        return node;
    }

    ast_node_t* exp_node = cpl_parse_expression(it, ctx, smt);
    if (!exp_node) {
        print_error("AST error during return parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, exp_node);
    return node;
}

ast_node_t* cpl_parse_funccall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;

    int args = 0;
    forward_token(it, 1);
    if ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
            if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
            if (arg) AST_add_node(node, arg);
            args++;
        }
    }

    func_info_t finfo;
    if (FNTB_get_info(node->token->value, &finfo, &smt->f)) {
        node->sinfo.v_id = finfo.id;
        for (ast_node_t* arg = finfo.args->child; arg && arg->token->t_type != SCOPE_TOKEN; arg = arg->sibling) {
            if (args-- > 0 || !arg->child->sibling || !arg->child->sibling->token) continue;
            AST_add_node(node, AST_copy_node(arg->child->sibling, 0, 0, 1));
        }
    }

    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_function(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
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

    ast_node_t* args_node = AST_create_node(TKN_create_token(SCOPE_TOKEN, NULL, 0, 0));
    if (!args_node) {
        AST_unload(node);
        return NULL;
    }

    scope_push_id(&ctx->scopes.stack, ++ctx->scopes.s_id);
    args_node->sinfo.s_id = ctx->scopes.s_id;

    forward_token(it, 1);
    while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
        if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }
        
        if (!TKN_isdecl(((token_t*)list_iter_current(it)))) forward_token(it, 1);
        else {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("AST error during function arg parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
                AST_unload(node);
                AST_unload(args_node);
                return NULL;
            }

            AST_add_node(args_node, arg);
        }
    }

    forward_token(it, 1);
    if ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type == RETURN_TYPE_TOKEN) {
        forward_token(it, 1);
        ast_node_t* ret_type = AST_create_node((token_t*)list_iter_current(it));
        AST_add_node(name_node, ret_type);
        forward_token(it, 1);
    }

    ast_node_t* body_node = cpl_parse_scope(it, ctx, smt);
    if (!body_node) {
        print_error("AST error during function body parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
        AST_unload(node);
        return NULL;
    }

    AST_add_node(args_node, body_node);
    AST_add_node(node, args_node);

    scope_elem_t el;
    scope_pop_top(&ctx->scopes.stack, &el);

    name_node->sinfo.v_id = FNTB_add_info(
        name_node->token->value, name_node->token->flags.glob, name_node->token->flags.ext, 
        args_node, name_node->child, &smt->f
    );

    return node;
}
