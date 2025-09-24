#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
        if ((*curr)->t_type == COMMA_TOKEN) {
            forward_token(curr, 1);
            continue;
        }

        if (!VRS_isdecl((*curr))) forward_token(curr, 1);
        else {
            ast_node_t* arg = cpl_parse_variable_declaration(curr, ctx, smt);
            if (!arg) {
                print_error("AST error during function arg parsing! line=%i", (*curr)->lnum);
                AST_unload(node);
                return NULL;
            }

            AST_add_node(node, arg);
        }
    }

    forward_token(curr, 1);
    ast_node_t* body = cpl_parse_block(curr, ctx, smt, CLOSE_BLOCK_TOKEN);
    AST_add_node(node, body);

    node->sinfo.v_id = FNTB_add_info("start", 1, 0, NULL, NULL, &smt->f);
    return node;
}
