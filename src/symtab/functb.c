#include <symtab/functb.h>

functab_ctx_t* FNT_create_ctx() {
    functab_ctx_t* ctx = (functab_ctx_t*)mm_malloc(sizeof(functab_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(functab_ctx_t));
    return ctx;
}

int FNT_destroy_ctx(functab_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int FNT_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx) {
    func_info_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(fname, h->name)) {
            if (out) str_memcpy(out, h, sizeof(func_info_t));
            return 1;
        }
        
        h = h->next;
    }

    return 0;
}

static func_info_t* _create_func_info(const char* name, ast_node_t* args, ast_node_t* rtype) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));

    if (name) str_strncpy(fn->name, name, TOKEN_MAX_SIZE);
    fn->args  = args;
    fn->rtype = rtype;
    fn->next  = NULL;
    return fn;
}

int FNT_add_info(const char* name, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    func_info_t* nnd = _create_func_info(name, args, rtype);
    if (!nnd) return 0;
    if (!ctx->h) {
        ctx->h = nnd;
        return 1;
    }

    func_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return 1;
}

int FNT_unload(functab_ctx_t* ctx) {
    func_info_t* h = ctx->h;
    while (h) {
        func_info_t* n = h->next;
        mm_free(h);
        h = n;
    }
    
    ctx->h = NULL;
    return 1;
}
