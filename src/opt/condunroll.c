#include <condunroll.h>

/* Delete all unreacheble conditional blocks */
static int _clean_blocks(ast_node_t* root, syntax_ctx_t* ctx) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (!t->token) {
            _clean_blocks(t, ctx);
            continue;
        }

        if (t->token->t_type == SCOPE_TOKEN) {
            scope_push(&ctx->scope.stack, ++ctx->scope.s_id, t->info.offset);
            _clean_blocks(t, ctx);
            scope_pop(&ctx->scope.stack);
            continue;
        }

        switch (t->token->t_type) {
            case IF_TOKEN: {
                _clean_blocks(t, ctx);

                ast_node_t* condition = t->child;
                ast_node_t* lbranch   = condition->sibling; // main
                ast_node_t* rbranch   = lbranch->sibling;   // else

                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int val = str_atoi(condition->token->value);
                    if (val && rbranch) { /* delete else branch */
                        AST_remove_node(t, rbranch);
                        AST_unload(rbranch);
                    }
                    else if (!val && rbranch) { /* delete main branch */
                        AST_remove_node(t, lbranch);
                        AST_unload(lbranch);
                    }
                    else if (!val && !rbranch) { /* delete entire if block */
                        AST_remove_node(root, t);
                        AST_unload(t);
                    }
                }

                break;
            }

            case WHILE_TOKEN: {
                _clean_blocks(t, ctx);

                ast_node_t* condition = t->child;
                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int val = str_atoi(condition->token->value);
                    if (!val) { /* Delete entire while block */
                        AST_remove_node(root, t);
                        AST_unload(t);
                    }
                }

                break;
            }

            case SWITCH_TOKEN: {
                _clean_blocks(t, ctx);

                ast_node_t* stmt  = t->child;
                ast_node_t* cases = stmt->sibling;
                
                if (stmt->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int isfound = 0;
                    ast_node_t* defcase = NULL;

                    ast_node_t* prev = NULL;
                    int val = str_atoi(stmt->token->value);
                    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
                        if (curr_case->token->t_type == DEFAULT_TOKEN) {
                            defcase = curr_case;
                            continue;
                        }

                        if (val == str_atoi(curr_case->token->value)) isfound = 1;
                        else {
                            prev->sibling = curr_case->sibling;
                            AST_remove_node(cases, curr_case);
                            AST_unload(curr_case);
                            curr_case = prev;
                        }

                        prev = curr_case;
                    }

                    if (isfound && defcase) {
                        AST_remove_node(cases, defcase);
                        AST_unload(defcase);
                    }
                }
                
                break;
            }
            
            case FUNC_TOKEN: _clean_blocks(t->child->sibling->sibling, ctx); continue;
            default: break;
        }

    }

    return 1;
}

int OPT_condunroll(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    
    scope_reset(&ctx->scope.stack); /* Reset scopes */
    ctx->scope.s_id = 0;

    int cleanup = _clean_blocks(ctx->r, ctx); /* Clean dead blocks */
    return cleanup;
}
