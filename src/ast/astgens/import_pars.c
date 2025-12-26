#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_import(list_iter_t* it, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        print_error("Can't create a base for the import statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    ast_node_t* source_node = AST_create_node(CURRENT_TOKEN);
    if (!source_node) {
        print_error("Can't create a base for the source file!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 2);
    while (CURRENT_TOKEN && (CURRENT_TOKEN)->t_type != DELIMITER_TOKEN) {
        ast_node_t* import_name = AST_create_node(CURRENT_TOKEN);
        if (!import_name) {
            print_error("Can't create a base for the function name!");
            AST_unload(node);
            AST_unload(source_node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(source_node, import_name);
        FNTB_add_info(import_name->t->body, 1, 0, 0, NULL, NULL, &smt->f);
        forward_token(it, 1);
    }

    AST_add_node(node, source_node);
    return node;
}
