#include <hir/dag.h>

static long _parse_const(dag_node_t* nd) {
    if (!nd) return 0;
    switch (nd->src->t) {
        case HIR_F64NUMBER:
        case HIR_I64NUMBER:
        case HIR_U64NUMBER:
        case HIR_F32NUMBER:
        case HIR_I32NUMBER:
        case HIR_U32NUMBER:
        case HIR_I16NUMBER:
        case HIR_U16NUMBER:
        case HIR_I8NUMBER:
        case HIR_U8NUMBER:
        case HIR_NUMBER: return str_atoi(nd->src->storage.num.value);

        case HIR_F64CONSTVAL:
        case HIR_I64CONSTVAL:
        case HIR_U64CONSTVAL:
        case HIR_F32CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_U32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_U16CONSTVAL:
        case HIR_I8CONSTVAL:
        case HIR_U8CONSTVAL:
        case HIR_CONSTVAL: return nd->src->storage.cnst.value;
        default: return 0;
    }
}

static int _const_args(dag_node_t* nd, dag_node_t** args) {
    int i = 0;
    set_iter_t it;
    set_iter_init(&nd->args, &it);
    dag_node_t* arg;
    while (set_iter_next(&it, (void**)&arg)) {
        if (!HIR_defined_type(arg->src->t)) return 0;
        args[i++] = arg;
    }

    return 1;
}
/* TODO */
int HIR_DAG_sparse_const_propagation(dag_ctx_t* dctx) {
    int changed = 0;
    do {
        changed = 0;
        map_iter_t it;
        map_iter_init(&dctx->dag, &it);
        dag_node_t* nd;
        while (map_iter_next(&it, (void**)&nd)) {
            dag_node_t* args[4] = { NULL };
            if (!_const_args(nd, args)) continue;

            hir_subject_type_t ntype;
            long a = _parse_const(args[0]), b = _parse_const(args[1]), c = 0;

            switch (nd->op) {
                /* Convertion */
                case HIR_TF64: ntype = HIR_F64CONSTVAL; goto _conv_operation_fold;
                case HIR_TF32: ntype = HIR_F32CONSTVAL; goto _conv_operation_fold;
                case HIR_TI64: ntype = HIR_I64CONSTVAL; goto _conv_operation_fold;
                case HIR_TI32: ntype = HIR_I32CONSTVAL; goto _conv_operation_fold;
                case HIR_TI16: ntype = HIR_I16CONSTVAL; goto _conv_operation_fold;
                case HIR_TI8:  ntype = HIR_I8CONSTVAL;  goto _conv_operation_fold;
                case HIR_TU64: ntype = HIR_U64CONSTVAL; goto _conv_operation_fold; 
                case HIR_TU32: ntype = HIR_U32CONSTVAL; goto _conv_operation_fold; 
                case HIR_TU16: ntype = HIR_U16CONSTVAL; goto _conv_operation_fold; 
                case HIR_TU8:  ntype = HIR_U8CONSTVAL;  goto _conv_operation_fold;
                {
_conv_operation_fold: {}
                    nd->src->t = ntype;
                    nd->src->storage.cnst.value = a;
                    HIR_DAG_unload_node(args[0]);
                    break;
                }
                
                /* Unary operation */
                case HIR_NOT: {
                    c = !a;
                    break;
                }

                /* Binary operation */
                case HIR_iADD:  c = a + b;  goto _binary_operation_fold;
                case HIR_iSUB:  c = a - b;  goto _binary_operation_fold;
                case HIR_iMUL:  c = a * b;  goto _binary_operation_fold;
                case HIR_iDIV:  c = a / b;  goto _binary_operation_fold;
                case HIR_iMOD:  c = a % b;  goto _binary_operation_fold;
                case HIR_iLRG:  c = a > b;  goto _binary_operation_fold;
                case HIR_iLGE:  c = a >= b; goto _binary_operation_fold;
                case HIR_iLWR:  c = a < b;  goto _binary_operation_fold;
                case HIR_iLRE:  c = a <= b; goto _binary_operation_fold;
                case HIR_iCMP:  c = a == b; goto _binary_operation_fold;
                case HIR_iNMP:  c = a != b; goto _binary_operation_fold;
                case HIR_iAND:  c = a && b; goto _binary_operation_fold;
                case HIR_iOR:   c = a || b; goto _binary_operation_fold;
                case HIR_iBLFT: c = a << b; goto _binary_operation_fold;
                case HIR_iBRHT: c = a >> b; goto _binary_operation_fold;
                case HIR_bAND:  c = a & b;  goto _binary_operation_fold;
                case HIR_bOR:   c = a | b;  goto _binary_operation_fold;
                case HIR_bXOR:  c = a ^ b;  goto _binary_operation_fold;
                {
_binary_operation_fold: {}
                    break;
                }

                default: break;
            }
        }
    } while (changed);
    return 1;
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
                    dag_node_t* nd = DAG_ACQUIRE_NODE(dctx, nodes[i]);
                    if (!nd) continue;
                    if (HIR_hash_subject(nd->src) == HIR_hash_subject(nodes[i])) continue;
                    switch (i) {
                        case 0: hh->farg->home->unused = 1; hh->farg = nd->src; break;
                        case 1: hh->sarg->home->unused = 1; hh->sarg = nd->src; break;
                        case 2: hh->targ->home->unused = 1; hh->targ = nd->src; break;
                    }
                }
            }

            if (hh == fb->exit) break;
            hh = hh->next;
        }
    }

    return 1;
}