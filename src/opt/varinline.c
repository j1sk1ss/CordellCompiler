#include <varinline.h>

/* Check binary tree for variable usage */
static int _inline_binary(ast_node_t* r, const char* v, ast_node_t** nv, int inl) {
    if (!inl) return 0;
    if (r->child && r->child->sibling) {
        return _inline_binary(r->child, v, nv, inl) || _inline_binary(r->child->sibling, v, nv, inl);
    }
    
    if (!str_strncmp(r->token->value, v, TOKEN_MAX_SIZE)) {
        r->token->t_type = (*nv)->token->t_type;
        str_strncpy(r->token->value, (*nv)->token->value, TOKEN_MAX_SIZE);
        str_memcpy(&r->token->vinfo, &(*nv)->token->vinfo, sizeof(tkn_var_info_t));
        str_memcpy(&r->info, &(*nv)->info, sizeof(syntax_info_t));
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
static int _inline_block(ast_node_t* d, const char* v, ast_node_t** nv, int loop, int* updated, int inl, int sl) {
    if (!d) return 0;

    int result = 0;
    ast_node_t* prev    = NULL;
    ast_node_t* curr    = d;
    ast_node_t* lchange = NULL;

    if (loop) {
        inl = MIN(!_find_variable_update(curr, v), inl);
        *updated = MAX(!inl, *updated);
    }
    else {
        inl = 1;
    }

    while (curr) {
        ast_node_t* next = curr->sibling;
        if (VRS_is_unpredicted(curr->token)) {
            int upredicted_future = _find_variable_update(curr->child, v);
            switch (curr->token->t_type) {
                case IF_TOKEN:
                case WHILE_TOKEN: {
                    int isupd = 0;
                    ast_node_t* pval = *nv;
                    int iswhile = curr->token->t_type == WHILE_TOKEN;
                    
                    result = _inline_block(curr->child->sibling->child, v, nv, MAX(iswhile, loop), &isupd, inl, sl) || result;
                    if (curr->child->sibling->sibling) {
                        *nv = pval;
                        result = _inline_block(curr->child->sibling->sibling->child, v, nv, MAX(iswhile, loop), &isupd, inl, sl) || result;
                    }

                    _inline_binary(curr->child, v, nv, inl && !isupd);
                    break;
                }

                case SWITCH_TOKEN: {
                    for (ast_node_t* c = curr->child->sibling->child; c; c = c->sibling) {
                        ast_node_t* pval = *nv;
                        result = _inline_block(c->child, v, nv, loop, updated, inl, sl) || result;
                        *nv = pval;
                    }

                    _inline_binary(curr->child, v, nv, inl);
                    break;
                }

                default: break;
            }

            if (upredicted_future) {
                result = 1;
                break;
            }
        }

        if (
            VRS_isblock(curr->token) || 
            VRS_isptr(curr->token) ||
            curr->token->t_type == EXIT_TOKEN || 
            curr->token->t_type == CALL_TOKEN
        ) {
            result = _inline_block(curr->child, v, nv, loop, updated, inl, sl) || result;
            prev = curr;
            curr = next;
            continue;
        }
        
        if (VRS_isdecl(curr->token) || curr->token->t_type == ASSIGN_TOKEN) { 
            if (str_strncmp(curr->child->token->value, v, TOKEN_MAX_SIZE)) _inline_binary(curr->child->sibling, v, nv, inl);
            else if (VRS_isnumeric(curr->child->sibling->token) || VRS_isvariable(curr->child->sibling->token)) {
                *nv = curr->child->sibling; 
                if (sl) lchange = curr;
            }
            else {
                if (sl) lchange = curr;
                result = 1;
                break;
            }
        }
        else if (VRS_isoperand(curr->token) || VRS_isvariable(curr->token)) {
            _inline_binary(curr, v, nv, inl);
        }

        prev = curr;
        curr = next;
    }

    if (lchange || !sl) {
        prev = NULL;
        curr = d;
        while (curr) {
            ast_node_t* next = curr->sibling;
            if (
                curr->token && curr->token->t_type == ASSIGN_TOKEN &&
                !str_strncmp(curr->child->token->value, v, TOKEN_MAX_SIZE) &&
                (VRS_isnumeric(curr->child->sibling->token) || VRS_isvariable(curr->child->sibling->token)) &&
                (!lchange || curr != lchange)
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

    ast_node_t* prev = NULL;
    ast_node_t* curr = r;

    while (curr) {
        ast_node_t* next = curr->sibling;
        _find_declrations(curr->child);

        if (
            curr->token &&
            VRS_isdecl(curr->token) && curr->child && curr->child->sibling &&
            VRS_instack(curr->token) &&
            VRS_one_slot(curr->child->sibling->token) && (
                VRS_isnumeric(curr->child->sibling->token) || VRS_isvariable(curr->child->sibling->token)
            )
        ) {
            int upd = 0;
            ast_node_t* inval = curr->child->sibling;
            if (!_inline_block(curr->sibling, curr->child->token->value, &inval, 0, &upd, 1, 1)) {
                _inline_block(curr->sibling, curr->child->token->value, &inval, 0, &upd, 1, 0);
                if (curr->parent) {
                    if (curr->parent->child == curr) curr->parent->child = next;
                    else if (prev) prev->sibling = next;
                }

                AST_remove_node(r, curr);
                AST_unload(curr);

                curr = next;
                continue;
            }
        }

        prev = curr;
        curr = next;
    }

    return 1;
}

int OPT_varinline(syntax_ctx_t* ctx) {
    _find_declrations(ctx->r);
    return 1;
}
