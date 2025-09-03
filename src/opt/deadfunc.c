#include <deadfunc.h>

static int _find_func_usage_file(ast_node_t* root, const char* func, int* is_used) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_func_usage_file(t, func, is_used);
            continue;
        }

        switch (t->token->t_type) {
            case FUNC_TOKEN: {
                if (str_strcmp(t->child->token->value, func)) {
                    ast_node_t* b = t->child;
                    for (; b && b->token->t_type != SCOPE_TOKEN; b = b->sibling);
                    _find_func_usage_file(b, func, is_used);
                }

                break;
            }

            case IMPORT_SELECT_TOKEN: continue;
            default: _find_func_usage_file(t, func, is_used); break;
        }

        if (t->token->t_type == CALL_TOKEN) {
            if (!str_strcmp(t->token->value, func)) {
                *is_used = 1;
                return 1;
            }
        }
    }

    return 1;
}

static int _find_func_usage(const char* func, int* is_used, deadfunc_ctx_t* dctx) {
    for (char i = dctx->size - 1; i >= 0; i--) {
        _find_func_usage_file(dctx->ctx[i]->r, func, is_used);
    }

    return 1;
}

static int _find_func(ast_node_t* root, int* delete, deadfunc_ctx_t* dctx) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_func(t, delete, dctx);
            continue;
        }

        switch (t->token->t_type) {
            case FUNC_TOKEN: 
                int used = 0;    
                _find_func_usage(t->child->token->value, &used, dctx);
                if (used) {
                    ast_node_t* b = t->child;
                    for (; b && b->token->t_type != SCOPE_TOKEN; b = b->sibling);
                    _find_func(b, delete, dctx);
                }
                else {
                    AST_remove_node(root, t);
                    AST_unload(t);
                    *delete = 1;
                }
            break;
            default: _find_func(t, delete, dctx); break;
        }
    }

    return 1;
}

int OPT_deadfunc_add(syntax_ctx_t* ctx, deadfunc_ctx_t* dctx) {
    if (!ctx->r) return 0;
    if (dctx->size + 1 >= DCTX_AST_MAX) return 0;
    dctx->ctx[dctx->size++] = ctx;
    return 1;
}

int OPT_deadfunc_clear(deadfunc_ctx_t* dctx) {
    for (int i = dctx->size - 1; i >= 0; i--) {
        int is_delete = 0;
        do {
            is_delete = 0;
            _find_func(dctx->ctx[i]->r, &is_delete, dctx);
        } while (is_delete);
    }

    return 1;
}
