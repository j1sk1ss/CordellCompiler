#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_lambda, {
    ast_node_t* base = AST_create_node_bt(CREATE_LAMBDA_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the lambda!");

    ast_node_t* args = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    PARSER_DO_OR_THROW(!args, base, "Can't create a base for the lambdas's arguments!");
    AST_add_node(base, args);

    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    args->sinfo.s_id = SCPTB_push_scope(&smt->sc, &ctx->scopes.stack);

    symbol_id_t preserved_tid = ctx->t_id;
    ctx->t_id = NO_SYMBOL_ID;

    PARSER_DO_OR_THROW_DO(
        !cpl_parse_funcdef_args(it, ctx, smt, (long)args), "Can't parse lambdas's arguments!", 
        { AST_unload(base); ctx->t_id = preserved_tid; stack_pop(&ctx->scopes.stack, NULL); }
    );

    PARSER_DO_OR_THROW_DO(
        !consume_token(it, LAMBDA_TOKEN), "Expected the 'LAMBDA_TOKEN'!",
        { AST_unload(base); ctx->t_id = preserved_tid; stack_pop(&ctx->scopes.stack, NULL); }
    );

    string_t* anon_name = create_string("__anon_function_lambda");
    base->sinfo.v_id = FNTB_add_info(anon_name, NULL,  (func_info_flags_t){ .local = 1 }, base->sinfo.s_id, args, NULL, &smt->f);
    FNTB_add_local(ctx->carry.pfunc, base->sinfo.v_id, &smt->f);
    destroy_string(anon_name);

    ast_node_t* body = NULL;
    PRESERVE_AST_CARRY_ARG({ 
        if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
        else body = cpl_parse_scope(it, ctx, smt, 1);
     }, base->sinfo.v_id);

    PARSER_DO_OR_THROW_DO(
        !body, "Error during the lambdas's body parsing!", 
        { AST_unload(base); ctx->t_id = preserved_tid; stack_pop(&ctx->scopes.stack, NULL); }
    );
    AST_add_node(args, body);

    ctx->t_id = preserved_tid;
    stack_pop(&ctx->scopes.stack, NULL);
    return base;
})
