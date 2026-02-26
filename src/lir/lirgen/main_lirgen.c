#include <lir/lirgens/lirgens.h>

static int _pass_params(lir_operation_t op, lir_ctx_t* ctx, list_t* hir_args, list_t* lir_args) {
    int argnum = 0;
    foreach (hir_subject_t* hir_arg, hir_args) {
        lir_subject_t* lir_arg = LIR_convert_hs_to_ls(hir_arg);
        list_add(lir_args, lir_arg);
        LIR_BLOCK2(ctx, op, lir_arg, LIR_SUBJ_CONST(argnum++));
    }

    return 1;
}

static inline void _store_var2var(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* dst, hir_subject_t* src) {
    LIR_BLOCK2(ctx, op, LIR_convert_hs_to_ls(dst), LIR_convert_hs_to_ls(src));
}

static int _convert_hir_to_lir(sstack_t* params, hir_block_t* h, lir_ctx_t* ctx, sym_table_t* smt) {
    switch (h->op) {
        case HIR_PHI_PREAMBLE:
        case HIR_STORE:    _store_var2var(LIR_iMOV, ctx, h->farg, h->sarg);                                                          break;
        case HIR_STARGLD:  LIR_BLOCK2(ctx, LIR_STARGLD, LIR_convert_hs_to_ls(h->farg), LIR_SUBJ_CONST(h->sarg->storage.cnst.value)); break;
        case HIR_STRT:     LIR_BLOCK1(ctx, LIR_STRT, LIR_SUBJ_FUNCNAME(h->farg));                                                    break;
        case HIR_OEXT:     LIR_BLOCK1(ctx, LIR_OEXT, LIR_SUBJ_CONST(h->farg->storage.cnst.value));                                   break;
        case HIR_FEXT:     LIR_BLOCK1(ctx, LIR_FEXT, LIR_SUBJ_CONST(h->farg->storage.cnst.value));                                   break;
        case HIR_EXITOP:   LIR_BLOCK1(ctx, LIR_EXITOP, LIR_convert_hs_to_ls(h->farg));                                               break;
        
        case HIR_FDCL:   LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));                                                      break;
        case HIR_FRET:   LIR_BLOCK1(ctx, LIR_FRET, LIR_convert_hs_to_ls(h->farg));                                                   break;
        case HIR_FARGLD: LIR_BLOCK2(ctx, LIR_LOADFARG, LIR_convert_hs_to_ls(h->farg), LIR_SUBJ_CONST(h->sarg->storage.cnst.value));  break;

        case HIR_UFCLL:
        case HIR_FCLL:
        case HIR_ECLL: 
        case HIR_STORE_UFCLL:
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            lir_subject_t* sargs = LIR_SUBJ_LIST();
            _pass_params(LIR_STFARG, ctx, &h->targ->storage.list.h, &sargs->storage.list.h);
            LIR_BLOCK3(
                ctx, LIR_FCLL, 
                h->sarg->t == HIR_FNAME ? LIR_SUBJ_FUNCNAME(h->sarg) : LIR_convert_hs_to_ls(h->sarg), 
                NULL, sargs
            );

            if (
                h->op == HIR_STORE_UFCLL || 
                h->op == HIR_STORE_FCLL  || 
                h->op == HIR_STORE_ECLL
            ) LIR_BLOCK1(ctx, LIR_LOADFRET, LIR_convert_hs_to_ls(h->farg));
            break;
        }

        case HIR_SYSC: 
        case HIR_STORE_SYSC: {
            lir_subject_t* sargs = LIR_SUBJ_LIST();
            _pass_params(LIR_STSARG, ctx, &h->targ->storage.list.h, &sargs->storage.list.h);
            LIR_BLOCK3(ctx, LIR_SYSC, NULL, NULL, sargs);
            if (h->op == HIR_STORE_SYSC) {
                LIR_BLOCK1(ctx, LIR_LOADFRET, LIR_convert_hs_to_ls(h->farg));
            }
            
            break;
        }

        case HIR_BREAKPOINT: LIR_BLOCK1(ctx, LIR_BREAKPOINT, LIR_SUBJ_STRING(h->farg->storage.str.s_id)); break;
        case HIR_RAW: {
            str_info_t si;
            if (!STTB_get_info_id(h->farg->storage.str.s_id, &si, &smt->s)) break;

            int argnum = -1;
            string_t* p = si.value->fchar(si.value, '%');
            if (p) {
                argnum = 0;
                char* head = p->head + 1;
                while (head && *head && str_isdigit(*head)) {
                    argnum = argnum * 10 + (*head - '0');
                    head++;
                }

                destroy_string(p);
            }

            lir_subject_t* subj = NULL;
            if (argnum >= 0) {
                subj = LIR_convert_hs_to_ls(params->data[params->top - argnum].d);
            }

            LIR_BLOCK2(ctx, LIR_RAW, LIR_SUBJ_RAWASM(h->farg->storage.str.s_id), subj);
            break;
        }

        case HIR_STASM: {
            list_iter_t it;
            list_iter_tinit(&h->targ->storage.list.h, &it);
            hir_subject_t* s;
            while ((s = list_iter_prev(&it))) stack_push(params, s);
            break;
        }

        case HIR_ENDASM: {
            for (int i = 0; i < h->targ->storage.cnst.value; i++) stack_pop(params, NULL);
            break;
        }

        case HIR_VRDEALL: LIR_BLOCK1(ctx, LIR_VRDEALL, LIR_SUBJ_CONST(h->farg->storage.cnst.value));                                 break;
        case HIR_STRDECL: LIR_BLOCK2(ctx, LIR_STRDECL, LIR_convert_hs_to_ls(h->farg), LIR_SUBJ_STRING(h->sarg->storage.str.s_id)); break;
        case HIR_ARRDECL: {
            lir_subject_t* lir_elems = LIR_SUBJ_LIST();
            foreach (hir_subject_t* hir_elem, &h->targ->storage.list.h) {
                list_add(&lir_elems->storage.list.h, LIR_convert_hs_to_ls(hir_elem));
            }

            LIR_BLOCK3(ctx, LIR_ARRDECL, LIR_convert_hs_to_ls(h->farg), NULL, lir_elems);
            break;
        }

        case HIR_REF:   _store_var2var(LIR_REF, ctx, h->farg, h->sarg);   break;
        case HIR_GDREF: _store_var2var(LIR_GDREF, ctx, h->farg, h->sarg); break;
        case HIR_LDREF: _store_var2var(LIR_LDREF, ctx, h->farg, h->sarg); break;
        
        case HIR_FEND:  LIR_BLOCK0(ctx, LIR_FEND);  break;
        case HIR_STEND: LIR_BLOCK0(ctx, LIR_STEND); break;

        case HIR_TF64: LIR_BLOCK2(ctx, LIR_TF64, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TF32: LIR_BLOCK2(ctx, LIR_TF32, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TI64: LIR_BLOCK2(ctx, LIR_TI64, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TI32: LIR_BLOCK2(ctx, LIR_TI32, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TI16: LIR_BLOCK2(ctx, LIR_TI16, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TI8:  LIR_BLOCK2(ctx, LIR_TI8,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TPTR:
        case HIR_TU64: LIR_BLOCK2(ctx, LIR_TU64, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TU32: LIR_BLOCK2(ctx, LIR_TU32, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TU16: LIR_BLOCK2(ctx, LIR_TU16, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_TU8:  LIR_BLOCK2(ctx, LIR_TU8,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;

        case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
        case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

        case HIR_NOT: LIR_BLOCK2(ctx, LIR_NOT, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg)); break;
        case HIR_IFOP2: {
            LIR_BLOCK2(ctx, LIR_CMP, LIR_convert_hs_to_ls(h->farg), LIR_SUBJ_CONST(0));
            LIR_BLOCK1(ctx, LIR_JE, LIR_SUBJ_LABEL(h->targ->id));
            LIR_BLOCK1(ctx, LIR_JNE, LIR_SUBJ_LABEL(h->sarg->id));
            break;
        }

        case HIR_iBLFT: LIR_BLOCK3(ctx, LIR_iBLFT, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iBRHT: LIR_BLOCK3(ctx, LIR_iBRHT, LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iLWR:  LIR_BLOCK3(ctx, LIR_iLWR,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iLRE:  LIR_BLOCK3(ctx, LIR_iLRE,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iLRG:  LIR_BLOCK3(ctx, LIR_iLRG,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iLGE:  LIR_BLOCK3(ctx, LIR_iLGE,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iCMP:  LIR_BLOCK3(ctx, LIR_iCMP,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iNMP:  LIR_BLOCK3(ctx, LIR_iNMP,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iOR:   LIR_BLOCK3(ctx, LIR_iOR,   LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iAND:  LIR_BLOCK3(ctx, LIR_iAND,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_bOR:   LIR_BLOCK3(ctx, LIR_bOR,   LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_bXOR:  LIR_BLOCK3(ctx, LIR_bXOR,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_bAND:  LIR_BLOCK3(ctx, LIR_bAND,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iMOD:  LIR_BLOCK3(ctx, LIR_iMOD,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iSUB:  LIR_BLOCK3(ctx, LIR_iSUB,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iDIV:  LIR_BLOCK3(ctx, LIR_iDIV,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iMUL:  LIR_BLOCK3(ctx, LIR_iMUL,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_iADD:  LIR_BLOCK3(ctx, LIR_iADD,  LIR_convert_hs_to_ls(h->farg), LIR_convert_hs_to_ls(h->sarg), LIR_convert_hs_to_ls(h->targ)); break;
        case HIR_VRUSE: LIR_BLOCK1(ctx, LIR_VRUSE, LIR_convert_hs_to_ls(h->farg));                                                                   break;

        default: break;
    }

    return 1;
}

static int _iterate_block(sstack_t* params, cfg_block_t* bb, lir_ctx_t* ctx, sym_table_t* smt) { 
    LIR_BLOCK1(ctx, LIR_BB, LIR_SUBJ_CONST(bb->id));
    bb->lmap.entry = ctx->t;

    hir_block_t* h = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
    while (h) {
        if (!h->unused) _convert_hir_to_lir(params, h, ctx, smt);
        h = HIR_get_next(h, bb->hmap.exit, 1);
    }

    if (!bb->lmap.entry) bb->lmap.entry = ctx->h;
    else bb->lmap.entry = bb->lmap.entry->next;
    bb->lmap.exit = ctx->t;
    return 1;
}

int LIR_generate_block(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    sstack_t params;
    stack_init(&params);

    /* Convert outer blocks to LIR context.
       Note: This blocks go to the highest part of the context. */
    foreach (hir_block_t* o, &cctx->out) {
        _convert_hir_to_lir(&params, o, ctx, smt);
    }

    /* Iterate on CFG blocks and convert all alligned HIR blocks
       to LIR blocks. */
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            _iterate_block(&params, cb, ctx, smt);
            if (!fb->lmap.entry) fb->lmap.entry = cb->lmap.entry;
        }

        fb->lmap.exit = ctx->t;
    }

    return 1;
}
