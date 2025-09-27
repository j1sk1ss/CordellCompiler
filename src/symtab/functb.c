#include <symtab/functb.h>

int FNTB_get_info_id(long id, func_info_t* out, functab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    func_info_t* fi;
    while ((fi = (func_info_t*)list_iter_next(&it))) {
        if (fi->id == id) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    return 0;
}

int FNTB_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    func_info_t* fi;
    while ((fi = (func_info_t*)list_iter_next(&it))) {
        if (!str_strcmp(fname, fi->name)) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    return 0;
}

static func_info_t* _create_func_info(const char* name, int global, int external, ast_node_t* args, ast_node_t* rtype) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));
    if (name) str_strncpy(fn->name, name, TOKEN_MAX_SIZE);
    fn->args     = args;
    fn->rtype    = rtype;
    fn->global   = global;
    fn->external = external;
    return fn;
}

int FNTB_add_info(const char* name, int global, int external, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_debug("FNTB_add_info(name=%s, global=%i, ext=%i)", name, global, external);
    func_info_t* nnd = _create_func_info(name, global, external, args, rtype);
    if (!nnd) return 0;
    nnd->id = ctx->curr_id++;
    list_add(&ctx->lst, nnd);
    return nnd->id;
}

int FNTB_unload(functab_ctx_t* ctx) {
    return list_free_force(&ctx->lst);
}
