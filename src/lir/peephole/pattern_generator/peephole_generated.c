/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
int peephole_first_pass(cfg_block_t* bb) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        switch (lh->op) {
            case LIR_CMP:
            case LIR_iCMP:
             {
                if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_TST;
                    lir_subject_t* _src_1_34 = lh->farg;
                    lir_subject_t* _old_35 = lh->farg;
                    if (_old_35 != _src_1_34) {
                        lh->farg = _src_1_34;
                        if (_old_35 && _old_35 != lh->farg && _old_35 != lh->sarg && _old_35 != lh->targ) {
                            LIR_unload_subject(_old_35);
                        }
                    }
                    lir_subject_t* _old_36 = lh->sarg;
                    if (_old_36 != _src_1_34) {
                        lh->sarg = _src_1_34;
                        if (_old_36 && _old_36 != lh->farg && _old_36 != lh->sarg && _old_36 != lh->targ) {
                            LIR_unload_subject(_old_36);
                        }
                    }
                }
                else if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSD2SS:
            case LIR_CVTSS2SD:
            case LIR_CVTTSD2SI:
            case LIR_CVTTSS2SI:
             {
                if ((lh->op == LIR_CVTSS2SD || lh->op == LIR_CVTSD2SS || lh->op == LIR_CVTTSS2SI || lh->op == LIR_CVTTSD2SI) &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSI2SD:
             {
                if (lh->op == LIR_CVTSI2SD &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_CVTSI2SS:
             {
                if (lh->op == LIR_CVTSI2SS &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVSX:
             {
                if (lh->op == LIR_MOVSX &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVSXD:
             {
                if (lh->op == LIR_MOVSXD &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_MOVZX:
             {
                if (lh->op == LIR_MOVZX &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_NOT:
             {
                if (lh->op == LIR_NOT &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_NOT &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lh->unused = 1;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                break;
            }

            case LIR_REF:
             {
                if (lh->op == LIR_REF &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_TST:
             {
                if (lh->op == LIR_TST &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_aMOV:
            case LIR_fMOV:
            case LIR_iMOV:
             {
                if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                LIR_subj_equals(lh->farg, lh->sarg)) {
                    lh->unused = 1;
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lh->op = LIR_bXOR;
                    lir_subject_t* _src_1_1 = lh->farg;
                    lir_subject_t* _old_2 = lh->sarg;
                    if (_old_2 != _src_1_1) {
                        lh->sarg = _src_1_1;
                        if (_old_2 && _old_2 != lh->farg && _old_2 != lh->sarg && _old_2 != lh->targ) {
                            LIR_unload_subject(_old_2);
                        }
                    }
                    lir_subject_t* _old_3 = lh->targ;
                    if (_old_3 != _src_1_1) {
                        lh->targ = _src_1_1;
                        if (_old_3 && _old_3 != lh->farg && _old_3 != lh->sarg && _old_3 != lh->targ) {
                            LIR_unload_subject(_old_3);
                        }
                    }
                    lir_subject_t* _old_4 = lh->farg;
                    if (_old_4 != _src_1_1) {
                        lh->farg = _src_1_1;
                        if (_old_4 && _old_4 != lh->farg && _old_4 != lh->sarg && _old_4 != lh->targ) {
                            LIR_unload_subject(_old_4);
                        }
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lh->op = LIR_iMOV;
                    lir_subject_t* _src_1_5 = lh->farg;
                    lir_subject_t* _src_2_6 = lh->sarg;
                    lir_subject_t* _old_7 = lh->farg;
                    if (_old_7 != _src_1_5) {
                        lh->farg = _src_1_5;
                        if (_old_7 && _old_7 != lh->farg && _old_7 != lh->sarg && _old_7 != lh->targ) {
                            LIR_unload_subject(_old_7);
                        }
                    }
                    lir_subject_t* _old_8 = lh->sarg;
                    if (_old_8 != _src_2_6) {
                        lh->sarg = _src_2_6;
                        if (_old_8 && _old_8 != lh->farg && _old_8 != lh->sarg && _old_8 != lh->targ) {
                            LIR_unload_subject(_old_8);
                        }
                    }
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_bAND:
             {
                if (lh->op == LIR_bAND &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_bXOR;
                    lir_subject_t* _src_1_37 = lh->farg;
                    lir_subject_t* _old_38 = lh->sarg;
                    if (_old_38 != _src_1_37) {
                        lh->sarg = _src_1_37;
                        if (_old_38 && _old_38 != lh->farg && _old_38 != lh->sarg && _old_38 != lh->targ) {
                            LIR_unload_subject(_old_38);
                        }
                    }
                    lir_subject_t* _old_39 = lh->targ;
                    if (_old_39 != _src_1_37) {
                        lh->targ = _src_1_37;
                        if (_old_39 && _old_39 != lh->farg && _old_39 != lh->sarg && _old_39 != lh->targ) {
                            LIR_unload_subject(_old_39);
                        }
                    }
                    lir_subject_t* _old_40 = lh->farg;
                    if (_old_40 != _src_1_37) {
                        lh->farg = _src_1_37;
                        if (_old_40 && _old_40 != lh->farg && _old_40 != lh->sarg && _old_40 != lh->targ) {
                            LIR_unload_subject(_old_40);
                        }
                    }
                }
                break;
            }

            case LIR_bOR:
             {
                if (lh->op == LIR_bOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_bXOR:
             {
                if (lh->op == LIR_bXOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lh->op = LIR_bXOR;
                    lir_subject_t* _src_1_9 = lh->farg;
                    LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                }
                break;
            }

            case LIR_iADD:
             {
                if (lh->op == LIR_iADD &&
                (lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                else if (lh->op == LIR_iADD &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_INC;
                }
                else if (lh->op == LIR_iADD &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_subj_equals(lh->farg, lh->targ)) {
                    lh->op = LIR_bSHL;
                    lir_subject_t* _src_1_25 = lh->farg;
                    lir_subject_t* _src_2_26 = lh->targ;
                    lir_subject_t* _old_27 = lh->sarg;
                    if (_old_27 != _src_1_25) {
                        lh->sarg = _src_1_25;
                        if (_old_27 && _old_27 != lh->farg && _old_27 != lh->sarg && _old_27 != lh->targ) {
                            LIR_unload_subject(_old_27);
                        }
                    }
                    lir_subject_t* _old_28 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    if (_old_28 && _old_28 != lh->farg && _old_28 != lh->sarg && _old_28 != lh->targ) {
                        LIR_unload_subject(_old_28);
                    }
                    lir_subject_t* _old_29 = lh->farg;
                    if (_old_29 != _src_1_25) {
                        lh->farg = _src_1_25;
                        if (_old_29 && _old_29 != lh->farg && _old_29 != lh->sarg && _old_29 != lh->targ) {
                            LIR_unload_subject(_old_29);
                        }
                    }
                }
                else if (lh->op == LIR_iADD &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iDIV:
             {
                if (lh->op == LIR_iDIV &&
                (lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                else if (lh->op == LIR_iDIV &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_peephole_get_long_number(lh->targ) != 1 && !(
                	LIR_peephole_get_long_number(lh->targ) & (LIR_peephole_get_long_number(lh->targ) - 1)
                )) {
                    lh->op = LIR_bSHR;
                    lir_subject_t* _src_1_10 = lh->farg;
                    lir_subject_t* _src_2_11 = lh->targ;
                    lir_subject_t* _old_12 = lh->sarg;
                    if (_old_12 != _src_1_10) {
                        lh->sarg = _src_1_10;
                        if (_old_12 && _old_12 != lh->farg && _old_12 != lh->sarg && _old_12 != lh->targ) {
                            LIR_unload_subject(_old_12);
                        }
                    }
                    lir_subject_t* _old_13 = lh->targ;
                    if (_old_13 != _src_2_11) {
                        lh->targ = _src_2_11;
                        if (_old_13 && _old_13 != lh->farg && _old_13 != lh->sarg && _old_13 != lh->targ) {
                            LIR_unload_subject(_old_13);
                        }
                    }
                    lir_subject_t* _old_14 = lh->farg;
                    if (_old_14 != _src_1_10) {
                        lh->farg = _src_1_10;
                        if (_old_14 && _old_14 != lh->farg && _old_14 != lh->sarg && _old_14 != lh->targ) {
                            LIR_unload_subject(_old_14);
                        }
                    }
                    lir_subject_t* _old_15 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(_old_15));
                    if (_old_15 && _old_15 != lh->farg && _old_15 != lh->sarg && _old_15 != lh->targ) {
                        LIR_unload_subject(_old_15);
                    }
                }
                else if (lh->op == LIR_iDIV &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iMUL:
             {
                if (lh->op == LIR_iMUL &&
                (lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_iMOV;
                    lir_subject_t* _old_16 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    if (_old_16 && _old_16 != lh->farg && _old_16 != lh->sarg && _old_16 != lh->targ) {
                        LIR_unload_subject(_old_16);
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_peephole_get_long_number(lh->targ) != 1 && !(
                	LIR_peephole_get_long_number(lh->targ) & (LIR_peephole_get_long_number(lh->targ) - 1)
                )) {
                    lh->op = LIR_bSHL;
                    lir_subject_t* _src_1_17 = lh->farg;
                    lir_subject_t* _src_2_18 = lh->targ;
                    lir_subject_t* _old_19 = lh->sarg;
                    if (_old_19 != _src_1_17) {
                        lh->sarg = _src_1_17;
                        if (_old_19 && _old_19 != lh->farg && _old_19 != lh->sarg && _old_19 != lh->targ) {
                            LIR_unload_subject(_old_19);
                        }
                    }
                    lir_subject_t* _old_20 = lh->targ;
                    if (_old_20 != _src_2_18) {
                        lh->targ = _src_2_18;
                        if (_old_20 && _old_20 != lh->farg && _old_20 != lh->sarg && _old_20 != lh->targ) {
                            LIR_unload_subject(_old_20);
                        }
                    }
                    lir_subject_t* _old_21 = lh->farg;
                    if (_old_21 != _src_1_17) {
                        lh->farg = _src_1_17;
                        if (_old_21 && _old_21 != lh->farg && _old_21 != lh->sarg && _old_21 != lh->targ) {
                            LIR_unload_subject(_old_21);
                        }
                    }
                    lir_subject_t* _old_22 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(_old_22));
                    if (_old_22 && _old_22 != lh->farg && _old_22 != lh->sarg && _old_22 != lh->targ) {
                        LIR_unload_subject(_old_22);
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == -1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_NEG;
                    lir_subject_t* _src_1_23 = lh->farg;
                    lir_subject_t* _old_24 = lh->farg;
                    if (_old_24 != _src_1_23) {
                        lh->farg = _src_1_23;
                        if (_old_24 && _old_24 != lh->farg && _old_24 != lh->sarg && _old_24 != lh->targ) {
                            LIR_unload_subject(_old_24);
                        }
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                break;
            }

            case LIR_iSUB:
             {
                if (lh->op == LIR_iSUB &&
                (lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->unused = 1;
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                (lh->targ->t == LIR_NUMBER && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_DEC;
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lh->op = LIR_bXOR;
                    lir_subject_t* _src_1_30 = lh->farg;
                    lir_subject_t* _old_31 = lh->sarg;
                    if (_old_31 != _src_1_30) {
                        lh->sarg = _src_1_30;
                        if (_old_31 && _old_31 != lh->farg && _old_31 != lh->sarg && _old_31 != lh->targ) {
                            LIR_unload_subject(_old_31);
                        }
                    }
                    lir_subject_t* _old_32 = lh->targ;
                    if (_old_32 != _src_1_30) {
                        lh->targ = _src_1_30;
                        if (_old_32 && _old_32 != lh->farg && _old_32 != lh->sarg && _old_32 != lh->targ) {
                            LIR_unload_subject(_old_32);
                        }
                    }
                    lir_subject_t* _old_33 = lh->farg;
                    if (_old_33 != _src_1_30) {
                        lh->farg = _src_1_30;
                        if (_old_33 && _old_33 != lh->farg && _old_33 != lh->sarg && _old_33 != lh->targ) {
                            LIR_unload_subject(_old_33);
                        }
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
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