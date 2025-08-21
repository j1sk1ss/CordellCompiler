#include <optimization.h>

static int _find_assign(
    ast_node_t* root, char* varname, short s_id, int* status, int local
) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_assign(t, varname, s_id, status, local);
            continue;
        }

        switch (t->token->t_type) {
            case CASE_TOKEN:
            case SWITCH_TOKEN:
            case DEFAULT_TOKEN: _find_assign(t, varname, s_id, status, local);                 continue;
            case IF_TOKEN:
            case WHILE_TOKEN:   _find_assign(t->child->sibling, varname, s_id, status, local); continue;
            case FUNC_TOKEN: 
                if (!local) {
                    _find_assign(t->child->sibling->sibling, varname, s_id, status, local);
                }
            continue;
            default: break;
        }

        if (t->token->t_type == ASSIGN_TOKEN) {
            if (!str_strncmp(varname, t->child->token->value, TOKEN_MAX_SIZE)) {
                *status = 1;
                return 1;
            }
        }
    }

    return 1;
}

static int _change_decl(ast_node_t* root, char* varname, int value, int local, int offset) {
    if (!root) return 0;

    int index = 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (index++ < offset) continue;
        if (!t->token) {
            _change_decl(t, varname, value, local, 0);
            continue;
        }
        
        switch (t->token->t_type) {
            case CASE_TOKEN:       _change_decl(t, varname, value, local, 0); break;
            case INT_TYPE_TOKEN:
            case LONG_TYPE_TOKEN:
            case CHAR_TYPE_TOKEN: 
            case SHORT_TYPE_TOKEN: _change_decl(t, varname, value, local, 1); continue;
            case IF_TOKEN:
            case EXIT_TOKEN:
            case CALL_TOKEN:
            case PLUS_TOKEN:
            case MINUS_TOKEN:
            case BITOR_TOKEN:
            case WHILE_TOKEN:
            case ASSIGN_TOKEN:
            case DIVIDE_TOKEN:
            case BITAND_TOKEN:
            case RETURN_TOKEN:
            case SWITCH_TOKEN:
            case SYSCALL_TOKEN:
            case DEFAULT_TOKEN:
            case COMPARE_TOKEN:
            case NCOMPARE_TOKEN:
            case MULTIPLY_TOKEN:
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN:
            case BITMOVE_LEFT_TOKEN:
            case BITMOVE_RIGHT_TOKEN:
            case ARRAY_TYPE_TOKEN: _change_decl(t, varname, value, local, 0); continue;
            case FUNC_TOKEN: 
                if (!local) {
                    _change_decl(t->child->sibling->sibling, varname, value, local, 0);
                }
            continue;
            default: break;
        }

        if (!str_strncmp(varname, (char*)t->token->value, TOKEN_MAX_SIZE)) {
            snprintf((char*)t->token->value, TOKEN_MAX_SIZE, "%d", value);
            t->token->t_type = t->token->t_type != CASE_TOKEN ? UNKNOWN_NUMERIC_TOKEN : CASE_TOKEN;
            t->token->glob = t->token->t_type != CASE_TOKEN ? 1 : 0;
        }   
    }

    return 1;
}

static int _find_decl(ast_node_t* root, ast_node_t* entry, int* change) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_decl(t, entry, change);
            continue;
        }

        switch (t->token->t_type) {
            case IF_TOKEN:
            case WHILE_TOKEN: 
            case SWITCH_TOKEN: _find_decl(t->child->sibling, entry, change);          continue;
            case FUNC_TOKEN:   _find_decl(t->child->sibling->sibling, entry, change); continue;
            default: break;
        }

        if (VRS_isdecl(t->token) && VRS_one_slot(t->token)) { /* Can be optimized */
            int is_changed = 0;
            ast_node_t* name_node = t->child;
            if (!VRS_intext(t->token)) _find_assign(entry, name_node->token->value, &is_changed, name_node->info.s_id, 0);
            else _find_assign(root, name_node->token->value, &is_changed, name_node->info.s_id, 1);

            if (!is_changed) {
                ast_node_t* val_node = name_node->sibling;
                if (val_node->token->t_type != UNKNOWN_NUMERIC_TOKEN) {
                    continue; /* Can be replaced by value */
                }
                
                int value = str_atoi(val_node->token->value);
                if (!VRS_intext(t->token)) _change_decl(entry, name_node->token->value, value, 0, 0);
                else _change_decl(root, name_node->token->value, value, 1, 0);
                *change = 1;
            }
        }
    }

    return 1;
}

int assign_optimization(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int is_changed = 0;
    _find_decl(ctx->r, ctx->r, &is_changed);
    return is_changed;
}
