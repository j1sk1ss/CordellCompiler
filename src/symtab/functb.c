#include <symtab/functb.h>

int FNTB_get_info_id(long id, func_info_t* out, functab_ctx_t* ctx) {
    func_info_t* h = ctx->h;
    while (h) {
        if (h->id == id) {
            if (out) str_memcpy(out, h, sizeof(func_info_t));
            return 1;
        }
        
        h = h->next;
    }

    return 0;
}

int FNTB_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx) {
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

static func_info_t* _create_func_info(
    const char* name, int global, int external, ast_node_t* args, ast_node_t* rtype
) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));

    if (name) str_strncpy(fn->name, name, TOKEN_MAX_SIZE);
    fn->args     = args;
    fn->rtype    = rtype;
    fn->global   = global;
    fn->external = external;
    fn->next     = NULL;
    return fn;
}

int FNTB_add_info(const char* name, int global, int external, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_debug("FNTB_add_info(name=%s, global=%i, ext=%i)", name, global, external);
    func_info_t* nnd = _create_func_info(name, global, external, args, rtype);
    if (!nnd) return 0;

    nnd->id = ctx->curr_id++;
    if (!ctx->h) {
        ctx->h = nnd;
        return nnd->id;
    }

    func_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return nnd->id;
}

int FNTB_unload(functab_ctx_t* ctx) {
    func_info_t* h = ctx->h;
    while (h) {
        func_info_t* n = h->next;
        mm_free(h);
        h = n;
    }
    
    ctx->h = NULL;
    return 1;
}
