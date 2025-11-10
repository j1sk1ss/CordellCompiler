#include <lir/peephole/x84_64_gnu_nasm.h>

static inline long _get_long_number(lir_subject_t* s) {
    if (s->t == LIR_CONSTVAL) return s->storage.cnst.value;
    else if (s->t == LIR_NUMBER) return str_atoi(s->storage.num.value);
    return 0;
}

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                switch (lh->op) {
                    case LIR_CVTTSS2SI:
                    case LIR_CVTTSD2SI:
                    case LIR_CVTSS2SD:
                    case LIR_CVTSD2SS: {
                        if (LIR_subj_equals(lh->farg, lh->sarg)) goto _delete_block;
                        if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) goto _delete_block;
                        break;
                    }

                    case LIR_MOVSXD:
                    case LIR_MOVSX:
                    case LIR_MOVZX:
                    case LIR_iMOV: {
                        if (LIR_subj_equals(lh->farg, lh->sarg)) goto _delete_block;
                        if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) goto _delete_block;
                        if (lh->farg->t != LIR_REGISTER) break;
                        if (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) {
                            if (!_get_long_number(lh->sarg)) {
                                lh->op = LIR_bXOR;
                                LIR_unload_subject(lh->sarg);
                                lh->sarg = lh->farg;
                                lh->targ = lh->farg;
                            }
                        }

                        break;
                    }

                    case LIR_CMP: {
                        if (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) {
                            if (lh->farg->t != LIR_REGISTER) break;
                            if (!_get_long_number(lh->sarg)) {
                                lh->op = LIR_TST;
                                LIR_unload_subject(lh->sarg);
                                lh->sarg = lh->farg;
                            }
                        }

                        break;
                    }

                    case LIR_iSUB: {
                        if (LIR_subj_equals(lh->sarg, lh->targ)) {
                            lh->op = LIR_bXOR;
                            LIR_unload_subject(lh->targ);
                            lh->targ = lh->sarg;
                            break;
                        }

                        if (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) {
                            if (!_get_long_number(lh->targ)) goto _delete_block;
                        }

                        break;
                    }

                    case LIR_iADD: {
                        if (LIR_subj_equals(lh->sarg, lh->targ)) {
                            lh->op = LIR_bSHL;
                            LIR_unload_subject(lh->targ);
                            lh->targ = LIR_SUBJ_CONST(1);
                            break;
                        }

                        if (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) {
                            if (!_get_long_number(lh->targ)) goto _delete_block;
                        }

                        break;
                    }

                    case LIR_iDIV:
                    case LIR_iMUL: {
                        if (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) {
                            int rval = _get_long_number(lh->targ);
                            if (rval == 1) goto _delete_block;
                            if (!(rval & (rval - 1))) {
                                int shift = 0;
                                while (rval >>= 1) {
                                    shift++;
                                }
                                
                                lh->op = (lh->op == LIR_iMUL) ? LIR_bSHL : LIR_bSHR;
                                LIR_unload_subject(lh->targ);
                                lh->targ = LIR_SUBJ_CONST(shift);
                            }
                        }
                        
                        break;
                    }
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
                continue;

_delete_block: {}
                lir_block_t* nlh = lh->next;
                LIR_unlink_block(lh);
                LIR_unlink_block(lh);
                lh = nlh;
            }
        }
    }

    return 1;
}
