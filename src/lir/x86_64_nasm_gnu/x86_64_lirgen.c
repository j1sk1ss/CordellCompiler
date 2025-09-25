#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

static int _simd_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {

}

static int _binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    if (HIR_is_floattype(h->sarg->t) || HIR_is_floattype(h->targ->t)) return _simd_binary_op(ctx, h, smt);
    LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
    LIR_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
    
    switch (h->op) {
        case HIR_iSUB: {
            LIR_reg_op(ctx, RBX, RAX, LIR_iSUB);
            LIR_reg_op(ctx, RAX, RBX, LIR_iMOV);
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

            if (h->op == HIR_iMOD) LIR_reg_op(ctx, RAX, RDX, LIR_iMOV);
            break;
        }

        case HIR_iMUL: LIR_reg_op(ctx, RAX, RBX, LIR_iMUL); break;
        case HIR_iADD: LIR_reg_op(ctx, RAX, RBX, LIR_iADD); break;

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

    LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
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
                LIR_deallocate_scope_heap(ctx, se.id, &heap);
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
                LIR_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
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

            case HIR_VRDEALL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
                variable_info_t vi;
                if (VRTB_get_info_id(h->farg->storage.var.v_id, &vi, ctx)) {
                    stack_map_free(vi.offset, LIR_get_hirtype_size(h->farg->t), &stackmap);
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
                LIR_store_var_reg(LIR_GDREF, ctx, h->sarg, RAX, smt);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                break;
            }

            case HIR_LDREF: {
                LIR_store_var_reg(LIR_REF, ctx, h->farg, RAX, smt);
                LIR_store_var_reg(LIR_LDREF, ctx, h->sarg, RAX, smt);
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
