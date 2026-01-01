#include <ast/opt/condunroll.h>

static int _clean_blocks(ast_node_t* root, ast_ctx_t* ctx) {
    if (!root) return 0;

    ast_node_t* tprev = NULL;
    ast_node_t* t = root->c;

    while (t) {
        ast_node_t* next = t->siblings.n;
        if (TKN_isblock(t->t)) _clean_blocks(t, ctx);
        else {
            switch (t->t->t_type) {
                case IF_TOKEN: {
                    ast_node_t* condition = t->c;
                    ast_node_t* lbranch   = condition->siblings.n;
                    ast_node_t* rbranch   = lbranch ? lbranch->siblings.n : NULL;

                    _clean_blocks(lbranch, ctx);
                    _clean_blocks(rbranch, ctx);

                    ast_node_t* unrolled_if = NULL;
                    if (TKN_isnumeric(condition->t)) {
                        long long val = condition->t->body->to_llong(condition->t->body);
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
                            if (tprev) tprev->siblings.n = next;
                            else root->c = next;
                            t = next;
                            continue;
                        }
                    }

                    if (unrolled_if) {
                        ast_node_t* replacement = AST_copy_node(unrolled_if, 1, 0, 1);
                        replacement->siblings.n = next;

                        AST_remove_node(root, t);
                        AST_unload(t);

                        if (tprev) tprev->siblings.n = replacement;
                        else root->c = replacement;

                        t = replacement;
                        tprev = replacement;
                        continue;
                    }

                    break;
                }

                case SWITCH_TOKEN: {
                    ast_node_t* stmt  = t->c;
                    ast_node_t* cases = stmt->siblings.n;

                    if (TKN_isnumeric(stmt->t)) {
                        ast_node_t* defcase = NULL;
                        ast_node_t* unrolled_switch = NULL;
                        ast_node_t* prev = NULL;
                        ast_node_t* curr = cases->c;

                        long long val = stmt->t->body->to_llong(stmt->t->body);
                        while (curr) {
                            ast_node_t* next_case = curr->siblings.n;
                            _clean_blocks(curr->c, ctx);

                            if (curr->t->t_type == DEFAULT_TOKEN && !unrolled_switch) {
                                unrolled_switch = curr->c;
                                defcase = curr;
                                prev = curr;
                            } 
                            else if (val == curr->t->body->to_llong(curr->t->body)) {
                                unrolled_switch = curr->c;
                                prev = curr;
                            } 
                            else {
                                if (prev) prev->siblings.n = next_case;
                                else cases->c = next_case;
                                AST_remove_node(cases, curr);
                                AST_unload(curr);
                            }

                            curr = next_case;
                        }

                        if (unrolled_switch) {
                            ast_node_t* replacement = AST_copy_node(unrolled_switch, 0, 0, 1);
                            replacement->siblings.n = next;

                            if (tprev) tprev->siblings.n = replacement;
                            else root->c = replacement;

                            AST_unload(t);
                            t = replacement;
                            tprev = replacement;
                            continue;
                        }
                    }

                    break;
                }

                case WHILE_TOKEN: {
                    ast_node_t* condition = t->c;
                    _clean_blocks(condition->siblings.n, ctx);
                    if (TKN_isnumeric(condition->t) && !condition->t->body->to_llong(condition->t->body)) {
                        AST_remove_node(root, t);
                        AST_unload(t);
                        if (tprev) tprev->siblings.n = next;
                        else root->c = next;
                        t = next;
                        continue;
                    }

                    break;
                }

                case FUNC_TOKEN: _clean_blocks(t->c->siblings.n->siblings.n, ctx); break;
                default: break;
            }
        }

        tprev = t;
        t = next;
    }

    return 1;
}

int OPT_condunroll(ast_ctx_t* ctx) {
    if (!ctx->r) return 0;
    return _clean_blocks(ctx->r, ctx);
}
