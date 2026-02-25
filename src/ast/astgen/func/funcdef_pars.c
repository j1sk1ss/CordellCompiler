#include <ast/astgen/astgen.h>

int cpl_parse_funcdef_args(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    ast_node_t* trg = (ast_node_t*)carry;
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (TKN_isdecl(CURRENT_TOKEN)) {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt, carry);
            if (!arg) {
                PARSE_ERROR("Error during the argument parsing! (<type> <name>)!");
                RESTORE_TOKEN_POINT;
                return 0;
            }

            AST_add_node(trg, arg);
        }
        else if (CURRENT_TOKEN->t_type == VAR_ARGUMENTS_TOKEN) {
            ast_node_t* arg = AST_create_node(CURRENT_TOKEN);
            if (!arg) {
                PARSE_ERROR("Error during the function's '...' creation!");
                RESTORE_TOKEN_POINT;
                return 0;
            }

            forward_token(it, 1);
            AST_add_node(trg, arg);
        }
        else {
            PARSE_ERROR("Error during the '%s' statement argument parsing! %s(<type> <name>)!", START_COMMAND, START_COMMAND);
            RESTORE_TOKEN_POINT;
            return 0;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    }

    return 1;
}

ast_node_t* cpl_parse_function(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the function!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, FUNC_NAME_TOKEN)) {
        PARSE_ERROR("Expected 'FUNC_NAME_TOKEN' token!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* name_node = AST_create_node(CURRENT_TOKEN);
    if (!name_node) {
        PARSE_ERROR("Can't create a base for the function's name!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, name_node);

    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected 'OPEN_BRACKET_TOKEN' token!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* args_node = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!args_node) {
        PARSE_ERROR("Can't create a base for the function's arguments!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, args_node);
    long func_scope;
    stack_top(&ctx->scopes.stack, (void**)&func_scope);
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    args_node->sinfo.s_id = ctx->scopes.s_id;

    forward_token(it, 1);
    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)args_node)) {
        PARSE_ERROR("Can't parse function's arguments!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (consume_token(it, RETURN_TYPE_TOKEN)) {
        forward_token(it, 1);
        ast_node_t* ret_type = AST_create_node(CURRENT_TOKEN);
        AST_add_node(name_node, ret_type);
        forward_token(it, 1);
    }

    name_node->sinfo.s_id = func_scope;
    name_node->sinfo.v_id = FNTB_add_info(
        name_node->t->body, node->t->flags.glob, ctx->carry.ptr ? 1 : 0, 0, func_scope, args_node, name_node->c, &smt->f
    );

    if (CURRENT_TOKEN->t_type == DELIMITER_TOKEN) {
        node->t->t_type = FUNC_PROT_TOKEN;
        stack_pop(&ctx->scopes.stack, NULL);
        return node;
    }

    ast_node_t* body_node = NULL;
    PRESERVE_AST_CARRY_ARG({ body_node = cpl_parse_scope(it, ctx, smt, carry); }, node);
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