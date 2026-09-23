#include <hir/dag.h>

/* Register a new DAG node in the DAG context.
Params:
    - `dctx` - DAG context.
    - `dst` - DAG node.
    - `farg` - First DAG's argument.
    - `sarg` - Second DAG's argument.

Returns 1 on success. */
static int _register_node(dag_ctx_t* dctx, dag_node_t* dst, dag_node_t* farg, dag_node_t* sarg) {
    map_put(&dctx->groups, dst->hash, dst);
    map_put(&dctx->dag, HIR_hash_subject(dst->src), dst);
    if (farg) set_add(&farg->users, dst);
    if (sarg) set_add(&sarg->users, dst);
    return 1;
}

static int _replace_duplicate_node(dag_ctx_t* dctx, dag_node_t* duplicate, dag_node_t* canonical) {
    if (!dctx || !duplicate || !canonical || duplicate == canonical) return 0;
    set_foreach (dag_node_t* user, &duplicate->users) {
        set_remove(&user->args, duplicate);
        set_add(&user->args, canonical);
        set_add(&canonical->users, user);
    }

    set_foreach (dag_node_t* arg, &duplicate->args) {
        set_remove(&arg->users, duplicate);
    }

    set_foreach (void* link, &duplicate->link) {
        set_add(&canonical->link, link);
    }

    map_remove(&dctx->dag, HIR_hash_subject(duplicate->src));
    HIR_DAG_unload_node(duplicate);
    return 1;
}

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_hir_instructions (bb) {
                switch (hh->op) {
                    case HIR_LDREF: dctx->memory_version++; break;
                    case HIR_PHI:
                    case HIR_FARGLD:     case HIR_STARGLD:
                    case HIR_STORE_ECLL: case HIR_STORE_FCLL: case HIR_STORE_SYSC: {
                        dag_node_t* dst = DAG_GET_NODE(dctx, hh->op == HIR_PHI ? hh->sarg : hh->farg);
                        if (!dst) break;
                        dst->op   = hh->op;
                        dst->hash = HIR_DAG_compute_hash(dst);
                        dst->home = bb;
                        break;
                    }
                    case HIR_STORE: case HIR_REF: case HIR_GDREF:
                    case HIR_TPTR:
                    case HIR_TF64:  case HIR_TF32:
                    case HIR_TI64:  case HIR_TI32: case HIR_TI16:  case HIR_TI8:
                    case HIR_TU64:  case HIR_TU32: case HIR_TU16:  case HIR_TU8:
                    case HIR_iADD:  case HIR_iSUB: case HIR_iMUL:  case HIR_iDIV:
                    case HIR_iMOD:  case HIR_iLRG: case HIR_iLGE:  case HIR_iLWR:
                    case HIR_iLRE:  case HIR_iCMP: case HIR_iNMP:  case HIR_NOT:   case HIR_NEG:
                    case HIR_iAND:  case HIR_iOR:  case HIR_iBLFT: case HIR_iBRHT:
                    case HIR_bAND:  case HIR_bOR:  case HIR_bXOR: {
                        dag_node_t* farg = DAG_GET_NODE(dctx, hh->sarg);
                        dag_node_t* sarg = DAG_GET_NODE(dctx, hh->targ);
                        dag_node_t* dst  = DAG_GET_NODE(dctx, hh->farg);
                        if (!dst) break;

                        dst->op   = hh->op;
                        dst->home = bb;
                        if (hh->op == HIR_GDREF) dst->memory_version = dctx->memory_version;

                        if (HIR_is_commutative_op(hh->op)) {
                            if (farg) set_add(&dst->args, farg);
                            if (sarg) set_add(&dst->args, sarg);
                            dst->hash = HIR_DAG_compute_hash(dst);
                        }
                        else {
                            dst->hash = HIR_DAG_compute_hash(dst);
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
                        if (
                            ALLIAS_get_owners(dst->src->storage.var.v_id, &owners, &smt->m) && 
                            set_size(&owners)
                        ) dst->hash ^= 321123 * set_size(&owners);
                        set_free(&owners);

                        dag_node_t* existed;
                        if (!map_get(&dctx->groups, dst->hash, (void**)&existed)) _register_node(dctx, dst, farg, sarg);
                        else {
                            if (!set_has(&dst->home->dom, existed->home)) _register_node(dctx, dst, farg, sarg);
                            else {
                                if (dst != existed) {
                                    _replace_duplicate_node(dctx, dst, existed);
                                }
                            }
                        }

                        break;
                    }

                    default: break;
                }
            }
        }
    }

    return 1;
}
