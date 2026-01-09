#include <symtab/functb.h>

int FNTB_get_info_id(long id, func_info_t* out, functab_ctx_t* ctx) {
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (out) str_memcpy(out, fi, sizeof(func_info_t));
        return 1;
    }

    return 0;
}

int FNTB_get_info(string_t* fname, func_info_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_get_info(name=%s)", fname ? fname->body : "(null)");
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (fi->name->equals(fi->name, fname)) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    print_warn("FNTB_get_info -> NF!");
    return 0;
}

static func_info_t* _create_func_info(string_t* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));
    if (name) fn->name = name->copy(name);
    fn->args     = args;
    fn->rtype    = rtype;
    fn->global   = global;
    fn->external = external;
    fn->entry    = entry;
    return fn;
}

long FNTB_add_info(string_t* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log("FNTB_add_info(name=%s, global=%i, ext=%i, entry=%i)", name ? name->body : "(null)", global, entry, external);
    
    func_info_t out;
    if (FNTB_get_info(name, &out, ctx)) return out.id;

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

static int _function_info_unload(func_info_t* info) {
    destroy_string(info->name);
    return mm_free(info);
}

int FNTB_unload(functab_ctx_t* ctx) {
    return map_free_force_op(&ctx->functb, (int (*)(void *))_function_info_unload);
}
