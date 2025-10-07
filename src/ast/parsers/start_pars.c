#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_start(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;

    forward_token(it, 1);
    while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
        if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        if (!VRS_isdecl(((token_t*)list_iter_current(it)))) forward_token(it, 1);
        else {
            ast_node_t* arg = cpl_parse_variable_declaration(it, ctx, smt);
            if (!arg) {
                print_error("AST error during function arg parsing! line=%i", ((token_t*)list_iter_current(it))->lnum);
                AST_unload(node);
                return NULL;
            }

            AST_add_node(node, arg);
        }
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    AST_add_node(node, body);

    node->sinfo.v_id = FNTB_add_info("_start", 1, 0, NULL, NULL, &smt->f);
    return node;
}
