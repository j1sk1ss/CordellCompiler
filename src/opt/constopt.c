#include <constopt.h>

static int _find_muldiv(ast_node_t* root, int* fold) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
#pragma region Navigation
        if (!t->token || t->token->t_type == SCOPE_TOKEN) {
            _find_muldiv(t, fold);
            continue;
        }

        switch (t->token->t_type) {
            case IF_TOKEN:
            case CASE_TOKEN:
            case EXIT_TOKEN:
            case CALL_TOKEN:
            case WHILE_TOKEN:
            case RETURN_TOKEN:
            case SWITCH_TOKEN:
            case SYSCALL_TOKEN:
            case DEFAULT_TOKEN:
            case ARRAY_TYPE_TOKEN: _find_muldiv(t, fold);                          continue;
            case FUNC_TOKEN:       _find_muldiv(t->child->sibling->sibling, fold); continue;
            default: break;
        }
#pragma endregion
        if (VRS_isoperand(t->token) || VRS_isdecl(t->token)) {
            _find_muldiv(t, fold);

            ast_node_t* left = t->child;
            ast_node_t* right = left->sibling;

            if ( /* Multiplication and division optimisation after folding with bitmove operations */
                t->token->t_type == MULTIPLY_TOKEN || 
                t->token->t_type == DIVIDE_TOKEN
            ) {
                if (right->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int right_val = str_atoi(right->token->value);
                    if ((right_val & (right_val - 1)) != 0) continue;
                    
                    int shift = 0;
                    while (right_val >>= 1) {
                        shift++;
                    }
                    
                    t->token->t_type = (t->token->t_type == MULTIPLY_TOKEN) ? BITMOVE_LEFT_TOKEN : BITMOVE_RIGHT_TOKEN;
                    snprintf(right->token->value, TOKEN_MAX_SIZE, "%d", shift);
                }
            }

            if ( /* We can't perform folding */
                left->token->t_type != UNKNOWN_NUMERIC_TOKEN || 
                right->token->t_type != UNKNOWN_NUMERIC_TOKEN
            ) continue;

            int result = 0;
            int l_val = str_atoi(left->token->value);
            int r_val = str_atoi(right->token->value);
            switch (t->token->t_type) {
                case PLUS_TOKEN:          result = l_val + r_val;  break;
                case MINUS_TOKEN:         result = l_val - r_val;  break;
                case BITOR_TOKEN:         result = l_val | r_val;  break;
                case BITAND_TOKEN:        result = l_val & r_val;  break;
                case BITMOVE_LEFT_TOKEN:  result = l_val << r_val; break;
                case BITMOVE_RIGHT_TOKEN: result = l_val >> r_val; break;
                case LARGER_TOKEN:        result = l_val > r_val;  break;
                case LOWER_TOKEN:         result = l_val < r_val;  break;
                case MODULO_TOKEN:        result = l_val % r_val;  break;
                case AND_TOKEN:           result = l_val && r_val; break;
                case OR_TOKEN:            result = l_val || r_val; break;
                case MULTIPLY_TOKEN:      result = l_val * r_val;  break;
                case DIVIDE_TOKEN: 
                    if (r_val == 0) continue;
                    result = l_val / r_val; 
                break;
                default: break;
            }

            *fold = 1;

            snprintf(t->token->value, TOKEN_MAX_SIZE, "%d", result);
            t->token->t_type = UNKNOWN_NUMERIC_TOKEN;
            t->token->glob = 1;
            AST_unload(t->child);
            t->child = NULL;
        }
    }

    return 1;
}

int OPT_constfold(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int is_fold = 0;
    _find_muldiv(ctx->r, &is_fold);
    return is_fold;
}
