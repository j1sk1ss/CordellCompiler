#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_import(list_iter_t* it, sym_table_t* smt) {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("AST_create_node error!");
        return NULL;
    }
    
    forward_token(it, 1);
    ast_node_t* source_node = AST_create_node(CURRENT_TOKEN);
    if (!source_node) {
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 2);
    while (CURRENT_TOKEN && (CURRENT_TOKEN)->t_type != DELIMITER_TOKEN) {
        ast_node_t* fname = AST_create_node(CURRENT_TOKEN);
        if (!fname) {
            AST_unload(node);
            AST_unload(source_node);
            return NULL;
        }

        AST_add_node(source_node, fname);
        FNTB_add_info(fname->token->body, 1, 0, 0, NULL, NULL, &smt->f);
        forward_token(it, 1);
    }

    AST_add_node(node, source_node);
    return node;
}
