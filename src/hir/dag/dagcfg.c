#include <hir/dag.h>

static hir_subject_t* _apply_dag_on_block(hir_subject_t* s, dag_ctx_t* dctx) {
    if (!s) return NULL;
    dag_node_t* nd = DAG_ACQUIRE_NODE(dctx, s);
    if (!nd) return NULL;
    if (HIR_hash_subject(nd->src) == HIR_hash_subject(s)) return NULL;
    return nd->src;
}

static inline int _check_home(hir_block_t* h, hir_subject_t* s) {
    hir_subject_t* args[] = { h->farg, h->sarg, h->targ };
    for (int i = 0; i < 3; i++) {
        if (!args[i]) continue;
        if (args[i] == s) return 1;
    }

    return 0;
}

static inline int _prepare_subject(hir_block_t* src, hir_subject_t* s) {
    if (s->home && s->home != src && _check_home(s->home, s)) s->home->unused = 1;
    else HIR_unload_subject(s);
    return 1; 
}

int HIR_DAG_CFG_rebuild(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        hir_block_t* hh = HIR_get_next(fb->hmap.entry, fb->hmap.exit, 0);
        while (hh) {
            if (hh->op != HIR_PHI && hh->op != HIR_PHI_PREAMBLE) {
                hir_subject_t* nodes[3] = { hh->farg, hh->sarg, hh->targ };
                for (int i = HIR_writeop(hh->op); i < 3; i++) {
                    if (!nodes[i]) continue;
                    if (nodes[i]->t == HIR_ARGLIST) {
                        list_iter_t el_it;
                        list_iter_hinit(&nodes[i]->storage.list.h, &el_it);
                        hir_subject_t* s;
                        while ((s = (hir_subject_t*)list_iter_current(&el_it))) {
                            hir_subject_t* n = _apply_dag_on_block(s, dctx);
                            if (n) {
                                list_iter_set(&el_it, n);
                                if (s->home) s->home->unused = 1;
                                else HIR_unload_subject(s);
                            }
                            
                            list_iter_next(&el_it);
                        }
                    }
                    else {
                        hir_subject_t* s = _apply_dag_on_block(nodes[i], dctx);
                        if (s) { 
                            switch (i) {
                                case 0: _prepare_subject(hh, hh->farg); hh->farg = s; break;
                                case 1: _prepare_subject(hh, hh->sarg); hh->sarg = s; break;
                                case 2: _prepare_subject(hh, hh->targ); hh->targ = s; break;
                            }
                        }
                    }
                }
            }

            hh = HIR_get_next(hh, fb->hmap.exit, 1);
        }
    }

    return 1;
}