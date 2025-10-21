/*
ig.c - Create interference graph
*/

#include <hir/ra.h>

igraph_node_t* HIR_RA_find_ig_node(igraph_t* g, long v_id) {
    igraph_node_t* n;
    if (map_get(&g->nodes, v_id, (void**)&n)) return n;
    return NULL;
}

static int _igraph_add_edge(igraph_t* g, long v1, long v2) {
    if (v1 == v2) return 0;
    igraph_node_t* n1 = HIR_RA_find_ig_node(g, v1);
    igraph_node_t* n2 = HIR_RA_find_ig_node(g, v2);
    if (!n1 || !n2) return 0;
    set_add(&n1->v, (void*)v2);
    set_add(&n2->v, (void*)v1);
    return 1;
}

static int _add_ig_node(long v_id, igraph_t* g) {
    igraph_node_t* n = (igraph_node_t*)mm_malloc(sizeof(igraph_node_t));
    if (!n) return 0;
    str_memset(n, 0, sizeof(igraph_node_t));
    n->v_id = v_id;
    set_init(&n->v);
    return map_put(&g->nodes, v_id, n);
}

int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    map_init(&g->nodes);

    map_iter_t vit;
    map_iter_init(&smt->v.vartb, &vit);
    variable_info_t* vi;
    while (map_iter_next(&vit, (void**)&vi)) {
        if (vi->glob || vi->type == ARRAY_TYPE_TOKEN || vi->type == STR_TYPE_TOKEN) continue;
        if (ALLIAS_get_owners(vi->v_id, NULL, &smt->m)) continue;
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
            set_copy(&live, &cb->curr_out);

            set_iter_t dit;
            set_iter_init(&cb->def, &dit);
            long d;
            while (set_iter_next(&dit, (void**)&d)) {
                set_iter_t lit;
                set_iter_init(&live, &lit);
                long l;
                while (set_iter_next(&lit, (void**)&l)) {
                    _igraph_add_edge(g, d, l);
                }
            }

            set_free(&live);
        }
    }

    return 1;
}

int HIR_RA_unload_igraph(igraph_t* g) {
    return map_free_force(&g->nodes);
}
