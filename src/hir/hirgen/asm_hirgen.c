#include <hir/hir.h>

static ast_node_t* _find_variable(ast_node_t* n, const char* name) {
    for (ast_node_t* t = n; t && t->token; t = t->sibling) {
        if (!str_strncmp(t->token->value, name, TOKEN_MAX_SIZE)) {
            return t;
        }
    }

    return NULL;
}

int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    ast_node_t* h = node->child;
    for (; h->token; h = h->sibling);

    for (ast_node_t* l = h->child; l; l = l->sibling) {
        HIR_BLOCK1(ctx, RAW, HIR_SUBJ_RAWASM(l->token->value));
    }

    return 1;
}
