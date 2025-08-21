#include <optimization.h>

static int _find_usage(ast_node_t* root, const char* varname, int* status, int local, int offset) {
    if (!root) return 0;

    int index = 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (index++ < offset) continue;
        if (!t->token) {
            _find_usage(t, varname, status, local, 0);
            continue;
        }
        
        if (t->token->ptr) {
            _find_usage(t, varname, status, local, 0);
        }
        
        switch (t->token->t_type) {
            case STR_TYPE_TOKEN:
            case LONG_TYPE_TOKEN:
            case INT_TYPE_TOKEN:
            case CHAR_TYPE_TOKEN: 
            case SHORT_TYPE_TOKEN: _find_usage(t, varname, status, local, 1); continue;
            case IF_TOKEN:
            case EXIT_TOKEN:
            case CALL_TOKEN:
            case PLUS_TOKEN:
            case ASSIGN_TOKEN:
            case MINUS_TOKEN:
            case BITOR_TOKEN:
            case WHILE_TOKEN:
            case LOWER_TOKEN:
            case SWITCH_TOKEN:
            case LARGER_TOKEN:
            case DIVIDE_TOKEN:
            case BITAND_TOKEN:
            case RETURN_TOKEN:
            case DEFAULT_TOKEN:
            case SYSCALL_TOKEN:
            case COMPARE_TOKEN:
            case NCOMPARE_TOKEN:
            case MULTIPLY_TOKEN:
            case ARR_VARIABLE_TOKEN:
            case BITMOVE_LEFT_TOKEN:
            case BITMOVE_RIGHT_TOKEN: _find_usage(t, varname, status, local, 0); continue;
            case CASE_TOKEN:
            case ARRAY_TYPE_TOKEN: _find_usage(t, varname, status, local, 0); break;
            case FUNC_TOKEN: if (!local) _find_usage(t->child->sibling->sibling, varname, status, local, 0); continue;
            default: break;
        }
        
        if (t->token->t_type == STRING_VALUE_TOKEN || t->token->t_type == CHAR_VALUE_TOKEN) continue;
        if (!str_strncmp(varname, (char*)t->token->value, TOKEN_MAX_SIZE)) {
            *status = 1;
            return 1;
        }
    }

    return 1;
}

static int _find_decl(ast_node_t* root, ast_node_t* entry, int* delete) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_decl(t, entry, delete);
            continue;
        }

        switch (t->token->t_type) {
            case CASE_TOKEN:   _find_decl(t, entry, delete); break; 
            case SWITCH_TOKEN: _find_decl(t->child->sibling, entry, delete); continue; 
            case IF_TOKEN:
            case WHILE_TOKEN:  _find_decl(t->child->sibling, entry, delete); continue;
            case FUNC_TOKEN:   _find_decl(t->child->sibling->sibling, entry, delete); continue;
            default: break;
        }

        if (VRS_isdecl(t->token)) {
            int is_used = 0;
            ast_node_t* name_node = t->child;
            if (t->token->ro || t->token->glob) _find_usage(entry, (char*)name_node->token->value, &is_used, 0, 0);
            else _find_usage(root, (char*)name_node->token->value, &is_used, 1, 0);

            if (!is_used) {
                *delete = 1;
                AST_remove_node(root, t);
                AST_unload(t);
            }
        }
    }

    return 1;
}

int OPT_varuse(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int delete = 0;
    do {
        delete = 0;
        _find_decl(ctx->r, ctx->r, &delete);
    } while (delete);
    return 1;
}
