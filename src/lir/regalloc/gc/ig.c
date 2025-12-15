/*
ig.c - Create interference graph
*/

#include <lir/regalloc/ra.h>

igraph_node_t* LIR_RA_find_ig_node(igraph_t* g, long v_id) {
    igraph_node_t* n;
    if (map_get(&g->nodes, v_id, (void**)&n)) return n;
    return NULL;
}

static int _igraph_add_edge(igraph_t* g, long v1, long v2) {
    if (v1 == v2) return 0;
    igraph_node_t* n1 = LIR_RA_find_ig_node(g, v1);
    igraph_node_t* n2 = LIR_RA_find_ig_node(g, v2);
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
    set_init(&n->v, SET_NO_CMP);
    return map_put(&g->nodes, v_id, n);
}

int LIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    map_init(&g->nodes, MAP_NO_CMP);

    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if (vi->glob || vi->type == ARRAY_TYPE_TOKEN || vi->type == STR_TYPE_TOKEN) continue;
        if (ALLIAS_get_owners(vi->v_id, NULL, &smt->m)) continue;
        _add_ig_node(vi->v_id, g);
    }

    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_iter_t dit;
            set_iter_init(&cb->def, &dit);
            long d;
            while (set_iter_next(&dit, (void**)&d)) {
                set_iter_t lit;
                set_iter_init(&cb->curr_in, &lit);
                long l;
                while (set_iter_next(&lit, (void**)&l)) {
                    _igraph_add_edge(g, d, l);
                }

                set_iter_init(&cb->curr_out, &lit);
                while (set_iter_next(&lit, (void**)&l)) {
                    _igraph_add_edge(g, d, l);
                }
            }
        }
    }

    return 1;
}

int LIR_RA_unload_igraph(igraph_t* g) {
    map_foreach (igraph_node_t* nd, &g->nodes) {
        set_free(&nd->v);
    }

    return map_free_force(&g->nodes);
}
