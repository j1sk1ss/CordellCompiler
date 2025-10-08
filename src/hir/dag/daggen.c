#include <hir/dag.h>

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            hir_block_t* hh = cb->entry;
            while (hh) {
                switch (hh->op) {
                    case HIR_STORE_ECLL:
                    case HIR_STORE_FCLL:
                    case HIR_STORE_SYSC:
                    case HIR_PHI: {
                        break;
                    }

                    case HIR_FARGLD:
                    case HIR_STARGLD: {
                    }

                    case HIR_TF64:
                    case HIR_TF32:
                    case HIR_TI64:
                    case HIR_TI32:
                    case HIR_TI16:
                    case HIR_TI8:
                    case HIR_TU64:
                    case HIR_TU32:
                    case HIR_TU16:
                    case HIR_TU8:
                    case HIR_iADD:
                    case HIR_iSUB:
                    case HIR_iMUL:
                    case HIR_iDIV:
                    case HIR_iMOD:
                    case HIR_iLRG:
                    case HIR_iLGE:
                    case HIR_iLWR:
                    case HIR_iLRE:
                    case HIR_iCMP:
                    case HIR_iNMP:
                    case HIR_iAND:
                    case HIR_iOR:
                    case HIR_iBLFT:
                    case HIR_iBRHT:
                    case HIR_bAND:
                    case HIR_bOR:
                    case HIR_bXOR:
                    case HIR_STORE: {
                        dag_node_t* snd = DAG_GET_NODE(dctx, hh->sarg);
                        dag_node_t* tnd = DAG_GET_NODE(dctx, hh->targ);
                        dag_node_t* nd  = DAG_SUBJ_ARG2(snd, tnd);
                        DAG_NODE(dctx, hh->op, hh->farg, nd);
                        if (snd) set_add(&snd->users, nd);
                        if (tnd) set_add(&tnd->users, nd);
                        break;
                    }
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}
