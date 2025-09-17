#include <semantic.h>

typedef struct def {
    short       s_id;
    char        name[TOKEN_MAX_SIZE];
    struct def* owners;
    struct def* next;
} def_t;

typedef struct {
    def_t* h;
} ownership_t;

static def_t* _create_def(short s_id, const char* name) {
    def_t* def = (def_t*)mm_malloc(sizeof(def_t));
    if (!def) return NULL;
    if (name) str_strncpy(def->name, name, TOKEN_MAX_SIZE);
    def->s_id   = s_id;
    def->next   = NULL;
    def->owners = NULL;
    return def;
}

static def_t* _get_var(short s_id, const char* name, ownership_t* ctx) {
    if (!ctx || !ctx->h) return NULL;
    def_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name) && h->s_id == s_id) return h;
        h = h->next;
    }

    return NULL;
}

static int _add_owner(short s_id, const char* name, def_t* var) {
    if (!var) return 0;

    def_t* h = var->owners;
    while (h) {
        if (!str_strcmp(h->name, name)) return 0;
        h = h->next;
    }

    def_t* owner = _create_def(s_id, name);
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

static def_t* _register_var(short s_id, const char* name, ownership_t* ctx) {
    def_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, name)) return NULL;
        h = h->next;
    }

    h = ctx->h;
    def_t* def = _create_def(s_id, name);
    if (!def) return NULL;

    if (!h) {
        ctx->h = def;
        return def;
    }

    while (h->next) {
        h = h->next;
    }

    h->next = def;
    return def;
}

static int _unregister_var(const char* name, ownership_t* ctx) {
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

static int _unload_ctx(ownership_t* ctx) {
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

static int _find_reference_var(ast_node_t* r, def_t* v) { 
    for (ast_node_t* t = r; t; t = t->sibling) { 
        if (_find_reference_var(t->child, v)) return 1; 
        if (
            t->token && !str_strncmp(t->token->value, v->name, TOKEN_MAX_SIZE) && 
            t->sinfo.s_id == v->s_id && t->token->flags.ref
        ) { 
            return 1; 
        } 
    } 
    
    return 0; 
} 

static int _track_ownership(ast_node_t* r, def_t* v, ownership_t* ctx) {
    for (ast_node_t* t = r; t; t = t->sibling) { 
        _track_ownership(t->child, v, ctx); 
        if (t->token && t->child && (VRS_isdecl(t->token) || t->token->t_type == ASSIGN_TOKEN)) {
            if (_find_reference_var(t->child->sibling, v)) {
                _add_owner(t->child->sinfo.s_id, t->child->token->value, v);
            } 
        } 
    } 
    
    return 1; 
} 

static int _find_declarations(ast_node_t* r, ownership_t* ctx) { 
    if (!r) return 0; 
    for (ast_node_t* t = r; t; t = t->sibling) { 
        _find_declarations(t->child, ctx); 
        if (t->token && VRS_isdecl(t->token) && t->child) {
            def_t* var = _register_var(t->child->sinfo.s_id, t->child->token->value, ctx);
            if (var) _track_ownership(t, var, ctx); 
        } 
    } 
    
    return 1; 
} 

int SMT_check_ownership(ast_node_t* node) { 
    ownership_t ctx = { .h = NULL };
    _find_declarations(node, &ctx);

    for (def_t* v = ctx.h; v; v = v->next) {
        int owners = 0;
        for (def_t* o = v->owners; o; o = o->next, owners++) {
            def_t* owner = _get_var(o->s_id, o->name, &ctx);
            for (def_t* oo = owner->owners; oo; oo = oo->next) {
                print_warn("Owner own another owner! %s -> %s -> %s", v->name, o->name, oo->name);
                owners++;
            }
        }

        if (owners > 1) {
            print_warn("Variable %s owned multiple times! Did you expect this?", v->name);
        }
    }

    _unload_ctx(&ctx);
    return 1; 
}
