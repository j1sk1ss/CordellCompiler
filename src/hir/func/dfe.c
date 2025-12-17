#include <hir/func.h>

static int _mark_block(call_graph_node_t* nd, call_graph_t* ctx) {
    nd->flag = 1;
    set_foreach (call_graph_node_t* nnd, &nd->edges) {
        if (nd->fid == nnd->fid) continue;
        _mark_block(nnd, ctx);
    }
    
    return 1;
}

int HIR_CG_perform_dfe(call_graph_t* ctx, sym_table_t* smt) {
    call_graph_node_t* entry;
    if (!map_get(&ctx->verts, ctx->e_fid, (void**)&entry)) return 0;
    _mark_block(entry, ctx);

    map_foreach (call_graph_node_t* nd, &ctx->verts) {
        FNTB_update_info(nd->fid, nd->flag, -1, NULL, NULL, &smt->f);
    }

    return 1;
}

int HIR_CG_apply_dfe(cfg_ctx_t* cctx, call_graph_t* ctx) {
    call_graph_node_t* nd;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!map_get(&ctx->verts, fb->fid, (void**)&nd)) continue;
        fb->used = nd->flag;
    }

    return 1;
}
