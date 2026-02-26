#include <ast/opt/deadscope.h>

static int _find_scope(ast_node_t* root, int* affect, short s_id) {
    if (!root) return 0;
    
    ast_node_t* prev = NULL;
    ast_node_t* curr = root->c;
    while (curr) {
        ast_node_t* next = curr->siblings.n;
        if (!curr->t) {
            _find_scope(curr, affect, s_id);
            prev = curr;
            curr = next;
            continue;
        }

        if (
            curr->t->t_type == EXIT_TOKEN   ||  /* If this scope invokes an exit command  */
            curr->t->t_type == RETURN_TOKEN ||  /* If this scope returns something        */
            curr->t->t_type == CALL_TOKEN   ||  /* If this scope contains a function call */
            curr->t->t_type == BREAK_TOKEN  ||  /* If this scope contains a break command */
            curr->t->t_type == BREAKPOINT_TOKEN /* If this scope breaks an execution      */
        ) *affect = 1;

        if (curr->t->t_type == START_TOKEN) {
            _find_scope(curr, affect, s_id);
            *affect = 1;
        }

        if (curr->t->t_type == SCOPE_TOKEN) {
            int is_affect = 0;
            _find_scope(curr, &is_affect, MAX(curr->sinfo.s_id, s_id));

            if (!is_affect) {
                AST_remove_node(root, curr);
                AST_unload(curr);

                if (prev) prev->siblings.n = next;
                else root->c = next;

                curr = next;
                continue;
            }

            *affect = 1;
        }

        switch (curr->t->t_type) {
            case IF_TOKEN:
            case WHILE_TOKEN:
            case SWITCH_TOKEN: {
                int is_affect = 0;
                _find_scope(curr, &is_affect, MAX(curr->sinfo.s_id, s_id));

                if (!is_affect) { 
                    AST_remove_node(root, curr);
                    AST_unload(curr);

                    if (prev) prev->siblings.n = next;
                    else root->c = next;

                    curr = next;
                    continue;
                }

                *affect = 1;
                break;
            }

            case CASE_TOKEN:
            case DEFAULT_TOKEN: {
                int is_affect = 0;
                ast_node_t* case_scope = curr->t->t_type == CASE_TOKEN ? curr->c->siblings.n : curr->c;
                _find_scope(case_scope, &is_affect, s_id);
                if (!is_affect) {
                    AST_remove_node(root, curr);
                    AST_unload(curr);
                    if (prev) prev->siblings.n = next;
                    else root->c = next;
                    curr = next;
                    continue;
                }

                *affect = 1;
                break;
            }
            
            case FUNC_TOKEN: {
                _find_scope(curr, affect, s_id);
                break;
            }

            default: break;
        }

        if (
            TKN_is_decl(curr->t) || 
            (TKN_isoperand(curr->t) && !TKN_update_operator(curr->t))
        ) {
            _find_scope(curr, affect, s_id);
            prev = curr;
            curr = next;
            continue;
        }

        if (TKN_update_operator(curr->t)) {
            ast_node_t* var = curr->c;
            if (var->sinfo.s_id < s_id) {
                *affect = 1;
                break;
            }
        }

        prev = curr;
        curr = next;
    }

    return 1;
}

int OPT_deadscope(ast_ctx_t* ctx) {
    int dummy = 0;
    return _find_scope(ctx->r, &dummy, 0);
}
