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
                    lir_subject_t* _src_1_231 = lh->farg;
                    lir_operation_t _match_op_0_232 = lh->op;
                    lir_subject_t* _keep_0_0_233 = lh->farg;
                    lir_subject_t* _keep_0_1_234 = lh->sarg;
                    lir_subject_t* _keep_0_2_235 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_236 = lh->farg;
                    if (_old_236 != _src_1_231) {
                        lh->farg = _src_1_231;
                        optimized = 1;
                        if (_old_236 && _old_236 != lh->farg && _old_236 != lh->sarg && _old_236 != lh->targ && _old_236 != _src_1_231 && _old_236 != _keep_0_0_233 && _old_236 != _keep_0_1_234 && _old_236 != _keep_0_2_235) {
                            LIR_unload_subject(_old_236);
                        }
                    }
                    lir_subject_t* _old_237 = lh->sarg;
                    if (_old_237 != _src_1_231) {
                        lh->sarg = _src_1_231;
                        optimized = 1;
                        if (_old_237 && _old_237 != lh->farg && _old_237 != lh->sarg && _old_237 != lh->targ && _old_237 != _src_1_231 && _old_237 != _keep_0_0_233 && _old_237 != _keep_0_1_234 && _old_237 != _keep_0_2_235) {
                            LIR_unload_subject(_old_237);
                        }
                    }
                    lir_subject_t* _old_238 = lh->targ;
                    if (_old_238) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_238 && _old_238 != lh->farg && _old_238 != lh->sarg && _old_238 != lh->targ && _old_238 != _src_1_231 && _old_238 != _keep_0_0_233 && _old_238 != _keep_0_1_234 && _old_238 != _keep_0_2_235) {
                            LIR_unload_subject(_old_238);
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
                    lir_subject_t* _src_1_1 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_MEMORY)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->farg)) {
                    lir_subject_t* _src_1_13 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_2_14 = lh->sarg;
                    lir_subject_t* _src_3_15 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_4_16 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_operation_t _match_op_0_17 = lh->op;
                    lir_operation_t _match_op_1_18 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_19 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_20 = lh->farg;
                    lir_subject_t* _keep_0_1_21 = lh->sarg;
                    lir_subject_t* _keep_0_2_22 = lh->targ;
                    lir_subject_t* _keep_1_0_23 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_24 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_25 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_26 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_27 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_28 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_17) {
                        lh->op = _match_op_0_17;
                        optimized = 1;
                    }
                    lir_subject_t* _old_29 = lh->farg;
                    if (_old_29 != _src_3_15) {
                        lh->farg = _src_3_15;
                        optimized = 1;
                        if (_old_29 && _old_29 != lh->farg && _old_29 != lh->sarg && _old_29 != lh->targ && _old_29 != _src_1_13 && _old_29 != _src_2_14 && _old_29 != _src_3_15 && _old_29 != _src_4_16 && _old_29 != _keep_0_0_20 && _old_29 != _keep_0_1_21 && _old_29 != _keep_0_2_22 && _old_29 != _keep_1_0_23 && _old_29 != _keep_1_1_24 && _old_29 != _keep_1_2_25 && _old_29 != _keep_2_0_26 && _old_29 != _keep_2_1_27 && _old_29 != _keep_2_2_28) {
                            LIR_unload_subject(_old_29);
                        }
                    }
                    lir_subject_t* _old_30 = lh->sarg;
                    if (_old_30 != _src_2_14) {
                        lh->sarg = _src_2_14;
                        optimized = 1;
                        if (_old_30 && _old_30 != lh->farg && _old_30 != lh->sarg && _old_30 != lh->targ && _old_30 != _src_1_13 && _old_30 != _src_2_14 && _old_30 != _src_3_15 && _old_30 != _src_4_16 && _old_30 != _keep_0_0_20 && _old_30 != _keep_0_1_21 && _old_30 != _keep_0_2_22 && _old_30 != _keep_1_0_23 && _old_30 != _keep_1_1_24 && _old_30 != _keep_1_2_25 && _old_30 != _keep_2_0_26 && _old_30 != _keep_2_1_27 && _old_30 != _keep_2_2_28) {
                            LIR_unload_subject(_old_30);
                        }
                    }
                    lir_subject_t* _old_31 = lh->targ;
                    if (_old_31) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_31 && _old_31 != lh->farg && _old_31 != lh->sarg && _old_31 != lh->targ && _old_31 != _src_1_13 && _old_31 != _src_2_14 && _old_31 != _src_3_15 && _old_31 != _src_4_16 && _old_31 != _keep_0_0_20 && _old_31 != _keep_0_1_21 && _old_31 != _keep_0_2_22 && _old_31 != _keep_1_0_23 && _old_31 != _keep_1_1_24 && _old_31 != _keep_1_2_25 && _old_31 != _keep_2_0_26 && _old_31 != _keep_2_1_27 && _old_31 != _keep_2_2_28) {
                            LIR_unload_subject(_old_31);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_18) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_18;
                        optimized = 1;
                    }
                    lir_subject_t* _old_32 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_32 != _src_1_13) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_13;
                        optimized = 1;
                        if (_old_32 && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_32 != _src_1_13 && _old_32 != _src_2_14 && _old_32 != _src_3_15 && _old_32 != _src_4_16 && _old_32 != _keep_0_0_20 && _old_32 != _keep_0_1_21 && _old_32 != _keep_0_2_22 && _old_32 != _keep_1_0_23 && _old_32 != _keep_1_1_24 && _old_32 != _keep_1_2_25 && _old_32 != _keep_2_0_26 && _old_32 != _keep_2_1_27 && _old_32 != _keep_2_2_28) {
                            LIR_unload_subject(_old_32);
                        }
                    }
                    lir_subject_t* _old_33 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_33 != _src_4_16) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_4_16;
                        optimized = 1;
                        if (_old_33 && _old_33 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_33 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_33 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_33 != _src_1_13 && _old_33 != _src_2_14 && _old_33 != _src_3_15 && _old_33 != _src_4_16 && _old_33 != _keep_0_0_20 && _old_33 != _keep_0_1_21 && _old_33 != _keep_0_2_22 && _old_33 != _keep_1_0_23 && _old_33 != _keep_1_1_24 && _old_33 != _keep_1_2_25 && _old_33 != _keep_2_0_26 && _old_33 != _keep_2_1_27 && _old_33 != _keep_2_2_28) {
                            LIR_unload_subject(_old_33);
                        }
                    }
                    lir_subject_t* _old_34 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_34) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_34 && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_34 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_34 != _src_1_13 && _old_34 != _src_2_14 && _old_34 != _src_3_15 && _old_34 != _src_4_16 && _old_34 != _keep_0_0_20 && _old_34 != _keep_0_1_21 && _old_34 != _keep_0_2_22 && _old_34 != _keep_1_0_23 && _old_34 != _keep_1_1_24 && _old_34 != _keep_1_2_25 && _old_34 != _keep_2_0_26 && _old_34 != _keep_2_1_27 && _old_34 != _keep_2_2_28) {
                            LIR_unload_subject(_old_34);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_35 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_36 = lh->sarg;
                    lir_operation_t _match_op_0_37 = lh->op;
                    lir_operation_t _match_op_1_38 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_39 = lh->farg;
                    lir_subject_t* _keep_0_1_40 = lh->sarg;
                    lir_subject_t* _keep_0_2_41 = lh->targ;
                    lir_subject_t* _keep_1_0_42 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_43 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_44 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_45 = lh->farg;
                    if (_old_45 != _src_2_36) {
                        lh->farg = _src_2_36;
                        optimized = 1;
                        if (_old_45 && _old_45 != lh->farg && _old_45 != lh->sarg && _old_45 != lh->targ && _old_45 != _src_1_35 && _old_45 != _src_2_36 && _old_45 != _keep_0_0_39 && _old_45 != _keep_0_1_40 && _old_45 != _keep_0_2_41 && _old_45 != _keep_1_0_42 && _old_45 != _keep_1_1_43 && _old_45 != _keep_1_2_44) {
                            LIR_unload_subject(_old_45);
                        }
                    }
                    lir_subject_t* _old_46 = lh->sarg;
                    if (_old_46 != _src_2_36) {
                        lh->sarg = _src_2_36;
                        optimized = 1;
                        if (_old_46 && _old_46 != lh->farg && _old_46 != lh->sarg && _old_46 != lh->targ && _old_46 != _src_1_35 && _old_46 != _src_2_36 && _old_46 != _keep_0_0_39 && _old_46 != _keep_0_1_40 && _old_46 != _keep_0_2_41 && _old_46 != _keep_1_0_42 && _old_46 != _keep_1_1_43 && _old_46 != _keep_1_2_44) {
                            LIR_unload_subject(_old_46);
                        }
                    }
                    lir_subject_t* _old_47 = lh->targ;
                    if (_old_47) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_47 && _old_47 != lh->farg && _old_47 != lh->sarg && _old_47 != lh->targ && _old_47 != _src_1_35 && _old_47 != _src_2_36 && _old_47 != _keep_0_0_39 && _old_47 != _keep_0_1_40 && _old_47 != _keep_0_2_41 && _old_47 != _keep_1_0_42 && _old_47 != _keep_1_1_43 && _old_47 != _keep_1_2_44) {
                            LIR_unload_subject(_old_47);
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
                    lir_subject_t* _src_1_48 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _src_2_49 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_operation_t _match_op_0_50 = lh->op;
                    lir_operation_t _match_op_1_51 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_52 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_operation_t _match_op_3_53 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op;
                    lir_subject_t* _keep_0_0_54 = lh->farg;
                    lir_subject_t* _keep_0_1_55 = lh->sarg;
                    lir_subject_t* _keep_0_2_56 = lh->targ;
                    lir_subject_t* _keep_1_0_57 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_58 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_59 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_60 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_61 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_62 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _keep_3_0_63 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_subject_t* _keep_3_1_64 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _keep_3_2_65 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_66 = lh->sarg;
                    if (_old_66 != _src_2_49) {
                        lh->sarg = _src_2_49;
                        optimized = 1;
                        if (_old_66 && _old_66 != lh->farg && _old_66 != lh->sarg && _old_66 != lh->targ && _old_66 != _src_1_48 && _old_66 != _src_2_49 && _old_66 != _keep_0_0_54 && _old_66 != _keep_0_1_55 && _old_66 != _keep_0_2_56 && _old_66 != _keep_1_0_57 && _old_66 != _keep_1_1_58 && _old_66 != _keep_1_2_59 && _old_66 != _keep_2_0_60 && _old_66 != _keep_2_1_61 && _old_66 != _keep_2_2_62 && _old_66 != _keep_3_0_63 && _old_66 != _keep_3_1_64 && _old_66 != _keep_3_2_65) {
                            LIR_unload_subject(_old_66);
                        }
                    }
                    lir_subject_t* _old_67 = lh->targ;
                    if (_old_67 != _src_2_49) {
                        lh->targ = _src_2_49;
                        optimized = 1;
                        if (_old_67 && _old_67 != lh->farg && _old_67 != lh->sarg && _old_67 != lh->targ && _old_67 != _src_1_48 && _old_67 != _src_2_49 && _old_67 != _keep_0_0_54 && _old_67 != _keep_0_1_55 && _old_67 != _keep_0_2_56 && _old_67 != _keep_1_0_57 && _old_67 != _keep_1_1_58 && _old_67 != _keep_1_2_59 && _old_67 != _keep_2_0_60 && _old_67 != _keep_2_1_61 && _old_67 != _keep_2_2_62 && _old_67 != _keep_3_0_63 && _old_67 != _keep_3_1_64 && _old_67 != _keep_3_2_65) {
                            LIR_unload_subject(_old_67);
                        }
                    }
                    lir_subject_t* _old_68 = lh->farg;
                    if (_old_68 != _src_2_49) {
                        lh->farg = _src_2_49;
                        optimized = 1;
                        if (_old_68 && _old_68 != lh->farg && _old_68 != lh->sarg && _old_68 != lh->targ && _old_68 != _src_1_48 && _old_68 != _src_2_49 && _old_68 != _keep_0_0_54 && _old_68 != _keep_0_1_55 && _old_68 != _keep_0_2_56 && _old_68 != _keep_1_0_57 && _old_68 != _keep_1_1_58 && _old_68 != _keep_1_2_59 && _old_68 != _keep_2_0_60 && _old_68 != _keep_2_1_61 && _old_68 != _keep_2_2_62 && _old_68 != _keep_3_0_63 && _old_68 != _keep_3_1_64 && _old_68 != _keep_3_2_65) {
                            LIR_unload_subject(_old_68);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_50) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_50;
                        optimized = 1;
                    }
                    lir_subject_t* _old_69 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_69 != _src_1_48) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_48;
                        optimized = 1;
                        if (_old_69 && _old_69 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_69 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_69 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_69 != _src_1_48 && _old_69 != _src_2_49 && _old_69 != _keep_0_0_54 && _old_69 != _keep_0_1_55 && _old_69 != _keep_0_2_56 && _old_69 != _keep_1_0_57 && _old_69 != _keep_1_1_58 && _old_69 != _keep_1_2_59 && _old_69 != _keep_2_0_60 && _old_69 != _keep_2_1_61 && _old_69 != _keep_2_2_62 && _old_69 != _keep_3_0_63 && _old_69 != _keep_3_1_64 && _old_69 != _keep_3_2_65) {
                            LIR_unload_subject(_old_69);
                        }
                    }
                    lir_subject_t* _old_70 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_70 != _src_1_48) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_48;
                        optimized = 1;
                        if (_old_70 && _old_70 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_70 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_70 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_70 != _src_1_48 && _old_70 != _src_2_49 && _old_70 != _keep_0_0_54 && _old_70 != _keep_0_1_55 && _old_70 != _keep_0_2_56 && _old_70 != _keep_1_0_57 && _old_70 != _keep_1_1_58 && _old_70 != _keep_1_2_59 && _old_70 != _keep_2_0_60 && _old_70 != _keep_2_1_61 && _old_70 != _keep_2_2_62 && _old_70 != _keep_3_0_63 && _old_70 != _keep_3_1_64 && _old_70 != _keep_3_2_65) {
                            LIR_unload_subject(_old_70);
                        }
                    }
                    lir_subject_t* _old_71 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_71) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_71 && _old_71 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_71 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_71 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_71 != _src_1_48 && _old_71 != _src_2_49 && _old_71 != _keep_0_0_54 && _old_71 != _keep_0_1_55 && _old_71 != _keep_0_2_56 && _old_71 != _keep_1_0_57 && _old_71 != _keep_1_1_58 && _old_71 != _keep_1_2_59 && _old_71 != _keep_2_0_60 && _old_71 != _keep_2_1_61 && _old_71 != _keep_2_2_62 && _old_71 != _keep_3_0_63 && _old_71 != _keep_3_1_64 && _old_71 != _keep_3_2_65) {
                            LIR_unload_subject(_old_71);
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
                    lir_subject_t* _src_1_72 = lh->farg;
                    lir_operation_t _match_op_0_73 = lh->op;
                    lir_subject_t* _keep_0_0_74 = lh->farg;
                    lir_subject_t* _keep_0_1_75 = lh->sarg;
                    lir_subject_t* _keep_0_2_76 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_77 = lh->sarg;
                    if (_old_77 != _src_1_72) {
                        lh->sarg = _src_1_72;
                        optimized = 1;
                        if (_old_77 && _old_77 != lh->farg && _old_77 != lh->sarg && _old_77 != lh->targ && _old_77 != _src_1_72 && _old_77 != _keep_0_0_74 && _old_77 != _keep_0_1_75 && _old_77 != _keep_0_2_76) {
                            LIR_unload_subject(_old_77);
                        }
                    }
                    lir_subject_t* _old_78 = lh->targ;
                    if (_old_78 != _src_1_72) {
                        lh->targ = _src_1_72;
                        optimized = 1;
                        if (_old_78 && _old_78 != lh->farg && _old_78 != lh->sarg && _old_78 != lh->targ && _old_78 != _src_1_72 && _old_78 != _keep_0_0_74 && _old_78 != _keep_0_1_75 && _old_78 != _keep_0_2_76) {
                            LIR_unload_subject(_old_78);
                        }
                    }
                    lir_subject_t* _old_79 = lh->farg;
                    if (_old_79 != _src_1_72) {
                        lh->farg = _src_1_72;
                        optimized = 1;
                        if (_old_79 && _old_79 != lh->farg && _old_79 != lh->sarg && _old_79 != lh->targ && _old_79 != _src_1_72 && _old_79 != _keep_0_0_74 && _old_79 != _keep_0_1_75 && _old_79 != _keep_0_2_76) {
                            LIR_unload_subject(_old_79);
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
                    lir_subject_t* _src_1_80 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_81 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_82 = lh->op;
                    lir_operation_t _match_op_1_83 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_84 = lh->farg;
                    lir_subject_t* _keep_0_1_85 = lh->sarg;
                    lir_subject_t* _keep_0_2_86 = lh->targ;
                    lir_subject_t* _keep_1_0_87 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_88 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_89 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_82) {
                        lh->op = _match_op_0_82;
                        optimized = 1;
                    }
                    lir_subject_t* _old_90 = lh->farg;
                    if (_old_90 != _src_1_80) {
                        lh->farg = _src_1_80;
                        optimized = 1;
                        if (_old_90 && _old_90 != lh->farg && _old_90 != lh->sarg && _old_90 != lh->targ && _old_90 != _src_1_80 && _old_90 != _src_2_81 && _old_90 != _keep_0_0_84 && _old_90 != _keep_0_1_85 && _old_90 != _keep_0_2_86 && _old_90 != _keep_1_0_87 && _old_90 != _keep_1_1_88 && _old_90 != _keep_1_2_89) {
                            LIR_unload_subject(_old_90);
                        }
                    }
                    lir_subject_t* _old_91 = lh->sarg;
                    if (_old_91 != _src_2_81) {
                        lh->sarg = _src_2_81;
                        optimized = 1;
                        if (_old_91 && _old_91 != lh->farg && _old_91 != lh->sarg && _old_91 != lh->targ && _old_91 != _src_1_80 && _old_91 != _src_2_81 && _old_91 != _keep_0_0_84 && _old_91 != _keep_0_1_85 && _old_91 != _keep_0_2_86 && _old_91 != _keep_1_0_87 && _old_91 != _keep_1_1_88 && _old_91 != _keep_1_2_89) {
                            LIR_unload_subject(_old_91);
                        }
                    }
                    lir_subject_t* _old_92 = lh->targ;
                    if (_old_92) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_92 && _old_92 != lh->farg && _old_92 != lh->sarg && _old_92 != lh->targ && _old_92 != _src_1_80 && _old_92 != _src_2_81 && _old_92 != _keep_0_0_84 && _old_92 != _keep_0_1_85 && _old_92 != _keep_0_2_86 && _old_92 != _keep_1_0_87 && _old_92 != _keep_1_1_88 && _old_92 != _keep_1_2_89) {
                            LIR_unload_subject(_old_92);
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
                    lir_subject_t* _src_1_93 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_94 = lh->sarg;
                    lir_subject_t* _src_3_95 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_96 = lh->op;
                    lir_operation_t _match_op_1_97 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_98 = lh->farg;
                    lir_subject_t* _keep_0_1_99 = lh->sarg;
                    lir_subject_t* _keep_0_2_100 = lh->targ;
                    lir_subject_t* _keep_1_0_101 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_102 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_103 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_97) {
                        lh->op = _match_op_1_97;
                        optimized = 1;
                    }
                    lir_subject_t* _old_104 = lh->farg;
                    if (_old_104 != _src_2_94) {
                        lh->farg = _src_2_94;
                        optimized = 1;
                        if (_old_104 && _old_104 != lh->farg && _old_104 != lh->sarg && _old_104 != lh->targ && _old_104 != _src_1_93 && _old_104 != _src_2_94 && _old_104 != _src_3_95 && _old_104 != _keep_0_0_98 && _old_104 != _keep_0_1_99 && _old_104 != _keep_0_2_100 && _old_104 != _keep_1_0_101 && _old_104 != _keep_1_1_102 && _old_104 != _keep_1_2_103) {
                            LIR_unload_subject(_old_104);
                        }
                    }
                    lir_subject_t* _old_105 = lh->sarg;
                    if (_old_105 != _src_3_95) {
                        lh->sarg = _src_3_95;
                        optimized = 1;
                        if (_old_105 && _old_105 != lh->farg && _old_105 != lh->sarg && _old_105 != lh->targ && _old_105 != _src_1_93 && _old_105 != _src_2_94 && _old_105 != _src_3_95 && _old_105 != _keep_0_0_98 && _old_105 != _keep_0_1_99 && _old_105 != _keep_0_2_100 && _old_105 != _keep_1_0_101 && _old_105 != _keep_1_1_102 && _old_105 != _keep_1_2_103) {
                            LIR_unload_subject(_old_105);
                        }
                    }
                    lir_subject_t* _old_106 = lh->targ;
                    if (_old_106) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_106 && _old_106 != lh->farg && _old_106 != lh->sarg && _old_106 != lh->targ && _old_106 != _src_1_93 && _old_106 != _src_2_94 && _old_106 != _src_3_95 && _old_106 != _keep_0_0_98 && _old_106 != _keep_0_1_99 && _old_106 != _keep_0_2_100 && _old_106 != _keep_1_0_101 && _old_106 != _keep_1_1_102 && _old_106 != _keep_1_2_103) {
                            LIR_unload_subject(_old_106);
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
                    lir_subject_t* _src_1_123 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_124 = lh->op;
                    lir_operation_t _match_op_1_125 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_126 = lh->farg;
                    lir_subject_t* _keep_0_1_127 = lh->sarg;
                    lir_subject_t* _keep_0_2_128 = lh->targ;
                    lir_subject_t* _keep_1_0_129 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_130 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_131 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_132 = lh->sarg;
                    if (_old_132 != _src_1_123) {
                        lh->sarg = _src_1_123;
                        optimized = 1;
                        if (_old_132 && _old_132 != lh->farg && _old_132 != lh->sarg && _old_132 != lh->targ && _old_132 != _src_1_123 && _old_132 != _keep_0_0_126 && _old_132 != _keep_0_1_127 && _old_132 != _keep_0_2_128 && _old_132 != _keep_1_0_129 && _old_132 != _keep_1_1_130 && _old_132 != _keep_1_2_131) {
                            LIR_unload_subject(_old_132);
                        }
                    }
                    lir_subject_t* _old_133 = lh->targ;
                    if (_old_133 != _src_1_123) {
                        lh->targ = _src_1_123;
                        optimized = 1;
                        if (_old_133 && _old_133 != lh->farg && _old_133 != lh->sarg && _old_133 != lh->targ && _old_133 != _src_1_123 && _old_133 != _keep_0_0_126 && _old_133 != _keep_0_1_127 && _old_133 != _keep_0_2_128 && _old_133 != _keep_1_0_129 && _old_133 != _keep_1_1_130 && _old_133 != _keep_1_2_131) {
                            LIR_unload_subject(_old_133);
                        }
                    }
                    lir_subject_t* _old_134 = lh->farg;
                    if (_old_134 != _src_1_123) {
                        lh->farg = _src_1_123;
                        optimized = 1;
                        if (_old_134 && _old_134 != lh->farg && _old_134 != lh->sarg && _old_134 != lh->targ && _old_134 != _src_1_123 && _old_134 != _keep_0_0_126 && _old_134 != _keep_0_1_127 && _old_134 != _keep_0_2_128 && _old_134 != _keep_1_0_129 && _old_134 != _keep_1_1_130 && _old_134 != _keep_1_2_131) {
                            LIR_unload_subject(_old_134);
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
                    lir_subject_t* _src_1_135 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_136 = lh->op;
                    lir_operation_t _match_op_1_137 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_138 = lh->farg;
                    lir_subject_t* _keep_0_1_139 = lh->sarg;
                    lir_subject_t* _keep_0_2_140 = lh->targ;
                    lir_subject_t* _keep_1_0_141 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_142 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_143 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_144 = lh->sarg;
                    if (_old_144 != _src_1_135) {
                        lh->sarg = _src_1_135;
                        optimized = 1;
                        if (_old_144 && _old_144 != lh->farg && _old_144 != lh->sarg && _old_144 != lh->targ && _old_144 != _src_1_135 && _old_144 != _keep_0_0_138 && _old_144 != _keep_0_1_139 && _old_144 != _keep_0_2_140 && _old_144 != _keep_1_0_141 && _old_144 != _keep_1_1_142 && _old_144 != _keep_1_2_143) {
                            LIR_unload_subject(_old_144);
                        }
                    }
                    lir_subject_t* _old_145 = lh->targ;
                    if (_old_145 != _src_1_135) {
                        lh->targ = _src_1_135;
                        optimized = 1;
                        if (_old_145 && _old_145 != lh->farg && _old_145 != lh->sarg && _old_145 != lh->targ && _old_145 != _src_1_135 && _old_145 != _keep_0_0_138 && _old_145 != _keep_0_1_139 && _old_145 != _keep_0_2_140 && _old_145 != _keep_1_0_141 && _old_145 != _keep_1_1_142 && _old_145 != _keep_1_2_143) {
                            LIR_unload_subject(_old_145);
                        }
                    }
                    lir_subject_t* _old_146 = lh->farg;
                    if (_old_146 != _src_1_135) {
                        lh->farg = _src_1_135;
                        optimized = 1;
                        if (_old_146 && _old_146 != lh->farg && _old_146 != lh->sarg && _old_146 != lh->targ && _old_146 != _src_1_135 && _old_146 != _keep_0_0_138 && _old_146 != _keep_0_1_139 && _old_146 != _keep_0_2_140 && _old_146 != _keep_1_0_141 && _old_146 != _keep_1_1_142 && _old_146 != _keep_1_2_143) {
                            LIR_unload_subject(_old_146);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iADD &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_147 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_148 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_3_149 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_150 = lh->op;
                    lir_operation_t _match_op_1_151 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_152 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_153 = lh->farg;
                    lir_subject_t* _keep_0_1_154 = lh->sarg;
                    lir_subject_t* _keep_0_2_155 = lh->targ;
                    lir_subject_t* _keep_1_0_156 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_157 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_158 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_159 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_160 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_161 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_150) {
                        lh->op = _match_op_0_150;
                        optimized = 1;
                    }
                    lir_subject_t* _old_162 = lh->farg;
                    if (_old_162 != _src_1_147) {
                        lh->farg = _src_1_147;
                        optimized = 1;
                        if (_old_162 && _old_162 != lh->farg && _old_162 != lh->sarg && _old_162 != lh->targ && _old_162 != _src_1_147 && _old_162 != _src_2_148 && _old_162 != _src_3_149 && _old_162 != _keep_0_0_153 && _old_162 != _keep_0_1_154 && _old_162 != _keep_0_2_155 && _old_162 != _keep_1_0_156 && _old_162 != _keep_1_1_157 && _old_162 != _keep_1_2_158 && _old_162 != _keep_2_0_159 && _old_162 != _keep_2_1_160 && _old_162 != _keep_2_2_161) {
                            LIR_unload_subject(_old_162);
                        }
                    }
                    lir_subject_t* _old_163 = lh->sarg;
                    if (_old_163 != _src_2_148) {
                        lh->sarg = _src_2_148;
                        optimized = 1;
                        if (_old_163 && _old_163 != lh->farg && _old_163 != lh->sarg && _old_163 != lh->targ && _old_163 != _src_1_147 && _old_163 != _src_2_148 && _old_163 != _src_3_149 && _old_163 != _keep_0_0_153 && _old_163 != _keep_0_1_154 && _old_163 != _keep_0_2_155 && _old_163 != _keep_1_0_156 && _old_163 != _keep_1_1_157 && _old_163 != _keep_1_2_158 && _old_163 != _keep_2_0_159 && _old_163 != _keep_2_1_160 && _old_163 != _keep_2_2_161) {
                            LIR_unload_subject(_old_163);
                        }
                    }
                    lir_subject_t* _old_164 = lh->targ;
                    if (_old_164) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_164 && _old_164 != lh->farg && _old_164 != lh->sarg && _old_164 != lh->targ && _old_164 != _src_1_147 && _old_164 != _src_2_148 && _old_164 != _src_3_149 && _old_164 != _keep_0_0_153 && _old_164 != _keep_0_1_154 && _old_164 != _keep_0_2_155 && _old_164 != _keep_1_0_156 && _old_164 != _keep_1_1_157 && _old_164 != _keep_1_2_158 && _old_164 != _keep_2_0_159 && _old_164 != _keep_2_1_160 && _old_164 != _keep_2_2_161) {
                            LIR_unload_subject(_old_164);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_165 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_165 != _src_2_148) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_148;
                        optimized = 1;
                        if (_old_165 && _old_165 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_165 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_165 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_165 != _src_1_147 && _old_165 != _src_2_148 && _old_165 != _src_3_149 && _old_165 != _keep_0_0_153 && _old_165 != _keep_0_1_154 && _old_165 != _keep_0_2_155 && _old_165 != _keep_1_0_156 && _old_165 != _keep_1_1_157 && _old_165 != _keep_1_2_158 && _old_165 != _keep_2_0_159 && _old_165 != _keep_2_1_160 && _old_165 != _keep_2_2_161) {
                            LIR_unload_subject(_old_165);
                        }
                    }
                    lir_subject_t* _old_166 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_166 != _src_3_149) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_3_149;
                        optimized = 1;
                        if (_old_166 && _old_166 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_166 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_166 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_166 != _src_1_147 && _old_166 != _src_2_148 && _old_166 != _src_3_149 && _old_166 != _keep_0_0_153 && _old_166 != _keep_0_1_154 && _old_166 != _keep_0_2_155 && _old_166 != _keep_1_0_156 && _old_166 != _keep_1_1_157 && _old_166 != _keep_1_2_158 && _old_166 != _keep_2_0_159 && _old_166 != _keep_2_1_160 && _old_166 != _keep_2_2_161) {
                            LIR_unload_subject(_old_166);
                        }
                    }
                    lir_subject_t* _old_167 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_167 != _src_2_148) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_148;
                        optimized = 1;
                        if (_old_167 && _old_167 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_167 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_167 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_167 != _src_1_147 && _old_167 != _src_2_148 && _old_167 != _src_3_149 && _old_167 != _keep_0_0_153 && _old_167 != _keep_0_1_154 && _old_167 != _keep_0_2_155 && _old_167 != _keep_1_0_156 && _old_167 != _keep_1_1_157 && _old_167 != _keep_1_2_158 && _old_167 != _keep_2_0_159 && _old_167 != _keep_2_1_160 && _old_167 != _keep_2_2_161) {
                            LIR_unload_subject(_old_167);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
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
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_176 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_177 = lh->sarg;
                    lir_subject_t* _src_3_178 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_179 = lh->op;
                    lir_operation_t _match_op_1_180 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_181 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_182 = lh->farg;
                    lir_subject_t* _keep_0_1_183 = lh->sarg;
                    lir_subject_t* _keep_0_2_184 = lh->targ;
                    lir_subject_t* _keep_1_0_185 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_186 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_187 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_188 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_189 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_190 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_191 = lh->farg;
                    if (_old_191 != _src_2_177) {
                        lh->farg = _src_2_177;
                        optimized = 1;
                        if (_old_191 && _old_191 != lh->farg && _old_191 != lh->sarg && _old_191 != lh->targ && _old_191 != _src_1_176 && _old_191 != _src_2_177 && _old_191 != _src_3_178 && _old_191 != _keep_0_0_182 && _old_191 != _keep_0_1_183 && _old_191 != _keep_0_2_184 && _old_191 != _keep_1_0_185 && _old_191 != _keep_1_1_186 && _old_191 != _keep_1_2_187 && _old_191 != _keep_2_0_188 && _old_191 != _keep_2_1_189 && _old_191 != _keep_2_2_190) {
                            LIR_unload_subject(_old_191);
                        }
                    }
                    lir_subject_t* _old_192 = lh->sarg;
                    if (_old_192) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_192 && _old_192 != lh->farg && _old_192 != lh->sarg && _old_192 != lh->targ && _old_192 != _src_1_176 && _old_192 != _src_2_177 && _old_192 != _src_3_178 && _old_192 != _keep_0_0_182 && _old_192 != _keep_0_1_183 && _old_192 != _keep_0_2_184 && _old_192 != _keep_1_0_185 && _old_192 != _keep_1_1_186 && _old_192 != _keep_1_2_187 && _old_192 != _keep_2_0_188 && _old_192 != _keep_2_1_189 && _old_192 != _keep_2_2_190) {
                            LIR_unload_subject(_old_192);
                        }
                    }
                    lir_subject_t* _old_193 = lh->targ;
                    if (_old_193) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_193 && _old_193 != lh->farg && _old_193 != lh->sarg && _old_193 != lh->targ && _old_193 != _src_1_176 && _old_193 != _src_2_177 && _old_193 != _src_3_178 && _old_193 != _keep_0_0_182 && _old_193 != _keep_0_1_183 && _old_193 != _keep_0_2_184 && _old_193 != _keep_1_0_185 && _old_193 != _keep_1_1_186 && _old_193 != _keep_1_2_187 && _old_193 != _keep_2_0_188 && _old_193 != _keep_2_1_189 && _old_193 != _keep_2_2_190) {
                            LIR_unload_subject(_old_193);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_179) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_179;
                        optimized = 1;
                    }
                    lir_subject_t* _old_194 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_194 != _src_3_178) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_178;
                        optimized = 1;
                        if (_old_194 && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_194 != _src_1_176 && _old_194 != _src_2_177 && _old_194 != _src_3_178 && _old_194 != _keep_0_0_182 && _old_194 != _keep_0_1_183 && _old_194 != _keep_0_2_184 && _old_194 != _keep_1_0_185 && _old_194 != _keep_1_1_186 && _old_194 != _keep_1_2_187 && _old_194 != _keep_2_0_188 && _old_194 != _keep_2_1_189 && _old_194 != _keep_2_2_190) {
                            LIR_unload_subject(_old_194);
                        }
                    }
                    lir_subject_t* _old_195 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_195 != _src_2_177) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_177;
                        optimized = 1;
                        if (_old_195 && _old_195 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_195 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_195 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_195 != _src_1_176 && _old_195 != _src_2_177 && _old_195 != _src_3_178 && _old_195 != _keep_0_0_182 && _old_195 != _keep_0_1_183 && _old_195 != _keep_0_2_184 && _old_195 != _keep_1_0_185 && _old_195 != _keep_1_1_186 && _old_195 != _keep_1_2_187 && _old_195 != _keep_2_0_188 && _old_195 != _keep_2_1_189 && _old_195 != _keep_2_2_190) {
                            LIR_unload_subject(_old_195);
                        }
                    }
                    lir_subject_t* _old_196 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_196) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_196 && _old_196 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_196 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_196 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_196 != _src_1_176 && _old_196 != _src_2_177 && _old_196 != _src_3_178 && _old_196 != _keep_0_0_182 && _old_196 != _keep_0_1_183 && _old_196 != _keep_0_2_184 && _old_196 != _keep_1_0_185 && _old_196 != _keep_1_1_186 && _old_196 != _keep_1_2_187 && _old_196 != _keep_2_0_188 && _old_196 != _keep_2_1_189 && _old_196 != _keep_2_2_190) {
                            LIR_unload_subject(_old_196);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
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
                    lir_subject_t* _src_2_197 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_1_198 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_199 = lh->op;
                    lir_operation_t _match_op_1_200 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_201 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_202 = lh->farg;
                    lir_subject_t* _keep_0_1_203 = lh->sarg;
                    lir_subject_t* _keep_0_2_204 = lh->targ;
                    lir_subject_t* _keep_1_0_205 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_206 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_207 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_208 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_209 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_210 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_211 = lh->farg;
                    if (_old_211 != _src_1_198) {
                        lh->farg = _src_1_198;
                        optimized = 1;
                        if (_old_211 && _old_211 != lh->farg && _old_211 != lh->sarg && _old_211 != lh->targ && _old_211 != _src_2_197 && _old_211 != _src_1_198 && _old_211 != _keep_0_0_202 && _old_211 != _keep_0_1_203 && _old_211 != _keep_0_2_204 && _old_211 != _keep_1_0_205 && _old_211 != _keep_1_1_206 && _old_211 != _keep_1_2_207 && _old_211 != _keep_2_0_208 && _old_211 != _keep_2_1_209 && _old_211 != _keep_2_2_210) {
                            LIR_unload_subject(_old_211);
                        }
                    }
                    lir_subject_t* _old_212 = lh->sarg;
                    if (_old_212) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_212 && _old_212 != lh->farg && _old_212 != lh->sarg && _old_212 != lh->targ && _old_212 != _src_2_197 && _old_212 != _src_1_198 && _old_212 != _keep_0_0_202 && _old_212 != _keep_0_1_203 && _old_212 != _keep_0_2_204 && _old_212 != _keep_1_0_205 && _old_212 != _keep_1_1_206 && _old_212 != _keep_1_2_207 && _old_212 != _keep_2_0_208 && _old_212 != _keep_2_1_209 && _old_212 != _keep_2_2_210) {
                            LIR_unload_subject(_old_212);
                        }
                    }
                    lir_subject_t* _old_213 = lh->targ;
                    if (_old_213) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_213 && _old_213 != lh->farg && _old_213 != lh->sarg && _old_213 != lh->targ && _old_213 != _src_2_197 && _old_213 != _src_1_198 && _old_213 != _keep_0_0_202 && _old_213 != _keep_0_1_203 && _old_213 != _keep_0_2_204 && _old_213 != _keep_1_0_205 && _old_213 != _keep_1_1_206 && _old_213 != _keep_1_2_207 && _old_213 != _keep_2_0_208 && _old_213 != _keep_2_1_209 && _old_213 != _keep_2_2_210) {
                            LIR_unload_subject(_old_213);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_199) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_199;
                        optimized = 1;
                    }
                    lir_subject_t* _old_214 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_214 != _src_2_197) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_197;
                        optimized = 1;
                        if (_old_214 && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_214 != _src_2_197 && _old_214 != _src_1_198 && _old_214 != _keep_0_0_202 && _old_214 != _keep_0_1_203 && _old_214 != _keep_0_2_204 && _old_214 != _keep_1_0_205 && _old_214 != _keep_1_1_206 && _old_214 != _keep_1_2_207 && _old_214 != _keep_2_0_208 && _old_214 != _keep_2_1_209 && _old_214 != _keep_2_2_210) {
                            LIR_unload_subject(_old_214);
                        }
                    }
                    lir_subject_t* _old_215 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_215 != _src_2_197) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_197;
                        optimized = 1;
                        if (_old_215 && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_215 != _src_2_197 && _old_215 != _src_1_198 && _old_215 != _keep_0_0_202 && _old_215 != _keep_0_1_203 && _old_215 != _keep_0_2_204 && _old_215 != _keep_1_0_205 && _old_215 != _keep_1_1_206 && _old_215 != _keep_1_2_207 && _old_215 != _keep_2_0_208 && _old_215 != _keep_2_1_209 && _old_215 != _keep_2_2_210) {
                            LIR_unload_subject(_old_215);
                        }
                    }
                    lir_subject_t* _old_216 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_216) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_216 && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_216 != _src_2_197 && _old_216 != _src_1_198 && _old_216 != _keep_0_0_202 && _old_216 != _keep_0_1_203 && _old_216 != _keep_0_2_204 && _old_216 != _keep_1_0_205 && _old_216 != _keep_1_1_206 && _old_216 != _keep_1_2_207 && _old_216 != _keep_2_0_208 && _old_216 != _keep_2_1_209 && _old_216 != _keep_2_2_210) {
                            LIR_unload_subject(_old_216);
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
                    lir_subject_t* _src_1_247 = lh->farg;
                    lir_operation_t _match_op_0_248 = lh->op;
                    lir_subject_t* _keep_0_0_249 = lh->farg;
                    lir_subject_t* _keep_0_1_250 = lh->sarg;
                    lir_subject_t* _keep_0_2_251 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_252 = lh->farg;
                    if (_old_252 != _src_1_247) {
                        lh->farg = _src_1_247;
                        optimized = 1;
                        if (_old_252 && _old_252 != lh->farg && _old_252 != lh->sarg && _old_252 != lh->targ && _old_252 != _src_1_247 && _old_252 != _keep_0_0_249 && _old_252 != _keep_0_1_250 && _old_252 != _keep_0_2_251) {
                            LIR_unload_subject(_old_252);
                        }
                    }
                    lir_subject_t* _old_253 = lh->sarg;
                    if (_old_253 != _src_1_247) {
                        lh->sarg = _src_1_247;
                        optimized = 1;
                        if (_old_253 && _old_253 != lh->farg && _old_253 != lh->sarg && _old_253 != lh->targ && _old_253 != _src_1_247 && _old_253 != _keep_0_0_249 && _old_253 != _keep_0_1_250 && _old_253 != _keep_0_2_251) {
                            LIR_unload_subject(_old_253);
                        }
                    }
                    lir_subject_t* _old_254 = lh->targ;
                    if (_old_254) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_254 && _old_254 != lh->farg && _old_254 != lh->sarg && _old_254 != lh->targ && _old_254 != _src_1_247 && _old_254 != _keep_0_0_249 && _old_254 != _keep_0_1_250 && _old_254 != _keep_0_2_251) {
                            LIR_unload_subject(_old_254);
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
                    lir_subject_t* _src_1_279 = lh->farg;
                    lir_operation_t _match_op_0_280 = lh->op;
                    lir_subject_t* _keep_0_0_281 = lh->farg;
                    lir_subject_t* _keep_0_1_282 = lh->sarg;
                    lir_subject_t* _keep_0_2_283 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_284 = lh->sarg;
                    if (_old_284 != _src_1_279) {
                        lh->sarg = _src_1_279;
                        optimized = 1;
                        if (_old_284 && _old_284 != lh->farg && _old_284 != lh->sarg && _old_284 != lh->targ && _old_284 != _src_1_279 && _old_284 != _keep_0_0_281 && _old_284 != _keep_0_1_282 && _old_284 != _keep_0_2_283) {
                            LIR_unload_subject(_old_284);
                        }
                    }
                    lir_subject_t* _old_285 = lh->targ;
                    if (_old_285 != _src_1_279) {
                        lh->targ = _src_1_279;
                        optimized = 1;
                        if (_old_285 && _old_285 != lh->farg && _old_285 != lh->sarg && _old_285 != lh->targ && _old_285 != _src_1_279 && _old_285 != _keep_0_0_281 && _old_285 != _keep_0_1_282 && _old_285 != _keep_0_2_283) {
                            LIR_unload_subject(_old_285);
                        }
                    }
                    lir_subject_t* _old_286 = lh->farg;
                    if (_old_286 != _src_1_279) {
                        lh->farg = _src_1_279;
                        optimized = 1;
                        if (_old_286 && _old_286 != lh->farg && _old_286 != lh->sarg && _old_286 != lh->targ && _old_286 != _src_1_279 && _old_286 != _keep_0_0_281 && _old_286 != _keep_0_1_282 && _old_286 != _keep_0_2_283) {
                            LIR_unload_subject(_old_286);
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
                    lir_subject_t* _src_1_239 = lh->farg;
                    lir_operation_t _match_op_0_240 = lh->op;
                    lir_subject_t* _keep_0_0_241 = lh->farg;
                    lir_subject_t* _keep_0_1_242 = lh->sarg;
                    lir_subject_t* _keep_0_2_243 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_244 = lh->farg;
                    if (_old_244 != _src_1_239) {
                        lh->farg = _src_1_239;
                        optimized = 1;
                        if (_old_244 && _old_244 != lh->farg && _old_244 != lh->sarg && _old_244 != lh->targ && _old_244 != _src_1_239 && _old_244 != _keep_0_0_241 && _old_244 != _keep_0_1_242 && _old_244 != _keep_0_2_243) {
                            LIR_unload_subject(_old_244);
                        }
                    }
                    lir_subject_t* _old_245 = lh->sarg;
                    if (_old_245 != _src_1_239) {
                        lh->sarg = _src_1_239;
                        optimized = 1;
                        if (_old_245 && _old_245 != lh->farg && _old_245 != lh->sarg && _old_245 != lh->targ && _old_245 != _src_1_239 && _old_245 != _keep_0_0_241 && _old_245 != _keep_0_1_242 && _old_245 != _keep_0_2_243) {
                            LIR_unload_subject(_old_245);
                        }
                    }
                    lir_subject_t* _old_246 = lh->targ;
                    if (_old_246) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_246 && _old_246 != lh->farg && _old_246 != lh->sarg && _old_246 != lh->targ && _old_246 != _src_1_239 && _old_246 != _keep_0_0_241 && _old_246 != _keep_0_1_242 && _old_246 != _keep_0_2_243) {
                            LIR_unload_subject(_old_246);
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
                    lir_subject_t* _src_1_107 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_108 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_109 = lh->op;
                    lir_operation_t _match_op_1_110 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_111 = lh->farg;
                    lir_subject_t* _keep_0_1_112 = lh->sarg;
                    lir_subject_t* _keep_0_2_113 = lh->targ;
                    lir_subject_t* _keep_1_0_114 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_115 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_116 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_117 = lh->sarg;
                    if (_old_117 != _src_1_107) {
                        lh->sarg = _src_1_107;
                        optimized = 1;
                        if (_old_117 && _old_117 != lh->farg && _old_117 != lh->sarg && _old_117 != lh->targ && _old_117 != _src_1_107 && _old_117 != _src_2_108 && _old_117 != _keep_0_0_111 && _old_117 != _keep_0_1_112 && _old_117 != _keep_0_2_113 && _old_117 != _keep_1_0_114 && _old_117 != _keep_1_1_115 && _old_117 != _keep_1_2_116) {
                            LIR_unload_subject(_old_117);
                        }
                    }
                    lir_subject_t* _old_118 = lh->targ;
                    if (_old_118 != _src_1_107) {
                        lh->targ = _src_1_107;
                        optimized = 1;
                        if (_old_118 && _old_118 != lh->farg && _old_118 != lh->sarg && _old_118 != lh->targ && _old_118 != _src_1_107 && _old_118 != _src_2_108 && _old_118 != _keep_0_0_111 && _old_118 != _keep_0_1_112 && _old_118 != _keep_0_2_113 && _old_118 != _keep_1_0_114 && _old_118 != _keep_1_1_115 && _old_118 != _keep_1_2_116) {
                            LIR_unload_subject(_old_118);
                        }
                    }
                    lir_subject_t* _old_119 = lh->farg;
                    if (_old_119 != _src_1_107) {
                        lh->farg = _src_1_107;
                        optimized = 1;
                        if (_old_119 && _old_119 != lh->farg && _old_119 != lh->sarg && _old_119 != lh->targ && _old_119 != _src_1_107 && _old_119 != _src_2_108 && _old_119 != _keep_0_0_111 && _old_119 != _keep_0_1_112 && _old_119 != _keep_0_2_113 && _old_119 != _keep_1_0_114 && _old_119 != _keep_1_1_115 && _old_119 != _keep_1_2_116) {
                            LIR_unload_subject(_old_119);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_120 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_120 != _src_2_108) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_108;
                        optimized = 1;
                        if (_old_120 && _old_120 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_120 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_120 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_120 != _src_1_107 && _old_120 != _src_2_108 && _old_120 != _keep_0_0_111 && _old_120 != _keep_0_1_112 && _old_120 != _keep_0_2_113 && _old_120 != _keep_1_0_114 && _old_120 != _keep_1_1_115 && _old_120 != _keep_1_2_116) {
                            LIR_unload_subject(_old_120);
                        }
                    }
                    lir_subject_t* _old_121 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_121 != _src_2_108) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_2_108;
                        optimized = 1;
                        if (_old_121 && _old_121 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_121 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_121 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_121 != _src_1_107 && _old_121 != _src_2_108 && _old_121 != _keep_0_0_111 && _old_121 != _keep_0_1_112 && _old_121 != _keep_0_2_113 && _old_121 != _keep_1_0_114 && _old_121 != _keep_1_1_115 && _old_121 != _keep_1_2_116) {
                            LIR_unload_subject(_old_121);
                        }
                    }
                    lir_subject_t* _old_122 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_122 != _src_2_108) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_108;
                        optimized = 1;
                        if (_old_122 && _old_122 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_122 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_122 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_122 != _src_1_107 && _old_122 != _src_2_108 && _old_122 != _keep_0_0_111 && _old_122 != _keep_0_1_112 && _old_122 != _keep_0_2_113 && _old_122 != _keep_1_0_114 && _old_122 != _keep_1_1_115 && _old_122 != _keep_1_2_116) {
                            LIR_unload_subject(_old_122);
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
                    lir_subject_t* _src_1_255 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_256 = lh->op;
                    lir_operation_t _match_op_1_257 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_258 = lh->farg;
                    lir_subject_t* _keep_0_1_259 = lh->sarg;
                    lir_subject_t* _keep_0_2_260 = lh->targ;
                    lir_subject_t* _keep_1_0_261 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_262 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_263 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_264 = lh->sarg;
                    if (_old_264 != _src_1_255) {
                        lh->sarg = _src_1_255;
                        optimized = 1;
                        if (_old_264 && _old_264 != lh->farg && _old_264 != lh->sarg && _old_264 != lh->targ && _old_264 != _src_1_255 && _old_264 != _keep_0_0_258 && _old_264 != _keep_0_1_259 && _old_264 != _keep_0_2_260 && _old_264 != _keep_1_0_261 && _old_264 != _keep_1_1_262 && _old_264 != _keep_1_2_263) {
                            LIR_unload_subject(_old_264);
                        }
                    }
                    lir_subject_t* _old_265 = lh->targ;
                    if (_old_265 != _src_1_255) {
                        lh->targ = _src_1_255;
                        optimized = 1;
                        if (_old_265 && _old_265 != lh->farg && _old_265 != lh->sarg && _old_265 != lh->targ && _old_265 != _src_1_255 && _old_265 != _keep_0_0_258 && _old_265 != _keep_0_1_259 && _old_265 != _keep_0_2_260 && _old_265 != _keep_1_0_261 && _old_265 != _keep_1_1_262 && _old_265 != _keep_1_2_263) {
                            LIR_unload_subject(_old_265);
                        }
                    }
                    lir_subject_t* _old_266 = lh->farg;
                    if (_old_266 != _src_1_255) {
                        lh->farg = _src_1_255;
                        optimized = 1;
                        if (_old_266 && _old_266 != lh->farg && _old_266 != lh->sarg && _old_266 != lh->targ && _old_266 != _src_1_255 && _old_266 != _keep_0_0_258 && _old_266 != _keep_0_1_259 && _old_266 != _keep_0_2_260 && _old_266 != _keep_1_0_261 && _old_266 != _keep_1_1_262 && _old_266 != _keep_1_2_263) {
                            LIR_unload_subject(_old_266);
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
                    lir_subject_t* _src_1_267 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_268 = lh->op;
                    lir_operation_t _match_op_1_269 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_270 = lh->farg;
                    lir_subject_t* _keep_0_1_271 = lh->sarg;
                    lir_subject_t* _keep_0_2_272 = lh->targ;
                    lir_subject_t* _keep_1_0_273 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_274 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_275 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_276 = lh->sarg;
                    if (_old_276 != _src_1_267) {
                        lh->sarg = _src_1_267;
                        optimized = 1;
                        if (_old_276 && _old_276 != lh->farg && _old_276 != lh->sarg && _old_276 != lh->targ && _old_276 != _src_1_267 && _old_276 != _keep_0_0_270 && _old_276 != _keep_0_1_271 && _old_276 != _keep_0_2_272 && _old_276 != _keep_1_0_273 && _old_276 != _keep_1_1_274 && _old_276 != _keep_1_2_275) {
                            LIR_unload_subject(_old_276);
                        }
                    }
                    lir_subject_t* _old_277 = lh->targ;
                    if (_old_277 != _src_1_267) {
                        lh->targ = _src_1_267;
                        optimized = 1;
                        if (_old_277 && _old_277 != lh->farg && _old_277 != lh->sarg && _old_277 != lh->targ && _old_277 != _src_1_267 && _old_277 != _keep_0_0_270 && _old_277 != _keep_0_1_271 && _old_277 != _keep_0_2_272 && _old_277 != _keep_1_0_273 && _old_277 != _keep_1_1_274 && _old_277 != _keep_1_2_275) {
                            LIR_unload_subject(_old_277);
                        }
                    }
                    lir_subject_t* _old_278 = lh->farg;
                    if (_old_278 != _src_1_267) {
                        lh->farg = _src_1_267;
                        optimized = 1;
                        if (_old_278 && _old_278 != lh->farg && _old_278 != lh->sarg && _old_278 != lh->targ && _old_278 != _src_1_267 && _old_278 != _keep_0_0_270 && _old_278 != _keep_0_1_271 && _old_278 != _keep_0_2_272 && _old_278 != _keep_1_0_273 && _old_278 != _keep_1_1_274 && _old_278 != _keep_1_2_275) {
                            LIR_unload_subject(_old_278);
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
                    lir_subject_t* _src_1_287 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_288 = lh->op;
                    lir_operation_t _match_op_1_289 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_290 = lh->farg;
                    lir_subject_t* _keep_0_1_291 = lh->sarg;
                    lir_subject_t* _keep_0_2_292 = lh->targ;
                    lir_subject_t* _keep_1_0_293 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_294 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_295 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_296 = lh->sarg;
                    if (_old_296 != _src_1_287) {
                        lh->sarg = _src_1_287;
                        optimized = 1;
                        if (_old_296 && _old_296 != lh->farg && _old_296 != lh->sarg && _old_296 != lh->targ && _old_296 != _src_1_287 && _old_296 != _keep_0_0_290 && _old_296 != _keep_0_1_291 && _old_296 != _keep_0_2_292 && _old_296 != _keep_1_0_293 && _old_296 != _keep_1_1_294 && _old_296 != _keep_1_2_295) {
                            LIR_unload_subject(_old_296);
                        }
                    }
                    lir_subject_t* _old_297 = lh->targ;
                    if (_old_297 != _src_1_287) {
                        lh->targ = _src_1_287;
                        optimized = 1;
                        if (_old_297 && _old_297 != lh->farg && _old_297 != lh->sarg && _old_297 != lh->targ && _old_297 != _src_1_287 && _old_297 != _keep_0_0_290 && _old_297 != _keep_0_1_291 && _old_297 != _keep_0_2_292 && _old_297 != _keep_1_0_293 && _old_297 != _keep_1_1_294 && _old_297 != _keep_1_2_295) {
                            LIR_unload_subject(_old_297);
                        }
                    }
                    lir_subject_t* _old_298 = lh->farg;
                    if (_old_298 != _src_1_287) {
                        lh->farg = _src_1_287;
                        optimized = 1;
                        if (_old_298 && _old_298 != lh->farg && _old_298 != lh->sarg && _old_298 != lh->targ && _old_298 != _src_1_287 && _old_298 != _keep_0_0_290 && _old_298 != _keep_0_1_291 && _old_298 != _keep_0_2_292 && _old_298 != _keep_1_0_293 && _old_298 != _keep_1_1_294 && _old_298 != _keep_1_2_295) {
                            LIR_unload_subject(_old_298);
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
                    lir_operation_t _match_op_0_225 = lh->op;
                    lir_subject_t* _keep_0_0_226 = lh->farg;
                    lir_subject_t* _keep_0_1_227 = lh->sarg;
                    lir_subject_t* _keep_0_2_228 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_229 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_229 && _old_229 != lh->farg && _old_229 != lh->sarg && _old_229 != lh->targ && _old_229 != _keep_0_0_226 && _old_229 != _keep_0_1_227 && _old_229 != _keep_0_2_228) {
                        LIR_unload_subject(_old_229);
                    }
                    lir_subject_t* _old_230 = lh->targ;
                    if (_old_230) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_230 && _old_230 != lh->farg && _old_230 != lh->sarg && _old_230 != lh->targ && _old_230 != _keep_0_0_226 && _old_230 != _keep_0_1_227 && _old_230 != _keep_0_2_228) {
                            LIR_unload_subject(_old_230);
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
                    lir_subject_t* _src_1_168 = lh->farg;
                    lir_operation_t _match_op_0_169 = lh->op;
                    lir_subject_t* _keep_0_0_170 = lh->farg;
                    lir_subject_t* _keep_0_1_171 = lh->sarg;
                    lir_subject_t* _keep_0_2_172 = lh->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_173 = lh->farg;
                    if (_old_173 != _src_1_168) {
                        lh->farg = _src_1_168;
                        optimized = 1;
                        if (_old_173 && _old_173 != lh->farg && _old_173 != lh->sarg && _old_173 != lh->targ && _old_173 != _src_1_168 && _old_173 != _keep_0_0_170 && _old_173 != _keep_0_1_171 && _old_173 != _keep_0_2_172) {
                            LIR_unload_subject(_old_173);
                        }
                    }
                    lir_subject_t* _old_174 = lh->sarg;
                    if (_old_174) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_174 && _old_174 != lh->farg && _old_174 != lh->sarg && _old_174 != lh->targ && _old_174 != _src_1_168 && _old_174 != _keep_0_0_170 && _old_174 != _keep_0_1_171 && _old_174 != _keep_0_2_172) {
                            LIR_unload_subject(_old_174);
                        }
                    }
                    lir_subject_t* _old_175 = lh->targ;
                    if (_old_175) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_175 && _old_175 != lh->farg && _old_175 != lh->sarg && _old_175 != lh->targ && _old_175 != _src_1_168 && _old_175 != _keep_0_0_170 && _old_175 != _keep_0_1_171 && _old_175 != _keep_0_2_172) {
                            LIR_unload_subject(_old_175);
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
                    lir_subject_t* _src_1_217 = lh->farg;
                    lir_operation_t _match_op_0_218 = lh->op;
                    lir_subject_t* _keep_0_0_219 = lh->farg;
                    lir_subject_t* _keep_0_1_220 = lh->sarg;
                    lir_subject_t* _keep_0_2_221 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_222 = lh->sarg;
                    if (_old_222 != _src_1_217) {
                        lh->sarg = _src_1_217;
                        optimized = 1;
                        if (_old_222 && _old_222 != lh->farg && _old_222 != lh->sarg && _old_222 != lh->targ && _old_222 != _src_1_217 && _old_222 != _keep_0_0_219 && _old_222 != _keep_0_1_220 && _old_222 != _keep_0_2_221) {
                            LIR_unload_subject(_old_222);
                        }
                    }
                    lir_subject_t* _old_223 = lh->targ;
                    if (_old_223 != _src_1_217) {
                        lh->targ = _src_1_217;
                        optimized = 1;
                        if (_old_223 && _old_223 != lh->farg && _old_223 != lh->sarg && _old_223 != lh->targ && _old_223 != _src_1_217 && _old_223 != _keep_0_0_219 && _old_223 != _keep_0_1_220 && _old_223 != _keep_0_2_221) {
                            LIR_unload_subject(_old_223);
                        }
                    }
                    lir_subject_t* _old_224 = lh->farg;
                    if (_old_224 != _src_1_217) {
                        lh->farg = _src_1_217;
                        optimized = 1;
                        if (_old_224 && _old_224 != lh->farg && _old_224 != lh->sarg && _old_224 != lh->targ && _old_224 != _src_1_217 && _old_224 != _keep_0_0_219 && _old_224 != _keep_0_1_220 && _old_224 != _keep_0_2_221) {
                            LIR_unload_subject(_old_224);
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