#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_start(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }

    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        if (!TKN_isdecl(CURRENT_TOKEN)) forward_token(it, 1);
        else {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("cpl_parse_variable_declaration return NULL!");
                AST_unload(node);
                return NULL;
            }

            AST_add_node(node, arg);
        }
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    if (!body) {
        print_error("cpl_parse_block return NULL!");
        AST_unload(node);
        return NULL;
    }

    AST_add_node(node, body);

    string_t* main_name = create_string((char*)ctx->fentry);
    node->sinfo.v_id = FNTB_add_info(main_name, 1, 0, 1, NULL, NULL, &smt->f);
    destroy_string(main_name);

    return node;
}
