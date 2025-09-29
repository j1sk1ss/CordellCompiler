#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

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
                        }
                        else {
                            arroff = stack_map_alloc(DEFAULT_TYPE_SIZE, &stackmap);
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

            case HIR_TF64:
            case HIR_TF32:
            case HIR_TI64:
            case HIR_TI32:
            case HIR_TI16:
            case HIR_TI8:
            case HIR_TU64:
            case HIR_TU32:
            case HIR_TU16:
            case HIR_TU8: x86_64_generate_conv(ctx, h, smt); break;

            case HIR_PRMST:
            case HIR_FARGST: stack_push_addr(&params, h->farg); break;

            case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
            case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

            case HIR_iLWR:
            case HIR_iLRE:
            case HIR_iLRG:
            case HIR_iLGE:
            case HIR_iCMP:
            case HIR_iNMP:
            case HIR_iOR:
            case HIR_iAND:
            case HIR_bOR:
            case HIR_bXOR:
            case HIR_bAND:
            case HIR_iMOD:
            case HIR_iSUB:
            case HIR_iDIV: 
            case HIR_iMUL: 
            case HIR_iADD: x86_64_generate_binary_op(ctx, h, smt); break;

            case HIR_IFOP:
            case HIR_IFCPOP:
            case HIR_IFNCPOP:
            case HIR_IFLWOP:
            case HIR_IFLWEOP:
            case HIR_IFLGOP:
            case HIR_IFLGEOP: x86_64_generate_ifop(ctx, h, smt); break;

            default: break;
        }
        
        h = h->next;
    }

    return 1;
}
