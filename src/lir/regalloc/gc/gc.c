#include <lir/regalloc/ra.h>

static long _precolored_reg_to_color(lir_registers_t reg) {
    if (CONF_get_full_bytness() == 4) {
        switch (LIR_format_register(reg, 4)) {
            case EBX: return 0;
            case EDX: return 1;
            default:  return reg;
        }
    }

    switch (LIR_format_register(reg, 8)) {
        case RCX: return 0;
        case RDX: return 1;
        case RBX: return 2;
        case RSI: return 3;
        case RDI: return 4;
        case R8:  return 5;
        case R9:  return 6;
        case R10: return 7;
        case R11: return 8;
        case R12: return 9;
        case R13: return 10;
        case R14: return 11;
        default: break;
    }
    return reg;
}

int LIR_RA_init_colors(map_t* colors, sym_table_t* smt) {
    print_log("LIR_RA_init_colors()");
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        long color = vi->vmi.reg >= 0 ? _precolored_reg_to_color(vi->vmi.reg) : vi->vmi.reg;
        map_put(colors, vi->v_id, (void*)color);
    }

    return 1;
}

int LIR_RA_color_igraph(igraph_t* g, map_t* colors) {
    print_log("LIR_RA_color_igraph()");
    if (!g || !colors) return 0;
    
    int node_count = g->nodes.size;
    if (!node_count) return 1;
    
    sstack_t stack;
    stack_init(&stack);

    int* degrees    = (int*)mm_malloc(node_count * sizeof(int));
    long* v_ids     = (long*)mm_malloc(node_count * sizeof(long));
    char* processed = (char*)mm_malloc(node_count);
    str_memset(processed, 1, node_count);
    
    if (!degrees || !v_ids || !processed) {
        mm_free(degrees);
        mm_free(v_ids);
        mm_free(processed);
        return 0;
    }
    
    int i = 0;
    map_foreach (igraph_node_t* n, &g->nodes) {
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
            set_foreach (long neighbor_id, &n->v) {
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
        
        set_foreach (long neighbor_id, &current_node->v) {
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
    
    stack_free(&stack);
    mm_free(degrees);
    mm_free(v_ids);
    mm_free(processed);
    return 1;
}
