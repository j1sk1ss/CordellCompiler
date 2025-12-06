#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = NULL;
    if (CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(it, 1);
        stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
        node = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
        if (node) {
            stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
            forward_token(it, 1);
        }
    }

    stack_pop(&ctx->scopes.stack, NULL);
    return node;
}
