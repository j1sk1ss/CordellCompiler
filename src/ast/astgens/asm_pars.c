#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
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
            else { print_warn("cpl_parse_expression return NULL!"); }
        }
    }

    ast_node_t* body = AST_create_node(NULL);
    if (!body) {
        print_error("AST_create_node error!");
        AST_unload(node);
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
            if (!arg) print_warn("AST_create_node return NULL!");
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
