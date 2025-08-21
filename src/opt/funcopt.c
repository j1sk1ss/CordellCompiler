#include <optimization.h>

static ast_node_t* _ast[100] = { NULL };
static int _ast_count = 0;

int funcopt_add_ast(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    _ast[_ast_count++] = ctx->r;
    return 1;
}

static int _find_func_usage_file(ast_node_t* root, const char* func, int* is_used) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_func_usage_file(t, func, is_used);
            continue;
        }

        switch (t->token->t_type) {
            case FUNC_TOKEN: 
                if (str_strcmp((char*)t->child->token->value, func)) {
                    _find_func_usage_file(t->child->sibling->sibling, func, is_used);
                }
            break;
            case IMPORT_SELECT_TOKEN: continue;
            default: _find_func_usage_file(t, func, is_used); break;
        }

        if (t->token->t_type == CALL_TOKEN) {
            if (!str_strcmp((char*)t->token->value, func)) {
                *is_used = 1;
                return 1;
            }
        }
    }

    return 1;
}

static int _find_func_usage(const char* func, int* is_used) {
    for (int i = _ast_count - 1; i >= 0; i--) _find_func_usage_file(_ast[i], func, is_used);
    return 1;
}

static int _find_func(ast_node_t* root, int* delete) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_func(t, delete);
            continue;
        }

        switch (t->token->t_type) {
            case FUNC_TOKEN: 
                int used = 0;    
                _find_func_usage((char*)t->child->token->value, &used);
                if (!used) {
                    AST_remove_node(root, t);
                    AST_unload(t);
                    *delete = 1;
                }
                else _find_func(t->child->sibling->sibling, delete);
            break;
            default: _find_func(t, delete); break;
        }
    }

    return 1;
}

int func_optimization() {
    for (int i = _ast_count - 1; i >= 0; i--) {
        int is_delete = 0;
        do {
            is_delete = 0;
            _find_func(_ast[i], &is_delete);
        } while (is_delete);
    }

    return 1;
}
