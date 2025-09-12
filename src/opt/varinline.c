#include <varinline.h>

/* Check binary tree for variable usage */
static int _inline_binary(ast_node_t* r, const char* v, int* value) {
    if (r->child && r->child->sibling) {
        return _inline_binary(r->child, v, value) || _inline_binary(r->child->sibling, v, value);
    }
    
    if (!str_strncmp(r->token->value, v, TOKEN_MAX_SIZE)) {
        snprintf(r->token->value, TOKEN_MAX_SIZE, "%d", *value);
        r->token->t_type = UNKNOWN_NUMERIC_TOKEN;
        r->token->vinfo.glob = 1;
        return 1;
    }

    return 0;
}

static int _find_variable_update(ast_node_t* d, const char* v) {
    if (!d) return 0;
    for (ast_node_t* t = d; t; t = t->sibling) {
        if (!t->token) {
            if (_find_variable_update(t->child, v)) return 1;
            continue;
        }

        if (t->token->t_type == ASSIGN_TOKEN) {
            if (!str_strncmp(t->child->token->value, v, TOKEN_MAX_SIZE)) return 1;
        }

        if (_find_variable_update(t->child, v)) {
            return 1;
        }
    }

    return 0;
}

/* Check scope and sub-scopes (without control flow changes) */
static int _inline_block(ast_node_t* d, const char* v, int* value) {
    if (!d) return 0;

    int result = 0;
    ast_node_t* prev    = NULL;
    ast_node_t* curr    = d;
    ast_node_t* lchange = NULL;

    while (curr) {
        ast_node_t* next = curr->sibling;
        if (VRS_is_unpredicted(curr->token) && _find_variable_update(curr->child, v)) {
            result = 1;
            switch (curr->token->t_type) {
                case IF_TOKEN:
                case WHILE_TOKEN: {
                    _inline_binary(curr->child, v, value);

                    int pval = *value;
                    result = _inline_block(curr->child->sibling->child, v, value) || result;
                    if (curr->child->sibling->sibling) {
                        *value = pval;
                        result = _inline_block(curr->child->sibling->sibling->child, v, value) || result;
                    }

                    break;
                }

                case SWITCH_TOKEN: {
                    _inline_binary(curr->child, v, value);
                    for (ast_node_t* c = curr->child->sibling->child; c; c = c->sibling) {
                        int pval = *value;
                        result = _inline_block(c->child, v, value) || result;
                        *value = pval;
                    }

                    break;
                }

                default: break;
            }

            break;
        }

        if (
            VRS_isblock(curr->token) || 
            curr->token->t_type == EXIT_TOKEN || 
            curr->token->t_type == CALL_TOKEN
        ) {
            result = _inline_block(curr->child, v, value) || result;
            prev = curr;
            curr = next;
            continue;
        }
        
        if (VRS_isdecl(curr->token) || curr->token->t_type == ASSIGN_TOKEN) { 
            if (str_strncmp(curr->child->token->value, v, TOKEN_MAX_SIZE)) _inline_binary(curr->child->sibling, v, value);
            else if (VRS_isnumeric(curr->child->sibling->token)) {
                *value = str_atoi(curr->child->sibling->token->value); 
                lchange = curr;
            }
            else {
                lchange = curr;
                break;
            }
        }
        else if (VRS_isoperand(curr->token) || VRS_isvariable(curr->token)) {
            _inline_binary(curr, v, value);
        }

        prev = curr;
        curr = next;
    }

    if (lchange) {
        prev = NULL;
        curr = d;
        while (curr) {
            ast_node_t* next = curr->sibling;
            if (
                curr->token->t_type == ASSIGN_TOKEN &&
                !str_strncmp(curr->child->token->value, v, TOKEN_MAX_SIZE) &&
                VRS_isnumeric(curr->child->sibling->token) &&
                curr != lchange
            ) {
                AST_remove_node(d, curr);
                AST_unload(curr);

                if (prev) prev->sibling = next;
                else {
                    curr->parent->child = next;
                    d = next;
                }

                curr = next;
                continue;
            }
            
            prev = curr;
            curr = next;
        }
    }

    return result;
}

/* Bruteforce declaration search */
static int _find_declrations(ast_node_t* r) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
        _find_declrations(t->child);
        if (!t->token) continue;

        /* We found variable that can be inlined */
        if (
            VRS_isdecl(t->token) && t->child && t->child->sibling && 
            VRS_isnumeric(t->child->sibling->token)
        ) {
            int inval = str_atoi(t->child->sibling->token->value);
            if (!_inline_block(t->sibling, t->child->token->value, &inval)) {
                /* Remove variable declaration */
            }
        }
    }

    return 1;
}

int OPT_varinline(syntax_ctx_t* ctx) {
    _find_declrations(ctx->r);
    return 1;
}
