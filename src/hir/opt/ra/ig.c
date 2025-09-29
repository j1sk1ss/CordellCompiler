#include <hir/opt/ra.h>

static igraph_node_t* _create_ig_node(long v_id) {
    igraph_node_t* n = (igraph_node_t*)mm_malloc(sizeof(igraph_node_t));
    if (!n) return NULL;
    str_memset(n, 0, sizeof(igraph_node_t));
    n->v_id = v_id;
    set_init(&n->v);
    return n;
}

igraph_node_t* HIR_RA_find_ig_node(igraph_t* g, long v_id) {
    list_iter_t it;
    list_iter_hinit(&g->nodes, &it);
    igraph_node_t* n;
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        if (n->v_id == v_id) return n;
    }

    return NULL;
}

static int _igraph_add_edge(igraph_t* g, long v1, long v2) {
    if (v1 == v2) return 0;
    igraph_node_t* n1 = HIR_RA_find_ig_node(g, v1);
    igraph_node_t* n2 = HIR_RA_find_ig_node(g, v2);
    if (!n1 || !n2) return 0;
    set_add_int(&n1->v, v2);
    set_add_int(&n2->v, v1);
    return 1;
}

static int _add_ig_node(long v_id, igraph_t* g) {
    igraph_node_t* n = _create_ig_node(v_id);
    if (!n) return 0;
    return list_add(&g->nodes, n);
}

int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    list_iter_t vit;
    list_iter_hinit(&smt->v.lst, &vit);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&vit))) {
        _add_ig_node(vi->v_id, g);
    }

    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_t live;
            set_init(&live);
            set_copy_int(&live, &cb->curr_out);

            set_iter_t dit;
            set_iter_init(&cb->def, &dit);
            long d;
            while ((d = set_iter_next_int(&dit)) >= 0) {
                set_iter_t lit;
                set_iter_init(&live, &lit);
                long l;
                while ((l = set_iter_next_int(&lit)) >= 0) {
                    _igraph_add_edge(g, d, l);
                }
            }

            set_free(&live);
        }
    }

    return 1;
}

int HIR_RA_unload_igraph(igraph_t* g) {
    return list_free_force(&g->nodes);
}
