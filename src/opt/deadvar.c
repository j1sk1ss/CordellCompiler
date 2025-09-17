#include <deadvar.h>

static int _affect_outer(ast_node_t* root, char* varname, int* affect, short scope) {
    if (!root) return 0;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (!t->token) continue;
        _affect_outer(t->child, varname, affect, scope);
        if (VRS_update_operator(t->token)) {
            if (str_strncmp(t->child->token->value, varname, TOKEN_MAX_SIZE) && t->sinfo.s_id > scope) {
                *affect = 1;
                return 1;
            }
        }
    }

    return 0;
}

static int _find_variable(ast_node_t* root, char* varname, int* found) {
    if (!root) return 0;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (!t->token) continue;
        _find_variable(t->child, varname, found);
        if (!VRS_isvariable(t->token)) return 0;
        if (!str_strncmp(t->token->value, varname, TOKEN_MAX_SIZE)) {
            *found = 1;
            return 1;
        }
    }

    return 0;
}

static int _find_usage(ast_node_t* root, char* varname, int* used) {
    if (!root) return 0;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _find_usage(t->child, varname, used);
            continue;
        }
        
        /* This variable participate in another variable declaration or updating? */
        if (
            VRS_isdecl(t->token) ||
            (VRS_update_operator(t->token) && str_strncmp(t->child->token->value, varname, TOKEN_MAX_SIZE))
        ) {
            _find_variable(t->child->sibling, varname, used);
        }

        /* This variable is responsible for env. change in future? */
        if (VRS_is_control_change(t->token)) {
            switch (t->token->t_type) {
                case IF_TOKEN:
                case WHILE_TOKEN: {
                    int is_used = 0;
                    _find_variable(t->child, varname, &is_used);

                    int is_affect = 0;
                    _affect_outer(t->child->sibling, varname, &is_affect, t->sinfo.s_id);
                    _affect_outer(t->child->sibling->sibling, varname, &is_affect, t->sinfo.s_id);
                    break;
                }

                case SWITCH_TOKEN: {
                    int is_used = 0;
                    _find_variable(t->child, varname, &is_used);

                    int is_affect = 0;
                    _affect_outer(t->child->sibling, varname, &is_affect, t->sinfo.s_id);
                    break;
                }

                case CALL_TOKEN: _find_variable(t->child, varname, used); break;
                default: break;
            }
        }
    }

    return 1;
}

static int _find_decl(ast_node_t* root, int* change) {
    if (!root) return 0;
    
    ast_node_t* prev = NULL;
    ast_node_t* curr = root->child;

    while (curr) {
        ast_node_t* next = curr->sibling;
        if (VRS_isblock(curr->token)) {
            _find_decl(curr, change);
            prev = curr;
            curr = next;
            continue;
        }

        switch (curr->token->t_type) {
            case IF_TOKEN:
            case WHILE_TOKEN:
            case SWITCH_TOKEN:
                _find_decl(curr, change);
                prev = curr;
                curr = next;
            continue;
            case FUNC_TOKEN:
                _find_decl(curr->child->sibling->sibling, change);
                prev = curr;
                curr = next;
            continue;
            default: break;
        }

        if (VRS_isdecl(curr->token)) {
            int is_updates = 0;
            _find_usage(curr->sibling, curr->child->token->value, &is_updates);
            if (!is_updates) {
                AST_remove_node(root, curr);
                AST_unload(curr);

                if (prev) prev->sibling = next;
                else root->child = next;
                curr = next;
                continue;
            }
        }

        prev = curr;
        curr = next;
    }

    return 1;
}

int OPT_deadvar(syntax_ctx_t* ctx) {
    return 1;
}