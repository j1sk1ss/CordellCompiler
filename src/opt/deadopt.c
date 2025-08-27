#include <deadopt.h>

typedef struct {
    code_node_t*      bh;
    func_code_node_t* fh;
} deadopt_ctx_t;

static code_node_t* _create_code_node(ast_node_t* start) {
    if (!start) return NULL;
    code_node_t* node = (code_node_t*)mm_malloc(sizeof(code_node_t));
    if (!node) return NULL;
    str_memset(node, 0, sizeof(code_node_t));
    node->start = start;
    return node;
}

static int _add_child_to_block(code_node_t* p, code_node_t* c) {
    if (!p || !c) return 0;
    c->parent = p;
    if (!p->child) p->child = c;
    else {
        code_node_t* sibling = p->child;
        while (sibling->sibling) sibling = sibling->sibling;
        sibling->sibling = c;
    }

    return 1;
}

static int _unload_code_block(code_node_t* node) {
    if (!node) return 0;
    _unload_code_block(node->child);
    _unload_code_block(node->sibling);
    mm_free(node);
    return 1;
}

static func_code_node_t* _create_func_code_block(char* name, code_node_t* head) {
    if (!name || !head) return NULL;
    func_code_node_t* node = (func_code_node_t*)mm_malloc(sizeof(func_code_node_t));
    if (!node) return NULL;

    str_strncpy(node->name, name, TOKEN_MAX_SIZE);
    node->head = head;
    node->next = NULL;
    return node;
}

static int _add_func_code_block(char* name, code_node_t* head, deadopt_ctx_t* dctx) {
    func_code_node_t* node = _create_func_code_block(name, head);
    if (!node) return 0;

    if (!dctx->fh) dctx->fh = node;
    else {
        func_code_node_t* h = dctx->fh;
        while (h->next) h = h->next;
        h->next = node;
    }

    return 1;
}

static func_code_node_t* _find_func_block(char* name, deadopt_ctx_t* dctx) {
    func_code_node_t* h = dctx->fh;
    while (h) {
        if (!str_strncmp(h->name, name, TOKEN_MAX_SIZE)) return h;
        h = h->next;
    }
    return NULL;
}

static int _unload_func_node_map(deadopt_ctx_t* dctx) {
    while (dctx->fh) {
        func_code_node_t* n = dctx->fh->next;
        _unload_code_block(dctx->fh->head);
        mm_free(dctx->fh);
        dctx->fh = n;
    }

    return 1;
}

static int _collect_func_table(deadopt_ctx_t* dctx, ast_node_t* curr) {
    if (!curr) return 0;
    for (ast_node_t* t = curr; t; t = t->sibling) {
        if (!t->token) continue;
        if (t->token->t_type == FUNC_TOKEN) {
            ast_node_t* name_node   = t->child;
            ast_node_t* params_node = name_node->sibling;
            ast_node_t* body_node   = params_node->sibling;
            code_node_t* stub = _create_code_node(body_node);
            _add_func_code_block(name_node->token->value, stub, dctx);
        }
    }
    return 1;
}

static code_node_t* _generate_blocks(deadopt_ctx_t* dctx, ast_node_t* curr) {
    if (!curr) return NULL;
    code_node_t* head = _create_code_node(curr);
    if (!head) return NULL;

    ast_node_t* curr_node = curr;
    for (ast_node_t* t = curr; t; t = t->sibling, curr_node = curr_node->sibling) {
        if (!t->token) continue;
        switch (t->token->t_type) {
            case SWITCH_TOKEN:
            case WHILE_TOKEN:
            case IF_TOKEN: break;

            case CALL_TOKEN: {
                func_code_node_t* fn = _find_func_block(t->token->value, dctx);
                if (fn && fn->head) _add_child_to_block(head, fn->head);
                break;
            }

            case RETURN_TOKEN:
            case EXIT_TOKEN: goto dead_flow;
            default: break;
        }
    }

dead_flow:
    head->end = curr_node;
    return head;
}

int OPT_deadcode(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    deadopt_ctx_t dctx = { .bh = NULL, .fh = NULL };

    _collect_func_table(&dctx, ctx->r->child);
    dctx.bh = _generate_blocks(&dctx, ctx->r->child);

    _unload_func_node_map(&dctx);
    _unload_code_block(dctx.bh);

    return 1;
}
