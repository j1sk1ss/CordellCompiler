#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_extern(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the extern statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != DELIMITER_TOKEN) {
        if (!TKN_isdecl(CURRENT_TOKEN) && CURRENT_TOKEN->t_type != FUNC_NAME_TOKEN) forward_token(it, 1);
        else if (TKN_isdecl(CURRENT_TOKEN)) {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("Extern variable declaration error! extern <type> <name>!");
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(node, arg); 
        }
        else if (CURRENT_TOKEN->t_type == FUNC_NAME_TOKEN) {
            ast_node_t* fname = AST_create_node(CURRENT_TOKEN);
            if (!fname) {
                print_error("Extern function declaration error! extern <name>!");
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(node, fname);
            FNTB_add_info(fname->t->body, 1, 1, 0, NULL, NULL, &smt->f);
            forward_token(it, 1);
        }
    }

    return node;
}

ast_node_t* cpl_parse_rexit(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create the base for either the return or the exit statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    forward_token(it, 1);
    if (TKN_isclose(CURRENT_TOKEN)) {
        return node;
    }

    ast_node_t* exp_node = cpl_parse_expression(it, ctx, smt);
    if (!exp_node) {
        print_error("Error during the exit or the return statement! return/exit <stmt>!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, exp_node);
    return node;
}

ast_node_t* cpl_parse_funccall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create the base for the function call!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    int args = 0;
    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
            if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
            if (arg) AST_add_node(node, arg);
            else { print_error("Error during the call argument parsing! <name>(<arg>)!"); }

            args++;
        }
    }

    func_info_t finfo;
    if (FNTB_get_info(node->t->body, &finfo, &smt->f)) {
        node->sinfo.v_id = finfo.id;
        for (ast_node_t* arg = finfo.args->c; arg && arg->t->t_type != SCOPE_TOKEN; arg = arg->siblings.n) {
            if (args-- > 0 || !arg->c->siblings.n || !arg->c->siblings.n->t) continue;
            AST_add_node(node, AST_copy_node(arg->c->siblings.n, 0, 0, 1));
        }
    }

    forward_token(it, 1);
    return node;
}

ast_node_t* cpl_parse_function(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create the base for the function!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name_node = AST_create_node(CURRENT_TOKEN);
    if (!name_node) {
        print_error("Can't create the base for the function's name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, name_node);

    forward_token(it, 1);
    ast_node_t* args_node = AST_create_node(TKN_create_token(SCOPE_TOKEN, NULL, CURRENT_TOKEN->lnum));
    if (!args_node) {
        print_error("Can't create the base for the function's arguments!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    args_node->bt = args_node->t;
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    args_node->sinfo.s_id = ctx->scopes.s_id;

    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (
            CURRENT_TOKEN->t_type == COMMA_TOKEN ||
            !TKN_isdecl(CURRENT_TOKEN)
        ) forward_token(it, 1);
        else {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("Error during the function's argument parsing! function <name>(<type> <name> (opt: = <stmt>))!");
                AST_unload(node);
                AST_unload(args_node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(args_node, arg);
        }
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == RETURN_TYPE_TOKEN) {
        forward_token(it, 1);
        ast_node_t* ret_type = AST_create_node(CURRENT_TOKEN);
        AST_add_node(name_node, ret_type);
        forward_token(it, 1);
    }

    name_node->sinfo.v_id = FNTB_add_info(
        name_node->t->body, name_node->t->flags.glob, name_node->t->flags.ext, 0,
        args_node, name_node->c, &smt->f
    );

    ast_node_t* body_node = cpl_parse_scope(it, ctx, smt);
    if (!body_node) {
        print_error("Error during the function's body parsing!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(args_node, body_node);
    AST_add_node(node, args_node);

    stack_pop(&ctx->scopes.stack, NULL);
    return node;
}
