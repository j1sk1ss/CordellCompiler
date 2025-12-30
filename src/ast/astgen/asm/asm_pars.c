/* ASM block related stuff.
   - 'asm' keyword */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the asm structure!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
            if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
            if (arg) AST_add_node(node, arg);
            else { print_error("Error during the asm structure provided value parsing! asm(<stmt>)!"); }
        }
    }

    ast_node_t* body = AST_create_node(NULL);
    if (!body) {
        print_error("Can't create a body for the asm structure!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(it, 1);
        while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN) {
            if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            int sid = STTB_add_info(CURRENT_TOKEN->body, STR_RAW_ASM, &smt->s);
            ast_node_t* arg = AST_create_node(CURRENT_TOKEN);
            if (!arg) { print_error("Can't create a body for the asm string!"); }
            else {
                arg->sinfo.v_id = sid;
                AST_add_node(body, arg);
            }

            forward_token(it, 1);
        }
    }

    forward_token(it, 1);
    AST_add_node(node, body);
    return node;
}
