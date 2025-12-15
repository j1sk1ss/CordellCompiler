#include <hir/dag.h>

typedef struct {
    long               value;
    hir_subject_type_t t;
} const_t;

static int _parse_const(dag_node_t* nd, const_t* cnst, sym_table_t* smt) {
    if (!nd) return 0;
    switch (nd->src->t) {
        case HIR_F64NUMBER: cnst->t = HIR_F64CONSTVAL; goto _complete_number_parse;
        case HIR_I64NUMBER: cnst->t = HIR_I64CONSTVAL; goto _complete_number_parse;
        case HIR_U64NUMBER: cnst->t = HIR_U64CONSTVAL; goto _complete_number_parse;
        case HIR_F32NUMBER: cnst->t = HIR_F32CONSTVAL; goto _complete_number_parse;
        case HIR_I32NUMBER: cnst->t = HIR_I32CONSTVAL; goto _complete_number_parse;
        case HIR_U32NUMBER: cnst->t = HIR_U32CONSTVAL; goto _complete_number_parse;
        case HIR_I16NUMBER: cnst->t = HIR_I16CONSTVAL; goto _complete_number_parse;
        case HIR_U16NUMBER: cnst->t = HIR_U16CONSTVAL; goto _complete_number_parse;
        case HIR_I8NUMBER:  cnst->t = HIR_I8CONSTVAL;  goto _complete_number_parse;
        case HIR_U8NUMBER:  cnst->t = HIR_U8CONSTVAL;  goto _complete_number_parse;
        case HIR_NUMBER:    cnst->t = HIR_CONSTVAL; {
_complete_number_parse: {}
            cnst->value = nd->src->storage.num.value->to_llong(nd->src->storage.num.value);
            return 1;
        }

        case HIR_F64CONSTVAL: cnst->t = HIR_F64CONSTVAL; goto _complete_constant_parse;
        case HIR_I64CONSTVAL: cnst->t = HIR_I64CONSTVAL; goto _complete_constant_parse;
        case HIR_U64CONSTVAL: cnst->t = HIR_U64CONSTVAL; goto _complete_constant_parse;
        case HIR_F32CONSTVAL: cnst->t = HIR_F32CONSTVAL; goto _complete_constant_parse;
        case HIR_I32CONSTVAL: cnst->t = HIR_I32CONSTVAL; goto _complete_constant_parse;
        case HIR_U32CONSTVAL: cnst->t = HIR_U32CONSTVAL; goto _complete_constant_parse;
        case HIR_I16CONSTVAL: cnst->t = HIR_I16CONSTVAL; goto _complete_constant_parse;
        case HIR_U16CONSTVAL: cnst->t = HIR_U16CONSTVAL; goto _complete_constant_parse;
        case HIR_I8CONSTVAL:  cnst->t = HIR_I8CONSTVAL;  goto _complete_constant_parse;
        case HIR_U8CONSTVAL:  cnst->t = HIR_U8CONSTVAL;  goto _complete_constant_parse;
        case HIR_CONSTVAL:    cnst->t = HIR_CONSTVAL; {
_complete_constant_parse: {}
            cnst->value = nd->src->storage.cnst.value;
            return 1;
        }

        default: {
            variable_info_t vi;
            if (HIR_is_vartype(nd->src->t) && VRTB_get_info_id(nd->src->storage.var.v_id, &vi, &smt->v) && vi.vdi.defined) {
                cnst->t     = HIR_CONSTVAL;
                cnst->value = vi.vdi.definition;
                return 1;
            }

            return 0;
        }
    }
}

static int _const_args(dag_node_t* nd, dag_node_t** args) {
    int i = 0;
    set_iter_t it;
    set_iter_init(&nd->args, &it);
    dag_node_t* arg;
    while (set_iter_next(&it, (void**)&arg)) {
        args[i++] = arg;
    }

    return 1;
}

