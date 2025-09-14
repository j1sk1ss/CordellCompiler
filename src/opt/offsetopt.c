#include <offsetopt.h>

typedef struct def {
    int         offset;
    char        name[TOKEN_MAX_SIZE];
    struct def* owners;
    struct def* next;
} def_t;

typedef struct {
    def_t* h;
} recalcoff_ctx_t;

static def_t* _create_def(int offset, const char* name) {
    def_t* def = (def_t*)mm_malloc(sizeof(def_t));
    if (!def) return NULL;
    if (name) str_strncpy(def->name, name, TOKEN_MAX_SIZE);
    def->offset = offset;
    def->next   = NULL;
    def->owners = NULL;
    return def;
}

static def_t* _get_var(const char* name, recalcoff_ctx_t* ctx) {
    if (!ctx || !ctx->h) return NULL;
    def_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name)) return h;
        h = h->next;
    }

    return NULL;
}

static int _add_owner(const char* name, def_t* var) {
    if (!var) return 0;

    def_t* h = var->owners;
    while (h) {
        if (!str_strcmp(h->name, name)) return 0;
        h = h->next;
    }

    def_t* owner = _create_def(0, name);
    if (!owner) return 0;

    if (!var->owners) {
        var->owners = owner;
        return 1;
    }

    h = var->owners;
    while (h->next) {
        h = h->next;
    }

    h->next = owner;
    return 1;
}

static int _remove_owner(const char* name, def_t* var) {
    if (!var || !var->owners) return 0;

    def_t* h = var->owners;
    def_t* prev = NULL;

    while (h) {
        if (!str_strcmp(h->name, name)) {
            if (!prev) var->owners = h->next;
            else prev->next = h->next;

            mm_free(h);
            return 1;
        }

        prev = h;
        h = h->next;
    }

    return 0;
}

static int _register_var(int offset, const char* name, recalcoff_ctx_t* ctx) {
    def_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name)) return 0;
        h = h->next;
    }

    h = ctx->h;
    def_t* def = _create_def(offset, name);
    if (!def) return 0;

    if (!h) {
        ctx->h = def;
        return 1;
    }

    while (h->next) {
        h = h->next;
    }

    h->next = def;
    return 1;
}

static int _unregister_var(const char* name, recalcoff_ctx_t* ctx) {
    def_t* h = ctx->h;
    def_t* prev = NULL;

    while (h) {
        if (!str_strcmp(h->name, name)) {
            if (h->owners) return 0;
            if (!prev) ctx->h = h->next;
            else prev->next = h->next;

            def_t* tmp = h->next;
            mm_free(h);
            h = tmp;

            def_t* cur = ctx->h;
            while (cur) {
                _remove_owner(name, cur);
                cur = cur->next;
            }

            return 1;
        }

        prev = h;
        h = h->next;
    }

    return 0;
}

static int _unload_ctx(recalcoff_ctx_t* ctx) {
    while (ctx->h) {
        def_t* cur = ctx->h;
        ctx->h = cur->next;

        while (cur->owners) {
            def_t* o = cur->owners;
            cur->owners = o->next;
            mm_free(o);
        }

        mm_free(cur);
    }

    return 1;
}

/* Find variable usage in scope below */
static int _find_usage(ast_node_t* r, def_t* v, int* used, int* ref) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _find_usage(t->child, v, used, ref);
            continue;
        }
        
        if (
            (VRS_isdecl(t->token) || t->token->t_type == ASSIGN_TOKEN) &&
            t->child && t->child->token->vinfo.ptr
        ) {
            int is_used = 0, is_ref = 0;
            _find_usage(t->child->sibling, v, &is_used, &is_ref);
            if (!is_used) continue;

            *used = 1;
            *ref  = 0;

            if (is_ref) {
                _add_owner(t->child->token->value, v);
            }
        }

        if (!str_strncmp(t->token->value, v->name, TOKEN_MAX_SIZE)) {
            *used = 1;
            *ref  = t->token->vinfo.ref;
            continue;
        }

        _find_usage(t->child, v, used, ref);
    }

    return 1;
}

/* Update variable offset in current scope */
static int _change_varoff_scope(ast_node_t* r, const char* name, int offset) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _change_varoff_scope(t->child, name, offset);
            continue;
        }

        if (!str_strcmp(t->token->value, name)) t->info.offset = offset;
        _change_varoff_scope(t->child, name, offset);
    }

    return 0;    
}

static int _recalc_offs(ast_node_t* r, syntax_ctx_t* ctx) {
    if (!r) return 0;

    scope_elem_t el;
    scope_top(&ctx->scopes.stack, &el);
    int offset = el.id >= 0 ? el.offset : 0;

    recalcoff_ctx_t scope_ctx = { .h = NULL };
    for (ast_node_t* t = r; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            scope_push(&ctx->scopes.stack, ++ctx->scopes.s_id, offset);
            _recalc_offs(t->child, ctx);
            scope_pop(&ctx->scopes.stack);
            continue;
        }
        
        if (VRS_isdecl(t->token) && t->child && VRS_instack(t->token)) {
            int varoff = -1;
            def_t* vh = scope_ctx.h;
            while (vh) {
                int is_used = 0, is_ref = 0;
                _find_usage(t, vh, &is_used, &is_ref);
                if (!is_used && _unregister_var(vh->name, &scope_ctx)) {
                    varoff = vh->offset;
                    break;
                }

                vh = vh->next;
            }
            
            ast_node_t* name = t->child;
            if (varoff < 0) {
                offset += ALIGN(name->info.size);
                varoff = offset;
            }

            t->info.offset = varoff;

            _register_var(varoff, name->token->value, &scope_ctx);
            _change_varoff_scope(t, name->token->value, varoff);
            continue;
        }

        scope_push(&ctx->scopes.stack, ctx->scopes.s_id, offset);
        _recalc_offs(t->child, ctx);
        scope_pop(&ctx->scopes.stack);
    }

    _unload_ctx(&scope_ctx);
    return 1;
}

int OPT_offrecalc(syntax_ctx_t* ctx) {
    if (!ctx || !ctx->r) return 0;
    scope_reset(&ctx->scopes.stack);
    ctx->scopes.s_id = 0;
    _recalc_offs(ctx->r, ctx);
    return 1;
}
