#include <lir/lirgens/lirgens.h>

static int _iterate_block(
    sstack_t* params, cfg_block_t* bb, lir_ctx_t* ctx, sym_table_t* smt
) { 
    hir_block_t* h = bb->hmap.entry;
    bb->lmap.entry = ctx->t;

    long offset = 0;
    while (h) {
        if (!h->unused) switch (h->op) {
            case HIR_PHI_PREAMBLE:
            case HIR_STORE:    x86_64_store_var2var(LIR_iMOV, ctx, h->farg, h->sarg);                      break;
            case HIR_STARGLD:  LIR_BLOCK1(ctx, LIR_STARGLD, x86_64_format_variable(h->farg));              break;
            case HIR_STRT:     LIR_BLOCK0(ctx, LIR_STRT);                                                  break;
            case HIR_STEND:    LIR_BLOCK0(ctx, LIR_STEND);                                                 break;
            case HIR_OEXT:     LIR_BLOCK1(ctx, LIR_OEXT, LIR_SUBJ_STRING(h->farg->storage.str.s_id));      break;
            case HIR_MKSCOPE:  LIR_BLOCK1(ctx, LIR_MKSCOPE, LIR_SUBJ_CONST(h->farg->storage.cnst.value));  break;
            case HIR_EXITOP:   LIR_BLOCK1(ctx, LIR_EXITOP, x86_64_format_variable(h->farg));               break;
            case HIR_ENDSCOPE: LIR_BLOCK1(ctx, LIR_ENDSCOPE, LIR_SUBJ_CONST(h->farg->storage.cnst.value)); break;
            
            case HIR_FRET:
            case HIR_SYSC:
            case HIR_STORE_SYSC:
            case HIR_FCLL:
            case HIR_STORE_FCLL:
            case HIR_ECLL:
            case HIR_STORE_ECLL:
            case HIR_FDCL:
            case HIR_FARGLD: x86_64_generate_func(ctx, h, smt); break;
            case HIR_FEND:   LIR_BLOCK0(ctx, LIR_FEND);         break;

            case HIR_BREAKPOINT: LIR_BLOCK0(ctx, LIR_BREAKPOINT); break;

            case HIR_STASM:
            case HIR_RAW:
            case HIR_ENDASM: x86_64_generate_asmblock(ctx, h, smt, params); break;

            case HIR_VRDEALL: LIR_BLOCK1(ctx, LIR_VRDEALL, LIR_SUBJ_CONST(h->farg->storage.cnst.value)); break;
            case HIR_STRDECL: 
                LIR_BLOCK2(ctx, LIR_STRDECL, x86_64_format_variable(h->farg), LIR_SUBJ_STRING(h->sarg->storage.str.s_id)); 
            break;
            case HIR_ARRDECL: 
                LIR_BLOCK1(ctx, LIR_STRDECL, x86_64_format_variable(h->farg));
            break;

            case HIR_REF:   x86_64_store_var2var(LIR_REF, ctx, h->farg, h->sarg);   break;
            case HIR_GDREF: x86_64_store_var2var(LIR_GDREF, ctx, h->farg, h->sarg); break;
            case HIR_LDREF: x86_64_store_var2var(LIR_LDREF, ctx, h->farg, h->sarg); break;
            
            case HIR_TF64: LIR_BLOCK2(ctx, LIR_TF64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TF32: LIR_BLOCK2(ctx, LIR_TF32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TI64: LIR_BLOCK2(ctx, LIR_TI64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TI32: LIR_BLOCK2(ctx, LIR_TI32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TI16: LIR_BLOCK2(ctx, LIR_TI16, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TI8:  LIR_BLOCK2(ctx, LIR_TI8, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg));  break;
            case HIR_TU64: LIR_BLOCK2(ctx, LIR_TU64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TU32: LIR_BLOCK2(ctx, LIR_TU32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TU16: LIR_BLOCK2(ctx, LIR_TU16, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_TU8:  LIR_BLOCK2(ctx, LIR_TU8, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg));  break;

            case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
            case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

            case HIR_NOT: LIR_BLOCK2(ctx, LIR_NOT, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
            case HIR_IFOP2: {
                LIR_BLOCK2(ctx, LIR_IFOP2, x86_64_format_variable(h->farg), LIR_SUBJ_CONST(0));
                LIR_BLOCK1(ctx, LIR_JE, LIR_SUBJ_LABEL(h->sarg->id));
                LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->targ->id));
                break;
            }

            case HIR_iBLFT: LIR_BLOCK3(ctx, LIR_iBLFT, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ)); break;
            case HIR_iBRHT: LIR_BLOCK3(ctx, LIR_iBRHT, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ)); break;
            case HIR_iLWR:  LIR_BLOCK3(ctx, LIR_iLWR, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iLRE:  LIR_BLOCK3(ctx, LIR_iLRE, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iLRG:  LIR_BLOCK3(ctx, LIR_iLRG, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iLGE:  LIR_BLOCK3(ctx, LIR_iLGE, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iCMP:  LIR_BLOCK3(ctx, LIR_iCMP, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iNMP:  LIR_BLOCK3(ctx, LIR_iNMP, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iOR:   LIR_BLOCK3(ctx, LIR_iOR, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));   break;
            case HIR_iAND:  LIR_BLOCK3(ctx, LIR_iAND, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_bOR:   LIR_BLOCK3(ctx, LIR_bOR, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));   break;
            case HIR_bXOR:  LIR_BLOCK3(ctx, LIR_bXOR, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_bAND:  LIR_BLOCK3(ctx, LIR_bAND, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iMOD:  LIR_BLOCK3(ctx, LIR_iMOD, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iSUB:  LIR_BLOCK3(ctx, LIR_iSUB, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iDIV:  LIR_BLOCK3(ctx, LIR_iDIV, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iMUL:  LIR_BLOCK3(ctx, LIR_iMUL, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;
            case HIR_iADD:  LIR_BLOCK3(ctx, LIR_iADD, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg), x86_64_format_variable(h->targ));  break;

            default: break;
        }
        
        if (h == bb->hmap.exit) break;
        h = h->next;
    }

    if (!bb->lmap.entry) bb->lmap.entry = ctx->h;
    else bb->lmap.entry = bb->lmap.entry->next;
    bb->lmap.exit = ctx->t;
    return 1;
}

int LIR_generate_block(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    sstack_t params = { .top = -1 };
    
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            _iterate_block(&params, bb, ctx, smt);
        }
    }

    return 1;
}