int HIR_sparse_const_propagation(dag_ctx_t* dctx, sym_table_t* smt) {
    int changed = 0;
    do {
        changed = 0;
        map_foreach (dag_node_t* nd, &dctx->dag) {
            dag_node_t* args[4] = { NULL };
            _const_args(nd, args);
            
            if (
                !HIR_is_vartype(nd->src->t) || 
                ALLIAS_get_owners(nd->src->storage.var.v_id, NULL, &smt->m)
            ) continue;

            long c = 0;
            const_t a, b;
            int a_pres = _parse_const(args[0], &a, smt);
            int b_pres = _parse_const(args[1], &b, smt);
            if (!args[1]) {
                str_memcpy(&b, &a, sizeof(const_t));
                b_pres = 1;
            }
            
            print_debug("src=%i, a_pres=%i, b_pres=%i", nd->src->storage.var.v_id, a_pres, b_pres);
            switch (nd->op) {
                case HIR_STORE: {
                    if (!a_pres) break;
                    if (VRTB_update_definition(nd->src->storage.var.v_id, a.value, &smt->v)) changed = 1;
                    print_debug("Store operation folded into val=%i", nd->op, a.value);
                    break;
                }

                case HIR_TF64:
                case HIR_TF32: {
                    if (!a_pres) break;
                    if (!HIR_is_float(a.t)) c = str_dob2bits((double)a.value);
                    else c = a.value;

                    if (VRTB_update_definition(nd->src->storage.var.v_id, c, &smt->v)) changed = 1;
                    print_debug("Convert op=%i folded into val=%i", nd->op, c);
                    break;
                }

                case HIR_TI64:
                case HIR_TI32:
                case HIR_TI16:
                case HIR_TI8:
                case HIR_TU64:
                case HIR_TU32:
                case HIR_TU16:
                case HIR_TU8: {
                    if (!a_pres) break;
                    if (HIR_is_float(a.t)) c = (long long)str_bits2dob(a.value);
                    else c = a.value;

                    if (VRTB_update_definition(nd->src->storage.var.v_id, c, &smt->v)) changed = 1;
                    print_debug("Convert op=%i folded into val=%i", nd->op, c);
                    break;
                }
                
                case HIR_NOT: {
                    if (!a_pres) break;
                    if (VRTB_update_definition(nd->src->storage.var.v_id, !a.value, &smt->v)) changed = 1;
                    print_debug("Convert op=%i folded into val=%i", nd->op, !a.value);
                    break;
                }

                case HIR_iADD:  c = a.value + b.value;  goto _binary_operation_fold;
                case HIR_iSUB:  c = a.value - b.value;  goto _binary_operation_fold;
                case HIR_iMUL:  c = a.value * b.value;  goto _binary_operation_fold;
                case HIR_iDIV:  c = a.value / b.value;  goto _binary_operation_fold;
                case HIR_iMOD:  c = a.value % b.value;  goto _binary_operation_fold;
                case HIR_iLRG:  c = a.value > b.value;  goto _binary_operation_fold;
                case HIR_iLGE:  c = a.value >= b.value; goto _binary_operation_fold;
                case HIR_iLWR:  c = a.value < b.value;  goto _binary_operation_fold;
                case HIR_iLRE:  c = a.value <= b.value; goto _binary_operation_fold;
                case HIR_iCMP:  c = a.value == b.value; goto _binary_operation_fold;
                case HIR_iNMP:  c = a.value != b.value; goto _binary_operation_fold;
                case HIR_iAND:  c = a.value && b.value; goto _binary_operation_fold;
                case HIR_iOR:   c = a.value || b.value; goto _binary_operation_fold;
                case HIR_iBLFT: c = a.value << b.value; goto _binary_operation_fold;
                case HIR_iBRHT: c = a.value >> b.value; goto _binary_operation_fold;
                case HIR_bAND:  c = a.value & b.value;  goto _binary_operation_fold;
                case HIR_bOR:   c = a.value | b.value;  goto _binary_operation_fold;
                case HIR_bXOR:  c = a.value ^ b.value; {
_binary_operation_fold: {}
                    if (!a_pres || !b_pres) break;
                    if (VRTB_update_definition(nd->src->storage.var.v_id, c, &smt->v)) changed = 1;
                    print_debug("%ld op=%i %ld folded into val=%i", a.value, nd->op, b.value, c);
                    break;
                }

                default: break;
            }
        }
    } while (changed);
    return 1;
}
