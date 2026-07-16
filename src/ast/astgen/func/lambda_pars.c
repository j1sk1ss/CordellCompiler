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
    args->sinfo.s_id = SCPTB_push_scope(&smt->sc, &ctx->scopes.stack);

    symbol_id_t preserved_tid = ctx->t_id;
    ctx->t_id = NO_SYMBOL_ID;

    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)args)) {
        PARSE_ERROR("Can't parse lambdas's arguments!");
        AST_unload(base);
        ctx->t_id = preserved_tid;
        stack_pop(&ctx->scopes.stack, NULL);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, LAMBDA_TOKEN)) {
        PARSE_ERROR("Expected the 'LAMBDA_TOKEN'!");
        AST_unload(base);
        ctx->t_id = preserved_tid;
        stack_pop(&ctx->scopes.stack, NULL);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    string_t* anon_name = create_string("__anon_function_lambda");
    base->sinfo.v_id = FNTB_add_info(anon_name, NULL,  (func_info_flags_t){ .local = 1 }, base->sinfo.s_id, args, NULL, &smt->f);
    FNTB_add_local(ctx->carry.pfunc, base->sinfo.v_id, &smt->f);
    destroy_string(anon_name);

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ 
        if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
        else body = cpl_parse_scope(it, ctx, smt, 1);
     }, base->sinfo.v_id);
    if (body) AST_add_node(args, body);
    else {
        PARSE_ERROR("Error during the lambdas's body parsing!");
        AST_unload(base);
        ctx->t_id = preserved_tid;
        stack_pop(&ctx->scopes.stack, NULL);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ctx->t_id = preserved_tid;
    stack_pop(&ctx->scopes.stack, NULL);
    return base;
}
