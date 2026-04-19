#include <lir/regalloc/ra.h>

igraph_node_t* LIR_RA_find_ig_node(igraph_t* g, long v_id) {
    igraph_node_t* n;
    if (map_get(&g->nodes, v_id, (void**)&n)) return n;
    return NULL;
}

static int _igraph_add_edge(igraph_t* g, long v1, long v2) {
    if (v1 == v2) return 0;
    igraph_node_t* n1 = LIR_RA_find_ig_node(g, v1);
    igraph_node_t* n2 = LIR_RA_find_ig_node(g, v2);
    if (!n1 || !n2) return 0;
    set_add(&n1->v, (void*)v2);
    set_add(&n2->v, (void*)v1);
    return 1;
}

static int _add_ig_node(long v_id, igraph_t* g) {
    igraph_node_t* n = (igraph_node_t*)mm_malloc(sizeof(igraph_node_t));
    if (!n) return 0;
    str_memset(n, 0, sizeof(igraph_node_t));
    n->v_id = v_id;
    set_init(&n->v, SET_NO_CMP);
    return map_put(&g->nodes, v_id, n);
}

static int _inst_usedef(lir_block_t* lh, set_t* use, set_t* def) {
    set_init(use, SET_CMP);
    set_init(def, SET_CMP);
    if (!lh || lh->unused) return 1;

    lir_subject_t* args[3] = { lh->farg, lh->sarg, lh->targ };
    for (int i = LIR_is_writeop(lh->op); i < 3; i++) {
        if (!args[i]) continue;
        switch (args[i]->t) {
            case LIR_VARIABLE: {
                long v = args[i]->storage.var.v_id;
                if (!set_has(def, (void*)v)) set_add(use, (void*)v);
                break;
            }
            case LIR_ARGLIST: {
                foreach (lir_subject_t* arg, &args[i]->storage.list.h) {
                    if (arg->t != LIR_VARIABLE) continue;
                    long v = arg->storage.var.v_id;
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

static inline int _count_lir_in_block(cfg_block_t* cb) {
    int n = 0;
    lir_block_t* lh = LIR_get_next(cb->lmap.entry, cb->lmap.exit, 0);
    while (lh) {
        n++;
        lh = LIR_get_next(lh, cb->lmap.exit, 1);
    }

    return n;
}

static inline int _collect_lir_in_block(cfg_block_t* cb, lir_block_t** arr) {
    int i = 0;
    lir_block_t* lh = LIR_get_next(cb->lmap.entry, cb->lmap.exit, 0);
    while (lh) {
        arr[i++] = lh;
        lh = LIR_get_next(lh, cb->lmap.exit, 1);
    }
    
    return 1;
}

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
        set_foreach (long d, &def) {
            set_foreach (long v, &live) {
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

int LIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt) {
    map_init(&g->nodes, MAP_NO_CMP);

    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if (
            vi->vfs.glob || vi->vfs.ro ||
            vi->type == ARRAY_TYPE_TOKEN ||
            vi->type == STR_TYPE_TOKEN ||
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