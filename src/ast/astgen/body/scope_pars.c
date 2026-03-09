#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_line_scope(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    ast_node_t* base = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a basic block for the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (carry) stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    ast_node_t* body = cpl_parse_element(it, ctx, smt, carry);
    if (body) AST_add_node(base, body);
    else {
        if (carry) stack_pop(&ctx->scopes.stack, NULL);
        PARSE_ERROR("Error during a parse of the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    if (carry) stack_pop(&ctx->scopes.stack, NULL);
    return base;
}

ast_node_t* cpl_parse_scope(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    if (carry) stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    if (CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) forward_token(it, 1);
    else {
        PARSE_ERROR("Expect the 'OPEN_BLOCK_TOKEN' token!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    if (body) stack_top(&ctx->scopes.stack, (void**)&body->sinfo.s_id);
    else {
        if (carry) stack_pop(&ctx->scopes.stack, NULL);
        PARSE_ERROR("Error during a parse of the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (carry) stack_pop(&ctx->scopes.stack, NULL);
    return body;
}
