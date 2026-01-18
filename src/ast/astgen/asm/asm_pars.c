/* ASM block related stuff.
   - 'asm' keyword */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' structure!", ASM_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    /* asm ( */
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token while the parse of the '%s' statement!", ASM_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    /* asm ( ... ) */
    forward_token(it, 1); 
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        if (arg) AST_add_node(node, arg);
        else { 
            PARSE_ERROR("Error during the '%s' argument value parsing! %s(<stmt>)!", ASM_COMMAND, ASM_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }

    ast_node_t* body = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!body) {
        PARSE_ERROR("Can't create a body for the '%s' structure!", ASM_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, body);

    /* asm ( ... ) { */
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token while the parse of the '%s' statement!", ASM_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    /* asm ( ... ) { ... */
    do {
        if (!consume_token(it, STRING_VALUE_TOKEN)) {
            PARSE_ERROR("Expected a string value in the '%s's body!", ASM_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        int sid = STTB_add_info(CURRENT_TOKEN->body, STR_RAW_ASM, &smt->s);
        ast_node_t* arg = AST_create_node(CURRENT_TOKEN);
        if (arg && sid >= 0) { 
            arg->sinfo.v_id = sid;
            AST_add_node(body, arg);
        }
        else {
            PARSE_ERROR("Can't create a body for the '%s'-string!", ASM_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (consume_token(it, COMMA_TOKEN)) {
            forward_token(it, 1);
        }
    } while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN);

    forward_token(it, 1); /* Move from the parser */
    return node;
}
