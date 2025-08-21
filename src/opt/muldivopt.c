#include <optimization.h>

static int _find_muldiv(ast_node_t* root, int* fold) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _find_muldiv(t, fold);
            continue;
        }

        switch (t->token->t_type) {
            case CALL_TOKEN:
            case SYSCALL_TOKEN:
            case INT_TYPE_TOKEN:
            case LONG_TYPE_TOKEN:
            case CHAR_TYPE_TOKEN: 
            case SHORT_TYPE_TOKEN:
            case ARRAY_TYPE_TOKEN:
            case RETURN_TOKEN: _find_muldiv(t, fold); continue;
            case IF_TOKEN:
            case FUNC_TOKEN:   _find_muldiv(t->child->sibling->sibling, fold); continue;
            case WHILE_TOKEN:  _find_muldiv(t->child->sibling, fold); continue;
            default: break;
        }

        /* Constant folding */
        if (VRS_isoperand(t->token)) {
            _find_muldiv(t, fold);
            ast_node_t* left = t->child;
            ast_node_t* right = left->sibling;
            if (left->token->t_type != UNKNOWN_NUMERIC_TOKEN || right->token->t_type != UNKNOWN_NUMERIC_TOKEN) break;

            int l_val = str_atoi((char*)left->token->value);
            int r_val = str_atoi((char*)right->token->value);
            int result = 0;
            switch (t->token->t_type) {
                case PLUS_TOKEN:     result = l_val + r_val; break;
                case MINUS_TOKEN:    result = l_val - r_val; break;
                case MULTIPLY_TOKEN: result = l_val * r_val; break;
                case DIVIDE_TOKEN: 
                    if (r_val == 0) break;
                    result = l_val / r_val; 
                    break;
                case BITOR_TOKEN:         result = l_val | r_val; break;
                case BITAND_TOKEN:        result = l_val & r_val; break;
                case BITMOVE_LEFT_TOKEN:  result = l_val << r_val; break;
                case BITMOVE_RIGHT_TOKEN: result = l_val >> r_val; break;
                default: break;
            }

            *fold = 1;

            snprintf((char*)t->token->value, TOKEN_MAX_SIZE, "%d", result);
            t->token->t_type = UNKNOWN_NUMERIC_TOKEN;
            t->token->glob = 1;
            AST_unload(t->child->sibling);
            AST_unload(t->child);
            t->child = NULL;
        }
        
        /* Mult and div optimisation after folding */
        if (t->token->t_type == MULTIPLY_TOKEN || t->token->t_type == DIVIDE_TOKEN) {
            ast_node_t* left = t->child;
            ast_node_t* right = left->sibling;
            if (right->token->t_type != UNKNOWN_NUMERIC_TOKEN) continue;

            int right_val = str_atoi((char*)right->token->value);
            if ((right_val & (right_val - 1)) != 0) continue;
            
            int shift = 0;
            while (right_val >>= 1) shift++;
            t->token->t_type = (t->token->t_type == MULTIPLY_TOKEN) ? BITMOVE_LEFT_TOKEN : BITMOVE_RIGHT_TOKEN;
            snprintf((char*)right->token->value, TOKEN_MAX_SIZE, "%d", shift);
        }
    }

    return 1;
}

int OPT_muldiv(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int is_fold = 0;
    _find_muldiv(ctx->r, &is_fold);
    return is_fold;
}
