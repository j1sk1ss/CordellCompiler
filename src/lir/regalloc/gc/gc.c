/*
gc.c - Color interference graph with euristic approuch
*/

#include <lir/regalloc/ra.h>

int LIR_RA_init_colors(map_t* colors, sym_table_t* smt) {
    map_iter_t it;
    map_iter_init(&smt->v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        map_put(colors, vi->v_id, (void*)-1);
    }

    return 1;
}

int LIR_RA_precolor_node(map_t* colors, long vid, long color) {
    print_log("LIR_RA_precolor_node(vid=%llu, color=%llu)", vid, color);
    return map_put(colors, vid, (void**)color);
}

int LIR_RA_color_igraph(igraph_t* g, map_t* colors) {
    if (!g || !colors) return 0;
    
    int node_count = g->nodes.size;
    if (!node_count) return 1;
    
    sstack_t stack;
    stack_init(&stack);

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

        long pre_color;
        if (map_get(colors, n->v_id, (void**)&pre_color) && pre_color >= 0) {
            processed[i] = 1;
        }

        i++;
    }
    
    int remaining = 0;
    for (i = 0; i < node_count; i++) {
        if (!processed[i]) remaining++;
    }
    
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
    
    while (stack.top >= 0) {
        long current_id;
        stack_pop(&stack, (void**)&current_id);
        
        igraph_node_t* current_node = LIR_RA_find_ig_node(g, current_id);
        if (!current_node) continue;
        
        long existing_color;
        if (map_get(colors, current_id, (void**)&existing_color) && existing_color >= 0) continue;

        set_t used_colors;
        set_init(&used_colors, SET_NO_CMP);
        
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
    
    stack_unload(&stack);
    mm_free(degrees);
    mm_free(v_ids);
    mm_free(processed);
    return 1;
}
