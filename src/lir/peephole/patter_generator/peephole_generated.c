
/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
int peephole_first_pass(cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        switch (lh->op) {
            case LIR_aMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->next->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_fMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->next->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iADD: {
                if ((lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL)) && !(peephole_get_long_number(lh->farg) & 1)) {
                    lh->op = LIR_bSHR;
                    LIR_unload_subject(lh->farg);
                    lh->farg = LIR_SUBJ_CONST(peephole_get_sqrt_number(lh->sarg));
                }
                break;
            }

            case LIR_iMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = lh->farg;
                }
                else if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->next->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iMUL: {
                if ((lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_iMOV;
                    LIR_unload_subject(lh->sarg);
                    lh->sarg = LIR_SUBJ_CONST(0);
                }
                else if ((lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 1))) {
                    lh->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iSUB: {
                if ((lh->sarg && (lh->sarg->t == LIR_NUMBER && peephole_get_long_number(lh->sarg) == 0))) {
                    lh->unused = 1;
                }
                else if ((lh->farg && (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            default: break;
        }

        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    return 1;
}