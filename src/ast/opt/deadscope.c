#include <ast/opt/deadscope.h>

static int _find_scope(ast_node_t* root, int* affect, short s_id) {
    if (!root) return 0;
    
    ast_node_t* prev = NULL;
    ast_node_t* curr = root->child;

    while (curr) {
        ast_node_t* next = curr->sibling;
        if (!curr->token) {
            _find_scope(curr, affect, s_id);
            prev = curr;
            curr = next;
            continue;
        }

        if (
            curr->token->t_type == EXIT_TOKEN   ||
            curr->token->t_type == RETURN_TOKEN ||
            curr->token->t_type == CALL_TOKEN   ||
            curr->token->t_type == BREAKPOINT_TOKEN
        ) *affect = 1;

        if (curr->token->t_type == START_TOKEN) {
            _find_scope(curr, affect, s_id);
            *affect = 1;
        }

        if (curr->token->t_type == SCOPE_TOKEN) {
            int is_affect = 0;
            _find_scope(curr, &is_affect, MAX(curr->sinfo.s_id, s_id));

            if (!is_affect) {
                AST_remove_node(root, curr);
                AST_unload(curr);

                if (prev) prev->sibling = next;
                else root->child = next;

                curr = next;
                continue;
            }

            *affect = 1;
        }

        switch (curr->token->t_type) {
            case IF_TOKEN:
            case WHILE_TOKEN:
            case SWITCH_TOKEN: {
                int is_affect = 0;
                _find_scope(curr, &is_affect, MAX(curr->sinfo.s_id, s_id));

                if (!is_affect) { 
                    AST_remove_node(root, curr);
                    AST_unload(curr);

                    if (prev) prev->sibling = next;
                    else root->child = next;

                    curr = next;
                    continue;
                }

                *affect = 1;
                break;
            }

            case CASE_TOKEN:
            case DEFAULT_TOKEN: {
                int is_affect = 0;
                ast_node_t* case_scope = curr->token->t_type == CASE_TOKEN ? curr->child->sibling : curr->child;
                _find_scope(case_scope, &is_affect, s_id);
                if (!is_affect) {
                    AST_remove_node(root, curr);
                    AST_unload(curr);

                    if (prev) prev->sibling = next;
                    else root->child = next;

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
            TKN_isdecl(curr->token) || 
            (TKN_isoperand(curr->token) && !TKN_update_operator(curr->token))
        ) {
            _find_scope(curr, affect, s_id);
            prev = curr;
            curr = next;
            continue;
        }

        if (TKN_update_operator(curr->token)) {
            ast_node_t* var = curr->child;
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
