#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_switch(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for a '%s' structure!", SWITCH_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
 
    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);

    forward_token(it, 1);
    ast_node_t* stmt = cpl_parse_expression(it, ctx, smt, 1);
    if (!stmt) {
        PARSE_ERROR("Error during the parsing of the '%s' statement! %s <stmt>!", SWITCH_COMMAND, SWITCH_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, stmt);

    ast_node_t* cases_scope = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!cases_scope) {
        PARSE_ERROR("Can't create a base for the case scope!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, cases_scope);

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token during a parse of the '%s' statement!", SWITCH_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    while (
        CURRENT_TOKEN->t_type == CASE_TOKEN || 
        CURRENT_TOKEN->t_type == DEFAULT_TOKEN
    ) {
        ast_node_t* case_node = AST_create_node(CURRENT_TOKEN);
        if (!case_node) {
            PARSE_ERROR("Can't create a base for the case in the '%s' statement!", SWITCH_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (CURRENT_TOKEN->t_type == CASE_TOKEN) {
            forward_token(it, 1);
            ast_node_t* case_stmt = cpl_parse_expression(it, ctx, smt, 1);
            AST_add_node(case_node, case_stmt);
        }

        if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
            PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token during a parse of the '%s' statement!", SWITCH_COMMAND);
            AST_unload(case_node);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        ast_node_t* case_body = cpl_parse_scope(it, ctx, smt);
        if (!case_body) {
            PARSE_ERROR("Error during the parsing process for the case!");
            AST_unload(case_node);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(case_node, case_body);
        AST_add_node(cases_scope, case_node);
    }

    forward_token(it, 1);
    return node;
}
