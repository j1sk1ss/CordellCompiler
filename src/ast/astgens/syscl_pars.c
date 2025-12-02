#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_syscall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type == OPEN_BRACKET_TOKEN) {
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

    return node;
}

ast_node_t* cpl_parse_breakpoint(list_iter_t* it) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }

    forward_token(it, 1);
    return node;
}
