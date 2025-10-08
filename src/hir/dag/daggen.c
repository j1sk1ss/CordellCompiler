#include <hir/dag.h>

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        hir_block_t* hh = fb->entry;
        while (hh) {
            switch (hh->op) {
                case HIR_STORE_ECLL:
                case HIR_STORE_FCLL:
                case HIR_STORE_SYSC: {
                    break;
                }

                case HIR_FARGLD:
                case HIR_STARGLD: {
                    dag_node_t* dst = DAG_GET_NODE(dctx, hh->farg);
                    dst->op = hh->op;
                    break;
                }

                case HIR_PHI_PREAMBLE:
                case HIR_STORE:
                case HIR_TF64:
                case HIR_TF32:
                case HIR_TI64:
                case HIR_TI32:
                case HIR_TI16:
                case HIR_TI8:
                case HIR_TU64:
                case HIR_TU32:
                case HIR_TU16:
                case HIR_TU8: {
                    dag_node_t* dst = DAG_GET_NODE(dctx, hh->farg);
                    dag_node_t* src = DAG_GET_NODE(dctx, hh->sarg);
                    set_add(&dst->args, src);
                    dst->op = hh->op;
                    if (src) set_add(&src->users, dst);
                    break;
                }

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
                case HIR_bXOR: {
                    dag_node_t* snd = DAG_GET_NODE(dctx, hh->sarg);
                    dag_node_t* tnd = DAG_GET_NODE(dctx, hh->targ);
                    dag_node_t* nd  = DAG_GET_NODE(dctx, hh->farg);
                    set_add(&nd->args, snd);
                    set_add(&nd->args, tnd);
                    nd->op = hh->op;
                    if (snd) set_add(&snd->users, nd);
                    if (tnd) set_add(&tnd->users, nd);
                    break;
                }

                default: break;
            }

            if (hh == fb->exit) break;
            hh = hh->next;
        }
    }

    return 1;
}
