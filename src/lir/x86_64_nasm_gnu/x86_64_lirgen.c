#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;

    scope_stack_t scopes = { .top = -1 };
    scope_stack_t heap   = { .top = -1 };
    sstack_t params      = { .top = -1 };

    long offset = 0;
    while (h) {
        switch (h->op) {
            case HIR_STORE: {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                break;
            }

            case HIR_STARGLD: {
                int vrsize = LIR_get_hirtype_size(h->farg->t);
                switch (h->sarg->storage.cnst.value) {
                    case 0: LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-8, vrsize)); break;
                    case 1: LIR_BLOCK2(ctx, LIR_REF, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-16, vrsize)); break;
                }

                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                break;
            }

            case HIR_PHI_PREAMBLE: {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
                break;
            }

            case HIR_STRT:    LIR_BLOCK0(ctx, LIR_STRT);                                             break;
            case HIR_STEND:   LIR_BLOCK0(ctx, LIR_STEND);                                            break;
            case HIR_OEXT:    LIR_BLOCK1(ctx, LIR_OEXT, LIR_SUBJ_STRING(h->farg->storage.str.s_id)); break;
            case HIR_MKSCOPE: scope_push(&scopes, h->farg->storage.cnst.value, offset);              break;
            case HIR_EXITOP: {
                LIR_deallocate_scope_heap(ctx, scope_id_top(&scopes), &heap);
                LIR_BLOCK1(ctx, LIR_EXITOP, LIR_format_variable(ctx, h->farg, smt));   
                break;
            }

            case HIR_ENDSCOPE: {
                scope_elem_t se;
                scope_pop_top(&scopes, &se);
                LIR_deallocate_scope_heap(ctx, h->farg->storage.cnst.value, &heap);
                stack_map_free_range(se.offset, -1, &ctx->stk);
                offset = se.offset;
                break;
            }
            
            case HIR_FRET: LIR_deallocate_scope_heap(ctx, scope_id_top(&scopes), &heap);
            case HIR_SYSC:
            case HIR_STORE_SYSC:
            case HIR_FCLL:
            case HIR_STORE_FCLL:
            case HIR_ECLL:
            case HIR_STORE_ECLL:
            case HIR_FDCL:
            case HIR_FARGLD: x86_64_generate_func(ctx, h, smt, &params); break;
            case HIR_FEND:   LIR_BLOCK0(ctx, LIR_FEND); break;

            case HIR_VRDEALL:
            case HIR_VARDECL:
            case HIR_ARRDECL:
            case HIR_STRDECL: x86_64_generate_declaration(ctx, h, smt, &params, &scopes, &heap, &offset); break;

            case HIR_REF:
            case HIR_GDREF:
            case HIR_LDREF:
            case HIR_GINDEX:
            case HIR_LINDEX: x86_64_generate_ref(ctx, h, smt); break;

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

            case HIR_NOT: x86_64_generate_unary_op(ctx, h, smt); break;
            case HIR_iBLFT:
            case HIR_iBRHT:
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
