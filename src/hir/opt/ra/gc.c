#include <hir/opt/ra.h>

int HIR_RA_color_igraph(igraph_t* g, map_t* colors) {
    if (!g || !colors) return 0;
    
    int node_count = list_size(&g->nodes);
    if (!node_count) return 1;
    
    sstack_t stack;
    stack.top = -1;
    
    int* degrees = (int*)mm_malloc(node_count * sizeof(int));
    long* v_ids = (long*)mm_malloc(node_count * sizeof(long));
    char* processed = (char*)mm_malloc(node_count * sizeof(char));
    
    if (!degrees || !v_ids || !processed) {
        mm_free(degrees);
        mm_free(v_ids);
        mm_free(processed);
        return 0;
    }
    
    list_iter_t it;
    list_iter_hinit(&g->nodes, &it);
    igraph_node_t* n;
    int i = 0;
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
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
        stack_push_int(&stack, v_ids[max_index]);
        processed[max_index] = 1;
        remaining--;
        
        n = HIR_RA_find_ig_node(g, v_ids[max_index]);
        if (n) {
            set_iter_t sit;
            set_iter_init(&n->v, &sit);
            long neighbor_id;
            while ((neighbor_id = set_iter_next_int(&sit)) >= 0) {
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
        stack_top_int(&stack, &e);
        stack_pop(&stack);
        
        long current_id = e.data.intdata;
        igraph_node_t* current_node = HIR_RA_find_ig_node(g, current_id);
        if (!current_node) continue;
        
        set_t used_colors;
        set_init(&used_colors);
        
        set_iter_t sit;
        set_iter_init(&current_node->v, &sit);
        long neighbor_id;
        while ((neighbor_id = set_iter_next_int(&sit)) >= 0) {
            long neighbor_color;
            if ((neighbor_color = (long)map_get(colors, neighbor_id))) {
                set_add_int(&used_colors, neighbor_color);
            }
        }
        
        long color = 1;
        while (set_has_int(&used_colors, color)) {
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
