#include <ast/ast.h>

ast_node_t* AST_create_node(token_t* tkn) {
    ast_node_t* node = mm_malloc(sizeof(ast_node_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(ast_node_t));
    node->t          = tkn;
    node->sinfo.v_id = NO_SYMBOL_ID;
    return node;
}

ast_node_t* AST_create_node_bt(token_t* tkn) {
    ast_node_t* node = AST_create_node(tkn);
    if (!node) return NULL;
    node->bt = tkn;
    return node;
}

ast_node_t* AST_copy_node(ast_node_t* n, int sp, int sib, int chld) {
    if (!n) return NULL;

    ast_node_t* dst = AST_create_node(TKN_copy_token(n->t));
    if (!dst) return NULL;
    dst->bt = dst->t;

    str_memcpy(&dst->sinfo, &n->sinfo, sizeof(syntax_info_t));
    if (chld) {
        dst->c = AST_copy_node(n->c, 0, 1, 1);
        if (dst->c) dst->c->p = dst;
    }

    if (sib) {
        dst->siblings.n = AST_copy_node(n->siblings.n, 0, 1, 1);
        ast_node_t* tail = dst->siblings.n;
        while (tail && tail->siblings.n)
            tail = tail->siblings.n;

        dst->siblings.t = tail;
    }

    if (sp) dst->p = NULL;
    return dst;
}

int AST_add_node(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return 0;
    child->p = parent;
    if (!parent->c) parent->c = child;
    else {
        ast_node_t* sibling = parent->c;
        if (!sibling->siblings.n) {
            sibling->siblings.n = child;
            sibling->siblings.t = child;
        }
        else {
            sibling->siblings.t->siblings.n = child;
            sibling->siblings.t = child;
        }
    }

    return 1;
}

int AST_remove_node(ast_node_t* parent, ast_node_t* child) {
    if (!parent || !child) return 0;

    int found = 0;
    ast_node_t* prev    = NULL;
    ast_node_t* current = parent->c;
    while (current) {
        if (current == child) {
            if (prev) prev->siblings.n = current->siblings.n;
            else parent->c = current->siblings.n;
            found = 1;
            break;
        }

        prev    = current;
        current = current->siblings.n;
    }

    if (found) {
        child->siblings.n = NULL;
        child->p = NULL;
    }

    return found;
}

int AST_unload(ast_node_t* node) {
    if (!node) return 0;
    AST_unload(node->c);
    AST_unload(node->siblings.n);
    TKN_unload_token(node->bt);
    mm_free(node);
    return 1;
}

/*
Recursively hash the input node.
Params:
    - `n` - Input node.
    - `s` - Hash the sibling node?
            Note: If 1 - This function will use
                  a hash from the sibling node.
    - `stp` - Stop token type.

Returns a hash based on the provided node.
*/
static unsigned long _hash_ast_node(ast_node_t* n, int s, token_type_t stp) {
    if (!n || n->t->t_type == stp) return 0;
    unsigned long hash = 0;
    if (n->t) hash ^= TKN_hash_token(n->t);
    if (s) hash ^= _hash_ast_node(n->siblings.n, 1, stp);
    hash ^= _hash_ast_node(n->c, 1, stp);
    return hash;
}

unsigned long AST_hash_node(ast_node_t* node) {
    return _hash_ast_node(node, 0, -1);
}

unsigned long AST_hash_node_stop(ast_node_t* node, token_type_t stp) {
    return _hash_ast_node(node, 1, stp);
}
