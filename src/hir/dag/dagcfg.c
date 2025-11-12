#include <hir/dag.h>

static hir_subject_t* _apply_dag_on_block(hir_subject_t* s, dag_ctx_t* dctx) {
    if (!s) return NULL;
    dag_node_t* nd = DAG_ACQUIRE_NODE(dctx, s);
    if (!nd) return NULL;
    if (HIR_hash_subject(nd->src) == HIR_hash_subject(s)) return NULL;
    return nd->src;
}

int HIR_DAG_CFG_rebuild(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        hir_block_t* hh = fb->entry;
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
                            }
                            
                            list_iter_next(&el_it);
                        }
                    }
                    else {
                        hir_subject_t* s = _apply_dag_on_block(nodes[i], dctx);
                        if (s) { 
                            switch (i) {
                                case 0: if (hh->farg->home) hh->farg->home->unused = 1; hh->farg = s; break;
                                case 1: if (hh->sarg->home) hh->sarg->home->unused = 1; hh->sarg = s; break;
                                case 2: if (hh->targ->home) hh->targ->home->unused = 1; hh->targ = s; break;
                            }
                        }
                    }
                }
            }

            if (hh == fb->exit) break;
            hh = hh->next;
        }
    }

    return 1;
}