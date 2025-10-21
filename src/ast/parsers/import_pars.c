#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_import(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;
    
    forward_token(it, 1);
    ast_node_t* source_node = AST_create_node((token_t*)list_iter_current(it));
    if (!source_node) {
        AST_unload(node);
        return NULL;
    }

    forward_token(it, 2);
    while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != DELIMITER_TOKEN) {
        ast_node_t* fname = AST_create_node((token_t*)list_iter_current(it));
        if (!fname) {
            AST_unload(node);
            AST_unload(source_node);
            return NULL;
        }

        AST_add_node(source_node, fname);
        forward_token(it, 1); /* TODO: Symtab function add */
    }

    AST_add_node(node, source_node);
    return node;
}
