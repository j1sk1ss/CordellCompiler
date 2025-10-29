#include <symtab/functb.h>

int FNTB_get_info_id(long id, func_info_t* out, functab_ctx_t* ctx) {
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (out) str_memcpy(out, fi, sizeof(func_info_t));
        return 1;
    }

    return 0;
}

int FNTB_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx) {
    map_iter_t it;
    map_iter_init(&ctx->functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        if (!str_strcmp(fname, fi->name)) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    return 0;
}

static func_info_t* _create_func_info(const char* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));
    if (name) str_strncpy(fn->name, name, TOKEN_MAX_SIZE);
    fn->args     = args;
    fn->rtype    = rtype;
    fn->global   = global;
    fn->external = external;
    fn->entry    = entry;
    return fn;
}

int FNTB_add_info(const char* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log("FNTB_add_info(name=%s, global=%i, ext=%i, entry=%i)", name, global, entry, external);
    func_info_t* nnd = _create_func_info(name, global, external, entry, args, rtype);
    if (!nnd) return 0;
    nnd->id = ctx->curr_id++;
    map_put(&ctx->functb, nnd->id, nnd);
    return nnd->id;
}

int FNTB_update_info(long id, int used, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log("FNTB_update_info(id=%llu, used=%i, entry=%i)", id, used, entry);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (used >= 0)  fi->used  = used;
        if (entry >= 0) fi->entry = entry;
        if (args)       fi->args  = args;
        if (rtype)      fi->rtype = rtype;
        return 1;
    }

    return 0;
}

int FNTB_unload(functab_ctx_t* ctx) {
    return map_free_force(&ctx->functb);
}
