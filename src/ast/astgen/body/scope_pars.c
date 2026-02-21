#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_line_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;
    ast_node_t* base = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a basic block for the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    ast_node_t* node = cpl_parse_element(it, ctx, smt);
    if (!node) {
        stack_pop(&ctx->scopes.stack, NULL);
        PARSE_ERROR("Error during a parse of the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    stack_pop(&ctx->scopes.stack, NULL);
    AST_add_node(base, node);
    return base;
}

ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));

    forward_token(it, 1);
    ast_node_t* node = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    if (node) stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    else {
        stack_pop(&ctx->scopes.stack, NULL);
        PARSE_ERROR("Error during a parse of the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_pop(&ctx->scopes.stack, NULL);
    return node;
}
