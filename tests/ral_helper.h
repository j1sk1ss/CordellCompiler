#ifndef RAL_HELPER_H_
#define RAL_HELPER_H_

#include <stdio.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/opt/ra.h>

static void igraph_dump_dot(igraph_t* g) {
    fprintf(stdout, "graph G {\n");
    fprintf(stdout, "  node [style=filled];\n");

    const char* colors[] = { "red", "green", "blue", "yellow", "cyan", "magenta", "orange", "grey", "brown", "pink" };
    int palette_size = sizeof(colors) / sizeof(colors[0]);

    for (long i = 0; i < g->nodes.capacity; i++) {
        if (g->nodes.entries[i].used) {
            igraph_node_t* n = (igraph_node_t*)g->nodes.entries[i].value;
            const char* fill = (n->color >= 0 && n->color < palette_size) ? colors[n->color] : "white";
            fprintf(stdout, "  v%ld [label=\"v%ld\\ncolor=%d\", fillcolor=%s];\n", n->v_id, n->v_id, n->color, fill);
        }
    }

    for (long i = 0; i < g->nodes.capacity; i++) {
        if (g->nodes.entries[i].used) {
            igraph_node_t* n = (igraph_node_t*)g->nodes.entries[i].value;
            set_iter_t sit;
            set_iter_init(&n->v, &sit);
            long neighbor;
            while (set_iter_next(&sit, (void**)&neighbor)) {
                if (n->v_id < neighbor) {
                    fprintf(stdout, "  v%ld -- v%ld;\n", n->v_id, neighbor);
                }
            }
        }
    }

    fprintf(stdout, "}\n");
}

#endif