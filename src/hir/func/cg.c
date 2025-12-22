#include <hir/func.h>

static call_graph_node_t* _create_cgraph_node(long fid) {
    call_graph_node_t* nd = (call_graph_node_t*)mm_malloc(sizeof(call_graph_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(call_graph_node_t));

    nd->fid = fid;
    set_init(&nd->edges, SET_NO_CMP);
    return nd;
}

static int _register_func(long fid, call_graph_t* ctx) {
    call_graph_node_t* f = _create_cgraph_node(fid);
    if (!f) return 0;
    return map_put(&ctx->verts, fid, f);
}

static int _add_vert(long src_id, long dst_id, call_graph_t* ctx) {
    call_graph_node_t *src, *dst;
    if (!map_get(&ctx->verts, src_id, (void**)&src) || !map_get(&ctx->verts, dst_id, (void**)&dst)) return 0;
    set_add(&src->edges, dst);
    return 1;
}

static int _register_functions(call_graph_t* ctx, sym_table_t* smt) {
    map_foreach (func_info_t* fi, &smt->f.functb) {
        if (fi->entry) ctx->e_fid = fi->id;
        _register_func(fi->id, ctx);
    }

    return 1;
}

static int _connect_edges(cfg_ctx_t* cctx, call_graph_t* ctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (HIR_funccall(hh->op) && !hh->unused) {
                    _add_vert(fb->fid, hh->sarg->storage.str.s_id, ctx);
                }
                
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
