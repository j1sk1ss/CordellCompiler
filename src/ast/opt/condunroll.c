#include <ast/opt/condunroll.h>

static int _clean_blocks(ast_node_t* root, syntax_ctx_t* ctx) {
    if (!root) return 0;

    ast_node_t* tprev = NULL;
    ast_node_t* t = root->child;

    while (t) {
        ast_node_t* next = t->sibling;

        if (TKN_isblock(t->token)) _clean_blocks(t->child, ctx);
        else {
            switch (t->token->t_type) {
                case IF_TOKEN: {
                    ast_node_t* condition = t->child;
                    ast_node_t* lbranch   = condition->sibling;
                    ast_node_t* rbranch   = lbranch ? lbranch->sibling : NULL;

                    _clean_blocks(lbranch, ctx);
                    _clean_blocks(rbranch, ctx);

                    ast_node_t* unrolled_if = NULL;

                    if (TKN_isnumeric(condition->token)) {
                        int val = str_atoi(condition->token->value);
                        if (val && lbranch) {
                            unrolled_if = lbranch;
                            if (rbranch) { 
                                AST_remove_node(t, rbranch); 
                                AST_unload(rbranch); 
                            }
                        } 
                        else if (!val && rbranch) {
                            unrolled_if = rbranch;
                            if (lbranch) { 
                                AST_remove_node(t, lbranch); 
                                AST_unload(lbranch); 
                            }
                        } 
                        else {
                            AST_remove_node(root, t);
                            AST_unload(t);
                            if (tprev) tprev->sibling = next;
                            else root->child = next;
                            t = next;
                            continue;
                        }
                    }

                    if (unrolled_if) {
                        ast_node_t* replacement = AST_copy_node(unrolled_if, 1, 0, 1);
                        replacement->sibling = next;

                        if (tprev) tprev->sibling = replacement;
                        else root->child = replacement;

                        AST_unload(t);
                        t = replacement;
                        tprev = replacement;
                        continue;
                    }
                    break;
                }

                case SWITCH_TOKEN: {
                    ast_node_t* stmt  = t->child;
                    ast_node_t* cases = stmt->sibling;

                    if (TKN_isnumeric(stmt->token)) {
                        ast_node_t* defcase = NULL;
                        ast_node_t* unrolled_switch = NULL;
                        ast_node_t* prev = NULL;
                        ast_node_t* curr = cases->child;

                        int val = str_atoi(stmt->token->value);

                        while (curr) {
                            ast_node_t* next_case = curr->sibling;
                            _clean_blocks(curr->child, ctx);

                            if (curr->token->t_type == DEFAULT_TOKEN && !unrolled_switch) {
                                unrolled_switch = curr->child;
                                defcase = curr;
                                prev = curr;
                            } 
                            else if (val == str_atoi(curr->token->value)) {
                                unrolled_switch = curr->child;
                                prev = curr;
                            } 
                            else {
                                if (prev) prev->sibling = next_case;
                                else cases->child = next_case;

                                AST_remove_node(cases, curr);
                                AST_unload(curr);
                            }

                            curr = next_case;
                        }

                        if (unrolled_switch) {
                            ast_node_t* replacement = AST_copy_node(unrolled_switch, 0, 0, 1);
                            replacement->sibling = next;

                            if (tprev) tprev->sibling = replacement;
                            else root->child = replacement;

                            AST_unload(t);
                            t = replacement;
                            tprev = replacement;
                            continue;
                        }
                    }
                    break;
                }

                case WHILE_TOKEN: {
                    ast_node_t* condition = t->child;
                    ast_node_t* lbranch   = condition->sibling;
                    ast_node_t* rbranch   = lbranch ? lbranch->sibling : NULL;

                    _clean_blocks(lbranch, ctx);
                    _clean_blocks(rbranch, ctx);

                    if (TKN_isnumeric(condition->token)) {
                        int val = str_atoi(condition->token->value);
                        if (!val && !rbranch) {
                            AST_remove_node(root, t);
                            AST_unload(t);
                            if (tprev) tprev->sibling = next;
                            else root->child = next;
                            t = next;
                            continue;
                        } 
                        else if (!val && rbranch) {
                            rbranch->sibling = next;

                            t->sibling = NULL;
                            if (lbranch) lbranch->sibling = NULL;
                            AST_unload(t);

                            t = rbranch;
                            if (tprev) tprev->sibling = t;
                            else root->child = t;
                            tprev = t;
                            continue;
                        }
                    }
                    break;
                }

                case FUNC_TOKEN:
                    _clean_blocks(t->child->sibling->sibling, ctx);
                    break;

                default: break;
            }
        }

        tprev = t;
        t = next;
    }

    return 1;
}

int OPT_condunroll(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    return _clean_blocks(ctx->r, ctx);
}
