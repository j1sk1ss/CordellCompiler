# Register allocation part
Now that we have the `IN`, `OUT`, `DEF`, and `USE` sets, we can construct an interference graph. The idea is straightforward: we create a vertex for each variable in the symbol table, and then, for every `CFG` block, we connect (i.e., add an edge between) each variable from the block’s `DEF` set with every variable from its `OUT` set. This connection represents that these two variables are live at the same time. The resulting structure is the interference graph, where:
- Vertices represent program variables.
- Edges represent liveness conflicts (interference) between variables.
![ig](../../media/not_colored_ig.png)

The implementation of the mentioned algorithm is shown below.
```c
int LIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
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
```

## Graph coloring
Now we can determine which variables can share the same register using graph coloring. The solution to this problem is purely mathematical, and there are many possible strategies to color a graph. In short, the goal is to assign a color to every node (variable) in such a way that no two connected nodes share the same color. The output of this algorithm is a colored interference graph, where each color represents a distinct physical register, and all variables with the same color can safely reuse the same register without overlapping lifetimes.
![colored_ig](../../media/colored_ig.png)

Cordell Compiler's solution of this problem uses Chaitin’s algorithm.
```c
int LIR_RA_color_igraph(igraph_t* g, map_t* colors) {
    if (!g || !colors) return 0;
    
    int node_count = g->nodes.size;
    if (!node_count) return 1;
    
    sstack_t stack  = { .top = -1 };
    int* degrees    = (int*)mm_malloc(node_count * sizeof(int));
    long* v_ids     = (long*)mm_malloc(node_count * sizeof(long));
    char* processed = (char*)mm_malloc(node_count * sizeof(char));
    
    if (!degrees || !v_ids || !processed) {
        mm_free(degrees);
        mm_free(v_ids);
        mm_free(processed);
        return 0;
    }
    
    int i = 0;
    map_iter_t it;
    map_iter_init(&g->nodes, &it);
    igraph_node_t* n;
    while (map_iter_next(&it, (void**)&n)) {
        v_ids[i]     = n->v_id;
        degrees[i]   = set_size(&n->v);
        processed[i] = 0;
        i++;
    }
    
    int remaining = node_count;
    while (remaining > 0) {
        int max_degree = -1;
        int max_index  = -1;
        
        for (i = 0; i < node_count; i++) {
            if (!processed[i] && degrees[i] > max_degree) {
                max_degree = degrees[i];
                max_index = i;
            }
        }
        
        if (max_index == -1) break;
        stack_push(&stack, (void*)v_ids[max_index]);
        processed[max_index] = 1;
        remaining--;
        
        igraph_node_t* n = LIR_RA_find_ig_node(g, v_ids[max_index]);
        if (n) {
            set_iter_t sit;
            set_iter_init(&n->v, &sit);
            long neighbor_id;
            while (set_iter_next(&sit, (void**)&neighbor_id)) {
                for (i = 0; i < node_count; i++) {
                    if (v_ids[i] == neighbor_id && !processed[i]) {
                        degrees[i]--;
                        break;
                    }
                }
            }
        }
    }
    
    map_init(colors);
    while (stack.top >= 0) {
        stack_elem_t e;
        stack_top(&stack, &e);
        stack_pop(&stack);
        
        long current_id = (long)e.data;
        igraph_node_t* current_node = LIR_RA_find_ig_node(g, current_id);
        if (!current_node) continue;
        
        set_t used_colors;
        set_init(&used_colors);
        
        set_iter_t sit;
        set_iter_init(&current_node->v, &sit);
        long neighbor_id;
        while (set_iter_next(&sit, (void**)&neighbor_id)) {
            long neighbor_color;
            if (map_get(colors, neighbor_id, (void**)&neighbor_color)) {
                set_add(&used_colors, (void*)neighbor_color);
            }
        }
        
        long color = 0;
        while (set_has(&used_colors, (void*)color)) {
            color++;
        }
        
        map_put(colors, current_id, (void*)color);
        current_node->color = color;
        set_free(&used_colors);
    }
    
    mm_free(degrees);
    mm_free(v_ids);
    mm_free(processed);
    return 1;
}
```
