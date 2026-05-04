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

int FNTB_collect_info(string_t* fname, symbol_id_t s_id, list_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_collect_info(name=%s, s_id=%li)", fname ? fname->body : "(null)", s_id);
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (
            fi->name->equals(fi->name, fname) && 
            fi->s_id == s_id && !fi->flags.generic
        ) list_add(out, fi);
    }

    print_warn("FNTB_collect_info -> %i!", list_size(out));
    return 1;
}

int FNTB_get_info(string_t* fname, symbol_id_t s_id, func_info_t* out, functab_ctx_t* ctx) {
    print_log("FNTB_get_info(name=%s, s_id=%li)", fname ? fname->body : "(null)", s_id);
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (fi->name->equals(fi->name, fname) && (fi->s_id == s_id)) {
            if (out) str_memcpy(out, fi, sizeof(func_info_t));
            return 1;
        }
    }

    print_warn("FNTB_get_info -> NF!");
    return 0;
}

static func_info_t* _create_func_info(
    string_t* name, 
    int global, int local, int entry, int naked, int vargs, int generic, /* flags */
    ast_node_t* args, ast_node_t* rtype
) {
    func_info_t* fn = (func_info_t*)mm_malloc(sizeof(func_info_t));
    if (!fn) return NULL;
    str_memset(fn, 0, sizeof(func_info_t));
    list_init(&fn->local);
    list_init(&fn->resolutions);
    if (name) {
        fn->name = name->copy(name);
    }

    fn->args          = AST_copy_node(args, 0, 0, 1, SCOPE_TOKEN);
    fn->rtype         = AST_copy_node(rtype, 0, 0, 1, SCOPE_TOKEN);
    fn->flags.global  = global;
    fn->flags.local   = local;
    fn->flags.entry   = entry;
    fn->flags.naked   = naked;
    fn->flags.vargs   = vargs;
    fn->flags.generic = generic;
    fn->generic       = RESOLVED_TYPE_TOKEN;
    return fn;
}

