#include <offsetopt.h>

typedef struct def {
    char        name[TOKEN_MAX_SIZE];
    token_t*    token;
    int         size;
    int         offset;
    short       s_id;
    struct def* next;
} def_t;

typedef struct {
    def_t* h;
} recalcoff_ctx_t;

static def_t* _create_def(const char* name, int size, int offset, short s_id, token_t* tkn) {
    def_t* def = (def_t*)mm_malloc(sizeof(def_t));
    if (!def) return NULL;
    str_strncpy(def->name, name, TOKEN_MAX_SIZE);
    def->s_id   = s_id,
    def->size   = size;
    def->offset = offset;
    def->token  = tkn;
    def->next   = NULL;
    return def;
}

static int _register_var(const char* name, int size, int offset, short s_id, token_t* tkn, recalcoff_ctx_t* ctx) {
    def_t* h = ctx->h;
    def_t* def = _create_def(name, size, offset, s_id, tkn);
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
        if (VRS_isblock(t->token)) {
            _update_offsets(t->child, name, offset);
            continue;
        }

        if (!str_strcmp(t->token->value, name)) t->info.offset = offset;
        _update_offsets(t->child, name, offset);
    }

    return 0;    
}

static int _find_usage(ast_node_t* r, const char* name) {
    if (!r) return 0;
    for (ast_node_t* t = r; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            if (_find_usage(t->child, name)) return 1;
            continue;
        }

        if (!str_strcmp(t->token->value, name)) return 1;
        if (_find_usage(t->child, name)) return 1;
    }

    return 0;
}

static int _recalc_offs(ast_node_t* r, syntax_ctx_t* ctx) {
    if (!r) return 0;

    scope_elem_t el;
    scope_top(&ctx->scopes.stack, &el);
    int offset = el.id >= 0 ? el.offset : 0;

    recalcoff_ctx_t scope_ctx = { .h = NULL };
    for (ast_node_t* t = r->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            scope_push(&ctx->scopes.stack, ++ctx->scopes.s_id, offset);
            _recalc_offs(t, ctx);
            scope_pop(&ctx->scopes.stack);
            continue;
        }
        
        if (VRS_isdecl(t->token) && t->child && VRS_intext(t->token)) {
            int varoff = -1;
            def_t* vars = scope_ctx.h;
            while (vars) {
                if (!_find_usage(t, vars->name) && t->child->info.s_id == vars->s_id) {
                    int occupied = 0;
                    def_t* nvars = scope_ctx.h;
                    while (nvars) {
                        if (
                            vars != nvars && vars->offset == nvars->offset && 
                            _find_usage(t, nvars->name) && t->child->info.s_id == vars->s_id
                        ) {
                            occupied = 1;
                            break;
                        }

                        nvars = nvars->next;
                    }

                    if (!occupied) {
                        varoff = vars->offset;
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

            t->info.offset = varoff;

            _register_var(name->token->value, name->info.size, varoff, name->info.s_id, name->token, &scope_ctx);
            _update_offsets(t, name->token->value, varoff);
            continue;
        }

        scope_push(&ctx->scopes.stack, ctx->scopes.s_id, offset);
        _recalc_offs(t, ctx);
        scope_pop(&ctx->scopes.stack);
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
