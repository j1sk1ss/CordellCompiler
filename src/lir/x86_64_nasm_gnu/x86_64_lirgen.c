#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

lir_subject_t* _format_variable(hir_subject_t* subj, sym_table_t* smt) {
    switch (subj->t) {
        case HIR_NUMBER:   return LIR_SUBJ_NUMBER(subj->storage.num.value);
        case HIR_CONSTVAL: return LIR_SUBJ_CONST(subj->storage.cnst.value);
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARF64: case HIR_TMPVARU64:
        case HIR_TMPVARI64: case HIR_TMPVARF32: case HIR_TMPVARU32: case HIR_TMPVARI32:
        case HIR_TMPVARU16: case HIR_TMPVARI16: case HIR_TMPVARU8:  case HIR_TMPVARI8:
        case HIR_STKVARSTR: case HIR_STKVARARR: case HIR_STKVARF64: case HIR_STKVARU64:
        case HIR_STKVARI64: case HIR_STKVARF32: case HIR_STKVARU32: case HIR_STKVARI32:
        case HIR_STKVARU16: case HIR_STKVARI16: case HIR_STKVARU8:  case HIR_STKVARI8:
        case HIR_GLBVARSTR: case HIR_GLBVARARR: case HIR_GLBVARF64: case HIR_GLBVARU64:
        case HIR_GLBVARI64: case HIR_GLBVARF32: case HIR_GLBVARU32: case HIR_GLBVARI32:
        case HIR_GLBVARU16: case HIR_GLBVARI16: case HIR_GLBVARU8:  case HIR_GLBVARI8: {
            variable_info_t vi;
            if (VRTB_get_info_id(subj->storage.var.v_id, &vi, &smt->v)) {
                if (!vi.glob) return LIR_SUBJ_OFF(vi.offset, VRS_variable_bitness(vi.type, vi.ptr));
                else return LIR_SUBJ_GLVAR(subj->storage.var.v_id);
            }
        }
        
        default: return NULL;
    }
}

/* Variable to register */
static inline int _store_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(reg, LIR_get_hirtype_size(subj->t)), _format_variable(subj, smt));
    return 1;
}

/* Variable from register */
static inline int _load_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, _format_variable(subj, smt), LIR_SUBJ_REG(reg, LIR_get_hirtype_size(subj->t)));
    return 1;
}

static inline int _reg_op(lir_ctx_t* ctx, int freg, int sreg, lir_operation_t op) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
    return 1;
}

static int _simd_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {

}