static int _is_function_presented(
    string_t* name, symbol_id_t s_id, ast_node_t* args, token_type_t gen, func_info_t* out, functab_ctx_t* ctx
) {
    map_foreach (func_info_t* fi, &ctx->functb) {
        if (
            (s_id == FIELD_NO_CHANGE || fi->s_id == s_id) &&
            fi->name->equals(fi->name, name) &&
            AST_hash_node_stop(args->c, SCOPE_TOKEN) == AST_hash_node_stop(fi->args->c, SCOPE_TOKEN) &&
            fi->generic == gen
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
    string_t* name, string_t* vname,
    int global, int local, int entry, int naked, int vargs, int generic, /* flags */
    symbol_id_t s_id, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx
) {
    print_log(
        "FNTB_add_info(name=%s, global=%i, entry=%i, naked=%i, args=%lu)", 
        name ? name->body : "(null)", global, entry, naked, args ? AST_hash_node_stop(args->c, SCOPE_TOKEN) : 0
    );
    
    func_info_t out;
    if (_is_function_presented(name, s_id, args, RESOLVED_TYPE_TOKEN, &out, ctx)) return out.id; 

    func_info_t* nnd = _create_func_info(name, global, local, entry, naked, vargs, generic, args, rtype);
    if (!nnd) return 0;
    nnd->s_id = s_id;
    
    nnd->id = ctx->curr_id++;
    if (!vname) nnd->virt = _create_virt_name(nnd->id, name);
    else nnd->virt = vname->copy(vname);

    map_put(&ctx->functb, nnd->id, nnd);
    return nnd->id;
}

int FNTB_add_copy(func_info_t* src, functab_ctx_t* ctx) {
    print_log("FNTB_add_copy(id=%llu)", src->id);
    func_info_t* nnd = _create_func_info(src->name, 0, 0, 0, 0, 0, 0, src->args, src->rtype);
    if (!nnd) return NO_SYMBOL_ID;

    str_memcpy(&nnd->flags, &src->flags, sizeof(src->flags));
    nnd->id = ctx->curr_id++;

    return nnd->id;
}

int FNTB_add_local(symbol_id_t f_id, symbol_id_t l_id, functab_ctx_t* ctx) {
    print_log("FNTB_add_local(id=%llu, local=%li)", f_id, l_id);
    func_info_t* fi;
    if (map_get(&ctx->functb, f_id, (void**)&fi)) {
        list_add(&fi->local, (void*)l_id);
        return 1;
    }

    return 0;
}

int FNTB_update_func(
    symbol_id_t id, 
    string_t* name, 
    int used, int external, int global, int local, int entry, int naked, int vargs, /* flags */
    ast_node_t* args, ast_node_t* rtype,
    functab_ctx_t* ctx
) {
    print_log("FNTB_update_func(id=%llu, name=%s)", id, name->body);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        if (name) {
            destroy_string(fi->name);
            destroy_string(fi->virt);
            fi->name = name->copy(name);
            fi->virt = _create_virt_name(id, name);
        }
        
        if (global != FIELD_NO_CHANGE)   fi->flags.global   = global;
        if (local != FIELD_NO_CHANGE)    fi->flags.local    = local;
        if (entry != FIELD_NO_CHANGE)    fi->flags.entry    = entry;
        if (naked != FIELD_NO_CHANGE)    fi->flags.naked    = naked;
        if (vargs != FIELD_NO_CHANGE)    fi->flags.vargs    = vargs;
        if (used != FIELD_NO_CHANGE)     fi->flags.used     = used;
        if (external != FIELD_NO_CHANGE) fi->flags.external = external;
        
        if (args) {
            AST_unload(fi->args);
            fi->args = AST_copy_node(args, 0, 0, 1, SCOPE_TOKEN);
        }

        if (rtype) {
            AST_unload(rtype);
            fi->rtype = AST_copy_node(rtype, 0, 0, 1, SCOPE_TOKEN);
        }

        return 1;
    }

    return 0;
}

static inline int _resolve_types(ast_node_t* node, token_type_t t) {
    if (!node) return 0;
    _resolve_types(node->siblings.n, t);
    _resolve_types(node->c, t);
    if (!node->t) return 0;
    if (node->t->t_type == GENERIC_TYPE_TOKEN)     node->t->t_type = t;
    if (node->t->t_type == GENERIC_VARIABLE_TOKEN) node->t->t_type = TKN_get_var_from_type(t);
    return 1;
}

symbol_id_t FNTB_create_resolved_copy(symbol_id_t id, token_type_t t, functab_ctx_t* ctx) {
    print_log("FNTB_create_resolved_copy(id=%llu, t=%i", id, t);
    func_info_t* fi;
    if (map_get(&ctx->functb, id, (void**)&fi)) {
        func_info_t existed;
        ast_node_t *args = AST_copy_node(fi->args, 0, 0, 1, SCOPE_TOKEN), *rtype = AST_copy_node(fi->rtype, 0, 0, 1, SCOPE_TOKEN);
        _resolve_types(args, t);
        _resolve_types(rtype, t);
        if (_is_function_presented(fi->name, fi->s_id, args, t, &existed, ctx)) {
            AST_unload(args);
            AST_unload(rtype);
            return existed.id;
        }

        func_info_t* n = _create_func_info(
            fi->name, 
            fi->flags.global, fi->flags.local, fi->flags.entry, fi->flags.naked, fi->flags.vargs, 0,
            args, rtype
        );
        
        AST_unload(args);
        AST_unload(rtype);

        n->generic = t;
        n->s_id    = fi->s_id;
        n->id      = ctx->curr_id++;
        n->virt    = _create_virt_name(n->id, n->name);

        if (n->virt) {
            n->virt->rcat(n->virt, "__");
            n->virt->rcat(n->virt, DUMP_format_token_type(t));
        }

        map_put(&ctx->functb, n->id, n);
        list_add(&fi->resolutions, (void*)n->id);
        return n->id;
    }

    return 0;
}

static int _function_info_unload(func_info_t* info) {
    destroy_string(info->name);
    destroy_string(info->virt);
    list_free(&info->local);
    list_free(&info->resolutions);
    if (info->args)  AST_unload(info->args);
    if (info->rtype) AST_unload(info->rtype);
    return mm_free(info);
}

int FNTB_unload(functab_ctx_t* ctx) {
    return map_free_force_op(&ctx->functb, (int (*)(void *))_function_info_unload);
}
