#include <hir/func.h>

static call_graph_node_t* _create_cgraph_node(long fid) {
    call_graph_node_t* nd = (call_graph_node_t*)mm_malloc(sizeof(call_graph_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(call_graph_node_t));

    nd->fid = fid;
    set_init(&nd->edges);
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
    map_iter_t it;
    map_iter_init(&smt->f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        if (fi->entry) ctx->e_fid = fi->id;
        _register_func(fi->id, ctx);
    }

    return 1;
}

static int _connect_edges(cfg_ctx_t* cctx, call_graph_t* ctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            hir_block_t* hh = cb->hmap.entry;
            while (hh) {
                if (HIR_funccall(hh->op) && !hh->unused) {
                    _add_vert(fb->fid, hh->sarg->storage.str.s_id, ctx);
                }
                
                if (hh == cb->hmap.exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}

int HIR_CG_build(cfg_ctx_t* cctx, call_graph_t* ctx, sym_table_t* smt) {
    map_init(&ctx->verts);
    _register_functions(ctx, smt);
    return _connect_edges(cctx, ctx);
}

int HIR_CG_unload(call_graph_t* ctx) {
    map_iter_t it;
    map_iter_init(&ctx->verts, &it);
    call_graph_node_t* node;
    while (map_iter_next(&it, (void**)&node)) {
        set_free(&node->edges);
    }

    return map_free_force(&ctx->verts);
}
