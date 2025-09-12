#include <condunroll.h>

/*
Delete all unreacheble conditional blocks.
Also unroll if, while and switch statements.
*/
static int _clean_blocks(ast_node_t* root, syntax_ctx_t* ctx) {
    if (!root) return 0;
    ast_node_t* tprev = NULL;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _clean_blocks(t->child, ctx);
            continue;
        }

        switch (t->token->t_type) {
            case IF_TOKEN: {
                ast_node_t* condition = t->child;
                ast_node_t* lbranch   = condition->sibling;
                ast_node_t* rbranch   = lbranch->sibling;

                _clean_blocks(lbranch, ctx);
                _clean_blocks(rbranch, ctx);

                ast_node_t* unrolled_if = NULL;
                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int val = str_atoi(condition->token->value);
                    if (val && rbranch) {        /* delete else branch     */
                        unrolled_if = lbranch;
                        AST_remove_node(t, rbranch);
                        AST_unload(rbranch);
                    }
                    else if (!val && rbranch) {  /* delete main branch     */
                        unrolled_if = rbranch;
                        AST_remove_node(t, lbranch);
                        AST_unload(lbranch);
                    }
                    else if (!val && !rbranch) { /* delete entire if block */
                        AST_remove_node(root, t);
                        AST_unload(t);
                    }
                }

                if (unrolled_if) {            /* If unrolling with scope saving */
                    unrolled_if->sibling = t->sibling;

                    t->sibling = NULL;        /* Saving last AST    */
                    t->child->sibling = NULL; /* Saving last branch */
                    AST_unload(t);

                    t = unrolled_if;
                    if (tprev) tprev->sibling = t;
                    else root->child = t;
                }

                break;
            }

            case SWITCH_TOKEN: {
                ast_node_t* stmt  = t->child;
                ast_node_t* cases = stmt->sibling;
                
                if (stmt->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int isfound = 0;
                    ast_node_t* defcase = NULL;
                    ast_node_t* unrolled_switch = NULL;

                    ast_node_t* prev = NULL;
                    int val = str_atoi(stmt->token->value);
                    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
                        _clean_blocks(curr_case->child, ctx);
                        if (curr_case->token->t_type == DEFAULT_TOKEN) {
                            if (!unrolled_switch) unrolled_switch = curr_case->child;
                            defcase = curr_case;
                            continue;
                        }

                        if (val == str_atoi(curr_case->token->value)) {
                            unrolled_switch = curr_case->child;
                            isfound = 1;
                        }
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

                    if (unrolled_switch) { /* Unroll last case to default code */
                        unrolled_switch->sibling = t->sibling;

                        t->sibling = NULL;                      /* Saving last AST  */
                        t->child->sibling->child->child = NULL; /* Saving last case */
                        AST_unload(t);

                        t = unrolled_switch;
                        if (tprev) tprev->sibling = t;
                        else root->child = t;
                    }
                }
                
                break;
            }
            
            case WHILE_TOKEN: {
                ast_node_t* condition = t->child;
                ast_node_t* lbranch   = condition->sibling;
                ast_node_t* rbranch   = lbranch->sibling;

                _clean_blocks(lbranch, ctx);
                _clean_blocks(rbranch, ctx);

                if (condition->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                    int val = str_atoi(condition->token->value);
                    if (!val && !rbranch) {
                        AST_remove_node(root, t);
                        AST_unload(t);
                    }
                    else if (!val && rbranch) {
                        rbranch->sibling = t->sibling;

                        t->sibling = NULL;
                        t->child->sibling = NULL;
                        AST_unload(t);

                        t = rbranch;
                        if (tprev) tprev->sibling = t;
                        else root->child = t;
                    }
                }

                break;
            }

            case FUNC_TOKEN: 
                _clean_blocks(t->child->sibling->sibling, ctx);
            continue;
            default: break;
        }

        tprev = t;
    }

    return 1;
}

int OPT_condunroll(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    return _clean_blocks(ctx->r, ctx); /* Clean dead blocks */
}
