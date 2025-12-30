#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_exit(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create the base for either the exit statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    forward_token(it, 1);
    if (TKN_isclose(CURRENT_TOKEN)) {
        return node;
    }

    ast_node_t* exp_node = cpl_parse_expression(it, ctx, smt);
    if (!exp_node) {
        print_error("Error during the exit statement! exit <stmt>!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, exp_node);
    return node;
}