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

    if (consume_token(it, LAMBDA_TOKEN)) forward_token(it, 1);
    else {
        PARSE_ERROR("Expected the 'LAMBDA_TOKEN'!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ body = cpl_parse_scope(it, ctx, smt, 1); }, base);
    if (body) AST_add_node(args, body);
    else {
        PARSE_ERROR("Error during the lambdas's body parsing!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    char name[32];
    snprintf(name, sizeof(name), "__anon_function_lambda");
    string_t* anon_name = create_string(name);

    base->sinfo.v_id = FNTB_add_info(anon_name, NULL,  0, 1, 0, 0,  base->sinfo.s_id, args, NULL, &smt->f);

    destroy_string(anon_name);
    stack_pop(&ctx->scopes.stack, NULL);
    return base;
}
