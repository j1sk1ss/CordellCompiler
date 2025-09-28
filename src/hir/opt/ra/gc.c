#include <hir/opt/ra.h>

static int _get_color_for_node(igraph_node_t* node, list_t* colored, int max_colors) {
    set_t used;
    set_init(&used);

    list_iter_t it;
    list_iter_hinit(colored, &it);
    igraph_node_t* c;
    while ((c = (igraph_node_t*)list_iter_next(&it))) {
        if (set_has_int(&node->v, c->v_id)) {
            set_add_int(&used, c->color);
        }
    }

    for (int color = 0; color < max_colors; color++) {
        if (!set_has_int(&used, color)) {
            set_free(&used);
            return color;
        }
    }

    set_free(&used);
    return -1;
}

int HIR_RA_color_igraph(igraph_t* g, int max_colors) {
    list_t colored;
    list_init(&colored);

    list_iter_t it;
    list_iter_hinit(&g->nodes, &it);
    igraph_node_t* n;
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        int color = _get_color_for_node(n, &colored, max_colors);
        n->color = color;
        list_add(&colored, n);
    }

    list_free(&colored);
    return 0;
}
