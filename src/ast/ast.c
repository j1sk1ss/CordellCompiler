#include <ast/ast.h>

ast_node_t* AST_create_node(token_t* tkn) {
    ast_node_t* node = mm_malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(ast_node_t));
    node->token = tkn;
    return node;
}

ast_node_t* AST_copy_node(ast_node_t* n, int sp, int sib, int chld) {
    if (!n) return NULL;
    ast_node_t* dst = AST_create_node(n->token);
    if (!dst) return NULL;
    if (sp)   dst->parent  = n->parent;
    if (sib)  dst->sibling = AST_copy_node(n->sibling, sp, sib, chld);
    if (chld) dst->child   = AST_copy_node(n->child, 1, 1, 1);
    return dst;
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

    int found = 0;
    ast_node_t* prev    = NULL;
    ast_node_t* current = parent->child;
    while (current) {
        if (current == child) {
            if (prev) prev->sibling = current->sibling;
            else parent->child = current->sibling;
            found = 1;
            break;
        }

        prev    = current;
        current = current->sibling;
    }

    if (found) {
        child->sibling = NULL;
        child->parent  = NULL;
    }

    return 1;
}

int AST_unload(ast_node_t* node) {
    if (!node) return 0;
    AST_unload(node->child);
    AST_unload(node->sibling);
    mm_free(node->token);
    mm_free(node);
    return 1;
}
