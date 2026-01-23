/* The 'start' keyword parser
   - start */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_start(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", START_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token during a parse of the '%s' statement!", START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    do {
        if (CURRENT_TOKEN->t_type == CLOSE_BRACKET_TOKEN) break;
        if (TKN_isdecl(CURRENT_TOKEN)) {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                PARSE_ERROR("Error during the '%s' statement argument parsing! %s(<type> <name>)!", START_COMMAND, START_COMMAND);
                AST_unload(node);
                RESTORE_TOKEN_POINT;
                return NULL;
            }

            AST_add_node(node, arg);
        }
        else {
            PARSE_ERROR("Error during the '%s' statement argument parsing! %s(<type> <name>)!", START_COMMAND, START_COMMAND);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    } while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN);

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' in a body of the '%s' statement! %s( ... ) { ... }!", START_COMMAND, START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = cpl_parse_scope(it, ctx, smt);
    if (!body) {
        PARSE_ERROR("Error during the parsing of the '%s' body!", START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, body);

    string_t* main_name = create_string(ctx->fentry);
    if (FNTB_get_info(main_name, NULL, &smt->f)) {
        PARSE_ERROR("The main function already exists!");
        AST_unload(node);
        destroy_string(main_name);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    node->sinfo.v_id = FNTB_add_info(main_name, 1, 0, 1, NULL, NULL, &smt->f);
    destroy_string(main_name);
    return node;
}
