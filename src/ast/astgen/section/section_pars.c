#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_section(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    
    if (!consume_token(it, OPEN_BRACKET_TOKEN) || !consume_token(it, STRING_VALUE_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' and then the 'STRING_VALUE_TOKEN'!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* section = AST_create_node(CURRENT_TOKEN);
    if (!section) {
        PARSE_ERROR("Error during the '%s' argument value parsing! %s(<stmt>)!", SECTION_COMMAND, SECTION_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

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
        if (TKN_is_decl(decl->t)) {
            SCTB_add_to_section(section->t->body, decl->c->sinfo.v_id, SECTION_ELEMENT_VARIABLE, &smt->c);
        }
        else if (decl->t->t_type == FUNC_TOKEN) {
            SCTB_add_to_section(section->t->body, decl->c->sinfo.v_id, SECTION_ELEMENT_FUNCTION, &smt->c);
        }
    }

    AST_unload(section);
    return body;
}
