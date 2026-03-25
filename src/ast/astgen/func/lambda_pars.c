#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_lambda(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    ast_node_t* base = AST_create_node_bt(CREATE_LAMBDA_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the lambda!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* args = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (args) AST_add_node(base, args);
    else {
        PARSE_ERROR("Can't create a base for the lambdas's arguments!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    args->sinfo.s_id = ctx->scopes.s_id;

    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)args)) {
        PARSE_ERROR("Can't parse lambdas's arguments!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, LAMBDA_TOKEN)) {
        PARSE_ERROR("Expected the 'LAMBDA_TOKEN'!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ 
        if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
        else body = cpl_parse_scope(it, ctx, smt, 1);
     }, base);
    if (body) AST_add_node(args, body);
    else {
        PARSE_ERROR("Error during the lambdas's body parsing!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    string_t* anon_name = create_string("__anon_function_lambda");
    base->sinfo.v_id = FNTB_add_info(anon_name, NULL,  0, 1, 0, 0,  base->sinfo.s_id, args, NULL, &smt->f);

    destroy_string(anon_name);
    stack_pop(&ctx->scopes.stack, NULL);
    return base;
}
