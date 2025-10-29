#include <hir/func.h>

static int _mark_block(call_graph_node_t* nd, call_graph_t* ctx) {
    nd->flag = 1;
    
    set_iter_t it;
    set_iter_init(&nd->edges, &it);
    call_graph_node_t* nnd;
    while (set_iter_next(&it, (void**)&nnd)) {
        if (nd->fid == nnd->fid) continue;
        _mark_block(nnd, ctx);
    }
    
    return 1;
}

int HIR_CG_perform_dfe(call_graph_t* ctx, sym_table_t* smt) {
    call_graph_node_t* entry;
    if (!map_get(&ctx->verts, ctx->e_fid, (void**)&entry)) return 0;
    _mark_block(entry, ctx);

    map_iter_t it;
    map_iter_init(&ctx->verts, &it);
    call_graph_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        FNTB_update_info(nd->fid, nd->flag, -1, NULL, NULL, &smt->f);
    }

    return 1;
}
