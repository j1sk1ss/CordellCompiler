#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_align(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    if (!consume_token(it, OPEN_BRACKET_TOKEN) || !consume_token(it, UNKNOWN_NUMERIC_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' and then the 'UNKNOWN_NUMERIC_TOKEN'!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* align = AST_create_node(CURRENT_TOKEN);
    if (!align) {
        PARSE_ERROR("Error during the '%s' argument value parsing! %s(<stmt>)!", ALIGN_COMMAND, ALIGN_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    short align_num = (short)align->t->body->to_llong(align->t->body);
    AST_unload(align);

    if (!consume_token(it, CLOSE_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the 'CLOSE_BRACKET_TOKEN'!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = NULL;
    if (consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_scope(it, ctx, smt, 0);
    else body = cpl_parse_line_scope(it, ctx, smt, carry);
    if (!body) {
        PARSE_ERROR("Can't create a body for the '%s' structure!", ALIGN_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    for (ast_node_t* decl = body->c; decl; decl = decl->siblings.n) {
        if (!TKN_is_decl(decl->t)) continue;
        VRTB_update_memory(decl->c->sinfo.v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, align_num, &smt->v);
    }

    return body;
}
