#include <ast.h>

ast_node_t* AST_create_node(token_t* tkn) {
    ast_node_t* node = mm_malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(ast_node_t));
    node->token = tkn;
    return node;
}

int AST_add_node(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return 0;
    child->parent = parent;
    if (!parent->first_child) parent->first_child = child;
    else {
        ast_node_t* sibling = parent->first_child;
        while (sibling->next_sibling) sibling = sibling->next_sibling;
        sibling->next_sibling = child;
    }

    return 1;
}

int AST_remove_node(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return 0;

    ast_node_t* prev = NULL;
    ast_node_t* current = parent->first_child;
    while (current) {
        if (current == child) {
            if (prev) prev->next_sibling = current->next_sibling;
            else parent->first_child = current->next_sibling;
            break;
        }

        prev = current;
        current = current->next_sibling;
    }

    return 1;
}

int AST_unload(ast_node_t* node) {
    if (!node) return 0;
    AST_unload(node->first_child);
    AST_unload(node->next_sibling);
    mm_free(node);
    return 1;
}
