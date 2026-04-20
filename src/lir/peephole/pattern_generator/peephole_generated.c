/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>
int peephole_first_pass(cfg_block_t* bb) {
    int optimized = 0;
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        if (!lh->unused) switch (lh->op) {
            case LIR_CMP:
            case LIR_iCMP:
             {
                if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lir_subject_t* _src_1_90 = lh->farg;
                    lir_operation_t _match_op_0_91 = lh->op;
                    lir_subject_t* _keep_0_0_92 = lh->farg;
                    lir_subject_t* _keep_0_1_93 = lh->sarg;
                    lir_subject_t* _keep_0_2_94 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_95 = lh->farg;
                    if (_old_95 != _src_1_90) {
                        lh->farg = _src_1_90;
                        optimized = 1;
                        if (_old_95 && _old_95 != lh->farg && _old_95 != lh->sarg && _old_95 != lh->targ && _old_95 != _src_1_90 && _old_95 != _keep_0_0_92 && _old_95 != _keep_0_1_93 && _old_95 != _keep_0_2_94) {
                            LIR_unload_subject(_old_95);
                        }
                    }
                    lir_subject_t* _old_96 = lh->sarg;
                    if (_old_96 != _src_1_90) {
                        lh->sarg = _src_1_90;
                        optimized = 1;
                        if (_old_96 && _old_96 != lh->farg && _old_96 != lh->sarg && _old_96 != lh->targ && _old_96 != _src_1_90 && _old_96 != _keep_0_0_92 && _old_96 != _keep_0_1_93 && _old_96 != _keep_0_2_94) {
                            LIR_unload_subject(_old_96);
                        }
                    }
                    lir_subject_t* _old_97 = lh->targ;
                    if (_old_97) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_97 && _old_97 != lh->farg && _old_97 != lh->sarg && _old_97 != lh->targ && _old_97 != _src_1_90 && _old_97 != _keep_0_0_92 && _old_97 != _keep_0_1_93 && _old_97 != _keep_0_2_94) {
                            LIR_unload_subject(_old_97);
                        }
                    }
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
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_aMOV:
            case LIR_fMOV:
            case LIR_iMOV:
             {
                if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                LIR_subj_equals(lh->farg, lh->sarg)) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lir_subject_t* _src_1_1 = lh->farg;
                    lir_operation_t _match_op_0_2 = lh->op;
                    lir_subject_t* _keep_0_0_3 = lh->farg;
                    lir_subject_t* _keep_0_1_4 = lh->sarg;
                    lir_subject_t* _keep_0_2_5 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_6 = lh->sarg;
                    if (_old_6 != _src_1_1) {
                        lh->sarg = _src_1_1;
                        optimized = 1;
                        if (_old_6 && _old_6 != lh->farg && _old_6 != lh->sarg && _old_6 != lh->targ && _old_6 != _src_1_1 && _old_6 != _keep_0_0_3 && _old_6 != _keep_0_1_4 && _old_6 != _keep_0_2_5) {
                            LIR_unload_subject(_old_6);
                        }
                    }
                    lir_subject_t* _old_7 = lh->targ;
                    if (_old_7 != _src_1_1) {
                        lh->targ = _src_1_1;
                        optimized = 1;
                        if (_old_7 && _old_7 != lh->farg && _old_7 != lh->sarg && _old_7 != lh->targ && _old_7 != _src_1_1 && _old_7 != _keep_0_0_3 && _old_7 != _keep_0_1_4 && _old_7 != _keep_0_2_5) {
                            LIR_unload_subject(_old_7);
                        }
                    }
                    lir_subject_t* _old_8 = lh->farg;
                    if (_old_8 != _src_1_1) {
                        lh->farg = _src_1_1;
                        optimized = 1;
                        if (_old_8 && _old_8 != lh->farg && _old_8 != lh->sarg && _old_8 != lh->targ && _old_8 != _src_1_1 && _old_8 != _keep_0_0_3 && _old_8 != _keep_0_1_4 && _old_8 != _keep_0_2_5) {
                            LIR_unload_subject(_old_8);
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
                    lir_subject_t* _src_1_9 = lh->farg;
                    lir_subject_t* _src_2_10 = lh->sarg;
                    lir_operation_t _match_op_0_11 = lh->op;
                    lir_operation_t _match_op_1_12 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_13 = lh->farg;
                    lir_subject_t* _keep_0_1_14 = lh->sarg;
                    lir_subject_t* _keep_0_2_15 = lh->targ;
                    lir_subject_t* _keep_1_0_16 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_17 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_18 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_11) {
                        lh->op = _match_op_0_11;
                        optimized = 1;
                    }
                    lir_subject_t* _old_19 = lh->farg;
                    if (_old_19 != _src_1_9) {
                        lh->farg = _src_1_9;
                        optimized = 1;
                        if (_old_19 && _old_19 != lh->farg && _old_19 != lh->sarg && _old_19 != lh->targ && _old_19 != _src_1_9 && _old_19 != _src_2_10 && _old_19 != _keep_0_0_13 && _old_19 != _keep_0_1_14 && _old_19 != _keep_0_2_15 && _old_19 != _keep_1_0_16 && _old_19 != _keep_1_1_17 && _old_19 != _keep_1_2_18) {
                            LIR_unload_subject(_old_19);
                        }
                    }
                    lir_subject_t* _old_20 = lh->sarg;
                    if (_old_20 != _src_2_10) {
                        lh->sarg = _src_2_10;
                        optimized = 1;
                        if (_old_20 && _old_20 != lh->farg && _old_20 != lh->sarg && _old_20 != lh->targ && _old_20 != _src_1_9 && _old_20 != _src_2_10 && _old_20 != _keep_0_0_13 && _old_20 != _keep_0_1_14 && _old_20 != _keep_0_2_15 && _old_20 != _keep_1_0_16 && _old_20 != _keep_1_1_17 && _old_20 != _keep_1_2_18) {
                            LIR_unload_subject(_old_20);
                        }
                    }
                    lir_subject_t* _old_21 = lh->targ;
                    if (_old_21) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_21 && _old_21 != lh->farg && _old_21 != lh->sarg && _old_21 != lh->targ && _old_21 != _src_1_9 && _old_21 != _src_2_10 && _old_21 != _keep_0_0_13 && _old_21 != _keep_0_1_14 && _old_21 != _keep_0_2_15 && _old_21 != _keep_1_0_16 && _old_21 != _keep_1_1_17 && _old_21 != _keep_1_2_18) {
                            LIR_unload_subject(_old_21);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_22 = lh->farg;
                    lir_subject_t* _src_2_23 = lh->sarg;
                    lir_subject_t* _src_3_24 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_25 = lh->op;
                    lir_operation_t _match_op_1_26 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_27 = lh->farg;
                    lir_subject_t* _keep_0_1_28 = lh->sarg;
                    lir_subject_t* _keep_0_2_29 = lh->targ;
                    lir_subject_t* _keep_1_0_30 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_31 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_32 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_26) {
                        lh->op = _match_op_1_26;
                        optimized = 1;
                    }
                    lir_subject_t* _old_33 = lh->farg;
                    if (_old_33 != _src_2_23) {
                        lh->farg = _src_2_23;
                        optimized = 1;
                        if (_old_33 && _old_33 != lh->farg && _old_33 != lh->sarg && _old_33 != lh->targ && _old_33 != _src_1_22 && _old_33 != _src_2_23 && _old_33 != _src_3_24 && _old_33 != _keep_0_0_27 && _old_33 != _keep_0_1_28 && _old_33 != _keep_0_2_29 && _old_33 != _keep_1_0_30 && _old_33 != _keep_1_1_31 && _old_33 != _keep_1_2_32) {
                            LIR_unload_subject(_old_33);
                        }
                    }
                    lir_subject_t* _old_34 = lh->sarg;
                    if (_old_34 != _src_3_24) {
                        lh->sarg = _src_3_24;
                        optimized = 1;
                        if (_old_34 && _old_34 != lh->farg && _old_34 != lh->sarg && _old_34 != lh->targ && _old_34 != _src_1_22 && _old_34 != _src_2_23 && _old_34 != _src_3_24 && _old_34 != _keep_0_0_27 && _old_34 != _keep_0_1_28 && _old_34 != _keep_0_2_29 && _old_34 != _keep_1_0_30 && _old_34 != _keep_1_1_31 && _old_34 != _keep_1_2_32) {
                            LIR_unload_subject(_old_34);
                        }
                    }
                    lir_subject_t* _old_35 = lh->targ;
                    if (_old_35) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_35 && _old_35 != lh->farg && _old_35 != lh->sarg && _old_35 != lh->targ && _old_35 != _src_1_22 && _old_35 != _src_2_23 && _old_35 != _src_3_24 && _old_35 != _keep_0_0_27 && _old_35 != _keep_0_1_28 && _old_35 != _keep_0_2_29 && _old_35 != _keep_1_0_30 && _old_35 != _keep_1_1_31 && _old_35 != _keep_1_2_32) {
                            LIR_unload_subject(_old_35);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                ((LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_next(lh, bb->lmap.exit, 1)->sarg) == 0)) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_52 = lh->farg;
                    lir_operation_t _match_op_0_53 = lh->op;
                    lir_operation_t _match_op_1_54 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_55 = lh->farg;
                    lir_subject_t* _keep_0_1_56 = lh->sarg;
                    lir_subject_t* _keep_0_2_57 = lh->targ;
                    lir_subject_t* _keep_1_0_58 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_59 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_60 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_61 = lh->sarg;
                    if (_old_61 != _src_1_52) {
                        lh->sarg = _src_1_52;
                        optimized = 1;
                        if (_old_61 && _old_61 != lh->farg && _old_61 != lh->sarg && _old_61 != lh->targ && _old_61 != _src_1_52 && _old_61 != _keep_0_0_55 && _old_61 != _keep_0_1_56 && _old_61 != _keep_0_2_57 && _old_61 != _keep_1_0_58 && _old_61 != _keep_1_1_59 && _old_61 != _keep_1_2_60) {
                            LIR_unload_subject(_old_61);
                        }
                    }
                    lir_subject_t* _old_62 = lh->targ;
                    if (_old_62 != _src_1_52) {
                        lh->targ = _src_1_52;
                        optimized = 1;
                        if (_old_62 && _old_62 != lh->farg && _old_62 != lh->sarg && _old_62 != lh->targ && _old_62 != _src_1_52 && _old_62 != _keep_0_0_55 && _old_62 != _keep_0_1_56 && _old_62 != _keep_0_2_57 && _old_62 != _keep_1_0_58 && _old_62 != _keep_1_1_59 && _old_62 != _keep_1_2_60) {
                            LIR_unload_subject(_old_62);
                        }
                    }
                    lir_subject_t* _old_63 = lh->farg;
                    if (_old_63 != _src_1_52) {
                        lh->farg = _src_1_52;
                        optimized = 1;
                        if (_old_63 && _old_63 != lh->farg && _old_63 != lh->sarg && _old_63 != lh->targ && _old_63 != _src_1_52 && _old_63 != _keep_0_0_55 && _old_63 != _keep_0_1_56 && _old_63 != _keep_0_2_57 && _old_63 != _keep_1_0_58 && _old_63 != _keep_1_1_59 && _old_63 != _keep_1_2_60) {
                            LIR_unload_subject(_old_63);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, LIR_get_next(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_64 = lh->farg;
                    lir_operation_t _match_op_0_65 = lh->op;
                    lir_operation_t _match_op_1_66 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_67 = lh->farg;
                    lir_subject_t* _keep_0_1_68 = lh->sarg;
                    lir_subject_t* _keep_0_2_69 = lh->targ;
                    lir_subject_t* _keep_1_0_70 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_71 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_72 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_73 = lh->sarg;
                    if (_old_73 != _src_1_64) {
                        lh->sarg = _src_1_64;
                        optimized = 1;
                        if (_old_73 && _old_73 != lh->farg && _old_73 != lh->sarg && _old_73 != lh->targ && _old_73 != _src_1_64 && _old_73 != _keep_0_0_67 && _old_73 != _keep_0_1_68 && _old_73 != _keep_0_2_69 && _old_73 != _keep_1_0_70 && _old_73 != _keep_1_1_71 && _old_73 != _keep_1_2_72) {
                            LIR_unload_subject(_old_73);
                        }
                    }
                    lir_subject_t* _old_74 = lh->targ;
                    if (_old_74 != _src_1_64) {
                        lh->targ = _src_1_64;
                        optimized = 1;
                        if (_old_74 && _old_74 != lh->farg && _old_74 != lh->sarg && _old_74 != lh->targ && _old_74 != _src_1_64 && _old_74 != _keep_0_0_67 && _old_74 != _keep_0_1_68 && _old_74 != _keep_0_2_69 && _old_74 != _keep_1_0_70 && _old_74 != _keep_1_1_71 && _old_74 != _keep_1_2_72) {
                            LIR_unload_subject(_old_74);
                        }
                    }
                    lir_subject_t* _old_75 = lh->farg;
                    if (_old_75 != _src_1_64) {
                        lh->farg = _src_1_64;
                        optimized = 1;
                        if (_old_75 && _old_75 != lh->farg && _old_75 != lh->sarg && _old_75 != lh->targ && _old_75 != _src_1_64 && _old_75 != _keep_0_0_67 && _old_75 != _keep_0_1_68 && _old_75 != _keep_0_2_69 && _old_75 != _keep_1_0_70 && _old_75 != _keep_1_1_71 && _old_75 != _keep_1_2_72) {
                            LIR_unload_subject(_old_75);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_bAND:
             {
                if (lh->op == LIR_bAND &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ))) {
                    lir_subject_t* _src_1_106 = lh->farg;
                    lir_operation_t _match_op_0_107 = lh->op;
                    lir_subject_t* _keep_0_0_108 = lh->farg;
                    lir_subject_t* _keep_0_1_109 = lh->sarg;
                    lir_subject_t* _keep_0_2_110 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_111 = lh->farg;
                    if (_old_111 != _src_1_106) {
                        lh->farg = _src_1_106;
                        optimized = 1;
                        if (_old_111 && _old_111 != lh->farg && _old_111 != lh->sarg && _old_111 != lh->targ && _old_111 != _src_1_106 && _old_111 != _keep_0_0_108 && _old_111 != _keep_0_1_109 && _old_111 != _keep_0_2_110) {
                            LIR_unload_subject(_old_111);
                        }
                    }
                    lir_subject_t* _old_112 = lh->sarg;
                    if (_old_112 != _src_1_106) {
                        lh->sarg = _src_1_106;
                        optimized = 1;
                        if (_old_112 && _old_112 != lh->farg && _old_112 != lh->sarg && _old_112 != lh->targ && _old_112 != _src_1_106 && _old_112 != _keep_0_0_108 && _old_112 != _keep_0_1_109 && _old_112 != _keep_0_2_110) {
                            LIR_unload_subject(_old_112);
                        }
                    }
                    lir_subject_t* _old_113 = lh->targ;
                    if (_old_113) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_113 && _old_113 != lh->farg && _old_113 != lh->sarg && _old_113 != lh->targ && _old_113 != _src_1_106 && _old_113 != _keep_0_0_108 && _old_113 != _keep_0_1_109 && _old_113 != _keep_0_2_110) {
                            LIR_unload_subject(_old_113);
                        }
                    }
                }
                else if (lh->op == LIR_bAND &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_138 = lh->farg;
                    lir_operation_t _match_op_0_139 = lh->op;
                    lir_subject_t* _keep_0_0_140 = lh->farg;
                    lir_subject_t* _keep_0_1_141 = lh->sarg;
                    lir_subject_t* _keep_0_2_142 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_143 = lh->sarg;
                    if (_old_143 != _src_1_138) {
                        lh->sarg = _src_1_138;
                        optimized = 1;
                        if (_old_143 && _old_143 != lh->farg && _old_143 != lh->sarg && _old_143 != lh->targ && _old_143 != _src_1_138 && _old_143 != _keep_0_0_140 && _old_143 != _keep_0_1_141 && _old_143 != _keep_0_2_142) {
                            LIR_unload_subject(_old_143);
                        }
                    }
                    lir_subject_t* _old_144 = lh->targ;
                    if (_old_144 != _src_1_138) {
                        lh->targ = _src_1_138;
                        optimized = 1;
                        if (_old_144 && _old_144 != lh->farg && _old_144 != lh->sarg && _old_144 != lh->targ && _old_144 != _src_1_138 && _old_144 != _keep_0_0_140 && _old_144 != _keep_0_1_141 && _old_144 != _keep_0_2_142) {
                            LIR_unload_subject(_old_144);
                        }
                    }
                    lir_subject_t* _old_145 = lh->farg;
                    if (_old_145 != _src_1_138) {
                        lh->farg = _src_1_138;
                        optimized = 1;
                        if (_old_145 && _old_145 != lh->farg && _old_145 != lh->sarg && _old_145 != lh->targ && _old_145 != _src_1_138 && _old_145 != _keep_0_0_140 && _old_145 != _keep_0_1_141 && _old_145 != _keep_0_2_142) {
                            LIR_unload_subject(_old_145);
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
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ))) {
                    lir_subject_t* _src_1_98 = lh->farg;
                    lir_operation_t _match_op_0_99 = lh->op;
                    lir_subject_t* _keep_0_0_100 = lh->farg;
                    lir_subject_t* _keep_0_1_101 = lh->sarg;
                    lir_subject_t* _keep_0_2_102 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_103 = lh->farg;
                    if (_old_103 != _src_1_98) {
                        lh->farg = _src_1_98;
                        optimized = 1;
                        if (_old_103 && _old_103 != lh->farg && _old_103 != lh->sarg && _old_103 != lh->targ && _old_103 != _src_1_98 && _old_103 != _keep_0_0_100 && _old_103 != _keep_0_1_101 && _old_103 != _keep_0_2_102) {
                            LIR_unload_subject(_old_103);
                        }
                    }
                    lir_subject_t* _old_104 = lh->sarg;
                    if (_old_104 != _src_1_98) {
                        lh->sarg = _src_1_98;
                        optimized = 1;
                        if (_old_104 && _old_104 != lh->farg && _old_104 != lh->sarg && _old_104 != lh->targ && _old_104 != _src_1_98 && _old_104 != _keep_0_0_100 && _old_104 != _keep_0_1_101 && _old_104 != _keep_0_2_102) {
                            LIR_unload_subject(_old_104);
                        }
                    }
                    lir_subject_t* _old_105 = lh->targ;
                    if (_old_105) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_105 && _old_105 != lh->farg && _old_105 != lh->sarg && _old_105 != lh->targ && _old_105 != _src_1_98 && _old_105 != _keep_0_0_100 && _old_105 != _keep_0_1_101 && _old_105 != _keep_0_2_102) {
                            LIR_unload_subject(_old_105);
                        }
                    }
                }
                break;
            }

            case LIR_bSHL:
             {
                if (lh->op == LIR_bSHL &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_bSHR:
             {
                if (lh->op == LIR_bSHR &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
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
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_36 = lh->farg;
                    lir_subject_t* _src_2_37 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_38 = lh->op;
                    lir_operation_t _match_op_1_39 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_40 = lh->farg;
                    lir_subject_t* _keep_0_1_41 = lh->sarg;
                    lir_subject_t* _keep_0_2_42 = lh->targ;
                    lir_subject_t* _keep_1_0_43 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_44 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_45 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_46 = lh->sarg;
                    if (_old_46 != _src_1_36) {
                        lh->sarg = _src_1_36;
                        optimized = 1;
                        if (_old_46 && _old_46 != lh->farg && _old_46 != lh->sarg && _old_46 != lh->targ && _old_46 != _src_1_36 && _old_46 != _src_2_37 && _old_46 != _keep_0_0_40 && _old_46 != _keep_0_1_41 && _old_46 != _keep_0_2_42 && _old_46 != _keep_1_0_43 && _old_46 != _keep_1_1_44 && _old_46 != _keep_1_2_45) {
                            LIR_unload_subject(_old_46);
                        }
                    }
                    lir_subject_t* _old_47 = lh->targ;
                    if (_old_47 != _src_1_36) {
                        lh->targ = _src_1_36;
                        optimized = 1;
                        if (_old_47 && _old_47 != lh->farg && _old_47 != lh->sarg && _old_47 != lh->targ && _old_47 != _src_1_36 && _old_47 != _src_2_37 && _old_47 != _keep_0_0_40 && _old_47 != _keep_0_1_41 && _old_47 != _keep_0_2_42 && _old_47 != _keep_1_0_43 && _old_47 != _keep_1_1_44 && _old_47 != _keep_1_2_45) {
                            LIR_unload_subject(_old_47);
                        }
                    }
                    lir_subject_t* _old_48 = lh->farg;
                    if (_old_48 != _src_1_36) {
                        lh->farg = _src_1_36;
                        optimized = 1;
                        if (_old_48 && _old_48 != lh->farg && _old_48 != lh->sarg && _old_48 != lh->targ && _old_48 != _src_1_36 && _old_48 != _src_2_37 && _old_48 != _keep_0_0_40 && _old_48 != _keep_0_1_41 && _old_48 != _keep_0_2_42 && _old_48 != _keep_1_0_43 && _old_48 != _keep_1_1_44 && _old_48 != _keep_1_2_45) {
                            LIR_unload_subject(_old_48);
                        }
                    }
                    if (LIR_get_next(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_49 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_49 != _src_2_37) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->sarg = _src_2_37;
                        optimized = 1;
                        if (_old_49 && _old_49 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_49 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_49 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_49 != _src_1_36 && _old_49 != _src_2_37 && _old_49 != _keep_0_0_40 && _old_49 != _keep_0_1_41 && _old_49 != _keep_0_2_42 && _old_49 != _keep_1_0_43 && _old_49 != _keep_1_1_44 && _old_49 != _keep_1_2_45) {
                            LIR_unload_subject(_old_49);
                        }
                    }
                    lir_subject_t* _old_50 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (_old_50 != _src_2_37) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->targ = _src_2_37;
                        optimized = 1;
                        if (_old_50 && _old_50 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_50 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_50 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_50 != _src_1_36 && _old_50 != _src_2_37 && _old_50 != _keep_0_0_40 && _old_50 != _keep_0_1_41 && _old_50 != _keep_0_2_42 && _old_50 != _keep_1_0_43 && _old_50 != _keep_1_1_44 && _old_50 != _keep_1_2_45) {
                            LIR_unload_subject(_old_50);
                        }
                    }
                    lir_subject_t* _old_51 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    if (_old_51 != _src_2_37) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->farg = _src_2_37;
                        optimized = 1;
                        if (_old_51 && _old_51 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_51 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_51 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_51 != _src_1_36 && _old_51 != _src_2_37 && _old_51 != _keep_0_0_40 && _old_51 != _keep_0_1_41 && _old_51 != _keep_0_2_42 && _old_51 != _keep_1_0_43 && _old_51 != _keep_1_1_44 && _old_51 != _keep_1_2_45) {
                            LIR_unload_subject(_old_51);
                        }
                    }
                }
                else if (lh->op == LIR_bXOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                ((LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_next(lh, bb->lmap.exit, 1)->sarg) == 0)) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_114 = lh->farg;
                    lir_operation_t _match_op_0_115 = lh->op;
                    lir_operation_t _match_op_1_116 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_117 = lh->farg;
                    lir_subject_t* _keep_0_1_118 = lh->sarg;
                    lir_subject_t* _keep_0_2_119 = lh->targ;
                    lir_subject_t* _keep_1_0_120 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_121 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_122 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_123 = lh->sarg;
                    if (_old_123 != _src_1_114) {
                        lh->sarg = _src_1_114;
                        optimized = 1;
                        if (_old_123 && _old_123 != lh->farg && _old_123 != lh->sarg && _old_123 != lh->targ && _old_123 != _src_1_114 && _old_123 != _keep_0_0_117 && _old_123 != _keep_0_1_118 && _old_123 != _keep_0_2_119 && _old_123 != _keep_1_0_120 && _old_123 != _keep_1_1_121 && _old_123 != _keep_1_2_122) {
                            LIR_unload_subject(_old_123);
                        }
                    }
                    lir_subject_t* _old_124 = lh->targ;
                    if (_old_124 != _src_1_114) {
                        lh->targ = _src_1_114;
                        optimized = 1;
                        if (_old_124 && _old_124 != lh->farg && _old_124 != lh->sarg && _old_124 != lh->targ && _old_124 != _src_1_114 && _old_124 != _keep_0_0_117 && _old_124 != _keep_0_1_118 && _old_124 != _keep_0_2_119 && _old_124 != _keep_1_0_120 && _old_124 != _keep_1_1_121 && _old_124 != _keep_1_2_122) {
                            LIR_unload_subject(_old_124);
                        }
                    }
                    lir_subject_t* _old_125 = lh->farg;
                    if (_old_125 != _src_1_114) {
                        lh->farg = _src_1_114;
                        optimized = 1;
                        if (_old_125 && _old_125 != lh->farg && _old_125 != lh->sarg && _old_125 != lh->targ && _old_125 != _src_1_114 && _old_125 != _keep_0_0_117 && _old_125 != _keep_0_1_118 && _old_125 != _keep_0_2_119 && _old_125 != _keep_1_0_120 && _old_125 != _keep_1_1_121 && _old_125 != _keep_1_2_122) {
                            LIR_unload_subject(_old_125);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if (lh->op == LIR_bXOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, LIR_get_next(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_126 = lh->farg;
                    lir_operation_t _match_op_0_127 = lh->op;
                    lir_operation_t _match_op_1_128 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_129 = lh->farg;
                    lir_subject_t* _keep_0_1_130 = lh->sarg;
                    lir_subject_t* _keep_0_2_131 = lh->targ;
                    lir_subject_t* _keep_1_0_132 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_133 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_134 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_135 = lh->sarg;
                    if (_old_135 != _src_1_126) {
                        lh->sarg = _src_1_126;
                        optimized = 1;
                        if (_old_135 && _old_135 != lh->farg && _old_135 != lh->sarg && _old_135 != lh->targ && _old_135 != _src_1_126 && _old_135 != _keep_0_0_129 && _old_135 != _keep_0_1_130 && _old_135 != _keep_0_2_131 && _old_135 != _keep_1_0_132 && _old_135 != _keep_1_1_133 && _old_135 != _keep_1_2_134) {
                            LIR_unload_subject(_old_135);
                        }
                    }
                    lir_subject_t* _old_136 = lh->targ;
                    if (_old_136 != _src_1_126) {
                        lh->targ = _src_1_126;
                        optimized = 1;
                        if (_old_136 && _old_136 != lh->farg && _old_136 != lh->sarg && _old_136 != lh->targ && _old_136 != _src_1_126 && _old_136 != _keep_0_0_129 && _old_136 != _keep_0_1_130 && _old_136 != _keep_0_2_131 && _old_136 != _keep_1_0_132 && _old_136 != _keep_1_1_133 && _old_136 != _keep_1_2_134) {
                            LIR_unload_subject(_old_136);
                        }
                    }
                    lir_subject_t* _old_137 = lh->farg;
                    if (_old_137 != _src_1_126) {
                        lh->farg = _src_1_126;
                        optimized = 1;
                        if (_old_137 && _old_137 != lh->farg && _old_137 != lh->sarg && _old_137 != lh->targ && _old_137 != _src_1_126 && _old_137 != _keep_0_0_129 && _old_137 != _keep_0_1_130 && _old_137 != _keep_0_2_131 && _old_137 != _keep_1_0_132 && _old_137 != _keep_1_1_133 && _old_137 != _keep_1_2_134) {
                            LIR_unload_subject(_old_137);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if (lh->op == LIR_bXOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_bXOR &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->targ &&
                LIR_get_next(lh, bb->lmap.exit, 1)->targ->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, LIR_get_next(lh, bb->lmap.exit, 1)->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, LIR_get_next(lh, bb->lmap.exit, 1)->targ)) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_146 = lh->farg;
                    lir_operation_t _match_op_0_147 = lh->op;
                    lir_operation_t _match_op_1_148 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_149 = lh->farg;
                    lir_subject_t* _keep_0_1_150 = lh->sarg;
                    lir_subject_t* _keep_0_2_151 = lh->targ;
                    lir_subject_t* _keep_1_0_152 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_153 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_154 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_155 = lh->sarg;
                    if (_old_155 != _src_1_146) {
                        lh->sarg = _src_1_146;
                        optimized = 1;
                        if (_old_155 && _old_155 != lh->farg && _old_155 != lh->sarg && _old_155 != lh->targ && _old_155 != _src_1_146 && _old_155 != _keep_0_0_149 && _old_155 != _keep_0_1_150 && _old_155 != _keep_0_2_151 && _old_155 != _keep_1_0_152 && _old_155 != _keep_1_1_153 && _old_155 != _keep_1_2_154) {
                            LIR_unload_subject(_old_155);
                        }
                    }
                    lir_subject_t* _old_156 = lh->targ;
                    if (_old_156 != _src_1_146) {
                        lh->targ = _src_1_146;
                        optimized = 1;
                        if (_old_156 && _old_156 != lh->farg && _old_156 != lh->sarg && _old_156 != lh->targ && _old_156 != _src_1_146 && _old_156 != _keep_0_0_149 && _old_156 != _keep_0_1_150 && _old_156 != _keep_0_2_151 && _old_156 != _keep_1_0_152 && _old_156 != _keep_1_1_153 && _old_156 != _keep_1_2_154) {
                            LIR_unload_subject(_old_156);
                        }
                    }
                    lir_subject_t* _old_157 = lh->farg;
                    if (_old_157 != _src_1_146) {
                        lh->farg = _src_1_146;
                        optimized = 1;
                        if (_old_157 && _old_157 != lh->farg && _old_157 != lh->sarg && _old_157 != lh->targ && _old_157 != _src_1_146 && _old_157 != _keep_0_0_149 && _old_157 != _keep_0_1_150 && _old_157 != _keep_0_2_151 && _old_157 != _keep_1_0_152 && _old_157 != _keep_1_1_153 && _old_157 != _keep_1_2_154) {
                            LIR_unload_subject(_old_157);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_iADD:
             {
                if (lh->op == LIR_iADD &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_iDIV:
             {
                if (lh->op == LIR_iDIV &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_iMUL:
             {
                if (lh->op == LIR_iMUL &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_operation_t _match_op_0_84 = lh->op;
                    lir_subject_t* _keep_0_0_85 = lh->farg;
                    lir_subject_t* _keep_0_1_86 = lh->sarg;
                    lir_subject_t* _keep_0_2_87 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_88 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_88 && _old_88 != lh->farg && _old_88 != lh->sarg && _old_88 != lh->targ && _old_88 != _keep_0_0_85 && _old_88 != _keep_0_1_86 && _old_88 != _keep_0_2_87) {
                        LIR_unload_subject(_old_88);
                    }
                    lir_subject_t* _old_89 = lh->targ;
                    if (_old_89) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_89 && _old_89 != lh->farg && _old_89 != lh->sarg && _old_89 != lh->targ && _old_89 != _keep_0_0_85 && _old_89 != _keep_0_1_86 && _old_89 != _keep_0_2_87) {
                            LIR_unload_subject(_old_89);
                        }
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_iSUB:
             {
                if (lh->op == LIR_iSUB &&
                (lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ))) {
                    lir_subject_t* _src_1_76 = lh->farg;
                    lir_operation_t _match_op_0_77 = lh->op;
                    lir_subject_t* _keep_0_0_78 = lh->farg;
                    lir_subject_t* _keep_0_1_79 = lh->sarg;
                    lir_subject_t* _keep_0_2_80 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_81 = lh->sarg;
                    if (_old_81 != _src_1_76) {
                        lh->sarg = _src_1_76;
                        optimized = 1;
                        if (_old_81 && _old_81 != lh->farg && _old_81 != lh->sarg && _old_81 != lh->targ && _old_81 != _src_1_76 && _old_81 != _keep_0_0_78 && _old_81 != _keep_0_1_79 && _old_81 != _keep_0_2_80) {
                            LIR_unload_subject(_old_81);
                        }
                    }
                    lir_subject_t* _old_82 = lh->targ;
                    if (_old_82 != _src_1_76) {
                        lh->targ = _src_1_76;
                        optimized = 1;
                        if (_old_82 && _old_82 != lh->farg && _old_82 != lh->sarg && _old_82 != lh->targ && _old_82 != _src_1_76 && _old_82 != _keep_0_0_78 && _old_82 != _keep_0_1_79 && _old_82 != _keep_0_2_80) {
                            LIR_unload_subject(_old_82);
                        }
                    }
                    lir_subject_t* _old_83 = lh->farg;
                    if (_old_83 != _src_1_76) {
                        lh->farg = _src_1_76;
                        optimized = 1;
                        if (_old_83 && _old_83 != lh->farg && _old_83 != lh->sarg && _old_83 != lh->targ && _old_83 != _src_1_76 && _old_83 != _keep_0_0_78 && _old_83 != _keep_0_1_79 && _old_83 != _keep_0_2_80) {
                            LIR_unload_subject(_old_83);
                        }
                    }
                }
                break;
            }

            default: break;
        }
        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }
    return optimized;
}