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
                    lir_subject_t* _src_1_154 = lh->farg;
                    lir_operation_t _match_op_0_155 = lh->op;
                    lir_subject_t* _keep_0_0_156 = lh->farg;
                    lir_subject_t* _keep_0_1_157 = lh->sarg;
                    lir_subject_t* _keep_0_2_158 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_159 = lh->farg;
                    if (_old_159 != _src_1_154) {
                        lh->farg = _src_1_154;
                        optimized = 1;
                        if (_old_159 && _old_159 != lh->farg && _old_159 != lh->sarg && _old_159 != lh->targ && _old_159 != _src_1_154 && _old_159 != _keep_0_0_156 && _old_159 != _keep_0_1_157 && _old_159 != _keep_0_2_158) {
                            LIR_unload_subject(_old_159);
                        }
                    }
                    lir_subject_t* _old_160 = lh->sarg;
                    if (_old_160 != _src_1_154) {
                        lh->sarg = _src_1_154;
                        optimized = 1;
                        if (_old_160 && _old_160 != lh->farg && _old_160 != lh->sarg && _old_160 != lh->targ && _old_160 != _src_1_154 && _old_160 != _keep_0_0_156 && _old_160 != _keep_0_1_157 && _old_160 != _keep_0_2_158) {
                            LIR_unload_subject(_old_160);
                        }
                    }
                    lir_subject_t* _old_161 = lh->targ;
                    if (_old_161) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_161 && _old_161 != lh->farg && _old_161 != lh->sarg && _old_161 != lh->targ && _old_161 != _src_1_154 && _old_161 != _keep_0_0_156 && _old_161 != _keep_0_1_157 && _old_161 != _keep_0_2_158) {
                            LIR_unload_subject(_old_161);
                        }
                    }
                }
                break;
            }

            case LIR_JMP:
             {
                if (lh->op == LIR_JMP &&
                (lh->farg &&
                lh->farg->t == LIR_LABEL) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_MKLB &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_LABEL) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_1 = lh->farg;
                    lir_operation_t _match_op_0_2 = lh->op;
                    lir_operation_t _match_op_1_3 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_4 = lh->farg;
                    lir_subject_t* _keep_0_1_5 = lh->sarg;
                    lir_subject_t* _keep_0_2_6 = lh->targ;
                    lir_subject_t* _keep_1_0_7 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_8 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_9 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_MKLB) {
                        lh->op = LIR_MKLB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_10 = lh->farg;
                    if (_old_10 != _src_1_1) {
                        lh->farg = _src_1_1;
                        optimized = 1;
                        if (_old_10 && _old_10 != lh->farg && _old_10 != lh->sarg && _old_10 != lh->targ && _old_10 != _src_1_1 && _old_10 != _keep_0_0_4 && _old_10 != _keep_0_1_5 && _old_10 != _keep_0_2_6 && _old_10 != _keep_1_0_7 && _old_10 != _keep_1_1_8 && _old_10 != _keep_1_2_9) {
                            LIR_unload_subject(_old_10);
                        }
                    }
                    lir_subject_t* _old_11 = lh->sarg;
                    if (_old_11) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_11 && _old_11 != lh->farg && _old_11 != lh->sarg && _old_11 != lh->targ && _old_11 != _src_1_1 && _old_11 != _keep_0_0_4 && _old_11 != _keep_0_1_5 && _old_11 != _keep_0_2_6 && _old_11 != _keep_1_0_7 && _old_11 != _keep_1_1_8 && _old_11 != _keep_1_2_9) {
                            LIR_unload_subject(_old_11);
                        }
                    }
                    lir_subject_t* _old_12 = lh->targ;
                    if (_old_12) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_12 && _old_12 != lh->farg && _old_12 != lh->sarg && _old_12 != lh->targ && _old_12 != _src_1_1 && _old_12 != _keep_0_0_4 && _old_12 != _keep_0_1_5 && _old_12 != _keep_0_2_6 && _old_12 != _keep_1_0_7 && _old_12 != _keep_1_1_8 && _old_12 != _keep_1_2_9) {
                            LIR_unload_subject(_old_12);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_NOT:
             {
                if (lh->op == LIR_NOT &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_NOT &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
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
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iSUB &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_MEMORY) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 3) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_13 = lh->farg;
                    lir_subject_t* _src_2_14 = lh->sarg;
                    lir_operation_t _match_op_0_15 = lh->op;
                    lir_operation_t _match_op_1_16 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_17 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_operation_t _match_op_3_18 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op;
                    lir_subject_t* _keep_0_0_19 = lh->farg;
                    lir_subject_t* _keep_0_1_20 = lh->sarg;
                    lir_subject_t* _keep_0_2_21 = lh->targ;
                    lir_subject_t* _keep_1_0_22 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_23 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_24 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_25 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_26 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_27 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _keep_3_0_28 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_subject_t* _keep_3_1_29 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _keep_3_2_30 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_31 = lh->sarg;
                    if (_old_31 != _src_2_14) {
                        lh->sarg = _src_2_14;
                        optimized = 1;
                        if (_old_31 && _old_31 != lh->farg && _old_31 != lh->sarg && _old_31 != lh->targ && _old_31 != _src_1_13 && _old_31 != _src_2_14 && _old_31 != _keep_0_0_19 && _old_31 != _keep_0_1_20 && _old_31 != _keep_0_2_21 && _old_31 != _keep_1_0_22 && _old_31 != _keep_1_1_23 && _old_31 != _keep_1_2_24 && _old_31 != _keep_2_0_25 && _old_31 != _keep_2_1_26 && _old_31 != _keep_2_2_27 && _old_31 != _keep_3_0_28 && _old_31 != _keep_3_1_29 && _old_31 != _keep_3_2_30) {
                            LIR_unload_subject(_old_31);
                        }
                    }
                    lir_subject_t* _old_32 = lh->targ;
                    if (_old_32 != _src_2_14) {
                        lh->targ = _src_2_14;
                        optimized = 1;
                        if (_old_32 && _old_32 != lh->farg && _old_32 != lh->sarg && _old_32 != lh->targ && _old_32 != _src_1_13 && _old_32 != _src_2_14 && _old_32 != _keep_0_0_19 && _old_32 != _keep_0_1_20 && _old_32 != _keep_0_2_21 && _old_32 != _keep_1_0_22 && _old_32 != _keep_1_1_23 && _old_32 != _keep_1_2_24 && _old_32 != _keep_2_0_25 && _old_32 != _keep_2_1_26 && _old_32 != _keep_2_2_27 && _old_32 != _keep_3_0_28 && _old_32 != _keep_3_1_29 && _old_32 != _keep_3_2_30) {
                            LIR_unload_subject(_old_32);
                        }
                    }
                    lir_subject_t* _old_33 = lh->farg;
                    if (_old_33 != _src_2_14) {
                        lh->farg = _src_2_14;
                        optimized = 1;
                        if (_old_33 && _old_33 != lh->farg && _old_33 != lh->sarg && _old_33 != lh->targ && _old_33 != _src_1_13 && _old_33 != _src_2_14 && _old_33 != _keep_0_0_19 && _old_33 != _keep_0_1_20 && _old_33 != _keep_0_2_21 && _old_33 != _keep_1_0_22 && _old_33 != _keep_1_1_23 && _old_33 != _keep_1_2_24 && _old_33 != _keep_2_0_25 && _old_33 != _keep_2_1_26 && _old_33 != _keep_2_2_27 && _old_33 != _keep_3_0_28 && _old_33 != _keep_3_1_29 && _old_33 != _keep_3_2_30) {
                            LIR_unload_subject(_old_33);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_15) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_15;
                        optimized = 1;
                    }
                    lir_subject_t* _old_34 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_34 != _src_1_13) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_13;
                        optimized = 1;
                        if (_old_34 && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_34 != _src_1_13 && _old_34 != _src_2_14 && _old_34 != _keep_0_0_19 && _old_34 != _keep_0_1_20 && _old_34 != _keep_0_2_21 && _old_34 != _keep_1_0_22 && _old_34 != _keep_1_1_23 && _old_34 != _keep_1_2_24 && _old_34 != _keep_2_0_25 && _old_34 != _keep_2_1_26 && _old_34 != _keep_2_2_27 && _old_34 != _keep_3_0_28 && _old_34 != _keep_3_1_29 && _old_34 != _keep_3_2_30) {
                            LIR_unload_subject(_old_34);
                        }
                    }
                    lir_subject_t* _old_35 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_35 != _src_1_13) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_13;
                        optimized = 1;
                        if (_old_35 && _old_35 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_35 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_35 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_35 != _src_1_13 && _old_35 != _src_2_14 && _old_35 != _keep_0_0_19 && _old_35 != _keep_0_1_20 && _old_35 != _keep_0_2_21 && _old_35 != _keep_1_0_22 && _old_35 != _keep_1_1_23 && _old_35 != _keep_1_2_24 && _old_35 != _keep_2_0_25 && _old_35 != _keep_2_1_26 && _old_35 != _keep_2_2_27 && _old_35 != _keep_3_0_28 && _old_35 != _keep_3_1_29 && _old_35 != _keep_3_2_30) {
                            LIR_unload_subject(_old_35);
                        }
                    }
                    lir_subject_t* _old_36 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_36) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_36 && _old_36 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_36 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_36 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_36 != _src_1_13 && _old_36 != _src_2_14 && _old_36 != _keep_0_0_19 && _old_36 != _keep_0_1_20 && _old_36 != _keep_0_2_21 && _old_36 != _keep_1_0_22 && _old_36 != _keep_1_1_23 && _old_36 != _keep_1_2_24 && _old_36 != _keep_2_0_25 && _old_36 != _keep_2_1_26 && _old_36 != _keep_2_2_27 && _old_36 != _keep_3_0_28 && _old_36 != _keep_3_1_29 && _old_36 != _keep_3_2_30) {
                            LIR_unload_subject(_old_36);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
                        optimized = 1;
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 3)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 3)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
                lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY)) &&
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
                    lir_subject_t* _src_1_37 = lh->farg;
                    lir_operation_t _match_op_0_38 = lh->op;
                    lir_subject_t* _keep_0_0_39 = lh->farg;
                    lir_subject_t* _keep_0_1_40 = lh->sarg;
                    lir_subject_t* _keep_0_2_41 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_42 = lh->sarg;
                    if (_old_42 != _src_1_37) {
                        lh->sarg = _src_1_37;
                        optimized = 1;
                        if (_old_42 && _old_42 != lh->farg && _old_42 != lh->sarg && _old_42 != lh->targ && _old_42 != _src_1_37 && _old_42 != _keep_0_0_39 && _old_42 != _keep_0_1_40 && _old_42 != _keep_0_2_41) {
                            LIR_unload_subject(_old_42);
                        }
                    }
                    lir_subject_t* _old_43 = lh->targ;
                    if (_old_43 != _src_1_37) {
                        lh->targ = _src_1_37;
                        optimized = 1;
                        if (_old_43 && _old_43 != lh->farg && _old_43 != lh->sarg && _old_43 != lh->targ && _old_43 != _src_1_37 && _old_43 != _keep_0_0_39 && _old_43 != _keep_0_1_40 && _old_43 != _keep_0_2_41) {
                            LIR_unload_subject(_old_43);
                        }
                    }
                    lir_subject_t* _old_44 = lh->farg;
                    if (_old_44 != _src_1_37) {
                        lh->farg = _src_1_37;
                        optimized = 1;
                        if (_old_44 && _old_44 != lh->farg && _old_44 != lh->sarg && _old_44 != lh->targ && _old_44 != _src_1_37 && _old_44 != _keep_0_0_39 && _old_44 != _keep_0_1_40 && _old_44 != _keep_0_2_41) {
                            LIR_unload_subject(_old_44);
                        }
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_MEMORY) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_45 = lh->farg;
                    lir_subject_t* _src_2_46 = lh->sarg;
                    lir_operation_t _match_op_0_47 = lh->op;
                    lir_operation_t _match_op_1_48 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_49 = lh->farg;
                    lir_subject_t* _keep_0_1_50 = lh->sarg;
                    lir_subject_t* _keep_0_2_51 = lh->targ;
                    lir_subject_t* _keep_1_0_52 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_53 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_54 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_47) {
                        lh->op = _match_op_0_47;
                        optimized = 1;
                    }
                    lir_subject_t* _old_55 = lh->farg;
                    if (_old_55 != _src_1_45) {
                        lh->farg = _src_1_45;
                        optimized = 1;
                        if (_old_55 && _old_55 != lh->farg && _old_55 != lh->sarg && _old_55 != lh->targ && _old_55 != _src_1_45 && _old_55 != _src_2_46 && _old_55 != _keep_0_0_49 && _old_55 != _keep_0_1_50 && _old_55 != _keep_0_2_51 && _old_55 != _keep_1_0_52 && _old_55 != _keep_1_1_53 && _old_55 != _keep_1_2_54) {
                            LIR_unload_subject(_old_55);
                        }
                    }
                    lir_subject_t* _old_56 = lh->sarg;
                    if (_old_56 != _src_2_46) {
                        lh->sarg = _src_2_46;
                        optimized = 1;
                        if (_old_56 && _old_56 != lh->farg && _old_56 != lh->sarg && _old_56 != lh->targ && _old_56 != _src_1_45 && _old_56 != _src_2_46 && _old_56 != _keep_0_0_49 && _old_56 != _keep_0_1_50 && _old_56 != _keep_0_2_51 && _old_56 != _keep_1_0_52 && _old_56 != _keep_1_1_53 && _old_56 != _keep_1_2_54) {
                            LIR_unload_subject(_old_56);
                        }
                    }
                    lir_subject_t* _old_57 = lh->targ;
                    if (_old_57) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_57 && _old_57 != lh->farg && _old_57 != lh->sarg && _old_57 != lh->targ && _old_57 != _src_1_45 && _old_57 != _src_2_46 && _old_57 != _keep_0_0_49 && _old_57 != _keep_0_1_50 && _old_57 != _keep_0_2_51 && _old_57 != _keep_1_0_52 && _old_57 != _keep_1_1_53 && _old_57 != _keep_1_2_54) {
                            LIR_unload_subject(_old_57);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_MEMORY)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_58 = lh->farg;
                    lir_subject_t* _src_2_59 = lh->sarg;
                    lir_subject_t* _src_3_60 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_61 = lh->op;
                    lir_operation_t _match_op_1_62 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_63 = lh->farg;
                    lir_subject_t* _keep_0_1_64 = lh->sarg;
                    lir_subject_t* _keep_0_2_65 = lh->targ;
                    lir_subject_t* _keep_1_0_66 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_67 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_68 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_62) {
                        lh->op = _match_op_1_62;
                        optimized = 1;
                    }
                    lir_subject_t* _old_69 = lh->farg;
                    if (_old_69 != _src_2_59) {
                        lh->farg = _src_2_59;
                        optimized = 1;
                        if (_old_69 && _old_69 != lh->farg && _old_69 != lh->sarg && _old_69 != lh->targ && _old_69 != _src_1_58 && _old_69 != _src_2_59 && _old_69 != _src_3_60 && _old_69 != _keep_0_0_63 && _old_69 != _keep_0_1_64 && _old_69 != _keep_0_2_65 && _old_69 != _keep_1_0_66 && _old_69 != _keep_1_1_67 && _old_69 != _keep_1_2_68) {
                            LIR_unload_subject(_old_69);
                        }
                    }
                    lir_subject_t* _old_70 = lh->sarg;
                    if (_old_70 != _src_3_60) {
                        lh->sarg = _src_3_60;
                        optimized = 1;
                        if (_old_70 && _old_70 != lh->farg && _old_70 != lh->sarg && _old_70 != lh->targ && _old_70 != _src_1_58 && _old_70 != _src_2_59 && _old_70 != _src_3_60 && _old_70 != _keep_0_0_63 && _old_70 != _keep_0_1_64 && _old_70 != _keep_0_2_65 && _old_70 != _keep_1_0_66 && _old_70 != _keep_1_1_67 && _old_70 != _keep_1_2_68) {
                            LIR_unload_subject(_old_70);
                        }
                    }
                    lir_subject_t* _old_71 = lh->targ;
                    if (_old_71) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_71 && _old_71 != lh->farg && _old_71 != lh->sarg && _old_71 != lh->targ && _old_71 != _src_1_58 && _old_71 != _src_2_59 && _old_71 != _src_3_60 && _old_71 != _keep_0_0_63 && _old_71 != _keep_0_1_64 && _old_71 != _keep_0_2_65 && _old_71 != _keep_1_0_66 && _old_71 != _keep_1_1_67 && _old_71 != _keep_1_2_68) {
                            LIR_unload_subject(_old_71);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                ((LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg) == 0)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_88 = lh->farg;
                    lir_operation_t _match_op_0_89 = lh->op;
                    lir_operation_t _match_op_1_90 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_91 = lh->farg;
                    lir_subject_t* _keep_0_1_92 = lh->sarg;
                    lir_subject_t* _keep_0_2_93 = lh->targ;
                    lir_subject_t* _keep_1_0_94 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_95 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_96 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_97 = lh->sarg;
                    if (_old_97 != _src_1_88) {
                        lh->sarg = _src_1_88;
                        optimized = 1;
                        if (_old_97 && _old_97 != lh->farg && _old_97 != lh->sarg && _old_97 != lh->targ && _old_97 != _src_1_88 && _old_97 != _keep_0_0_91 && _old_97 != _keep_0_1_92 && _old_97 != _keep_0_2_93 && _old_97 != _keep_1_0_94 && _old_97 != _keep_1_1_95 && _old_97 != _keep_1_2_96) {
                            LIR_unload_subject(_old_97);
                        }
                    }
                    lir_subject_t* _old_98 = lh->targ;
                    if (_old_98 != _src_1_88) {
                        lh->targ = _src_1_88;
                        optimized = 1;
                        if (_old_98 && _old_98 != lh->farg && _old_98 != lh->sarg && _old_98 != lh->targ && _old_98 != _src_1_88 && _old_98 != _keep_0_0_91 && _old_98 != _keep_0_1_92 && _old_98 != _keep_0_2_93 && _old_98 != _keep_1_0_94 && _old_98 != _keep_1_1_95 && _old_98 != _keep_1_2_96) {
                            LIR_unload_subject(_old_98);
                        }
                    }
                    lir_subject_t* _old_99 = lh->farg;
                    if (_old_99 != _src_1_88) {
                        lh->farg = _src_1_88;
                        optimized = 1;
                        if (_old_99 && _old_99 != lh->farg && _old_99 != lh->sarg && _old_99 != lh->targ && _old_99 != _src_1_88 && _old_99 != _keep_0_0_91 && _old_99 != _keep_0_1_92 && _old_99 != _keep_0_2_93 && _old_99 != _keep_1_0_94 && _old_99 != _keep_1_1_95 && _old_99 != _keep_1_2_96) {
                            LIR_unload_subject(_old_99);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_100 = lh->farg;
                    lir_operation_t _match_op_0_101 = lh->op;
                    lir_operation_t _match_op_1_102 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_103 = lh->farg;
                    lir_subject_t* _keep_0_1_104 = lh->sarg;
                    lir_subject_t* _keep_0_2_105 = lh->targ;
                    lir_subject_t* _keep_1_0_106 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_107 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_108 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_109 = lh->sarg;
                    if (_old_109 != _src_1_100) {
                        lh->sarg = _src_1_100;
                        optimized = 1;
                        if (_old_109 && _old_109 != lh->farg && _old_109 != lh->sarg && _old_109 != lh->targ && _old_109 != _src_1_100 && _old_109 != _keep_0_0_103 && _old_109 != _keep_0_1_104 && _old_109 != _keep_0_2_105 && _old_109 != _keep_1_0_106 && _old_109 != _keep_1_1_107 && _old_109 != _keep_1_2_108) {
                            LIR_unload_subject(_old_109);
                        }
                    }
                    lir_subject_t* _old_110 = lh->targ;
                    if (_old_110 != _src_1_100) {
                        lh->targ = _src_1_100;
                        optimized = 1;
                        if (_old_110 && _old_110 != lh->farg && _old_110 != lh->sarg && _old_110 != lh->targ && _old_110 != _src_1_100 && _old_110 != _keep_0_0_103 && _old_110 != _keep_0_1_104 && _old_110 != _keep_0_2_105 && _old_110 != _keep_1_0_106 && _old_110 != _keep_1_1_107 && _old_110 != _keep_1_2_108) {
                            LIR_unload_subject(_old_110);
                        }
                    }
                    lir_subject_t* _old_111 = lh->farg;
                    if (_old_111 != _src_1_100) {
                        lh->farg = _src_1_100;
                        optimized = 1;
                        if (_old_111 && _old_111 != lh->farg && _old_111 != lh->sarg && _old_111 != lh->targ && _old_111 != _src_1_100 && _old_111 != _keep_0_0_103 && _old_111 != _keep_0_1_104 && _old_111 != _keep_0_2_105 && _old_111 != _keep_1_0_106 && _old_111 != _keep_1_1_107 && _old_111 != _keep_1_2_108) {
                            LIR_unload_subject(_old_111);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_DEC &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_2_120 = lh->farg;
                    lir_subject_t* _src_1_121 = lh->sarg;
                    lir_operation_t _match_op_0_122 = lh->op;
                    lir_operation_t _match_op_1_123 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_124 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_125 = lh->farg;
                    lir_subject_t* _keep_0_1_126 = lh->sarg;
                    lir_subject_t* _keep_0_2_127 = lh->targ;
                    lir_subject_t* _keep_1_0_128 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_129 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_130 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_131 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_132 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_133 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_134 = lh->farg;
                    if (_old_134 != _src_1_121) {
                        lh->farg = _src_1_121;
                        optimized = 1;
                        if (_old_134 && _old_134 != lh->farg && _old_134 != lh->sarg && _old_134 != lh->targ && _old_134 != _src_2_120 && _old_134 != _src_1_121 && _old_134 != _keep_0_0_125 && _old_134 != _keep_0_1_126 && _old_134 != _keep_0_2_127 && _old_134 != _keep_1_0_128 && _old_134 != _keep_1_1_129 && _old_134 != _keep_1_2_130 && _old_134 != _keep_2_0_131 && _old_134 != _keep_2_1_132 && _old_134 != _keep_2_2_133) {
                            LIR_unload_subject(_old_134);
                        }
                    }
                    lir_subject_t* _old_135 = lh->sarg;
                    if (_old_135) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_135 && _old_135 != lh->farg && _old_135 != lh->sarg && _old_135 != lh->targ && _old_135 != _src_2_120 && _old_135 != _src_1_121 && _old_135 != _keep_0_0_125 && _old_135 != _keep_0_1_126 && _old_135 != _keep_0_2_127 && _old_135 != _keep_1_0_128 && _old_135 != _keep_1_1_129 && _old_135 != _keep_1_2_130 && _old_135 != _keep_2_0_131 && _old_135 != _keep_2_1_132 && _old_135 != _keep_2_2_133) {
                            LIR_unload_subject(_old_135);
                        }
                    }
                    lir_subject_t* _old_136 = lh->targ;
                    if (_old_136) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_136 && _old_136 != lh->farg && _old_136 != lh->sarg && _old_136 != lh->targ && _old_136 != _src_2_120 && _old_136 != _src_1_121 && _old_136 != _keep_0_0_125 && _old_136 != _keep_0_1_126 && _old_136 != _keep_0_2_127 && _old_136 != _keep_1_0_128 && _old_136 != _keep_1_1_129 && _old_136 != _keep_1_2_130 && _old_136 != _keep_2_0_131 && _old_136 != _keep_2_1_132 && _old_136 != _keep_2_2_133) {
                            LIR_unload_subject(_old_136);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_122) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_122;
                        optimized = 1;
                    }
                    lir_subject_t* _old_137 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_137 != _src_2_120) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_120;
                        optimized = 1;
                        if (_old_137 && _old_137 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_137 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_137 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_137 != _src_2_120 && _old_137 != _src_1_121 && _old_137 != _keep_0_0_125 && _old_137 != _keep_0_1_126 && _old_137 != _keep_0_2_127 && _old_137 != _keep_1_0_128 && _old_137 != _keep_1_1_129 && _old_137 != _keep_1_2_130 && _old_137 != _keep_2_0_131 && _old_137 != _keep_2_1_132 && _old_137 != _keep_2_2_133) {
                            LIR_unload_subject(_old_137);
                        }
                    }
                    lir_subject_t* _old_138 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_138 != _src_2_120) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_120;
                        optimized = 1;
                        if (_old_138 && _old_138 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_138 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_138 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_138 != _src_2_120 && _old_138 != _src_1_121 && _old_138 != _keep_0_0_125 && _old_138 != _keep_0_1_126 && _old_138 != _keep_0_2_127 && _old_138 != _keep_1_0_128 && _old_138 != _keep_1_1_129 && _old_138 != _keep_1_2_130 && _old_138 != _keep_2_0_131 && _old_138 != _keep_2_1_132 && _old_138 != _keep_2_2_133) {
                            LIR_unload_subject(_old_138);
                        }
                    }
                    lir_subject_t* _old_139 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_139) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_139 && _old_139 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_139 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_139 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_139 != _src_2_120 && _old_139 != _src_1_121 && _old_139 != _keep_0_0_125 && _old_139 != _keep_0_1_126 && _old_139 != _keep_0_2_127 && _old_139 != _keep_1_0_128 && _old_139 != _keep_1_1_129 && _old_139 != _keep_1_2_130 && _old_139 != _keep_2_0_131 && _old_139 != _keep_2_1_132 && _old_139 != _keep_2_2_133) {
                            LIR_unload_subject(_old_139);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
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
                    lir_subject_t* _src_1_170 = lh->farg;
                    lir_operation_t _match_op_0_171 = lh->op;
                    lir_subject_t* _keep_0_0_172 = lh->farg;
                    lir_subject_t* _keep_0_1_173 = lh->sarg;
                    lir_subject_t* _keep_0_2_174 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_175 = lh->farg;
                    if (_old_175 != _src_1_170) {
                        lh->farg = _src_1_170;
                        optimized = 1;
                        if (_old_175 && _old_175 != lh->farg && _old_175 != lh->sarg && _old_175 != lh->targ && _old_175 != _src_1_170 && _old_175 != _keep_0_0_172 && _old_175 != _keep_0_1_173 && _old_175 != _keep_0_2_174) {
                            LIR_unload_subject(_old_175);
                        }
                    }
                    lir_subject_t* _old_176 = lh->sarg;
                    if (_old_176 != _src_1_170) {
                        lh->sarg = _src_1_170;
                        optimized = 1;
                        if (_old_176 && _old_176 != lh->farg && _old_176 != lh->sarg && _old_176 != lh->targ && _old_176 != _src_1_170 && _old_176 != _keep_0_0_172 && _old_176 != _keep_0_1_173 && _old_176 != _keep_0_2_174) {
                            LIR_unload_subject(_old_176);
                        }
                    }
                    lir_subject_t* _old_177 = lh->targ;
                    if (_old_177) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_177 && _old_177 != lh->farg && _old_177 != lh->sarg && _old_177 != lh->targ && _old_177 != _src_1_170 && _old_177 != _keep_0_0_172 && _old_177 != _keep_0_1_173 && _old_177 != _keep_0_2_174) {
                            LIR_unload_subject(_old_177);
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
                    lir_subject_t* _src_1_202 = lh->farg;
                    lir_operation_t _match_op_0_203 = lh->op;
                    lir_subject_t* _keep_0_0_204 = lh->farg;
                    lir_subject_t* _keep_0_1_205 = lh->sarg;
                    lir_subject_t* _keep_0_2_206 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_207 = lh->sarg;
                    if (_old_207 != _src_1_202) {
                        lh->sarg = _src_1_202;
                        optimized = 1;
                        if (_old_207 && _old_207 != lh->farg && _old_207 != lh->sarg && _old_207 != lh->targ && _old_207 != _src_1_202 && _old_207 != _keep_0_0_204 && _old_207 != _keep_0_1_205 && _old_207 != _keep_0_2_206) {
                            LIR_unload_subject(_old_207);
                        }
                    }
                    lir_subject_t* _old_208 = lh->targ;
                    if (_old_208 != _src_1_202) {
                        lh->targ = _src_1_202;
                        optimized = 1;
                        if (_old_208 && _old_208 != lh->farg && _old_208 != lh->sarg && _old_208 != lh->targ && _old_208 != _src_1_202 && _old_208 != _keep_0_0_204 && _old_208 != _keep_0_1_205 && _old_208 != _keep_0_2_206) {
                            LIR_unload_subject(_old_208);
                        }
                    }
                    lir_subject_t* _old_209 = lh->farg;
                    if (_old_209 != _src_1_202) {
                        lh->farg = _src_1_202;
                        optimized = 1;
                        if (_old_209 && _old_209 != lh->farg && _old_209 != lh->sarg && _old_209 != lh->targ && _old_209 != _src_1_202 && _old_209 != _keep_0_0_204 && _old_209 != _keep_0_1_205 && _old_209 != _keep_0_2_206) {
                            LIR_unload_subject(_old_209);
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
                    lir_subject_t* _src_1_162 = lh->farg;
                    lir_operation_t _match_op_0_163 = lh->op;
                    lir_subject_t* _keep_0_0_164 = lh->farg;
                    lir_subject_t* _keep_0_1_165 = lh->sarg;
                    lir_subject_t* _keep_0_2_166 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_167 = lh->farg;
                    if (_old_167 != _src_1_162) {
                        lh->farg = _src_1_162;
                        optimized = 1;
                        if (_old_167 && _old_167 != lh->farg && _old_167 != lh->sarg && _old_167 != lh->targ && _old_167 != _src_1_162 && _old_167 != _keep_0_0_164 && _old_167 != _keep_0_1_165 && _old_167 != _keep_0_2_166) {
                            LIR_unload_subject(_old_167);
                        }
                    }
                    lir_subject_t* _old_168 = lh->sarg;
                    if (_old_168 != _src_1_162) {
                        lh->sarg = _src_1_162;
                        optimized = 1;
                        if (_old_168 && _old_168 != lh->farg && _old_168 != lh->sarg && _old_168 != lh->targ && _old_168 != _src_1_162 && _old_168 != _keep_0_0_164 && _old_168 != _keep_0_1_165 && _old_168 != _keep_0_2_166) {
                            LIR_unload_subject(_old_168);
                        }
                    }
                    lir_subject_t* _old_169 = lh->targ;
                    if (_old_169) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_169 && _old_169 != lh->farg && _old_169 != lh->sarg && _old_169 != lh->targ && _old_169 != _src_1_162 && _old_169 != _keep_0_0_164 && _old_169 != _keep_0_1_165 && _old_169 != _keep_0_2_166) {
                            LIR_unload_subject(_old_169);
                        }
                    }
                }
                break;
            }

            case LIR_bSHL:
             {
                if (lh->op == LIR_bSHL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_72 = lh->farg;
                    lir_subject_t* _src_2_73 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_74 = lh->op;
                    lir_operation_t _match_op_1_75 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_76 = lh->farg;
                    lir_subject_t* _keep_0_1_77 = lh->sarg;
                    lir_subject_t* _keep_0_2_78 = lh->targ;
                    lir_subject_t* _keep_1_0_79 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_80 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_81 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_82 = lh->sarg;
                    if (_old_82 != _src_1_72) {
                        lh->sarg = _src_1_72;
                        optimized = 1;
                        if (_old_82 && _old_82 != lh->farg && _old_82 != lh->sarg && _old_82 != lh->targ && _old_82 != _src_1_72 && _old_82 != _src_2_73 && _old_82 != _keep_0_0_76 && _old_82 != _keep_0_1_77 && _old_82 != _keep_0_2_78 && _old_82 != _keep_1_0_79 && _old_82 != _keep_1_1_80 && _old_82 != _keep_1_2_81) {
                            LIR_unload_subject(_old_82);
                        }
                    }
                    lir_subject_t* _old_83 = lh->targ;
                    if (_old_83 != _src_1_72) {
                        lh->targ = _src_1_72;
                        optimized = 1;
                        if (_old_83 && _old_83 != lh->farg && _old_83 != lh->sarg && _old_83 != lh->targ && _old_83 != _src_1_72 && _old_83 != _src_2_73 && _old_83 != _keep_0_0_76 && _old_83 != _keep_0_1_77 && _old_83 != _keep_0_2_78 && _old_83 != _keep_1_0_79 && _old_83 != _keep_1_1_80 && _old_83 != _keep_1_2_81) {
                            LIR_unload_subject(_old_83);
                        }
                    }
                    lir_subject_t* _old_84 = lh->farg;
                    if (_old_84 != _src_1_72) {
                        lh->farg = _src_1_72;
                        optimized = 1;
                        if (_old_84 && _old_84 != lh->farg && _old_84 != lh->sarg && _old_84 != lh->targ && _old_84 != _src_1_72 && _old_84 != _src_2_73 && _old_84 != _keep_0_0_76 && _old_84 != _keep_0_1_77 && _old_84 != _keep_0_2_78 && _old_84 != _keep_1_0_79 && _old_84 != _keep_1_1_80 && _old_84 != _keep_1_2_81) {
                            LIR_unload_subject(_old_84);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_85 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_85 != _src_2_73) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_73;
                        optimized = 1;
                        if (_old_85 && _old_85 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_85 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_85 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_85 != _src_1_72 && _old_85 != _src_2_73 && _old_85 != _keep_0_0_76 && _old_85 != _keep_0_1_77 && _old_85 != _keep_0_2_78 && _old_85 != _keep_1_0_79 && _old_85 != _keep_1_1_80 && _old_85 != _keep_1_2_81) {
                            LIR_unload_subject(_old_85);
                        }
                    }
                    lir_subject_t* _old_86 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_86 != _src_2_73) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_2_73;
                        optimized = 1;
                        if (_old_86 && _old_86 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_86 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_86 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_86 != _src_1_72 && _old_86 != _src_2_73 && _old_86 != _keep_0_0_76 && _old_86 != _keep_0_1_77 && _old_86 != _keep_0_2_78 && _old_86 != _keep_1_0_79 && _old_86 != _keep_1_1_80 && _old_86 != _keep_1_2_81) {
                            LIR_unload_subject(_old_86);
                        }
                    }
                    lir_subject_t* _old_87 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_87 != _src_2_73) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_73;
                        optimized = 1;
                        if (_old_87 && _old_87 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_87 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_87 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_87 != _src_1_72 && _old_87 != _src_2_73 && _old_87 != _keep_0_0_76 && _old_87 != _keep_0_1_77 && _old_87 != _keep_0_2_78 && _old_87 != _keep_1_0_79 && _old_87 != _keep_1_1_80 && _old_87 != _keep_1_2_81) {
                            LIR_unload_subject(_old_87);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                ((LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg) == 0)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_178 = lh->farg;
                    lir_operation_t _match_op_0_179 = lh->op;
                    lir_operation_t _match_op_1_180 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_181 = lh->farg;
                    lir_subject_t* _keep_0_1_182 = lh->sarg;
                    lir_subject_t* _keep_0_2_183 = lh->targ;
                    lir_subject_t* _keep_1_0_184 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_185 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_186 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_187 = lh->sarg;
                    if (_old_187 != _src_1_178) {
                        lh->sarg = _src_1_178;
                        optimized = 1;
                        if (_old_187 && _old_187 != lh->farg && _old_187 != lh->sarg && _old_187 != lh->targ && _old_187 != _src_1_178 && _old_187 != _keep_0_0_181 && _old_187 != _keep_0_1_182 && _old_187 != _keep_0_2_183 && _old_187 != _keep_1_0_184 && _old_187 != _keep_1_1_185 && _old_187 != _keep_1_2_186) {
                            LIR_unload_subject(_old_187);
                        }
                    }
                    lir_subject_t* _old_188 = lh->targ;
                    if (_old_188 != _src_1_178) {
                        lh->targ = _src_1_178;
                        optimized = 1;
                        if (_old_188 && _old_188 != lh->farg && _old_188 != lh->sarg && _old_188 != lh->targ && _old_188 != _src_1_178 && _old_188 != _keep_0_0_181 && _old_188 != _keep_0_1_182 && _old_188 != _keep_0_2_183 && _old_188 != _keep_1_0_184 && _old_188 != _keep_1_1_185 && _old_188 != _keep_1_2_186) {
                            LIR_unload_subject(_old_188);
                        }
                    }
                    lir_subject_t* _old_189 = lh->farg;
                    if (_old_189 != _src_1_178) {
                        lh->farg = _src_1_178;
                        optimized = 1;
                        if (_old_189 && _old_189 != lh->farg && _old_189 != lh->sarg && _old_189 != lh->targ && _old_189 != _src_1_178 && _old_189 != _keep_0_0_181 && _old_189 != _keep_0_1_182 && _old_189 != _keep_0_2_183 && _old_189 != _keep_1_0_184 && _old_189 != _keep_1_1_185 && _old_189 != _keep_1_2_186) {
                            LIR_unload_subject(_old_189);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_190 = lh->farg;
                    lir_operation_t _match_op_0_191 = lh->op;
                    lir_operation_t _match_op_1_192 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_193 = lh->farg;
                    lir_subject_t* _keep_0_1_194 = lh->sarg;
                    lir_subject_t* _keep_0_2_195 = lh->targ;
                    lir_subject_t* _keep_1_0_196 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_197 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_198 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_199 = lh->sarg;
                    if (_old_199 != _src_1_190) {
                        lh->sarg = _src_1_190;
                        optimized = 1;
                        if (_old_199 && _old_199 != lh->farg && _old_199 != lh->sarg && _old_199 != lh->targ && _old_199 != _src_1_190 && _old_199 != _keep_0_0_193 && _old_199 != _keep_0_1_194 && _old_199 != _keep_0_2_195 && _old_199 != _keep_1_0_196 && _old_199 != _keep_1_1_197 && _old_199 != _keep_1_2_198) {
                            LIR_unload_subject(_old_199);
                        }
                    }
                    lir_subject_t* _old_200 = lh->targ;
                    if (_old_200 != _src_1_190) {
                        lh->targ = _src_1_190;
                        optimized = 1;
                        if (_old_200 && _old_200 != lh->farg && _old_200 != lh->sarg && _old_200 != lh->targ && _old_200 != _src_1_190 && _old_200 != _keep_0_0_193 && _old_200 != _keep_0_1_194 && _old_200 != _keep_0_2_195 && _old_200 != _keep_1_0_196 && _old_200 != _keep_1_1_197 && _old_200 != _keep_1_2_198) {
                            LIR_unload_subject(_old_200);
                        }
                    }
                    lir_subject_t* _old_201 = lh->farg;
                    if (_old_201 != _src_1_190) {
                        lh->farg = _src_1_190;
                        optimized = 1;
                        if (_old_201 && _old_201 != lh->farg && _old_201 != lh->sarg && _old_201 != lh->targ && _old_201 != _src_1_190 && _old_201 != _keep_0_0_193 && _old_201 != _keep_0_1_194 && _old_201 != _keep_0_2_195 && _old_201 != _keep_1_0_196 && _old_201 != _keep_1_1_197 && _old_201 != _keep_1_2_198) {
                            LIR_unload_subject(_old_201);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_bXOR &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_210 = lh->farg;
                    lir_operation_t _match_op_0_211 = lh->op;
                    lir_operation_t _match_op_1_212 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_213 = lh->farg;
                    lir_subject_t* _keep_0_1_214 = lh->sarg;
                    lir_subject_t* _keep_0_2_215 = lh->targ;
                    lir_subject_t* _keep_1_0_216 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_217 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_218 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_219 = lh->sarg;
                    if (_old_219 != _src_1_210) {
                        lh->sarg = _src_1_210;
                        optimized = 1;
                        if (_old_219 && _old_219 != lh->farg && _old_219 != lh->sarg && _old_219 != lh->targ && _old_219 != _src_1_210 && _old_219 != _keep_0_0_213 && _old_219 != _keep_0_1_214 && _old_219 != _keep_0_2_215 && _old_219 != _keep_1_0_216 && _old_219 != _keep_1_1_217 && _old_219 != _keep_1_2_218) {
                            LIR_unload_subject(_old_219);
                        }
                    }
                    lir_subject_t* _old_220 = lh->targ;
                    if (_old_220 != _src_1_210) {
                        lh->targ = _src_1_210;
                        optimized = 1;
                        if (_old_220 && _old_220 != lh->farg && _old_220 != lh->sarg && _old_220 != lh->targ && _old_220 != _src_1_210 && _old_220 != _keep_0_0_213 && _old_220 != _keep_0_1_214 && _old_220 != _keep_0_2_215 && _old_220 != _keep_1_0_216 && _old_220 != _keep_1_1_217 && _old_220 != _keep_1_2_218) {
                            LIR_unload_subject(_old_220);
                        }
                    }
                    lir_subject_t* _old_221 = lh->farg;
                    if (_old_221 != _src_1_210) {
                        lh->farg = _src_1_210;
                        optimized = 1;
                        if (_old_221 && _old_221 != lh->farg && _old_221 != lh->sarg && _old_221 != lh->targ && _old_221 != _src_1_210 && _old_221 != _keep_0_0_213 && _old_221 != _keep_0_1_214 && _old_221 != _keep_0_2_215 && _old_221 != _keep_1_0_216 && _old_221 != _keep_1_1_217 && _old_221 != _keep_1_2_218) {
                            LIR_unload_subject(_old_221);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_iADD:
             {
                if (lh->op == LIR_iADD &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_operation_t _match_op_0_148 = lh->op;
                    lir_subject_t* _keep_0_0_149 = lh->farg;
                    lir_subject_t* _keep_0_1_150 = lh->sarg;
                    lir_subject_t* _keep_0_2_151 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_152 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_152 && _old_152 != lh->farg && _old_152 != lh->sarg && _old_152 != lh->targ && _old_152 != _keep_0_0_149 && _old_152 != _keep_0_1_150 && _old_152 != _keep_0_2_151) {
                        LIR_unload_subject(_old_152);
                    }
                    lir_subject_t* _old_153 = lh->targ;
                    if (_old_153) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_153 && _old_153 != lh->farg && _old_153 != lh->sarg && _old_153 != lh->targ && _old_153 != _keep_0_0_149 && _old_153 != _keep_0_1_150 && _old_153 != _keep_0_2_151) {
                            LIR_unload_subject(_old_153);
                        }
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_112 = lh->farg;
                    lir_operation_t _match_op_0_113 = lh->op;
                    lir_subject_t* _keep_0_0_114 = lh->farg;
                    lir_subject_t* _keep_0_1_115 = lh->sarg;
                    lir_subject_t* _keep_0_2_116 = lh->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_117 = lh->farg;
                    if (_old_117 != _src_1_112) {
                        lh->farg = _src_1_112;
                        optimized = 1;
                        if (_old_117 && _old_117 != lh->farg && _old_117 != lh->sarg && _old_117 != lh->targ && _old_117 != _src_1_112 && _old_117 != _keep_0_0_114 && _old_117 != _keep_0_1_115 && _old_117 != _keep_0_2_116) {
                            LIR_unload_subject(_old_117);
                        }
                    }
                    lir_subject_t* _old_118 = lh->sarg;
                    if (_old_118) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_118 && _old_118 != lh->farg && _old_118 != lh->sarg && _old_118 != lh->targ && _old_118 != _src_1_112 && _old_118 != _keep_0_0_114 && _old_118 != _keep_0_1_115 && _old_118 != _keep_0_2_116) {
                            LIR_unload_subject(_old_118);
                        }
                    }
                    lir_subject_t* _old_119 = lh->targ;
                    if (_old_119) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_119 && _old_119 != lh->farg && _old_119 != lh->sarg && _old_119 != lh->targ && _old_119 != _src_1_112 && _old_119 != _keep_0_0_114 && _old_119 != _keep_0_1_115 && _old_119 != _keep_0_2_116) {
                            LIR_unload_subject(_old_119);
                        }
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY) &&
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
                    lir_subject_t* _src_1_140 = lh->farg;
                    lir_operation_t _match_op_0_141 = lh->op;
                    lir_subject_t* _keep_0_0_142 = lh->farg;
                    lir_subject_t* _keep_0_1_143 = lh->sarg;
                    lir_subject_t* _keep_0_2_144 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_145 = lh->sarg;
                    if (_old_145 != _src_1_140) {
                        lh->sarg = _src_1_140;
                        optimized = 1;
                        if (_old_145 && _old_145 != lh->farg && _old_145 != lh->sarg && _old_145 != lh->targ && _old_145 != _src_1_140 && _old_145 != _keep_0_0_142 && _old_145 != _keep_0_1_143 && _old_145 != _keep_0_2_144) {
                            LIR_unload_subject(_old_145);
                        }
                    }
                    lir_subject_t* _old_146 = lh->targ;
                    if (_old_146 != _src_1_140) {
                        lh->targ = _src_1_140;
                        optimized = 1;
                        if (_old_146 && _old_146 != lh->farg && _old_146 != lh->sarg && _old_146 != lh->targ && _old_146 != _src_1_140 && _old_146 != _keep_0_0_142 && _old_146 != _keep_0_1_143 && _old_146 != _keep_0_2_144) {
                            LIR_unload_subject(_old_146);
                        }
                    }
                    lir_subject_t* _old_147 = lh->farg;
                    if (_old_147 != _src_1_140) {
                        lh->farg = _src_1_140;
                        optimized = 1;
                        if (_old_147 && _old_147 != lh->farg && _old_147 != lh->sarg && _old_147 != lh->targ && _old_147 != _src_1_140 && _old_147 != _keep_0_0_142 && _old_147 != _keep_0_1_143 && _old_147 != _keep_0_2_144) {
                            LIR_unload_subject(_old_147);
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