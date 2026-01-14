#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_function(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the function!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* name_node = AST_create_node(CURRENT_TOKEN);
    if (!name_node) {
        PARSE_ERROR("Can't create a base for the function's name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, name_node);

    forward_token(it, 1);
    ast_node_t* args_node = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!args_node) {
        PARSE_ERROR("Can't create a base for the function's arguments!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

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
                PARSE_ERROR("Error during the function's argument parsing! function <name>(<type> <name> (opt: = <stmt>))!");
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

    AST_add_node(node, args_node);
    name_node->sinfo.v_id = FNTB_add_info(
        name_node->t->body, name_node->t->flags.glob, name_node->t->flags.ext, 0,
        args_node, name_node->c, &smt->f
    );

    if (CURRENT_TOKEN->t_type == DELIMITER_TOKEN) {
        node->t->t_type = FUNC_PROT_TOKEN;
        stack_pop(&ctx->scopes.stack, NULL);
        return node;
    }

    ast_node_t* body_node = cpl_parse_scope(it, ctx, smt);
    if (!body_node) {
        PARSE_ERROR("Error during the function's body parsing!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(args_node, body_node);
    stack_pop(&ctx->scopes.stack, NULL);
    return node;
}