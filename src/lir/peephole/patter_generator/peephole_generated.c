
/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
static int _first_pass(cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        switch (lh->op) {
            case LIR_aMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->farg = lh->farg;
                    lh->sarg = lh->sarg;
                    lh->next->unused = 1;
                }
                break;
            }

            case LIR_fMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->farg = lh->farg;
                    lh->sarg = lh->sarg;
                    lh->next->unused = 1;
                }
                break;
            }

            case LIR_iADD: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL)) && !(peephole_get_long_number(lh->sarg) & 1)) {
                    lh->op = LIR_bSHR;
                    lh->farg = lh->farg;
                    lh->sarg = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(peephole_get_sqrt_number(lh->sarg));
                }
                break;
            }

            case LIR_iMOV: {
                if ((lh->farg && lh->farg->t == LIR_REGISTER && lh->sarg && lh->sarg->t == LIR_REGISTER) && lh->next && (lh->next->farg && lh->next->farg->t == LIR_REGISTER && lh->next->sarg && lh->next->sarg->t == LIR_REGISTER) && LIR_subj_equals(lh->next->farg, lh->sarg) && LIR_subj_equals(lh->next->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lh->farg = lh->farg;
                    lh->sarg = lh->sarg;
                    lh->next->unused = 1;
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