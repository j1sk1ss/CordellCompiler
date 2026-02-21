
/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
int peephole_first_pass(cfg_block_t* bb) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        switch (lh->op) {
            case LIR_CMP: {
                if ((lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_TST;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                }
                else if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSD2SS: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSI2SD: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSI2SS: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSS2SD: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTTSD2SI: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTTSS2SI: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVSX: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVSXD: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVZX: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_NOT: {
                if ((lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lh->unused = 1;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                break;
            }

            case LIR_REF: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_TST: {
                if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_aMOV: {
                if (LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                    LIR_unload_subject(lh->targ);
                    lh->targ = lh->farg;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                else if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_bAND: {
                if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->targ);
                    lh->targ = lh->sarg;
                }
                break;
            }

            case LIR_bOR: {
                if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_fMOV: {
                if (LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                    LIR_unload_subject(lh->targ);
                    lh->targ = lh->farg;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                else if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iADD: {
                if ((lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0))) {
                    lh->unused = 1;
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL)) &&
                LIR_peephole_get_long_number(lh->targ) != 1 && !(
                	LIR_peephole_get_long_number(lh->targ) & (LIR_peephole_get_long_number(lh->targ) - 1)
                )) {
                    lh->op = LIR_bSHR;
                    LIR_unload_subject(lh->targ);
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(lh->targ));
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1))) {
                    lh->op = LIR_INC;
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER) &&
                LIR_subj_equals(lh->sarg, lh->targ)) {
                    lh->op = LIR_bSHL;
                    LIR_unload_subject(lh->targ);
                    lh->targ = LIR_SUBJ_CONST(1);
                }
                else if ((lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iCMP: {
                if ((lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_TST;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                }
                else if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iDIV: {
                if ((lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iMOV: {
                if (LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                    LIR_unload_subject(lh->targ);
                    lh->targ = lh->farg;
                }
                else if ((lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                else if ((lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iMUL: {
                if ((lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0))) {
                    lh->op = LIR_iMOV;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = LIR_SUBJ_CONST(0);
                }
                else if ((lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1))) {
                    lh->unused = 1;
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL)) &&
                LIR_peephole_get_long_number(lh->targ) != 1 && !(
                	LIR_peephole_get_long_number(lh->targ) & (LIR_peephole_get_long_number(lh->targ) - 1)
                )) {
                    lh->op = LIR_bSHL;
                    LIR_unload_subject(lh->targ);
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(lh->targ));
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == -1))) {
                    lh->op = LIR_NEG;
                    LIR_unload_subject(lh->farg);
                    lh->farg = lh->sarg;
                }
                else if ((lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iSUB: {
                if ((lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0))) {
                    lh->unused = 1;
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1))) {
                    lh->op = LIR_DEC;
                }
                else if ((lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER)) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->targ;
                }
                else if ((lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            default: break;
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}