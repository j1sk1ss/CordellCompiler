#include <lir/regalloc/ra.h>

igraph_node_t* LIR_RA_find_ig_node(igraph_t* g, symbol_id_t v_id) {
    igraph_node_t* n;
    if (map_get(&g->nodes, v_id, (void**)&n)) return n;
    return NULL;
}

/*
Add an undirected interference edge between two graph nodes.
Params:
    - `g` - Interference graph.
    - `v1` - First variable ID.
    - `v2` - Second variable ID.

Returns 1 on success, otherwise 0.
*/
static int _igraph_add_edge(igraph_t* g, symbol_id_t v1, symbol_id_t v2) {
    if (v1 == v2) return 0;
    igraph_node_t* n1 = LIR_RA_find_ig_node(g, v1);
    igraph_node_t* n2 = LIR_RA_find_ig_node(g, v2);
    if (!n1 || !n2) return 0;
    set_add(&n1->v, (void*)v2);
    set_add(&n2->v, (void*)v1);
    return 1;
}

/*
Collect use and def variable sets for a single LIR instruction.
Params:
    - `lh` - LIR instruction.
    - `use` - Output set of variables read before definition.
    - `def` - Output set of variables defined by the instruction.

Returns 1 if succeeds.
*/
static int _inst_usedef(lir_block_t* lh, set_t* use, set_t* def) {
    set_init(use, SET_CMP);
    set_init(def, SET_CMP);
    if (!lh || lh->unused) return 1;

    iterate_lir_args (lir_subject_t* arg, lh, LIR_is_writeop(lh->op)) {
        switch (arg->t) {
            case LIR_VARIABLE: {
                long v = arg->storage.var.v_id;
                if (!set_has(def, (void*)v)) set_add(use, (void*)v);
                break;
            }
            case LIR_ARGLIST: {
                foreach (lir_subject_t* param, &arg->storage.list.h) {
                    if (param->t != LIR_VARIABLE) continue;
                    long v = param->storage.var.v_id;
                    if (!set_has(def, (void*)v)) set_add(use, (void*)v);
                }
                
                break;
            }

            default: break;
        }
    }
    
    if (
        LIR_is_writeop(lh->op) &&
        lh->farg &&
        lh->farg->t == LIR_VARIABLE
    ) set_add(def, (void*)lh->farg->storage.var.v_id);
    return 1;
}

/*
Count LIR instructions in a CFG block.
Params:
    - `cb` - CFG block to inspect.

Returns number of LIR instructions in the block.
*/
static inline int _count_lir_in_block(cfg_block_t* bb) {
    int n = 0;
    iterate_lir_instructions (bb) {
        n++;
    }

    return n;
}

/*
Collect LIR instructions from a CFG block into a preallocated array.
Params:
    - `cb` - CFG block to inspect.
    - `arr` - Output array with enough capacity for all block instructions.

Returns 1 if succeeds.
*/
static inline int _collect_lir_in_block(cfg_block_t* bb, lir_block_t** arr) {
    int i = 0;
    iterate_lir_instructions (bb) {
        arr[i++] = lh;
    }
    
    return 1;
}

/*
Build interference edges for one CFG block using backward liveness.
Params:
    - `cb` - CFG block with live-out information.
    - `g` - Interference graph to update.

Returns 1 on success, otherwise 0.
*/
static int _build_igraph_block(cfg_block_t* cb, igraph_t* g) {
    int n = _count_lir_in_block(cb);
    if (n <= 0) return 1;

    lir_block_t** arr = (lir_block_t**)mm_malloc(sizeof(lir_block_t*) * n);
    if (!arr) return 0;
    _collect_lir_in_block(cb, arr);

    set_t live;
    set_copy(&live, &cb->curr_out);
    for (int i = n - 1; i >= 0; i--) {
        set_t use, def, tmp;
        _inst_usedef(arr[i], &use, &def);
        set_foreach (symbol_id_t d, &def) {
            set_foreach (symbol_id_t v, &live) {
                _igraph_add_edge(g, d, v);
            }
        }

        set_copy(&tmp, &live);
        set_minus_set(&tmp, &def);
        set_free(&live);
        set_union(&live, &tmp, &use);

        set_free(&tmp);
        set_free(&use);
        set_free(&def);
    }

    set_free(&live);
    mm_free(arr);
    return 1;
}

/*
Create and register an interference graph node for a variable.
Params:
    - `v_id` - Variable ID.
    - `g` - Interference graph.

Returns 1 on success, otherwise 0.
*/
static inline int _add_ig_node(symbol_id_t v_id, igraph_t* g) {
    igraph_node_t* n = (igraph_node_t*)mm_malloc(sizeof(igraph_node_t));
    if (!n) return 0;
    str_memset(n, 0, sizeof(igraph_node_t));
    n->v_id = v_id;
    set_init(&n->v, SET_NO_CMP);
    return map_put(&g->nodes, v_id, n);
}

int LIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    map_init(&g->nodes, MAP_NO_CMP);
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if (
            vi->vfs.glob || vi->vfs.ro                 ||
            vi->type == ARRAY_TYPE_TOKEN               ||
            ALLIAS_get_owners(vi->v_id, NULL, &smt->m) ||
            vi->vmi.align > CONF_get_full_bytness()
        ) continue;
        _add_ig_node(vi->v_id, g);
    }

    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (!_build_igraph_block(cb, g)) return 0;
        }
    }

    return 1;
}

int LIR_RA_unload_igraph(igraph_t* g) {
    map_foreach (igraph_node_t* nd, &g->nodes) {
        set_free(&nd->v);
    }

    return map_free_force(&g->nodes);
}