static int _binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    if (HIR_is_floattype(h->sarg->t) || HIR_is_floattype(h->targ->t)) return _simd_binary_op(ctx, h, smt);
    _store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
    _store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
    
    switch (h->op) {
        case HIR_iSUB: {
            _reg_op(ctx, RBX, RAX, LIR_iSUB);
            _reg_op(ctx, RAX, RBX, LIR_iMOV);
            break;
        }
        
        case HIR_iMOD:
        case HIR_iDIV: {
            LIR_BLOCK2(ctx, LIR_XCHG, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            if (HIR_is_signtype(h->sarg->t) && HIR_is_signtype(h->targ->t)) {
                LIR_BLOCK0(ctx, LIR_CDQ);
                LIR_BLOCK1(ctx, LIR_iDIV, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            } 
            else {
                LIR_BLOCK2(ctx, LIR_bXOR, LIR_SUBJ_REG(RDX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RDX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK1(ctx, LIR_DIV, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            }

            if (h->op == HIR_iMOD) _reg_op(ctx, RAX, RDX, LIR_iMOV);
            break;
        }

        case HIR_iMUL: _reg_op(ctx, RAX, RBX, LIR_iMUL); break;
        case HIR_iADD: _reg_op(ctx, RAX, RBX, LIR_iADD); break;

        case HIR_iAND: {
            break;
        }

        case HIR_iOR: {
            break;
        }

        case HIR_bAND: {
            break;
        }

        case HIR_bOR: {
            break;
        }
    }

    _load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
    return 1;
}

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;

    stack_map_t stackmap = { .offset = 0 };
    scope_stack_t scopes = { .top = -1 };
    scope_stack_t heap   = { .top = -1 };
    sstack_t params      = { .top = -1 };

    long offset = 0;
    while (h) {
        switch (h->op) {
            case HIR_STRT: LIR_BLOCK0(ctx, LIR_STRT); break;

            case HIR_STARGLD: {
                int vrsize = LIR_get_hirtype_size(h->farg->t);
                switch (h->sarg->storage.cnst.value) {
                    case 0: LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-8, vrsize)); break;
                    case 1: LIR_BLOCK2(ctx, LIR_REF, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-16, vrsize)); break;
                }

                break;
            }

            case HIR_MKSCOPE: scope_push(&scopes, h->farg->storage.cnst.value, offset); break;

            case HIR_FEND:
            case HIR_ENDSCOPE: {
                scope_elem_t se;
                scope_pop_top(&scopes, &se);

                if (se.id == scope_id_top(&heap)) {
                    scope_elem_t he;
                    scope_pop_top(&heap, &he);
                    print_debug("Heap deallocation after scope, heap_head=%i", he.offset);
                }

                print_debug("Stack deallocation after scope, off=%i", se.offset);
                stack_map_free_range(se.offset, -1, &stackmap);
                offset = se.offset;
                break;
            }

            case HIR_FDCL: {
                func_info_t fi;
                if (FNTB_get_info_id(h->farg->storage.str.s_id, &fi, &smt->f)) {
                    if (fi.global) LIR_BLOCK1(ctx, LIR_MKGLB, LIR_SUBJ_STRING(h->farg->storage.str.s_id));
                    LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
                    scope_push(&scopes, h->farg->storage.cnst.value, offset);
                }

                break;
            }

            case HIR_FRET: {
                int vrsize = LIR_get_hirtype_size(h->farg->t);
                _store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                LIR_BLOCK0(ctx, LIR_FRET);
                break;
            }

            case HIR_FARGLD: {
                alloc_info_t alloc;
                if (LIR_allocate_var(h->farg, &stackmap, smt, &alloc, &offset)) {
                    static const int abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
                    LIR_BLOCK2(
                        ctx, LIR_iMOV, LIR_SUBJ_OFF(alloc.offset, alloc.size), 
                        LIR_SUBJ_REG(abi_regs[h->sarg->storage.cnst.value], alloc.size)
                    );
                }

                break;
            }

            case HIR_VARDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
                alloc_info_t alloc;
                if (LIR_allocate_var(h->farg, &stackmap, smt, &alloc, &offset)) {
                    if (h->sarg) {
                        LIR_BLOCK2(
                            ctx, LIR_iMOV, LIR_SUBJ_OFF(alloc.offset, alloc.size), 
                            LIR_SUBJ_REG(RAX, alloc.size)
                        );
                    }
                }

                break;
            }

            case HIR_ARRDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;

                variable_info_t vi;
                if (VRTB_get_info_id(h->farg->storage.var.v_id, &vi, &smt->v)) {
                    array_info_t ai;
                    if (ARTB_get_info(vi.name, vi.s_id, &ai, &smt->a)) {
                        int arroff = -1;
                        int elsize = LIR_get_asttype_size(ai.el_type);

                        if (!ai.heap) {
                            int arrsize = elsize * h->sarg->storage.cnst.value;
                            arroff = stack_map_alloc(arrsize, &stackmap);
                            print_debug("HIR_ARRDECL allocation, size=%i, off=%i", arrsize, arroff);
                        }
                        else {
                            arroff = stack_map_alloc(DEFAULT_TYPE_SIZE, &stackmap);
                            print_debug("Heap allocation in scope=%i, heap_head=%i", scope_id_top(&scopes), arroff);
                            scope_push(&heap, scope_id_top(&scopes), arroff);
                        }

                        VRTB_update_offset(h->farg->storage.var.v_id, arroff, &smt->v);
                    }

                }

                break;
            }

            case HIR_STRDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
                break;
            }

            case HIR_GDREF: {
                _store_var_reg(LIR_GDREF, ctx, h->sarg, RAX, smt);
                _load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                break;
            }

            case HIR_LDREF: {
                _store_var_reg(LIR_REF, ctx, h->farg, RAX, smt);
                _store_var_reg(LIR_LDREF, ctx, h->sarg, RAX, smt);
                break;
            }

            case HIR_FARGST:
            case HIR_PRMST: stack_push_addr(&params, h->farg); break;

            case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;
            case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;

            case HIR_iOR:
            case HIR_iAND:
            case HIR_bOR:
            case HIR_bAND:
            case HIR_iMOD:
            case HIR_iSUB:
            case HIR_iDIV: 
            case HIR_iMUL: 
            case HIR_iADD: _binary_op(ctx, h, smt); break;
            default: break;
        }
        
        h = h->next;
    }

    return 1;
}
