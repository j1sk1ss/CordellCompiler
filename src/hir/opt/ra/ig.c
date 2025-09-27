#include <hir/opt/ra.h>

static igraph_node_t* _create_ig_node(long v_id) {
    igraph_node_t* n = (igraph_node_t*)mm_malloc(sizeof(igraph_node_t));
    if (!n) return NULL;
    str_memset(n, 0, sizeof(igraph_node_t));
    n->v_id = v_id;
    set_init(&n->v);
    return n;
}

static int _add_ig_node(long v_id, igraph_t* g) {
    igraph_node_t* n = _create_ig_node(v_id);
    if (!n) return 0;
    if (!g->h) {
        g->h = n;
        return 1;
    }

    igraph_node_t* h = g->h;
    while (h->next) h = h->next;
    h->next = n;
    return 1;
}

int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    variable_info_t* vh = smt->v.h;
    while (vh) {
        _add_ig_node(vh->v_id, g);
        vh = vh->next;
    }
}
