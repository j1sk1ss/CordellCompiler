#include <ast/opt/varinline.h>

static int _inline_node(ast_node_t* dst, ast_node_t* src, int s) {
    if (!dst || !src) return 0;

    dst->token->t_type = src->token->t_type;
    str_strncpy(dst->token->value, src->token->value, TOKEN_MAX_SIZE);
    str_memcpy(&dst->token->flags, &src->token->flags, sizeof(token_flags_t));
    str_memcpy(&dst->sinfo, &src->sinfo, sizeof(syntax_info_t));

    if (s) {
        if (dst->sibling && src->sibling)       _inline_node(dst->sibling, src->sibling, 1);
        else if (!dst->sibling && src->sibling) dst->sibling = AST_copy_node(src->sibling, 0, 1, 1);
    }

    if (dst->child && src->child)       _inline_node(dst->child, src->child, 1);
    else if (!dst->child && src->child) dst->child = AST_copy_node(src->child, 0, 1, 1);
    return 1;
}

static int _inline_binary(ast_node_t* r, const char* v, ast_node_t** nv, int inl) {
    if (!inl) return 0;
    if (r->child && r->child->sibling) {
        return _inline_binary(r->child, v, nv, inl) || _inline_binary(r->child->sibling, v, nv, inl);
    }
    
    if (!str_strncmp(r->token->value, v, TOKEN_MAX_SIZE)) {
        return _inline_node(r, *nv, 0);
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

        if (VRS_update_operator(t->token)) {
            if (!str_strncmp(t->child->token->value, v, TOKEN_MAX_SIZE)) return 1;
        }

        if (_find_variable_update(t->child, v)) {
            return 1;
        }
    }

    return 0;
}

/*
- d       - root node
- v       - inline variable name
- nv      - current variable value
- loop    - loop status
- updated - was updated
- inl     - inline in current function
- sl      - save last update
Return 0 if variable unused in current scope
*/
static int _inline_block(ast_node_t* d, const char* v, ast_node_t** nv, int loop, int* updated, int inl, int sl) {
    if (!d) return 0;

    int result = 0;
    ast_node_t* lchange = NULL;
    if (loop) {
        inl = MIN(!_find_variable_update(d, v), inl);
        *updated = MAX(!inl, *updated);
    }
    else {
        inl = 1;
    }
    
    for (ast_node_t* t = d; t; t = t->sibling) {
        if (VRS_is_unpredicted(t->token)) {
            int upredicted_future = _find_variable_update(t->child, v);
            switch (t->token->t_type) {
                case IF_TOKEN:
                case WHILE_TOKEN: {
                    int isupd = 0;
                    ast_node_t* pval = *nv;
                    int iswhile = t->token->t_type == WHILE_TOKEN;
                    
                    result = _inline_block(t->child->sibling->child, v, nv, MAX(iswhile, loop), &isupd, inl, sl) || result;
                    if (t->child->sibling->sibling) {
                        *nv = pval;
                        result = _inline_block(t->child->sibling->sibling->child, v, nv, MAX(iswhile, loop), &isupd, inl, sl) || result;
                    }

                    _inline_binary(t->child, v, nv, inl && !isupd);
                    break;
                }

                case SWITCH_TOKEN: {
                    for (ast_node_t* c = t->child->sibling->child; c; c = c->sibling) {
                        ast_node_t* pval = *nv;
                        result = _inline_block(c->child, v, nv, loop, updated, inl, sl) || result;
                        *nv = pval;
                    }

                    _inline_binary(t->child, v, nv, inl);
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
            VRS_isblock(t->token) ||             /* To nested scope                 */
            (
                (!VRS_one_slot(t->token) || VRS_isptr(t->token)) && 
                t->child
            ) ||                                 /* To possible indexation brackets */
            t->token->t_type == EXIT_TOKEN ||    /* To EXIT body                    */
            t->token->t_type == CALL_TOKEN       /* To CALL arguments               */
        ) {
            result = _inline_block(t->child, v, nv, loop, updated, inl, sl) || result;
            continue;
        }
        
        if (t->child && (VRS_isdecl(t->token) || t->token->t_type == ASSIGN_TOKEN)) {                                  /* Found declaration or upd operation */
            if (str_strncmp(t->child->token->value, v, TOKEN_MAX_SIZE)) _inline_binary(t->child->sibling, v, nv, inl); /* This is not a target               */
            else if (VRS_isnumeric(t->child->sibling->token) || VRS_isvariable(t->child->sibling->token)) {            /* Target updated to new value        */
                *nv = t->child->sibling; 
                if (sl) lchange = t;
            }
            else {                                                                                                      /* Upredicted update operation        */
                if (sl) lchange = t;
                result = 1;
                break;
            }
        }
        else if (VRS_isoperand(t->token) || VRS_isvariable(t->token)) {
            _inline_binary(t, v, nv, inl);
        }
    }

    if (lchange || !sl) {
        ast_node_t* prev = NULL;
        ast_node_t* curr = d;
        while (curr) {
            ast_node_t* next = curr->sibling;
            if (
                curr->token && curr->token->t_type == ASSIGN_TOKEN &&                                          /* This is an update operation      */
                !str_strncmp(curr->child->token->value, v, TOKEN_MAX_SIZE) &&                                  /* Updating of a target variable    */
                (VRS_isnumeric(curr->child->sibling->token) || VRS_isvariable(curr->child->sibling->token)) && /* Updating to a variable / numeric */
                (!lchange || curr != lchange)                                                                  /* Not the last operation           */
            ) {
                AST_remove_node(d->parent, curr);
                AST_unload(curr);
                curr = next;
                continue;
            }
            
            prev = curr;
            curr = next;
        }
    }

    return result;
}

static int _find_declarations(ast_node_t* r) {
    if (!r) return 0;

    ast_node_t* prev = NULL;
    ast_node_t* curr = r;

    while (curr) {
        ast_node_t* next = curr->sibling;
        _find_declarations(curr->child);

        if (
            curr->token &&
            VRS_isdecl(curr->token) && curr->child && curr->child->sibling && VRS_one_slot(curr->token) && /* One slot variable declaration       */
            VRS_instack(curr->token) &&                                                                    /* Not global or read only             */
            (VRS_isnumeric(curr->child->sibling->token) || VRS_isvariable(curr->child->sibling->token))    /* Variable value is variable or num   */
            && VRS_one_slot(curr->child->sibling->token) /* TODO: make optional. Check is variable is one slot. Otherwise this is overhead inline */
        ) {
            int upd = 0;
            ast_node_t* inval = curr->child->sibling;
            print_debug("Variable=%s, inline_to=%s (h)", curr->child->token->value, curr->child->sibling->token->value);
            if (!_inline_block(curr->sibling, curr->child->token->value, &inval, 0, &upd, 1, 1)) {
                _inline_block(curr->sibling, curr->child->token->value, &inval, 0, &upd, 1, 0);
                AST_remove_node(r->parent, curr);
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
    _find_declarations(ctx->r);
    return 1;
}
