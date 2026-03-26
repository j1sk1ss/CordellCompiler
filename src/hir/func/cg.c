#include <hir/func.h>

/*
Create a call graph node.
Params:
    - `f_id` - Function ID.

Returns a new call graph node or NULL.
*/
static call_graph_node_t* _create_cgraph_node(symbol_id_t f_id) {
    call_graph_node_t* nd = (call_graph_node_t*)mm_malloc(sizeof(call_graph_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(call_graph_node_t));
    nd->f_id = f_id;
    set_init(&nd->edges, SET_NO_CMP);
    return nd;
}

/*
Create and register a new call node in the call context.
Params:
    - `f_id` - Function ID.
    - `ctx` - Call graph context.

Returns 1 if succeeds. Otherwise will return 0. 
*/
static inline int _register_func(symbol_id_t f_id, call_graph_t* ctx) {
    call_graph_node_t* f = _create_cgraph_node(f_id);
    if (!f) return 0;
    return map_put(&ctx->verts, f_id, f);
}

/*
Add vertex into the call graph context.
Connects two existed vertices in the call graph context. 
Params:
    - `src_id` - Source function ID.
    - `dst_id` - Destination function ID.
    - `ctx` - Call graph context.

Returns 1 if succeeds.
*/
static int _add_vert(symbol_id_t src_id, symbol_id_t dst_id, call_graph_t* ctx) {
    call_graph_node_t *src, *dst;
    if (
        !map_get(&ctx->verts, src_id, (void**)&src) || /* If the source ID isn't presented in the context      */
        !map_get(&ctx->verts, dst_id, (void**)&dst)    /* If the destination ID isn't presented in the context */
    ) return 0;
    set_add(&src->edges, dst);
    return 1;
}

/*
Register all existed functions from the symtable.
Params:
    - `ctx` - Call graph context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
static int _register_functions(call_graph_t* ctx, sym_table_t* smt) {
    map_foreach (func_info_t* fi, &smt->f.functb) {
        if (fi->flags.entry) ctx->e_fid = fi->id;
        _register_func(fi->id, ctx);
    }

    return 1;
}

/*
Traverse the list of functions, connect each function with called functions.
Params:
    - `cctx` - CFG context.
    - `ctx` - Call graph context.

Returns 1 if succeeds. Oterwise will return 0.
*/
static int _connect_edges(cfg_ctx_t* cctx, call_graph_t* ctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (
                    (
                        HIR_is_funccall(hh->op) ||
                        (hh->op == HIR_REF && hh->sarg->t == HIR_FNAME)
                    ) && 
                    !hh->unused
                ) _add_vert(fb->f_id, hh->sarg->storage.str.s_id, ctx);
                hh = HIR_get_next(hh, cb->hmap.exit, 1);
            }
        }
    }

    return 1;
}

int HIR_CG_build(cfg_ctx_t* cctx, call_graph_t* ctx, sym_table_t* smt) {
    map_init(&ctx->verts, MAP_NO_CMP);
    _register_functions(ctx, smt);
    return _connect_edges(cctx, ctx);
}

int HIR_CG_unload(call_graph_t* ctx) {
    map_foreach (call_graph_node_t* node, &ctx->verts) {
        set_free(&node->edges);
    }

    return map_free_force(&ctx->verts);
}
