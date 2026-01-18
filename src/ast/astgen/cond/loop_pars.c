#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_loop(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", LOOP_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token during a parse of the '%s' statement!", LOOP_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    ast_node_t* branch = cpl_parse_scope(it, ctx, smt);
    if (!branch) {
        PARSE_ERROR("Error during parsing in the '%s' statement body!", LOOP_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, branch);
    return node;
}
