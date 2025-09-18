#include <ast/opt/offsetopt.h>

typedef struct def {
    int         heap;
    int         size;
    int         offset;
    char        name[TOKEN_MAX_SIZE];
    short       s_id;
    struct def* owners;
    struct def* next;
} def_t;

typedef struct {
    def_t* h;
} recalcoff_ctx_t;

static def_t* _create_def(int size, int offset, const char* name, short s_id, int heap) {
    def_t* def = (def_t*)mm_malloc(sizeof(def_t));
    if (!def) return NULL;
    if (name) str_strncpy(def->name, name, TOKEN_MAX_SIZE);
    def->heap   = heap;
    def->offset = offset;
    def->size   = size;
    def->s_id   = s_id;
    def->next   = NULL;
    def->owners = NULL;
    return def;
}

static int _add_owner(const char* name, short s_id, int heap, def_t* var) {
    if (!var) return 0;

    def_t* h = var->owners;
    while (h) {
        if (!str_strcmp(h->name, name)) return 0;
        h = h->next;
    }

    def_t* owner = _create_def(0, 0, name, s_id, heap);
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

static int _remove_owner(const char* name, short s_id, def_t* var) {
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

static int _register_var(int size, int offset, const char* name, short s_id, int heap, recalcoff_ctx_t* ctx) {
    def_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name)) return 0;
        h = h->next;
    }

    h = ctx->h;
    def_t* def = _create_def(size, offset, name, s_id, heap);
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

static void _clean_by_sid(short s_id, stack_map_t* stack, recalcoff_ctx_t* ctx) {
    print_debug("_clean_by_sid(s_id=%i)", s_id);

    def_t* h = ctx->h;
    def_t* prev = NULL;

    while (h) {
        def_t* next = h->next;
        if (h->s_id != s_id) prev = h;
        else {
            if (!prev) ctx->h = h->next;
            else prev->next = h->next;

            stack_map_free(h->offset, h->size, stack);
            mm_free(h);

            def_t* cur = ctx->h;
            while (cur) {
                _remove_owner(h->name, s_id, cur);
                cur = cur->next;
            }
        }

        h = next;
    }
}

static int _unregister_var(const char* name, short s_id, recalcoff_ctx_t* ctx) {
    print_debug("_unregister_var(name=%s, s_id=%i)", name, s_id);

    def_t* h = ctx->h;
    def_t* prev = NULL;

    while (h) {
        if (!str_strcmp(h->name, name)) {
            if (h->owners) return 0;
            if (!prev) ctx->h = h->next;
            else prev->next = h->next;

            mm_free(h);

            def_t* cur = ctx->h;
            while (cur) {
                _remove_owner(name, s_id, cur);
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
            (VRS_isdecl(t->token) || VRS_update_operator(t->token)) &&
            t->child && t->child->token->flags.ptr
        ) {
            int is_used = 0, is_ref = 0;
            _find_usage(t->child->sibling, v, &is_used, &is_ref);
            if (!is_used) continue;

            *used = 1;
            *ref  = 0;

            if (is_ref) {
                print_debug("Variable %s.%i owned by %s.%i", v->name, v->s_id, t->child->token->value, t->child->sinfo.s_id);
                _add_owner(t->child->token->value, t->child->sinfo.s_id, t->child->token->flags.heap, v);
            }
        }

        if (!str_strncmp(t->token->value, v->name, TOKEN_MAX_SIZE)) {
            *used = 1;
            *ref  = t->token->flags.ref;
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

        if (!str_strcmp(t->token->value, name)) t->sinfo.offset = offset;
        _change_varoff_scope(t->child, name, offset);
    }

    return 0;    
}

static int _recalc_offs(ast_node_t* r, stack_map_t* stack, recalcoff_ctx_t* rctx, syntax_ctx_t* ctx) {
    if (!r) return 0;

    scope_elem_t el;
    scope_top(&ctx->scopes.stack, &el);
    int offset = el.id >= 0 ? el.offset : 8;

    for (ast_node_t* t = r; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            scope_push(&ctx->scopes.stack, t->sinfo.s_id, offset);
            _recalc_offs(t->child, stack, rctx, ctx);
            int scvars = scope_id_top(&ctx->scopes.stack);
            _clean_by_sid(scvars, stack, rctx);
            scope_pop(&ctx->scopes.stack);
            continue;
        }
        
        if (VRS_isdecl(t->token) && t->child && VRS_instack(t->token)) {
            int unreg_var = 0;
            do {
                unreg_var = 0;
                def_t* vh = rctx->h;
                while (vh) {
                    def_t* next = vh->next;
                    int is_used = 0, is_ref = 0;
                    _find_usage(t, vh, &is_used, &is_ref);
                    if (!is_used && t->child->sinfo.s_id <= vh->s_id && !vh->heap) {
                        int foffset = vh->offset, fsize = vh->size;
                        print_debug("Try to free %s, size=%i, off=%i", vh->name, vh->size, vh->offset);
                        if (_unregister_var(vh->name, vh->s_id, rctx)) {
                            print_debug("Free success size=%i, off=%i", fsize, foffset);
                            stack_map_free(foffset, fsize, stack);
                            unreg_var = 1;
                        }
                    }
                    
                    vh = next;
                }
            } while (unreg_var);
            
            ast_node_t* name = t->child;
            t->sinfo.offset = stack_map_alloc(name->sinfo.size, stack);
            print_debug("Alloc for %s size=%i, off=%i", name->token->value, name->sinfo.size, t->sinfo.offset);
            offset = MAX(offset, t->sinfo.offset);

            _register_var(name->sinfo.size, t->sinfo.offset, name->token->value, name->sinfo.s_id, name->token->flags.heap, rctx);
            _change_varoff_scope(t, name->token->value, t->sinfo.offset);
            continue;
        }

        _recalc_offs(t->child, stack, rctx, ctx);
    }
    
    return 1;
}

int OPT_offrecalc(syntax_ctx_t* ctx) {
    if (!ctx || !ctx->r) return 0;
    
    scope_reset(&ctx->scopes.stack);
    ctx->scopes.s_id = 0;

    stack_map_t stack;
    stack_map_init(8, &stack);

    recalcoff_ctx_t rctx = { .h = NULL };
    _recalc_offs(ctx->r, &stack, &rctx, ctx);
    _unload_ctx(&rctx);
    
    return 1;
}
