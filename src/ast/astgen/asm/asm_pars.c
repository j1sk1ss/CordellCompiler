#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_asm(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the '%s' structure!", ASM_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token while the parse of the '%s' statement!", ASM_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1); 
    ast_node_t* args = cpl_parse_call_arguments(it, ctx, smt, 0);
    if (args) AST_add_node(base, args);
    else {
        PARSE_ERROR("Error during the '%s' argument value parsing! %s(<stmt>)!", ASM_COMMAND, ASM_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (body) AST_add_node(base, body);
    else {
        PARSE_ERROR("Can't create a body for the '%s' structure!", ASM_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' token while the parse of the '%s' statement!", ASM_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    do {
        if (!consume_token(it, STRING_VALUE_TOKEN)) {
            PARSE_ERROR("Expected a string value in the '%s's body!", ASM_COMMAND);
            AST_unload(base);
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
            AST_unload(base);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        consume_token(it, COMMA_TOKEN);
    } while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN);
    forward_token(it, 1);
    return base;
}
