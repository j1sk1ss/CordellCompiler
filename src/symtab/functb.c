#include <symtab/functb.h>

int FNTB_get_info_id(symbol_id_t id, func_info_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_get_info(id=%li)", id);
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

int FNTB_get_info(string_t* fname, short sid, func_info_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_get_info(name=%s, sid=%i)", fname ? fname->body : "(null)", sid);
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (fi->name->equals(fi->name, fname) && (sid == -1 || fi->sid == sid)) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    print_warn("FNTB_get_info -> NF!");
    return 0;
}

static func_info_t* _create_func_info(
    string_t* name, 
    int global, int local, int entry, int naked, /* flags */
    ast_node_t* args, ast_node_t* rtype
) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));
    if (name) {
        fn->name = name->copy(name);
    }

    fn->args         = args;
    fn->rtype        = rtype;
    fn->flags.global = global;
    fn->flags.local  = local;
    fn->flags.entry  = entry;
    fn->flags.naked  = naked;
    return fn;
}

static int _is_function_presented(string_t* name, short sid, ast_node_t* args, func_info_t* out, functab_ctx_t* ctx) {
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (
            (sid == -1 || fi->sid == sid) &&
            fi->name->equals(fi->name, name) &&
            AST_hash_node_stop(args->c, SCOPE_TOKEN) == AST_hash_node_stop(fi->args->c, SCOPE_TOKEN)
        ) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    return 0;
}

static string_t* _create_virt_name(symbol_id_t id, string_t* name) {
    string_t* virt  = name->copy(name);
    string_t* index = create_string_from_int(id);
    virt->cat(virt, index);
    destroy_string(index);
    return virt;
}

symbol_id_t FNTB_add_info(
    string_t* name, 
    int global, int local, int entry, int naked, /* flags */
    short sid, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx
) {
    print_log(
        "FNTB_add_info(name=%s, global=%i, ext=%i, entry=%i, naked=%i, args=%lu)", 
        name ? name->body : "(null)", global, entry, naked, args ? AST_hash_node_stop(args->c, SCOPE_TOKEN) : 0
    );
    
    func_info_t out;
    if (_is_function_presented(name, sid, args, &out, ctx)) return out.id; 

    func_info_t* nnd = _create_func_info(name, global, local, entry, naked, args, rtype);
    if (!nnd) return 0;
    nnd->sid = sid;
    
    nnd->id = ctx->curr_id++;
    if (nnd->name) {
        nnd->virt = _create_virt_name(nnd->id, name);
    }

    map_put(&ctx->functb, nnd->id, nnd);
    return nnd->id;
}

int FNTB_rename_func(symbol_id_t id, string_t* name, functab_ctx_t* ctx) {
    print_log("FNTB_rename_func(id=%llu, name=%s)", id, name->body);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        destroy_string(fi->name);
        destroy_string(fi->virt);
        fi->name = name->copy(name);
        fi->virt = _create_virt_name(id, name);
        return 1;
    }

    return 0;
}

int FNTB_update_info(symbol_id_t id, int used, int entry, int ext, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx) {
    print_log("FNTB_update_info(id=%llu, used=%i, entry=%i, ext=%i)", id, used, entry, ext);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (used >= 0)  fi->flags.used     = used;
        if (entry >= 0) fi->flags.entry    = entry;
        if (ext >= 0)   fi->flags.external = ext;
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
