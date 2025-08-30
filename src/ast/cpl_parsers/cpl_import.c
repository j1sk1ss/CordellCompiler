#include <cpl_parser.h>

ast_node_t* cpl_parse_import(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    /* "from" statement -> "source path" */
    forward_token(curr, 1);
    ast_node_t* source_node = AST_create_node(*curr); // example: src.cpl
    if (!source_node) {
        AST_unload(node);
        return NULL;
    }

    /* Skip "import" statement to list of functions */
    forward_token(curr, 2);
    while (*curr && (*curr)->t_type != DELIMITER_TOKEN) {
        ast_node_t* fname = AST_create_node(*curr); // example: func1
        if (!fname) {
            AST_unload(node);
            AST_unload(source_node);
            return NULL;
        }

        AST_add_node(source_node, fname);
        forward_token(curr, 1);
    }

    AST_add_node(node, source_node);
    return node;
}