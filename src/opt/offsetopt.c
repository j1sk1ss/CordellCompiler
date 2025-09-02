#include <offsetopt.h>

typedef struct def {
    char        name[TOKEN_MAX_SIZE];
    int         size;
    int         offset;
    struct def* next;
} def_t;

typedef struct {
    def_t* h;
} recalcoff_ctx_t;

static def_t* _create_def(const char* name, int size, int offset) {
    def_t* def = (def_t*)mm_malloc(sizeof(def_t));
    if (!def) return NULL;
    str_strncpy(def->name, name, TOKEN_MAX_SIZE);
    def->size   = size;
    def->offset = offset;
    def->next   = NULL;
    return def;
}

static int _register_var(const char* name, int size, int offset, recalcoff_ctx_t* ctx) {
    def_t* h = ctx->h;
    def_t* def = _create_def(name, size, offset);
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

static int _unload_ctx(recalcoff_ctx_t* ctx) {
    while (ctx->h) {
        def_t* n = ctx->h->next;
        mm_free(ctx->h);
        ctx->h = n;
    }

    return 1;
}

static int _update_offsets(ast_node_t* r, const char* name, int offset) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
#pragma region Navigation
        if (!t->token || t->token->t_type == SCOPE_TOKEN) {
            _update_offsets(t->child, name, offset);
            continue;
        }
#pragma endregion
        if (!str_strcmp(t->token->value, name)) {
            t->info.offset = offset;
        }

        _update_offsets(t->child, name, offset);
    }

    return 0;    
}

static int _find_usage(ast_node_t* r, const char* name) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
#pragma region Navigation
        if (!t->token || t->token->t_type == SCOPE_TOKEN) {
            if (_find_usage(t->child, name)) return 1;
            continue;
        }
#pragma endregion
        if (!str_strcmp(t->token->value, name)) return 1;
        if (_find_usage(t->child, name)) return 1;
    }

    return 0;
}

static int _recalc_offs(ast_node_t* r, syntax_ctx_t* ctx) {
    if (!r) return 0;

    scope_elem_t el;
    scope_top(&ctx->scopes.stack, &el);
    int offset = el.offset;

    recalcoff_ctx_t scope_ctx = { .h = NULL };
    for (ast_node_t* t = r->child; t; t = t->sibling) {
#pragma region Navigation
        if (!t->token) {
            _recalc_offs(t, ctx);
            continue;
        }

        if (VRS_isblock(t->token)) {
            scope_push(&ctx->scopes.stack, ++ctx->scopes.s_id, offset);
            _recalc_offs(t, ctx);
            scope_pop(&ctx->scopes.stack);
            continue;
        }
#pragma endregion
        if (VRS_isdecl(t->token) && VRS_intext(t->token)) {
            int varoff = -1;
            def_t* vars = scope_ctx.h;
            while (vars) {
                if (!_find_usage(t, vars->name)) {
                    varoff = vars->offset;

                    int occupied = 0;
                    def_t* nvars = scope_ctx.h;
                    while (nvars) {
                        if (
                            vars != nvars && 
                            varoff == nvars->offset && 
                            _find_usage(t, nvars->name)
                        ) {
                            occupied = 1;
                            break;
                        }

                        nvars = nvars->next;
                    }

                    if (!occupied) {
                        break;
                    }
                }

                vars = vars->next;
            }

            ast_node_t* name = t->child;
            if (varoff < 0) {
                offset += ALIGN(name->info.size);
                varoff = offset;
            }

            offset = varoff;
            t->info.offset = varoff;

            _register_var(name->token->value, name->info.size, varoff, &scope_ctx);
            _update_offsets(t, name->token->value, varoff);
            continue;
        }

        _recalc_offs(t, ctx);
    }

    _unload_ctx(&scope_ctx);
    return 1;
}

int OPT_offrecalc(syntax_ctx_t* ctx) {
    if (!ctx || !ctx->r) return 0;
    VRT_destroy_ctx(ctx->vars);
    scope_reset(&ctx->scopes.stack);
    ctx->scopes.s_id = 0;
    _recalc_offs(ctx->r, ctx);
    return 1;
}
