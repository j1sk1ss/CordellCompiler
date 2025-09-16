#include <deadopt.h>

typedef struct deadfunc_info {
    struct deadfunc_info* next;
    char                  name[TOKEN_MAX_SIZE];
    int                   deadend;
} deadfunc_info_t;

typedef struct {
    deadfunc_info_t* h;
} deadopt_ctx_t;

deadfunc_info_t* _create_func_info(const char* name, int deadend, deadopt_ctx_t* ctx) {
    deadfunc_info_t* fi = (deadfunc_info_t*)mm_malloc(sizeof(deadfunc_info_t));
    if (!fi) return NULL;
    str_strncpy(fi->name, name, TOKEN_MAX_SIZE);
    fi->deadend = deadend;
    fi->next    = NULL;
    return fi;
}

int _register_func(const char* name, int deadend, deadopt_ctx_t* ctx) {
    print_debug("_register_func(name=%s, deadend=%i)", name, deadend);
    if (!ctx) return 0;
    deadfunc_info_t* fi = _create_func_info(name, deadend, ctx);
    if (!fi) return 0;
    fi->next = ctx->h;
    ctx->h   = fi;
    return 1;
}

int _unload_ctx(deadopt_ctx_t* ctx) {
    if (!ctx) return 0;
    deadfunc_info_t* curr = ctx->h;
    while (curr) {
        deadfunc_info_t* next = curr->next;
        mm_free(curr);
        curr = next;
    }

    ctx->h = NULL;
    return 1;
}

int _get_func(const char* name, deadopt_ctx_t* ctx) {
    if (!ctx) return 0;
    deadfunc_info_t* curr = ctx->h;
    while (curr) {
        if (!str_strncmp(curr->name, name, TOKEN_MAX_SIZE)) return curr->deadend;
        curr = curr->next;
    }

    return 0;
}

static int _block_walk(ast_node_t* r, int* dead, int* ret, deadopt_ctx_t* ctx) {
    if (!r) return 0;

    ast_node_t* end = NULL;
    for (end = r; end; end = end->sibling) {
        if (!end->token || end->token->t_type == START_TOKEN) {
            _block_walk(end->child, dead, ret, ctx);
            continue;
        }

        switch (end->token->t_type) {
            case SCOPE_TOKEN: {
                _block_walk(end->child, dead, ret, ctx);
                if (*dead || *ret) goto _deadend;
                break;
            }
            case FUNC_TOKEN: {
                int killfunc = 0;
                _block_walk(end->child->sibling, &killfunc, ret, ctx);
                _register_func(end->child->token->value, killfunc, ctx);
                break;
            }
            case IF_TOKEN:
            case WHILE_TOKEN: {
                int ldead_sc = 0, lret_sc = 0;
                int rdead_sc = 0, rret_sc = 0;
                ast_node_t* lbranch = end->child->sibling;
                ast_node_t* rbranch = lbranch->sibling;

                if (lbranch) _block_walk(lbranch, &ldead_sc, &lret_sc, ctx);
                if (rbranch) _block_walk(rbranch, &rdead_sc, &rret_sc, ctx);
                if (lbranch && lbranch && ldead_sc && rdead_sc) *dead = 1;
                else if (
                    lbranch && lbranch && 
                    ((rdead_sc || rret_sc )&& (ldead_sc || lret_sc))
                ) *ret = 1;

                if (lbranch && rbranch && (*dead || *ret)) goto _deadend;
                break;
            }
            case SWITCH_TOKEN: {
                int dead_switch = 1, ret_switch = 1;
                ast_node_t* cases = end->child->sibling->child;
                for (ast_node_t* c = cases; c; c = c->sibling) {
                    int dead_case = 0, ret_case = 0;
                    if (c->token->t_type == DEFAULT_TOKEN) _block_walk(c->child, &dead_case, &ret_case, ctx);
                    else _block_walk(c->child->sibling, &dead_case, &ret_case, ctx);
                    dead_switch = dead_switch && dead_case;
                    ret_switch  = ret_switch && ret_case;
                }

                if (dead_switch) *dead = 1;
                else if (ret_switch) *ret = 1;
                if ((*dead || *ret)) goto _deadend;
                break;
            }
            case CALL_TOKEN: {
                int iskiller = _get_func(end->token->value, ctx);
                if (iskiller) goto _deadend;
                break;
            }
        }

        if (end->token->t_type == RETURN_TOKEN) *ret = 1;
        else if (end->token->t_type == EXIT_TOKEN) *dead = 1;
    }
_deadend: {}
    if (end && end->sibling) {
        ast_node_t* next = end->sibling;
        while (next) {
            ast_node_t* tmp = next->sibling;
            AST_unload(next);
            next = tmp;
        }

        end->sibling = NULL;
    }

    return 1;
}

int OPT_deadcode(syntax_ctx_t* ctx) {
    deadopt_ctx_t dctx = { .h = NULL };
    int dead = 0, ret = 0;
    _block_walk(ctx->r, &dead, &ret, &dctx);
    _unload_ctx(&dctx);
    return 1;
}
