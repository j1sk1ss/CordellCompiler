#include <lir/regalloc/x84_64_gnu_nasm.h>

#ifdef DEBUG
static void __igraph_dump_dot(igraph_t* g) {
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

static int _regalloc_precolor(map_t* cmap, sym_table_t* smt) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if (vi->vmi.allocated && vi->vmi.reg >= 0) {
            map_put(cmap, vi->v_id, (void*)((long)vi->vmi.reg));
        }
    }

    return 1;
}

int x86_64_regalloc_graph(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors) {
    igraph_t ig;
    LIR_RA_build_igraph(cctx, &ig, smt);
    _regalloc_precolor(colors, smt);
    LIR_RA_color_igraph(&ig, colors);
#ifdef DEBUG
    __igraph_dump_dot(&ig);
#endif
    LIR_RA_unload_igraph(&ig);
    return 1;
}

int x86_64_regalloc_linear(__attribute__ ((unused)) cfg_ctx_t* cctx, __attribute__ ((unused)) sym_table_t* smt, __attribute__ ((unused)) map_t* colors) {
    return 1;
}
