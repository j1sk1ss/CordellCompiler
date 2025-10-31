#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

static int _iterate_block(
    scope_stack_t* scopes, scope_stack_t* heap, sstack_t* params, 
    cfg_block_t* bb, long prev_bid, lir_ctx_t* ctx, sym_table_t* smt
) {
    if (!bb || set_has(&bb->visitors, (void*)prev_bid)) return 0;
    hir_block_t* h = bb->hmap.entry;
    bb->lmap.entry = ctx->t;

    long offset = 0;
    while (h) {
        if (!h->unused) switch (h->op) {
            case HIR_STORE: {
                x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, -1, smt);
                x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
                break;
            }

            case HIR_STARGLD: {
                int vrsize = LIR_get_hirtype_size(h->farg->t);
                switch (h->sarg->storage.cnst.value) {
                    case 0: LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-8, vrsize)); break;
                    case 1: LIR_BLOCK2(ctx, LIR_REF, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-16, vrsize)); break;
                }

                x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
                break;
            }

            case HIR_PHI_PREAMBLE: {
                x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, -1, smt);
                x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
                break;
            }

            case HIR_STRT:    LIR_BLOCK0(ctx, LIR_STRT);                                             break;
            case HIR_STEND:   LIR_BLOCK0(ctx, LIR_STEND);                                            break;
            case HIR_OEXT:    LIR_BLOCK1(ctx, LIR_OEXT, LIR_SUBJ_STRING(h->farg->storage.str.s_id)); break;
            case HIR_MKSCOPE: scope_push(scopes, h->farg->storage.cnst.value, offset);               break;
            case HIR_EXITOP: {
                x86_64_deallocate_heap(ctx, scope_id_top(scopes), heap);
                if (h->farg) x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RDI, -1, smt);
                LIR_BLOCK0(ctx, LIR_EXITOP);   
                break;
            }
            
            case HIR_ENDSCOPE: {
                scope_elem_t se;
                scope_pop_top(scopes, &se);
                x86_64_deallocate_heap(ctx, h->farg->storage.cnst.value, heap);
                offset = se.offset;
                break;
            }
            
            case HIR_FRET: x86_64_deallocate_heap(ctx, scope_id_top(scopes), heap);
            case HIR_SYSC:
            case HIR_STORE_SYSC:
            case HIR_FCLL:
            case HIR_STORE_FCLL:
            case HIR_ECLL:
            case HIR_STORE_ECLL:
            case HIR_FDCL:
            case HIR_FARGLD: x86_64_generate_func(ctx, h, smt); break;
            case HIR_FEND:   LIR_BLOCK0(ctx, LIR_FEND); break;

            case HIR_BREAKPOINT: LIR_BLOCK0(ctx, LIR_BREAKPOINT); break;

            case HIR_STASM:
            case HIR_RAW:
            case HIR_ENDASM: x86_64_generate_asmblock(ctx, h, smt, params); break;

            case HIR_VRDEALL:
            case HIR_VARDECL:
            case HIR_ARRDECL:
            case HIR_STRDECL: x86_64_generate_declaration(ctx, h, smt, params, scopes, heap, &offset); break;

            case HIR_REF:
            case HIR_GDREF:
            case HIR_LDREF: x86_64_generate_ref(ctx, h, smt); break;

            case HIR_TF64: case HIR_TF32:
            case HIR_TI64: case HIR_TI32: case HIR_TI16: case HIR_TI8:
            case HIR_TU64: case HIR_TU32: case HIR_TU16: case HIR_TU8: 
            x86_64_generate_conv(ctx, h, smt); break;

            case HIR_PRMST:
            case HIR_FARGST: stack_push(params, h->farg); break;

            case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
            case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

            case HIR_iBLFT: case HIR_iBRHT:
            case HIR_iLWR: case HIR_iLRE: case HIR_iLRG: case HIR_iLGE:
            case HIR_iCMP: case HIR_iNMP:
            case HIR_iOR: case HIR_iAND:
            case HIR_bOR: case HIR_bXOR: case HIR_bAND:
            case HIR_iMOD: 
            case HIR_iSUB: case HIR_iDIV: case HIR_iMUL: case HIR_iADD: 
            x86_64_generate_binary_op(ctx, h, smt); break;

            case HIR_NOT:   x86_64_generate_unary_op(ctx, h, smt); break;
            case HIR_IFOP2: x86_64_generate_ifop(ctx, h, smt);     break;

            default: break;
        }
        
        if (h == bb->hmap.exit) break;
        h = h->next;
    }

    if (!bb->lmap.entry) bb->lmap.entry = ctx->h;
    else bb->lmap.entry = bb->lmap.entry->next;
    bb->lmap.exit = ctx->t;
    
    set_add(&bb->visitors, (void*)prev_bid);
    _iterate_block(scopes, heap, params, bb->jmp, bb->id, ctx, smt);
    _iterate_block(scopes, heap, params, bb->l, bb->id, ctx, smt);
    return 1;
}

int x86_64_generate_lir(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    scope_stack_t scopes = { .top = -1 };
    scope_stack_t heap   = { .top = -1 };
    sstack_t params      = { .top = -1 };
    
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        _iterate_block(&scopes, &heap, &params, list_get_head(&fb->blocks), 0, ctx, smt);
    }

    return 1;
}
