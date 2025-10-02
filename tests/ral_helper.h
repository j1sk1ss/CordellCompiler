#ifndef RAL_HELPER_H_
#define RAL_HELPER_H_

#include <stdio.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/opt/ra.h>

static void igraph_dump_dot(igraph_t* g) {
    fprintf(stdout, "graph G {\n");
    fprintf(stdout, "  node [style=filled];\n");

    list_iter_t it;
    list_iter_hinit(&g->nodes, &it);
    igraph_node_t* n;
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        const char* colors[] = { "red", "green", "blue", "yellow", "cyan", "magenta", "orange", "grey", "brown", "pink" };
        int palette_size = sizeof(colors)/sizeof(colors[0]);
        const char* fill = (n->color >= 0 && n->color < palette_size) ? colors[n->color] : "white";
        fprintf(stdout, "  v%ld [label=\"v%ld\\ncolor=%d\", fillcolor=%s];\n", n->v_id, n->v_id, n->color, fill);
    }

    list_iter_hinit(&g->nodes, &it);
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        set_iter_t sit;
        set_iter_init(&n->v, &sit);
        long neighbor;
        while ((neighbor = set_iter_next_int(&sit)) >= 0) {
            if (n->v_id < neighbor) {
                fprintf(stdout, "  v%ld -- v%ld;\n", n->v_id, neighbor);
            }
        }
    }

    fprintf(stdout, "}\n");
}

#endif