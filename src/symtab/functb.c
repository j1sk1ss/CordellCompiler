#include <symtab/functb.h>

int FNTB_get_info_id(symbol_id_t id, func_info_t* out, functab_ctx_t* ctx) {
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (out) str_memcpy(out, fi, sizeof(func_info_t));
        return 1;
    }

    return 0;
}

int FNTB_collect_info(string_t* fname, list_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_collect_info(name=%s)", fname ? fname->body : "(null)");
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (fi->name->equals(fi->name, fname)) {
            list_add(out, fi);
        }
    }

    print_warn("FNTB_collect_info -> %i!", list_size(out));
    return 1;
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
    if (name) {
        fn->name = name->copy(name);
    }

    fn->args           = args;
    fn->rtype          = rtype;
    fn->flags.global   = global;
    fn->flags.external = external;
    fn->flags.entry    = entry;
    return fn;
}

static int _is_function_presented(string_t* name, ast_node_t* args, func_info_t* out, functab_ctx_t* ctx) {
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (
            fi->name->equals(fi->name, name) &&
            AST_hash_node_stop(args->c, SCOPE_TOKEN) == AST_hash_node_stop(fi->args->c, SCOPE_TOKEN)
        ) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    return 0;
}

symbol_id_t FNTB_add_info(string_t* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log(
        "FNTB_add_info(name=%s, global=%i, ext=%i, entry=%i, args=%lu)", 
        name ? name->body : "(null)", global, entry, external, args ? AST_hash_node_stop(args->c, SCOPE_TOKEN) : 0
    );
    
    func_info_t out;
    if (_is_function_presented(name, args, &out, ctx)) return out.id; 

    func_info_t* nnd = _create_func_info(name, global, external, entry, args, rtype);
    if (!nnd) return 0;
    
    nnd->id = ctx->curr_id++;
    if (nnd->name) {
        nnd->virt = nnd->name->copy(nnd->name);
        string_t* index = create_string_from_int(nnd->id);
        nnd->virt->cat(nnd->virt, index);
        destroy_string(index);
    }

    map_put(&ctx->functb, nnd->id, nnd);
    return nnd->id;
}

int FNTB_update_info(symbol_id_t id, int used, int entry, int ext, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log("FNTB_update_info(id=%llu, used=%i, entry=%i)", id, used, entry);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (used >= 0)  fi->flags.used     = used;
        if (entry >= 0) fi->flags.entry    = entry;
        if (entry >= 0) fi->flags.external = ext;
        if (args)       fi->args  = args;
        if (rtype)      fi->rtype = rtype;
        return 1;
    }

    return 0;
}

static int _function_info_unload(func_info_t* info) {
    destroy_string(info->name);
    destroy_string(info->virt);
    return mm_free(info);
}

int FNTB_unload(functab_ctx_t* ctx) {
    return map_free_force_op(&ctx->functb, (int (*)(void *))_function_info_unload);
}
