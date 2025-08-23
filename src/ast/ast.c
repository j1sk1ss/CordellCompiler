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
    if (!parent->child) parent->child = child;
    else {
        ast_node_t* sibling = parent->child;
        while (sibling->sibling) sibling = sibling->sibling;
        sibling->sibling = child;
    }

    return 1;
}

int AST_remove_node(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return 0;

    ast_node_t* prev = NULL;
    ast_node_t* current = parent->child;
    while (current) {
        if (current == child) {
            if (prev) prev->sibling = current->sibling;
            else parent->child = current->sibling;
            break;
        }

        prev = current;
        current = current->sibling;
    }

    child->sibling = NULL;
    return 1;
}

int AST_unload(ast_node_t* node) {
    if (!node) return 0;
    AST_unload(node->child);
    AST_unload(node->sibling);
    mm_free(node);
    return 1;
}
