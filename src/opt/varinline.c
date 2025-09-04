#include <varinline.h>

static int _find_usage(
    ast_node_t* root, char* varname, short s_id, int* assign, int offset
) {
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (offset-- > 0) continue;
        if (VRS_isblock(t->token)) {
            _find_usage(t, varname, s_id, assign, 0);
            continue;
        }

        _find_usage(t, varname, s_id, assign, 0);
        if (VRS_isdecl(t->token) || VRS_isoperand(t->token)) {
            if (
                t->token->t_type == ASSIGN_TOKEN && /* If variable assign somewhere, we can't tell that we able inline */
                !str_strncmp(varname, t->child->token->value, TOKEN_MAX_SIZE)
            ) {
                *assign = 1;
                return 1;
            }

            _find_usage(t, varname, s_id, assign, VRS_isdecl(t->token));
            continue;
        }

        if (!str_strncmp(varname, t->token->value, TOKEN_MAX_SIZE) && t->info.s_id == s_id) {
            return 1;
        }
    }

    return 1;
}

static int _inline_var(
    ast_node_t* root, char* varname, short s_id, int value, int offset
) {
    if (!root) return 0;
    int index = 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
        if (index++ < offset) continue;
        if (VRS_isblock(t->token)) {
            _inline_var(t, varname, s_id, value, 0);
            continue;
        }
              
        _inline_var(t, varname, s_id, value, 0);
        if (VRS_isdecl(t->token) || VRS_isoperand(t->token)) {
            _inline_var(t, varname, s_id, value, (VRS_isdecl(t->token) || t->token->t_type == ASSIGN_TOKEN));
            continue;
        }

        if (!str_strncmp(varname, t->token->value, TOKEN_MAX_SIZE) && t->info.s_id == s_id) {
            snprintf(t->token->value, TOKEN_MAX_SIZE, "%d", value);
            
            t->token->t_type = t->token->t_type != CASE_TOKEN ? UNKNOWN_NUMERIC_TOKEN : CASE_TOKEN;
            t->token->vinfo.glob = t->token->t_type != CASE_TOKEN ? 1 : 0;
            
            t->info.offset = 0;
            t->info.size   = BASE_BITNESS / 8;
        }   
    }

    return 1;
}

static int _find_decl(ast_node_t* root, ast_node_t* entry, int* change) {
    if (!root) return 0;
    
    ast_node_t* prev = NULL;
    ast_node_t* curr = root->child;

    while (curr) {
        ast_node_t* next = curr->sibling;
        if (VRS_isblock(curr->token)) {
            _find_decl(curr, entry, change);
            prev = curr;
            curr = next;
            continue;
        }

        switch (curr->token->t_type) {
            case IF_TOKEN:
            case WHILE_TOKEN:
            case SWITCH_TOKEN:
                _find_decl(curr, entry, change);
                prev = curr;
                curr = next;
            continue;
            case FUNC_TOKEN:
                _find_decl(curr->child->sibling->sibling, entry, change);
                prev = curr;
                curr = next;
            continue;
            default: break;
        }

        if (VRS_isdecl(curr->token) && VRS_one_slot(curr->token)) {
            ast_node_t* name_node = curr->child;
            ast_node_t* val_node  = name_node->sibling;

            if (!val_node || val_node->token->t_type != UNKNOWN_NUMERIC_TOKEN) {
                prev = curr;
                curr = next;
                continue;
            }

            int is_updates = 0;
            _find_usage(entry, name_node->token->value, name_node->info.s_id, &is_updates, 0);
            if (!is_updates) { /* If variable never updates, we can replace it by value and remove declaration node */
                char varname[TOKEN_MAX_SIZE] = { 0 };
                str_strncpy(varname, name_node->token->value, TOKEN_MAX_SIZE);

                AST_remove_node(root, curr);
                AST_unload(curr);

                int value = str_atoi(val_node->token->value);
                _inline_var(entry, varname, name_node->info.s_id, value, 0);
                *change = 1;

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

int OPT_varinline(syntax_ctx_t* ctx) {
    if (!ctx->r) return 0;
    int is_changed = 0;
    _find_decl(ctx->r, ctx->r, &is_changed);
    return is_changed;
}
