#include <hir/dag.h>

static int _register_node(dag_ctx_t* dctx, dag_node_t* dst, dag_node_t* src1, dag_node_t* src2) {
    map_put(&dctx->groups, dst->hash, dst);
    map_put(&dctx->dag, HIR_hash_subject(dst->src), dst);
    if (src1) set_add(&src1->users, dst);
    if (src2) set_add(&src2->users, dst);
    return 1;
}

int HIR_DAG_init(dag_ctx_t* dctx) {
    if (!dctx) return 0;
    map_init(&dctx->dag);
    map_init(&dctx->groups);
    return 1;
}

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx, sym_table_t* smt) {
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
                    case HIR_PHI_PREAMBLE:
                    case HIR_STORE_ECLL:
                    case HIR_STORE_FCLL:
                    case HIR_STORE_SYSC:
                    case HIR_FARGLD:
                    case HIR_STARGLD: {
                        dag_node_t* dst = DAG_GET_NODE(dctx, hh->farg);
                        if (!dst) break;
                        dst->op   = hh->op;
                        dst->hash = HIR_DAG_compute_hash(dst);
                        dst->home = cb;
                        break;
                    }

                    case HIR_STORE:
                    case HIR_TF64: case HIR_TF32:
                    case HIR_TI64: case HIR_TI32: case HIR_TI16: case HIR_TI8:
                    case HIR_TU64: case HIR_TU32: case HIR_TU16: case HIR_TU8:
                    case HIR_iADD: case HIR_iSUB: case HIR_iMUL: case HIR_iDIV:
                    case HIR_iMOD: case HIR_iLRG: case HIR_iLGE: case HIR_iLWR:
                    case HIR_iLRE: case HIR_iCMP: case HIR_iNMP:
                    case HIR_iAND: case HIR_iOR: case HIR_iBLFT: case HIR_iBRHT:
                    case HIR_bAND: case HIR_bOR: case HIR_bXOR: {
                        dag_node_t* src1 = DAG_GET_NODE(dctx, hh->sarg);
                        dag_node_t* src2 = DAG_GET_NODE(dctx, hh->targ);
                        dag_node_t* dst  = DAG_GET_NODE(dctx, hh->farg);
                        if (!dst) break;

                        if (src1) set_add(&dst->args, src1);
                        if (src2) set_add(&dst->args, src2);
                        dst->op   = hh->op;
                        dst->hash = HIR_DAG_compute_hash(dst);
                        dst->home = cb;

                        dag_node_t* existed;
                        if (!map_get(&dctx->groups, dst->hash, (void**)&existed)) _register_node(dctx, dst, src1, src2);
                        else {
                            set_t owners;
                            if (
                                !set_has(&dst->home->dom, existed->home) ||
                                (ALLIAS_get_owners(existed->src->storage.var.v_id, &owners, &smt->m) && set_size(&owners))
                            ) _register_node(dctx, dst, src1, src2);
                            else {
                                map_remove(&dctx->dag, HIR_hash_subject(dst->src));
                                HIR_DAG_unload_node(dst);
                                set_add(&existed->link, (void*)HIR_hash_subject(hh->farg));
                                set_free_force(&owners);
                            }
                        }

                        break;
                    }

                    default: break;
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}
