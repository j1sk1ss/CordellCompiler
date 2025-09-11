#include <constopt.h>

static int _find_op(ast_node_t* root, int* fold) {
    if (!root) return 0;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _find_op(t->child, fold);
            continue;
        }

        if (VRS_isdecl(t->token) && t->child) {
            _find_op(t->child->sibling, fold);
            continue;
        }

        if (VRS_isoperand(t->token)) {
            ast_node_t* left  = t->child;
            ast_node_t* right = left->sibling;

            _find_op(left, fold);
            _find_op(right, fold);

            if ( /* Peephole optimization */
                t->token->t_type == MULTIPLY_TOKEN || 
                t->token->t_type == DIVIDE_TOKEN
            ) {
                if (right->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int right_val = str_atoi(right->token->value);
                    if (!(right_val & (right_val - 1))) {
                        int shift = 0;
                        while (right_val >>= 1) {
                            shift++;
                        }
                        
                        t->token->t_type = (t->token->t_type == MULTIPLY_TOKEN) ? BITMOVE_LEFT_TOKEN : BITMOVE_RIGHT_TOKEN;
                        snprintf(right->token->value, TOKEN_MAX_SIZE, "%d", shift);
                    }
                }
            }

            if ( /* We can't perform folding */
                !VRS_isnumeric(left->token) || !VRS_isnumeric(right->token)
            ) continue;

            /* Constant folding */
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
                    if (!r_val) continue;
                    result = l_val / r_val; 
                break;
                default: break;
            }

            *fold = 1;
            
            snprintf(t->token->value, TOKEN_MAX_SIZE, "%d", result);
            t->token->t_type = UNKNOWN_NUMERIC_TOKEN;
            t->token->vinfo.glob = 1;
            AST_unload(t->child);
            t->child = NULL;
        }

        _find_op(t->child, fold);
    }

    return 1;
}

int OPT_constfold(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int is_fold = 0;
    _find_op(ctx->r, &is_fold);
    return is_fold;
}
