#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_line_scope, {
    ast_node_t* base = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a basic block for a scope block!");

    if (carry) SCPTB_push_scope(&smt->sc, &ctx->scopes.stack);
    ast_node_t* body = cpl_parse_element(it, ctx, smt, carry);
    PARSER_ASSERT_DO(!body, "Error during parse of a scope block!", { if (carry) stack_pop(&ctx->scopes.stack, NULL); });
    AST_add_node(base, body);

    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    if (carry) stack_pop(&ctx->scopes.stack, NULL);
    if (CURRENT_TOKEN->t_type == DELIMITER_TOKEN) {
        forward_token(it, 1);
    }
    
    return base;
})

DEFINE_PARSER(cpl_parse_scope, {
    if (carry) SCPTB_push_scope(&smt->sc, &ctx->scopes.stack);
    PARSER_ASSERT_DO(
        CURRENT_TOKEN->t_type != OPEN_BLOCK_TOKEN, "Expect the '{' token!", 
        { if (carry) stack_pop(&ctx->scopes.stack, NULL); }
    );
    
    forward_token(it, 1);

    ast_node_t* body = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    PARSER_ASSERT_DO(!body, "Error during parse of a scope block!", { if (carry) stack_pop(&ctx->scopes.stack, NULL); });
    if (carry) stack_pop(&ctx->scopes.stack, NULL);
    return body;
})
