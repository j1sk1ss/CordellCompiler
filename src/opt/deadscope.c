#include <deadscope.h>

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
            curr->token->t_type == EXIT_TOKEN ||
            curr->token->t_type == RETURN_TOKEN
        ) *affect = 1;

        if (curr->token->t_type == START_TOKEN) {
            *affect = 1;
            _find_scope(curr, affect, s_id);
        }

        if (curr->token->t_type == SCOPE_TOKEN) {
            int is_affect = 0;
            _find_scope(curr, &is_affect, curr->info.s_id);

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
                _find_scope(curr, &is_affect, s_id);
                if (!is_affect) {
                    AST_remove_node(root, curr);
                    AST_unload(curr);

                    if (prev) prev->sibling = next;
                    else root->child = next;

                    curr = next;
                    continue;
                }

                break;
            }
            case CASE_TOKEN:
            case DEFAULT_TOKEN: {
                int is_affect = 0;
                _find_scope(curr, &is_affect, s_id);
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
            VRS_isdecl(curr->token) || 
            (VRS_isoperand(curr->token) && curr->token->t_type != ASSIGN_TOKEN)
        ) {
            _find_scope(curr, affect, s_id);
            prev = curr;
            curr = next;
            continue;
        }

        if (curr->token->t_type == ASSIGN_TOKEN) {
            ast_node_t* var = curr->child;
            if (var->info.s_id < s_id) { /* This scope affect to outer variable */
                *affect = 1;
                break;
            }
        }

        if (curr->token->t_type == CALL_TOKEN) { /* This scope invokes some function, that can affect outer env */
            *affect = 1;
            break;
        }

        prev = curr;
        curr = next;
    }

    return 1;
}

int OPT_deadscope(syntax_ctx_t* ctx) {
    int dummy = 0;
    return _find_scope(ctx->r, &dummy, 0);
}
