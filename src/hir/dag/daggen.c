#include <hir/dag.h>

/*
Register a new DAG node in the DAG context.
Params:
    - `dctx` - DAG context.
    - `dst` - DAG node.
    - `farg` - First DAG's argument.
    - `sarg` - Second DAG's argument.

Return 1 if succeed.
*/
static int _register_node(dag_ctx_t* dctx, dag_node_t* dst, dag_node_t* farg, dag_node_t* sarg) {
    map_put(&dctx->groups, dst->hash, dst);
    map_put(&dctx->dag, HIR_hash_subject(dst->src), dst);
    if (farg) set_add(&farg->users, dst);
    if (sarg) set_add(&sarg->users, dst);
    return 1;
}

int HIR_DAG_init(dag_ctx_t* dctx) {
    if (!dctx) return 0;
    map_init(&dctx->dag, MAP_NO_CMP);
    map_init(&dctx->groups, MAP_NO_CMP);
    return 1;
}

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
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

                    case HIR_STORE: case HIR_REF:
                    case HIR_TF64:  case HIR_TF32:
                    case HIR_TI64:  case HIR_TI32: case HIR_TI16:  case HIR_TI8:
                    case HIR_TU64:  case HIR_TU32: case HIR_TU16:  case HIR_TU8:
                    case HIR_iADD:  case HIR_iSUB: case HIR_iMUL:  case HIR_iDIV:
                    case HIR_iMOD:  case HIR_iLRG: case HIR_iLGE:  case HIR_iLWR:
                    case HIR_iLRE:  case HIR_iCMP: case HIR_iNMP:  case HIR_NOT:
                    case HIR_iAND:  case HIR_iOR:  case HIR_iBLFT: case HIR_iBRHT:
                    case HIR_bAND:  case HIR_bOR:  case HIR_bXOR: {
                        dag_node_t* farg = DAG_GET_NODE(dctx, hh->sarg);
                        dag_node_t* sarg = DAG_GET_NODE(dctx, hh->targ);
                        dag_node_t* dst  = DAG_GET_NODE(dctx, hh->farg);
                        if (!dst) break;

                        if (HIR_commutative_op(hh->op)) {
                            if (farg) set_add(&dst->args, farg);
                            if (sarg) set_add(&dst->args, sarg);
                            dst->hash = HIR_DAG_compute_hash(dst);
                        }
                        else {
                            dst->hash = hh->op * 1315423911UL;
                            if (farg) {
                                set_add(&dst->args, farg);
                                dst->hash ^= HIR_DAG_compute_hash(farg) + (dst->hash << 6) + (dst->hash >> 2);
                            }

                            if (sarg) {
                                set_add(&dst->args, sarg);
                                dst->hash ^= HIR_DAG_compute_hash(sarg) + (dst->hash << 6) + (dst->hash >> 2);
                            }
                        }

                        set_t owners;
                        if ((ALLIAS_get_owners(dst->src->storage.var.v_id, &owners, &smt->m) && set_size(&owners))) dst->hash ^= 321123;
                        set_free(&owners);

                        dst->op   = hh->op;
                        dst->home = cb;

                        dag_node_t* existed;
                        if (!map_get(&dctx->groups, dst->hash, (void**)&existed)) _register_node(dctx, dst, farg, sarg);
                        else {
                            if (!set_has(&dst->home->dom, existed->home)) _register_node(dctx, dst, farg, sarg);
                            else {
                                if (dst != existed) {
                                    map_remove(&dctx->dag, HIR_hash_subject(dst->src));
                                    HIR_DAG_unload_node(dst);
                                    set_add(&existed->link, (void*)HIR_hash_subject(hh->farg));
                                }
                            }
                        }

                        break;
                    }

                    default: break;
                }

                hh = HIR_get_next(hh, cb->hmap.exit, 1);
            }
        }
    }

    return 1;
}
