/*
gc.c - Color interference graph with euristic approuch
*/

#include <hir/ra.h>

int HIR_RA_color_igraph(igraph_t* g, map_t* colors) {
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
        
        igraph_node_t* n = HIR_RA_find_ig_node(g, v_ids[max_index]);
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
        igraph_node_t* current_node = HIR_RA_find_ig_node(g, current_id);
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
