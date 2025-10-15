#include <hir/dag.h>

int HIR_DAG_CFG_rebuild(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        hir_block_t* hh = fb->entry;
        while (hh) {
            if (hh->op != HIR_PHI) {
                hir_subject_t* nodes[3] = { hh->farg, hh->sarg, hh->targ };
                for (int i = HIR_writeop(hh->op); i < 3; i++) {
                    if (!nodes[i]) continue;
                    dag_node_t* nd = DAG_ACQUIRE_NODE(dctx, nodes[i]);
                    if (!nd) continue;
                    if (HIR_hash_subject(nd->src) == HIR_hash_subject(nodes[i])) continue;
                    switch (i) {
                        case 0: HIR_unload_subject(hh->farg); hh->farg = nd->src; break;
                        case 1: hh->sarg = nd->src; break;
                        case 2: hh->targ = nd->src; break;
                    }
                }
            }

            if (hh == fb->exit) break;
            hh = hh->next;
        }
    }

    return 1;
}