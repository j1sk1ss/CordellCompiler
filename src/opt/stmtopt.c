#include <optimization.h>

static int _find_stmt(ast_node_t* root) {
    if (!root) return 0;
    for (ast_node_t* t = root->first_child; t; t = t->next_sibling) {
        if (!t->token) {
            _find_stmt(t);
            continue;
        }

        switch (t->token->t_type) {
            case IF_TOKEN: {
                ast_node_t* condition = t->first_child;
                ast_node_t* body = condition->next_sibling;
                ast_node_t* else_body = body->next_sibling;
                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int true_or_false = str_atoi((char*)condition->token->value);
                    if (true_or_false && else_body) {
                        AST_remove_node(t, else_body);
                        AST_unload(else_body);
                    }
                    else if (!true_or_false && else_body) {
                        AST_remove_node(t, body);
                        AST_unload(body);
                    }
                    else if (!true_or_false && !else_body) {
                        AST_remove_node(root, t);
                        AST_unload(t);
                        _find_stmt(root);
                        return 1;
                    }
                }

                break;
            }

            case WHILE_TOKEN: {
                ast_node_t* condition = t->first_child;
                // ast_node_t* body = condition->next_sibling->first_child;
                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int true_or_false = str_atoi((char*)condition->token->value);
                    if (!true_or_false) {
                        AST_remove_node(root, t);
                        AST_unload(t);
                        _find_stmt(root);
                        return 1;
                    }
                }

                break;
            }

            case SWITCH_TOKEN: {
                ast_node_t* stmt  = t->first_child;
                ast_node_t* cases = stmt->next_sibling;
                if (stmt->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int option_case = str_atoi((char*)stmt->token->value);
                    for (ast_node_t* curr_case = cases->first_child; curr_case; curr_case = curr_case->next_sibling) {
                        if (curr_case->token->t_type == DEFAULT_TOKEN) continue;
                        int case_value = str_atoi((char*)curr_case->token->value);
                        if (option_case != case_value) {
                            AST_remove_node(cases, curr_case);
                            AST_unload(curr_case);
                            _find_stmt(root);
                            return 1;
                        }
                    }
                }
                
                break;
            }
            
            case FUNC_TOKEN: _find_stmt(t->first_child->next_sibling->next_sibling); continue;
            default: break;
        }

    }

    return 1;
}

int stmt_optimization(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    return _find_stmt(ctx->r);
}
