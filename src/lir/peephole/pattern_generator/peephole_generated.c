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
                    lir_subject_t* _src_1_209 = lh->farg;
                    lir_operation_t _match_op_0_210 = lh->op;
                    lir_subject_t* _keep_0_0_211 = lh->farg;
                    lir_subject_t* _keep_0_1_212 = lh->sarg;
                    lir_subject_t* _keep_0_2_213 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_214 = lh->farg;
                    if (_old_214 != _src_1_209) {
                        lh->farg = _src_1_209;
                        optimized = 1;
                        if (_old_214 && _old_214 != lh->farg && _old_214 != lh->sarg && _old_214 != lh->targ && _old_214 != _src_1_209 && _old_214 != _keep_0_0_211 && _old_214 != _keep_0_1_212 && _old_214 != _keep_0_2_213) {
                            LIR_unload_subject(_old_214);
                        }
                    }
                    lir_subject_t* _old_215 = lh->sarg;
                    if (_old_215 != _src_1_209) {
                        lh->sarg = _src_1_209;
                        optimized = 1;
                        if (_old_215 && _old_215 != lh->farg && _old_215 != lh->sarg && _old_215 != lh->targ && _old_215 != _src_1_209 && _old_215 != _keep_0_0_211 && _old_215 != _keep_0_1_212 && _old_215 != _keep_0_2_213) {
                            LIR_unload_subject(_old_215);
                        }
                    }
                    lir_subject_t* _old_216 = lh->targ;
                    if (_old_216) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_216 && _old_216 != lh->farg && _old_216 != lh->sarg && _old_216 != lh->targ && _old_216 != _src_1_209 && _old_216 != _keep_0_0_211 && _old_216 != _keep_0_1_212 && _old_216 != _keep_0_2_213) {
                            LIR_unload_subject(_old_216);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_13 = lh->farg;
                    lir_subject_t* _src_2_14 = lh->sarg;
                    lir_operation_t _match_op_0_15 = lh->op;
                    lir_operation_t _match_op_1_16 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_17 = lh->farg;
                    lir_subject_t* _keep_0_1_18 = lh->sarg;
                    lir_subject_t* _keep_0_2_19 = lh->targ;
                    lir_subject_t* _keep_1_0_20 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_21 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_22 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_23 = lh->farg;
                    if (_old_23 != _src_2_14) {
                        lh->farg = _src_2_14;
                        optimized = 1;
                        if (_old_23 && _old_23 != lh->farg && _old_23 != lh->sarg && _old_23 != lh->targ && _old_23 != _src_1_13 && _old_23 != _src_2_14 && _old_23 != _keep_0_0_17 && _old_23 != _keep_0_1_18 && _old_23 != _keep_0_2_19 && _old_23 != _keep_1_0_20 && _old_23 != _keep_1_1_21 && _old_23 != _keep_1_2_22) {
                            LIR_unload_subject(_old_23);
                        }
                    }
                    lir_subject_t* _old_24 = lh->sarg;
                    if (_old_24 != _src_2_14) {
                        lh->sarg = _src_2_14;
                        optimized = 1;
                        if (_old_24 && _old_24 != lh->farg && _old_24 != lh->sarg && _old_24 != lh->targ && _old_24 != _src_1_13 && _old_24 != _src_2_14 && _old_24 != _keep_0_0_17 && _old_24 != _keep_0_1_18 && _old_24 != _keep_0_2_19 && _old_24 != _keep_1_0_20 && _old_24 != _keep_1_1_21 && _old_24 != _keep_1_2_22) {
                            LIR_unload_subject(_old_24);
                        }
                    }
                    lir_subject_t* _old_25 = lh->targ;
                    if (_old_25) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_25 && _old_25 != lh->farg && _old_25 != lh->sarg && _old_25 != lh->targ && _old_25 != _src_1_13 && _old_25 != _src_2_14 && _old_25 != _keep_0_0_17 && _old_25 != _keep_0_1_18 && _old_25 != _keep_0_2_19 && _old_25 != _keep_1_0_20 && _old_25 != _keep_1_1_21 && _old_25 != _keep_1_2_22) {
                            LIR_unload_subject(_old_25);
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
                    lir_subject_t* _src_1_26 = lh->farg;
                    lir_subject_t* _src_2_27 = lh->sarg;
                    lir_operation_t _match_op_0_28 = lh->op;
                    lir_operation_t _match_op_1_29 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_30 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_operation_t _match_op_3_31 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op;
                    lir_subject_t* _keep_0_0_32 = lh->farg;
                    lir_subject_t* _keep_0_1_33 = lh->sarg;
                    lir_subject_t* _keep_0_2_34 = lh->targ;
                    lir_subject_t* _keep_1_0_35 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_36 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_37 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_38 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_39 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_40 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _keep_3_0_41 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_subject_t* _keep_3_1_42 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _keep_3_2_43 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_44 = lh->sarg;
                    if (_old_44 != _src_2_27) {
                        lh->sarg = _src_2_27;
                        optimized = 1;
                        if (_old_44 && _old_44 != lh->farg && _old_44 != lh->sarg && _old_44 != lh->targ && _old_44 != _src_1_26 && _old_44 != _src_2_27 && _old_44 != _keep_0_0_32 && _old_44 != _keep_0_1_33 && _old_44 != _keep_0_2_34 && _old_44 != _keep_1_0_35 && _old_44 != _keep_1_1_36 && _old_44 != _keep_1_2_37 && _old_44 != _keep_2_0_38 && _old_44 != _keep_2_1_39 && _old_44 != _keep_2_2_40 && _old_44 != _keep_3_0_41 && _old_44 != _keep_3_1_42 && _old_44 != _keep_3_2_43) {
                            LIR_unload_subject(_old_44);
                        }
                    }
                    lir_subject_t* _old_45 = lh->targ;
                    if (_old_45 != _src_2_27) {
                        lh->targ = _src_2_27;
                        optimized = 1;
                        if (_old_45 && _old_45 != lh->farg && _old_45 != lh->sarg && _old_45 != lh->targ && _old_45 != _src_1_26 && _old_45 != _src_2_27 && _old_45 != _keep_0_0_32 && _old_45 != _keep_0_1_33 && _old_45 != _keep_0_2_34 && _old_45 != _keep_1_0_35 && _old_45 != _keep_1_1_36 && _old_45 != _keep_1_2_37 && _old_45 != _keep_2_0_38 && _old_45 != _keep_2_1_39 && _old_45 != _keep_2_2_40 && _old_45 != _keep_3_0_41 && _old_45 != _keep_3_1_42 && _old_45 != _keep_3_2_43) {
                            LIR_unload_subject(_old_45);
                        }
                    }
                    lir_subject_t* _old_46 = lh->farg;
                    if (_old_46 != _src_2_27) {
                        lh->farg = _src_2_27;
                        optimized = 1;
                        if (_old_46 && _old_46 != lh->farg && _old_46 != lh->sarg && _old_46 != lh->targ && _old_46 != _src_1_26 && _old_46 != _src_2_27 && _old_46 != _keep_0_0_32 && _old_46 != _keep_0_1_33 && _old_46 != _keep_0_2_34 && _old_46 != _keep_1_0_35 && _old_46 != _keep_1_1_36 && _old_46 != _keep_1_2_37 && _old_46 != _keep_2_0_38 && _old_46 != _keep_2_1_39 && _old_46 != _keep_2_2_40 && _old_46 != _keep_3_0_41 && _old_46 != _keep_3_1_42 && _old_46 != _keep_3_2_43) {
                            LIR_unload_subject(_old_46);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_28) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_28;
                        optimized = 1;
                    }
                    lir_subject_t* _old_47 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_47 != _src_1_26) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_26;
                        optimized = 1;
                        if (_old_47 && _old_47 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_47 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_47 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_47 != _src_1_26 && _old_47 != _src_2_27 && _old_47 != _keep_0_0_32 && _old_47 != _keep_0_1_33 && _old_47 != _keep_0_2_34 && _old_47 != _keep_1_0_35 && _old_47 != _keep_1_1_36 && _old_47 != _keep_1_2_37 && _old_47 != _keep_2_0_38 && _old_47 != _keep_2_1_39 && _old_47 != _keep_2_2_40 && _old_47 != _keep_3_0_41 && _old_47 != _keep_3_1_42 && _old_47 != _keep_3_2_43) {
                            LIR_unload_subject(_old_47);
                        }
                    }
                    lir_subject_t* _old_48 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_48 != _src_1_26) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_26;
                        optimized = 1;
                        if (_old_48 && _old_48 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_48 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_48 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_48 != _src_1_26 && _old_48 != _src_2_27 && _old_48 != _keep_0_0_32 && _old_48 != _keep_0_1_33 && _old_48 != _keep_0_2_34 && _old_48 != _keep_1_0_35 && _old_48 != _keep_1_1_36 && _old_48 != _keep_1_2_37 && _old_48 != _keep_2_0_38 && _old_48 != _keep_2_1_39 && _old_48 != _keep_2_2_40 && _old_48 != _keep_3_0_41 && _old_48 != _keep_3_1_42 && _old_48 != _keep_3_2_43) {
                            LIR_unload_subject(_old_48);
                        }
                    }
                    lir_subject_t* _old_49 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_49) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_49 && _old_49 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_49 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_49 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_49 != _src_1_26 && _old_49 != _src_2_27 && _old_49 != _keep_0_0_32 && _old_49 != _keep_0_1_33 && _old_49 != _keep_0_2_34 && _old_49 != _keep_1_0_35 && _old_49 != _keep_1_1_36 && _old_49 != _keep_1_2_37 && _old_49 != _keep_2_0_38 && _old_49 != _keep_2_1_39 && _old_49 != _keep_2_2_40 && _old_49 != _keep_3_0_41 && _old_49 != _keep_3_1_42 && _old_49 != _keep_3_2_43) {
                            LIR_unload_subject(_old_49);
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
                    lir_subject_t* _src_1_50 = lh->farg;
                    lir_operation_t _match_op_0_51 = lh->op;
                    lir_subject_t* _keep_0_0_52 = lh->farg;
                    lir_subject_t* _keep_0_1_53 = lh->sarg;
                    lir_subject_t* _keep_0_2_54 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_55 = lh->sarg;
                    if (_old_55 != _src_1_50) {
                        lh->sarg = _src_1_50;
                        optimized = 1;
                        if (_old_55 && _old_55 != lh->farg && _old_55 != lh->sarg && _old_55 != lh->targ && _old_55 != _src_1_50 && _old_55 != _keep_0_0_52 && _old_55 != _keep_0_1_53 && _old_55 != _keep_0_2_54) {
                            LIR_unload_subject(_old_55);
                        }
                    }
                    lir_subject_t* _old_56 = lh->targ;
                    if (_old_56 != _src_1_50) {
                        lh->targ = _src_1_50;
                        optimized = 1;
                        if (_old_56 && _old_56 != lh->farg && _old_56 != lh->sarg && _old_56 != lh->targ && _old_56 != _src_1_50 && _old_56 != _keep_0_0_52 && _old_56 != _keep_0_1_53 && _old_56 != _keep_0_2_54) {
                            LIR_unload_subject(_old_56);
                        }
                    }
                    lir_subject_t* _old_57 = lh->farg;
                    if (_old_57 != _src_1_50) {
                        lh->farg = _src_1_50;
                        optimized = 1;
                        if (_old_57 && _old_57 != lh->farg && _old_57 != lh->sarg && _old_57 != lh->targ && _old_57 != _src_1_50 && _old_57 != _keep_0_0_52 && _old_57 != _keep_0_1_53 && _old_57 != _keep_0_2_54) {
                            LIR_unload_subject(_old_57);
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
                    lir_subject_t* _src_1_58 = lh->farg;
                    lir_subject_t* _src_2_59 = lh->sarg;
                    lir_operation_t _match_op_0_60 = lh->op;
                    lir_operation_t _match_op_1_61 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_62 = lh->farg;
                    lir_subject_t* _keep_0_1_63 = lh->sarg;
                    lir_subject_t* _keep_0_2_64 = lh->targ;
                    lir_subject_t* _keep_1_0_65 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_66 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_67 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_60) {
                        lh->op = _match_op_0_60;
                        optimized = 1;
                    }
                    lir_subject_t* _old_68 = lh->farg;
                    if (_old_68 != _src_1_58) {
                        lh->farg = _src_1_58;
                        optimized = 1;
                        if (_old_68 && _old_68 != lh->farg && _old_68 != lh->sarg && _old_68 != lh->targ && _old_68 != _src_1_58 && _old_68 != _src_2_59 && _old_68 != _keep_0_0_62 && _old_68 != _keep_0_1_63 && _old_68 != _keep_0_2_64 && _old_68 != _keep_1_0_65 && _old_68 != _keep_1_1_66 && _old_68 != _keep_1_2_67) {
                            LIR_unload_subject(_old_68);
                        }
                    }
                    lir_subject_t* _old_69 = lh->sarg;
                    if (_old_69 != _src_2_59) {
                        lh->sarg = _src_2_59;
                        optimized = 1;
                        if (_old_69 && _old_69 != lh->farg && _old_69 != lh->sarg && _old_69 != lh->targ && _old_69 != _src_1_58 && _old_69 != _src_2_59 && _old_69 != _keep_0_0_62 && _old_69 != _keep_0_1_63 && _old_69 != _keep_0_2_64 && _old_69 != _keep_1_0_65 && _old_69 != _keep_1_1_66 && _old_69 != _keep_1_2_67) {
                            LIR_unload_subject(_old_69);
                        }
                    }
                    lir_subject_t* _old_70 = lh->targ;
                    if (_old_70) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_70 && _old_70 != lh->farg && _old_70 != lh->sarg && _old_70 != lh->targ && _old_70 != _src_1_58 && _old_70 != _src_2_59 && _old_70 != _keep_0_0_62 && _old_70 != _keep_0_1_63 && _old_70 != _keep_0_2_64 && _old_70 != _keep_1_0_65 && _old_70 != _keep_1_1_66 && _old_70 != _keep_1_2_67) {
                            LIR_unload_subject(_old_70);
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
                    lir_subject_t* _src_1_71 = lh->farg;
                    lir_subject_t* _src_2_72 = lh->sarg;
                    lir_subject_t* _src_3_73 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_74 = lh->op;
                    lir_operation_t _match_op_1_75 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_76 = lh->farg;
                    lir_subject_t* _keep_0_1_77 = lh->sarg;
                    lir_subject_t* _keep_0_2_78 = lh->targ;
                    lir_subject_t* _keep_1_0_79 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_80 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_81 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_75) {
                        lh->op = _match_op_1_75;
                        optimized = 1;
                    }
                    lir_subject_t* _old_82 = lh->farg;
                    if (_old_82 != _src_2_72) {
                        lh->farg = _src_2_72;
                        optimized = 1;
                        if (_old_82 && _old_82 != lh->farg && _old_82 != lh->sarg && _old_82 != lh->targ && _old_82 != _src_1_71 && _old_82 != _src_2_72 && _old_82 != _src_3_73 && _old_82 != _keep_0_0_76 && _old_82 != _keep_0_1_77 && _old_82 != _keep_0_2_78 && _old_82 != _keep_1_0_79 && _old_82 != _keep_1_1_80 && _old_82 != _keep_1_2_81) {
                            LIR_unload_subject(_old_82);
                        }
                    }
                    lir_subject_t* _old_83 = lh->sarg;
                    if (_old_83 != _src_3_73) {
                        lh->sarg = _src_3_73;
                        optimized = 1;
                        if (_old_83 && _old_83 != lh->farg && _old_83 != lh->sarg && _old_83 != lh->targ && _old_83 != _src_1_71 && _old_83 != _src_2_72 && _old_83 != _src_3_73 && _old_83 != _keep_0_0_76 && _old_83 != _keep_0_1_77 && _old_83 != _keep_0_2_78 && _old_83 != _keep_1_0_79 && _old_83 != _keep_1_1_80 && _old_83 != _keep_1_2_81) {
                            LIR_unload_subject(_old_83);
                        }
                    }
                    lir_subject_t* _old_84 = lh->targ;
                    if (_old_84) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_84 && _old_84 != lh->farg && _old_84 != lh->sarg && _old_84 != lh->targ && _old_84 != _src_1_71 && _old_84 != _src_2_72 && _old_84 != _src_3_73 && _old_84 != _keep_0_0_76 && _old_84 != _keep_0_1_77 && _old_84 != _keep_0_2_78 && _old_84 != _keep_1_0_79 && _old_84 != _keep_1_1_80 && _old_84 != _keep_1_2_81) {
                            LIR_unload_subject(_old_84);
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
                    lir_subject_t* _src_1_101 = lh->farg;
                    lir_operation_t _match_op_0_102 = lh->op;
                    lir_operation_t _match_op_1_103 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_104 = lh->farg;
                    lir_subject_t* _keep_0_1_105 = lh->sarg;
                    lir_subject_t* _keep_0_2_106 = lh->targ;
                    lir_subject_t* _keep_1_0_107 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_108 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_109 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_110 = lh->sarg;
                    if (_old_110 != _src_1_101) {
                        lh->sarg = _src_1_101;
                        optimized = 1;
                        if (_old_110 && _old_110 != lh->farg && _old_110 != lh->sarg && _old_110 != lh->targ && _old_110 != _src_1_101 && _old_110 != _keep_0_0_104 && _old_110 != _keep_0_1_105 && _old_110 != _keep_0_2_106 && _old_110 != _keep_1_0_107 && _old_110 != _keep_1_1_108 && _old_110 != _keep_1_2_109) {
                            LIR_unload_subject(_old_110);
                        }
                    }
                    lir_subject_t* _old_111 = lh->targ;
                    if (_old_111 != _src_1_101) {
                        lh->targ = _src_1_101;
                        optimized = 1;
                        if (_old_111 && _old_111 != lh->farg && _old_111 != lh->sarg && _old_111 != lh->targ && _old_111 != _src_1_101 && _old_111 != _keep_0_0_104 && _old_111 != _keep_0_1_105 && _old_111 != _keep_0_2_106 && _old_111 != _keep_1_0_107 && _old_111 != _keep_1_1_108 && _old_111 != _keep_1_2_109) {
                            LIR_unload_subject(_old_111);
                        }
                    }
                    lir_subject_t* _old_112 = lh->farg;
                    if (_old_112 != _src_1_101) {
                        lh->farg = _src_1_101;
                        optimized = 1;
                        if (_old_112 && _old_112 != lh->farg && _old_112 != lh->sarg && _old_112 != lh->targ && _old_112 != _src_1_101 && _old_112 != _keep_0_0_104 && _old_112 != _keep_0_1_105 && _old_112 != _keep_0_2_106 && _old_112 != _keep_1_0_107 && _old_112 != _keep_1_1_108 && _old_112 != _keep_1_2_109) {
                            LIR_unload_subject(_old_112);
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
                    lir_subject_t* _src_1_113 = lh->farg;
                    lir_operation_t _match_op_0_114 = lh->op;
                    lir_operation_t _match_op_1_115 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_116 = lh->farg;
                    lir_subject_t* _keep_0_1_117 = lh->sarg;
                    lir_subject_t* _keep_0_2_118 = lh->targ;
                    lir_subject_t* _keep_1_0_119 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_120 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_121 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_122 = lh->sarg;
                    if (_old_122 != _src_1_113) {
                        lh->sarg = _src_1_113;
                        optimized = 1;
                        if (_old_122 && _old_122 != lh->farg && _old_122 != lh->sarg && _old_122 != lh->targ && _old_122 != _src_1_113 && _old_122 != _keep_0_0_116 && _old_122 != _keep_0_1_117 && _old_122 != _keep_0_2_118 && _old_122 != _keep_1_0_119 && _old_122 != _keep_1_1_120 && _old_122 != _keep_1_2_121) {
                            LIR_unload_subject(_old_122);
                        }
                    }
                    lir_subject_t* _old_123 = lh->targ;
                    if (_old_123 != _src_1_113) {
                        lh->targ = _src_1_113;
                        optimized = 1;
                        if (_old_123 && _old_123 != lh->farg && _old_123 != lh->sarg && _old_123 != lh->targ && _old_123 != _src_1_113 && _old_123 != _keep_0_0_116 && _old_123 != _keep_0_1_117 && _old_123 != _keep_0_2_118 && _old_123 != _keep_1_0_119 && _old_123 != _keep_1_1_120 && _old_123 != _keep_1_2_121) {
                            LIR_unload_subject(_old_123);
                        }
                    }
                    lir_subject_t* _old_124 = lh->farg;
                    if (_old_124 != _src_1_113) {
                        lh->farg = _src_1_113;
                        optimized = 1;
                        if (_old_124 && _old_124 != lh->farg && _old_124 != lh->sarg && _old_124 != lh->targ && _old_124 != _src_1_113 && _old_124 != _keep_0_0_116 && _old_124 != _keep_0_1_117 && _old_124 != _keep_0_2_118 && _old_124 != _keep_1_0_119 && _old_124 != _keep_1_1_120 && _old_124 != _keep_1_2_121) {
                            LIR_unload_subject(_old_124);
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
                    lir_subject_t* _src_1_125 = lh->farg;
                    lir_subject_t* _src_2_126 = lh->sarg;
                    lir_subject_t* _src_3_127 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_128 = lh->op;
                    lir_operation_t _match_op_1_129 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_130 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_131 = lh->farg;
                    lir_subject_t* _keep_0_1_132 = lh->sarg;
                    lir_subject_t* _keep_0_2_133 = lh->targ;
                    lir_subject_t* _keep_1_0_134 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_135 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_136 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_137 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_138 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_139 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_128) {
                        lh->op = _match_op_0_128;
                        optimized = 1;
                    }
                    lir_subject_t* _old_140 = lh->farg;
                    if (_old_140 != _src_1_125) {
                        lh->farg = _src_1_125;
                        optimized = 1;
                        if (_old_140 && _old_140 != lh->farg && _old_140 != lh->sarg && _old_140 != lh->targ && _old_140 != _src_1_125 && _old_140 != _src_2_126 && _old_140 != _src_3_127 && _old_140 != _keep_0_0_131 && _old_140 != _keep_0_1_132 && _old_140 != _keep_0_2_133 && _old_140 != _keep_1_0_134 && _old_140 != _keep_1_1_135 && _old_140 != _keep_1_2_136 && _old_140 != _keep_2_0_137 && _old_140 != _keep_2_1_138 && _old_140 != _keep_2_2_139) {
                            LIR_unload_subject(_old_140);
                        }
                    }
                    lir_subject_t* _old_141 = lh->sarg;
                    if (_old_141 != _src_2_126) {
                        lh->sarg = _src_2_126;
                        optimized = 1;
                        if (_old_141 && _old_141 != lh->farg && _old_141 != lh->sarg && _old_141 != lh->targ && _old_141 != _src_1_125 && _old_141 != _src_2_126 && _old_141 != _src_3_127 && _old_141 != _keep_0_0_131 && _old_141 != _keep_0_1_132 && _old_141 != _keep_0_2_133 && _old_141 != _keep_1_0_134 && _old_141 != _keep_1_1_135 && _old_141 != _keep_1_2_136 && _old_141 != _keep_2_0_137 && _old_141 != _keep_2_1_138 && _old_141 != _keep_2_2_139) {
                            LIR_unload_subject(_old_141);
                        }
                    }
                    lir_subject_t* _old_142 = lh->targ;
                    if (_old_142) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_142 && _old_142 != lh->farg && _old_142 != lh->sarg && _old_142 != lh->targ && _old_142 != _src_1_125 && _old_142 != _src_2_126 && _old_142 != _src_3_127 && _old_142 != _keep_0_0_131 && _old_142 != _keep_0_1_132 && _old_142 != _keep_0_2_133 && _old_142 != _keep_1_0_134 && _old_142 != _keep_1_1_135 && _old_142 != _keep_1_2_136 && _old_142 != _keep_2_0_137 && _old_142 != _keep_2_1_138 && _old_142 != _keep_2_2_139) {
                            LIR_unload_subject(_old_142);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_143 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_143 != _src_2_126) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_126;
                        optimized = 1;
                        if (_old_143 && _old_143 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_143 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_143 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_143 != _src_1_125 && _old_143 != _src_2_126 && _old_143 != _src_3_127 && _old_143 != _keep_0_0_131 && _old_143 != _keep_0_1_132 && _old_143 != _keep_0_2_133 && _old_143 != _keep_1_0_134 && _old_143 != _keep_1_1_135 && _old_143 != _keep_1_2_136 && _old_143 != _keep_2_0_137 && _old_143 != _keep_2_1_138 && _old_143 != _keep_2_2_139) {
                            LIR_unload_subject(_old_143);
                        }
                    }
                    lir_subject_t* _old_144 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_144 != _src_3_127) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_3_127;
                        optimized = 1;
                        if (_old_144 && _old_144 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_144 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_144 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_144 != _src_1_125 && _old_144 != _src_2_126 && _old_144 != _src_3_127 && _old_144 != _keep_0_0_131 && _old_144 != _keep_0_1_132 && _old_144 != _keep_0_2_133 && _old_144 != _keep_1_0_134 && _old_144 != _keep_1_1_135 && _old_144 != _keep_1_2_136 && _old_144 != _keep_2_0_137 && _old_144 != _keep_2_1_138 && _old_144 != _keep_2_2_139) {
                            LIR_unload_subject(_old_144);
                        }
                    }
                    lir_subject_t* _old_145 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_145 != _src_2_126) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_126;
                        optimized = 1;
                        if (_old_145 && _old_145 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_145 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_145 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_145 != _src_1_125 && _old_145 != _src_2_126 && _old_145 != _src_3_127 && _old_145 != _keep_0_0_131 && _old_145 != _keep_0_1_132 && _old_145 != _keep_0_2_133 && _old_145 != _keep_1_0_134 && _old_145 != _keep_1_1_135 && _old_145 != _keep_1_2_136 && _old_145 != _keep_2_0_137 && _old_145 != _keep_2_1_138 && _old_145 != _keep_2_2_139) {
                            LIR_unload_subject(_old_145);
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
                    lir_subject_t* _src_1_154 = lh->farg;
                    lir_subject_t* _src_2_155 = lh->sarg;
                    lir_subject_t* _src_3_156 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_157 = lh->op;
                    lir_operation_t _match_op_1_158 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_159 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_160 = lh->farg;
                    lir_subject_t* _keep_0_1_161 = lh->sarg;
                    lir_subject_t* _keep_0_2_162 = lh->targ;
                    lir_subject_t* _keep_1_0_163 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_164 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_165 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_166 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_167 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_168 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_169 = lh->farg;
                    if (_old_169 != _src_2_155) {
                        lh->farg = _src_2_155;
                        optimized = 1;
                        if (_old_169 && _old_169 != lh->farg && _old_169 != lh->sarg && _old_169 != lh->targ && _old_169 != _src_1_154 && _old_169 != _src_2_155 && _old_169 != _src_3_156 && _old_169 != _keep_0_0_160 && _old_169 != _keep_0_1_161 && _old_169 != _keep_0_2_162 && _old_169 != _keep_1_0_163 && _old_169 != _keep_1_1_164 && _old_169 != _keep_1_2_165 && _old_169 != _keep_2_0_166 && _old_169 != _keep_2_1_167 && _old_169 != _keep_2_2_168) {
                            LIR_unload_subject(_old_169);
                        }
                    }
                    lir_subject_t* _old_170 = lh->sarg;
                    if (_old_170) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_170 && _old_170 != lh->farg && _old_170 != lh->sarg && _old_170 != lh->targ && _old_170 != _src_1_154 && _old_170 != _src_2_155 && _old_170 != _src_3_156 && _old_170 != _keep_0_0_160 && _old_170 != _keep_0_1_161 && _old_170 != _keep_0_2_162 && _old_170 != _keep_1_0_163 && _old_170 != _keep_1_1_164 && _old_170 != _keep_1_2_165 && _old_170 != _keep_2_0_166 && _old_170 != _keep_2_1_167 && _old_170 != _keep_2_2_168) {
                            LIR_unload_subject(_old_170);
                        }
                    }
                    lir_subject_t* _old_171 = lh->targ;
                    if (_old_171) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_171 && _old_171 != lh->farg && _old_171 != lh->sarg && _old_171 != lh->targ && _old_171 != _src_1_154 && _old_171 != _src_2_155 && _old_171 != _src_3_156 && _old_171 != _keep_0_0_160 && _old_171 != _keep_0_1_161 && _old_171 != _keep_0_2_162 && _old_171 != _keep_1_0_163 && _old_171 != _keep_1_1_164 && _old_171 != _keep_1_2_165 && _old_171 != _keep_2_0_166 && _old_171 != _keep_2_1_167 && _old_171 != _keep_2_2_168) {
                            LIR_unload_subject(_old_171);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_157) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_157;
                        optimized = 1;
                    }
                    lir_subject_t* _old_172 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_172 != _src_3_156) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_156;
                        optimized = 1;
                        if (_old_172 && _old_172 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_172 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_172 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_172 != _src_1_154 && _old_172 != _src_2_155 && _old_172 != _src_3_156 && _old_172 != _keep_0_0_160 && _old_172 != _keep_0_1_161 && _old_172 != _keep_0_2_162 && _old_172 != _keep_1_0_163 && _old_172 != _keep_1_1_164 && _old_172 != _keep_1_2_165 && _old_172 != _keep_2_0_166 && _old_172 != _keep_2_1_167 && _old_172 != _keep_2_2_168) {
                            LIR_unload_subject(_old_172);
                        }
                    }
                    lir_subject_t* _old_173 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_173 != _src_2_155) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_155;
                        optimized = 1;
                        if (_old_173 && _old_173 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_173 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_173 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_173 != _src_1_154 && _old_173 != _src_2_155 && _old_173 != _src_3_156 && _old_173 != _keep_0_0_160 && _old_173 != _keep_0_1_161 && _old_173 != _keep_0_2_162 && _old_173 != _keep_1_0_163 && _old_173 != _keep_1_1_164 && _old_173 != _keep_1_2_165 && _old_173 != _keep_2_0_166 && _old_173 != _keep_2_1_167 && _old_173 != _keep_2_2_168) {
                            LIR_unload_subject(_old_173);
                        }
                    }
                    lir_subject_t* _old_174 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_174) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_174 && _old_174 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_174 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_174 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_174 != _src_1_154 && _old_174 != _src_2_155 && _old_174 != _src_3_156 && _old_174 != _keep_0_0_160 && _old_174 != _keep_0_1_161 && _old_174 != _keep_0_2_162 && _old_174 != _keep_1_0_163 && _old_174 != _keep_1_1_164 && _old_174 != _keep_1_2_165 && _old_174 != _keep_2_0_166 && _old_174 != _keep_2_1_167 && _old_174 != _keep_2_2_168) {
                            LIR_unload_subject(_old_174);
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
                    lir_subject_t* _src_2_175 = lh->farg;
                    lir_subject_t* _src_1_176 = lh->sarg;
                    lir_operation_t _match_op_0_177 = lh->op;
                    lir_operation_t _match_op_1_178 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_179 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_180 = lh->farg;
                    lir_subject_t* _keep_0_1_181 = lh->sarg;
                    lir_subject_t* _keep_0_2_182 = lh->targ;
                    lir_subject_t* _keep_1_0_183 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_184 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_185 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_186 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_187 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_188 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_189 = lh->farg;
                    if (_old_189 != _src_1_176) {
                        lh->farg = _src_1_176;
                        optimized = 1;
                        if (_old_189 && _old_189 != lh->farg && _old_189 != lh->sarg && _old_189 != lh->targ && _old_189 != _src_2_175 && _old_189 != _src_1_176 && _old_189 != _keep_0_0_180 && _old_189 != _keep_0_1_181 && _old_189 != _keep_0_2_182 && _old_189 != _keep_1_0_183 && _old_189 != _keep_1_1_184 && _old_189 != _keep_1_2_185 && _old_189 != _keep_2_0_186 && _old_189 != _keep_2_1_187 && _old_189 != _keep_2_2_188) {
                            LIR_unload_subject(_old_189);
                        }
                    }
                    lir_subject_t* _old_190 = lh->sarg;
                    if (_old_190) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_190 && _old_190 != lh->farg && _old_190 != lh->sarg && _old_190 != lh->targ && _old_190 != _src_2_175 && _old_190 != _src_1_176 && _old_190 != _keep_0_0_180 && _old_190 != _keep_0_1_181 && _old_190 != _keep_0_2_182 && _old_190 != _keep_1_0_183 && _old_190 != _keep_1_1_184 && _old_190 != _keep_1_2_185 && _old_190 != _keep_2_0_186 && _old_190 != _keep_2_1_187 && _old_190 != _keep_2_2_188) {
                            LIR_unload_subject(_old_190);
                        }
                    }
                    lir_subject_t* _old_191 = lh->targ;
                    if (_old_191) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_191 && _old_191 != lh->farg && _old_191 != lh->sarg && _old_191 != lh->targ && _old_191 != _src_2_175 && _old_191 != _src_1_176 && _old_191 != _keep_0_0_180 && _old_191 != _keep_0_1_181 && _old_191 != _keep_0_2_182 && _old_191 != _keep_1_0_183 && _old_191 != _keep_1_1_184 && _old_191 != _keep_1_2_185 && _old_191 != _keep_2_0_186 && _old_191 != _keep_2_1_187 && _old_191 != _keep_2_2_188) {
                            LIR_unload_subject(_old_191);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_177) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_177;
                        optimized = 1;
                    }
                    lir_subject_t* _old_192 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_192 != _src_2_175) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_175;
                        optimized = 1;
                        if (_old_192 && _old_192 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_192 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_192 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_192 != _src_2_175 && _old_192 != _src_1_176 && _old_192 != _keep_0_0_180 && _old_192 != _keep_0_1_181 && _old_192 != _keep_0_2_182 && _old_192 != _keep_1_0_183 && _old_192 != _keep_1_1_184 && _old_192 != _keep_1_2_185 && _old_192 != _keep_2_0_186 && _old_192 != _keep_2_1_187 && _old_192 != _keep_2_2_188) {
                            LIR_unload_subject(_old_192);
                        }
                    }
                    lir_subject_t* _old_193 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_193 != _src_2_175) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_175;
                        optimized = 1;
                        if (_old_193 && _old_193 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_193 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_193 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_193 != _src_2_175 && _old_193 != _src_1_176 && _old_193 != _keep_0_0_180 && _old_193 != _keep_0_1_181 && _old_193 != _keep_0_2_182 && _old_193 != _keep_1_0_183 && _old_193 != _keep_1_1_184 && _old_193 != _keep_1_2_185 && _old_193 != _keep_2_0_186 && _old_193 != _keep_2_1_187 && _old_193 != _keep_2_2_188) {
                            LIR_unload_subject(_old_193);
                        }
                    }
                    lir_subject_t* _old_194 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_194) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_194 && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_194 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_194 != _src_2_175 && _old_194 != _src_1_176 && _old_194 != _keep_0_0_180 && _old_194 != _keep_0_1_181 && _old_194 != _keep_0_2_182 && _old_194 != _keep_1_0_183 && _old_194 != _keep_1_1_184 && _old_194 != _keep_1_2_185 && _old_194 != _keep_2_0_186 && _old_194 != _keep_2_1_187 && _old_194 != _keep_2_2_188) {
                            LIR_unload_subject(_old_194);
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
                    lir_subject_t* _src_1_225 = lh->farg;
                    lir_operation_t _match_op_0_226 = lh->op;
                    lir_subject_t* _keep_0_0_227 = lh->farg;
                    lir_subject_t* _keep_0_1_228 = lh->sarg;
                    lir_subject_t* _keep_0_2_229 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_230 = lh->farg;
                    if (_old_230 != _src_1_225) {
                        lh->farg = _src_1_225;
                        optimized = 1;
                        if (_old_230 && _old_230 != lh->farg && _old_230 != lh->sarg && _old_230 != lh->targ && _old_230 != _src_1_225 && _old_230 != _keep_0_0_227 && _old_230 != _keep_0_1_228 && _old_230 != _keep_0_2_229) {
                            LIR_unload_subject(_old_230);
                        }
                    }
                    lir_subject_t* _old_231 = lh->sarg;
                    if (_old_231 != _src_1_225) {
                        lh->sarg = _src_1_225;
                        optimized = 1;
                        if (_old_231 && _old_231 != lh->farg && _old_231 != lh->sarg && _old_231 != lh->targ && _old_231 != _src_1_225 && _old_231 != _keep_0_0_227 && _old_231 != _keep_0_1_228 && _old_231 != _keep_0_2_229) {
                            LIR_unload_subject(_old_231);
                        }
                    }
                    lir_subject_t* _old_232 = lh->targ;
                    if (_old_232) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_232 && _old_232 != lh->farg && _old_232 != lh->sarg && _old_232 != lh->targ && _old_232 != _src_1_225 && _old_232 != _keep_0_0_227 && _old_232 != _keep_0_1_228 && _old_232 != _keep_0_2_229) {
                            LIR_unload_subject(_old_232);
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
                    lir_subject_t* _src_1_257 = lh->farg;
                    lir_operation_t _match_op_0_258 = lh->op;
                    lir_subject_t* _keep_0_0_259 = lh->farg;
                    lir_subject_t* _keep_0_1_260 = lh->sarg;
                    lir_subject_t* _keep_0_2_261 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_262 = lh->sarg;
                    if (_old_262 != _src_1_257) {
                        lh->sarg = _src_1_257;
                        optimized = 1;
                        if (_old_262 && _old_262 != lh->farg && _old_262 != lh->sarg && _old_262 != lh->targ && _old_262 != _src_1_257 && _old_262 != _keep_0_0_259 && _old_262 != _keep_0_1_260 && _old_262 != _keep_0_2_261) {
                            LIR_unload_subject(_old_262);
                        }
                    }
                    lir_subject_t* _old_263 = lh->targ;
                    if (_old_263 != _src_1_257) {
                        lh->targ = _src_1_257;
                        optimized = 1;
                        if (_old_263 && _old_263 != lh->farg && _old_263 != lh->sarg && _old_263 != lh->targ && _old_263 != _src_1_257 && _old_263 != _keep_0_0_259 && _old_263 != _keep_0_1_260 && _old_263 != _keep_0_2_261) {
                            LIR_unload_subject(_old_263);
                        }
                    }
                    lir_subject_t* _old_264 = lh->farg;
                    if (_old_264 != _src_1_257) {
                        lh->farg = _src_1_257;
                        optimized = 1;
                        if (_old_264 && _old_264 != lh->farg && _old_264 != lh->sarg && _old_264 != lh->targ && _old_264 != _src_1_257 && _old_264 != _keep_0_0_259 && _old_264 != _keep_0_1_260 && _old_264 != _keep_0_2_261) {
                            LIR_unload_subject(_old_264);
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
                    lir_subject_t* _src_1_217 = lh->farg;
                    lir_operation_t _match_op_0_218 = lh->op;
                    lir_subject_t* _keep_0_0_219 = lh->farg;
                    lir_subject_t* _keep_0_1_220 = lh->sarg;
                    lir_subject_t* _keep_0_2_221 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_222 = lh->farg;
                    if (_old_222 != _src_1_217) {
                        lh->farg = _src_1_217;
                        optimized = 1;
                        if (_old_222 && _old_222 != lh->farg && _old_222 != lh->sarg && _old_222 != lh->targ && _old_222 != _src_1_217 && _old_222 != _keep_0_0_219 && _old_222 != _keep_0_1_220 && _old_222 != _keep_0_2_221) {
                            LIR_unload_subject(_old_222);
                        }
                    }
                    lir_subject_t* _old_223 = lh->sarg;
                    if (_old_223 != _src_1_217) {
                        lh->sarg = _src_1_217;
                        optimized = 1;
                        if (_old_223 && _old_223 != lh->farg && _old_223 != lh->sarg && _old_223 != lh->targ && _old_223 != _src_1_217 && _old_223 != _keep_0_0_219 && _old_223 != _keep_0_1_220 && _old_223 != _keep_0_2_221) {
                            LIR_unload_subject(_old_223);
                        }
                    }
                    lir_subject_t* _old_224 = lh->targ;
                    if (_old_224) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_224 && _old_224 != lh->farg && _old_224 != lh->sarg && _old_224 != lh->targ && _old_224 != _src_1_217 && _old_224 != _keep_0_0_219 && _old_224 != _keep_0_1_220 && _old_224 != _keep_0_2_221) {
                            LIR_unload_subject(_old_224);
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
                    lir_subject_t* _src_1_85 = lh->farg;
                    lir_subject_t* _src_2_86 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_87 = lh->op;
                    lir_operation_t _match_op_1_88 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_89 = lh->farg;
                    lir_subject_t* _keep_0_1_90 = lh->sarg;
                    lir_subject_t* _keep_0_2_91 = lh->targ;
                    lir_subject_t* _keep_1_0_92 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_93 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_94 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_95 = lh->sarg;
                    if (_old_95 != _src_1_85) {
                        lh->sarg = _src_1_85;
                        optimized = 1;
                        if (_old_95 && _old_95 != lh->farg && _old_95 != lh->sarg && _old_95 != lh->targ && _old_95 != _src_1_85 && _old_95 != _src_2_86 && _old_95 != _keep_0_0_89 && _old_95 != _keep_0_1_90 && _old_95 != _keep_0_2_91 && _old_95 != _keep_1_0_92 && _old_95 != _keep_1_1_93 && _old_95 != _keep_1_2_94) {
                            LIR_unload_subject(_old_95);
                        }
                    }
                    lir_subject_t* _old_96 = lh->targ;
                    if (_old_96 != _src_1_85) {
                        lh->targ = _src_1_85;
                        optimized = 1;
                        if (_old_96 && _old_96 != lh->farg && _old_96 != lh->sarg && _old_96 != lh->targ && _old_96 != _src_1_85 && _old_96 != _src_2_86 && _old_96 != _keep_0_0_89 && _old_96 != _keep_0_1_90 && _old_96 != _keep_0_2_91 && _old_96 != _keep_1_0_92 && _old_96 != _keep_1_1_93 && _old_96 != _keep_1_2_94) {
                            LIR_unload_subject(_old_96);
                        }
                    }
                    lir_subject_t* _old_97 = lh->farg;
                    if (_old_97 != _src_1_85) {
                        lh->farg = _src_1_85;
                        optimized = 1;
                        if (_old_97 && _old_97 != lh->farg && _old_97 != lh->sarg && _old_97 != lh->targ && _old_97 != _src_1_85 && _old_97 != _src_2_86 && _old_97 != _keep_0_0_89 && _old_97 != _keep_0_1_90 && _old_97 != _keep_0_2_91 && _old_97 != _keep_1_0_92 && _old_97 != _keep_1_1_93 && _old_97 != _keep_1_2_94) {
                            LIR_unload_subject(_old_97);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_98 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_98 != _src_2_86) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_86;
                        optimized = 1;
                        if (_old_98 && _old_98 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_98 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_98 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_98 != _src_1_85 && _old_98 != _src_2_86 && _old_98 != _keep_0_0_89 && _old_98 != _keep_0_1_90 && _old_98 != _keep_0_2_91 && _old_98 != _keep_1_0_92 && _old_98 != _keep_1_1_93 && _old_98 != _keep_1_2_94) {
                            LIR_unload_subject(_old_98);
                        }
                    }
                    lir_subject_t* _old_99 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_99 != _src_2_86) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_2_86;
                        optimized = 1;
                        if (_old_99 && _old_99 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_99 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_99 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_99 != _src_1_85 && _old_99 != _src_2_86 && _old_99 != _keep_0_0_89 && _old_99 != _keep_0_1_90 && _old_99 != _keep_0_2_91 && _old_99 != _keep_1_0_92 && _old_99 != _keep_1_1_93 && _old_99 != _keep_1_2_94) {
                            LIR_unload_subject(_old_99);
                        }
                    }
                    lir_subject_t* _old_100 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_100 != _src_2_86) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_86;
                        optimized = 1;
                        if (_old_100 && _old_100 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_100 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_100 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_100 != _src_1_85 && _old_100 != _src_2_86 && _old_100 != _keep_0_0_89 && _old_100 != _keep_0_1_90 && _old_100 != _keep_0_2_91 && _old_100 != _keep_1_0_92 && _old_100 != _keep_1_1_93 && _old_100 != _keep_1_2_94) {
                            LIR_unload_subject(_old_100);
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
                    lir_subject_t* _src_1_233 = lh->farg;
                    lir_operation_t _match_op_0_234 = lh->op;
                    lir_operation_t _match_op_1_235 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_236 = lh->farg;
                    lir_subject_t* _keep_0_1_237 = lh->sarg;
                    lir_subject_t* _keep_0_2_238 = lh->targ;
                    lir_subject_t* _keep_1_0_239 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_240 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_241 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_242 = lh->sarg;
                    if (_old_242 != _src_1_233) {
                        lh->sarg = _src_1_233;
                        optimized = 1;
                        if (_old_242 && _old_242 != lh->farg && _old_242 != lh->sarg && _old_242 != lh->targ && _old_242 != _src_1_233 && _old_242 != _keep_0_0_236 && _old_242 != _keep_0_1_237 && _old_242 != _keep_0_2_238 && _old_242 != _keep_1_0_239 && _old_242 != _keep_1_1_240 && _old_242 != _keep_1_2_241) {
                            LIR_unload_subject(_old_242);
                        }
                    }
                    lir_subject_t* _old_243 = lh->targ;
                    if (_old_243 != _src_1_233) {
                        lh->targ = _src_1_233;
                        optimized = 1;
                        if (_old_243 && _old_243 != lh->farg && _old_243 != lh->sarg && _old_243 != lh->targ && _old_243 != _src_1_233 && _old_243 != _keep_0_0_236 && _old_243 != _keep_0_1_237 && _old_243 != _keep_0_2_238 && _old_243 != _keep_1_0_239 && _old_243 != _keep_1_1_240 && _old_243 != _keep_1_2_241) {
                            LIR_unload_subject(_old_243);
                        }
                    }
                    lir_subject_t* _old_244 = lh->farg;
                    if (_old_244 != _src_1_233) {
                        lh->farg = _src_1_233;
                        optimized = 1;
                        if (_old_244 && _old_244 != lh->farg && _old_244 != lh->sarg && _old_244 != lh->targ && _old_244 != _src_1_233 && _old_244 != _keep_0_0_236 && _old_244 != _keep_0_1_237 && _old_244 != _keep_0_2_238 && _old_244 != _keep_1_0_239 && _old_244 != _keep_1_1_240 && _old_244 != _keep_1_2_241) {
                            LIR_unload_subject(_old_244);
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
                    lir_subject_t* _src_1_245 = lh->farg;
                    lir_operation_t _match_op_0_246 = lh->op;
                    lir_operation_t _match_op_1_247 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_248 = lh->farg;
                    lir_subject_t* _keep_0_1_249 = lh->sarg;
                    lir_subject_t* _keep_0_2_250 = lh->targ;
                    lir_subject_t* _keep_1_0_251 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_252 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_253 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_254 = lh->sarg;
                    if (_old_254 != _src_1_245) {
                        lh->sarg = _src_1_245;
                        optimized = 1;
                        if (_old_254 && _old_254 != lh->farg && _old_254 != lh->sarg && _old_254 != lh->targ && _old_254 != _src_1_245 && _old_254 != _keep_0_0_248 && _old_254 != _keep_0_1_249 && _old_254 != _keep_0_2_250 && _old_254 != _keep_1_0_251 && _old_254 != _keep_1_1_252 && _old_254 != _keep_1_2_253) {
                            LIR_unload_subject(_old_254);
                        }
                    }
                    lir_subject_t* _old_255 = lh->targ;
                    if (_old_255 != _src_1_245) {
                        lh->targ = _src_1_245;
                        optimized = 1;
                        if (_old_255 && _old_255 != lh->farg && _old_255 != lh->sarg && _old_255 != lh->targ && _old_255 != _src_1_245 && _old_255 != _keep_0_0_248 && _old_255 != _keep_0_1_249 && _old_255 != _keep_0_2_250 && _old_255 != _keep_1_0_251 && _old_255 != _keep_1_1_252 && _old_255 != _keep_1_2_253) {
                            LIR_unload_subject(_old_255);
                        }
                    }
                    lir_subject_t* _old_256 = lh->farg;
                    if (_old_256 != _src_1_245) {
                        lh->farg = _src_1_245;
                        optimized = 1;
                        if (_old_256 && _old_256 != lh->farg && _old_256 != lh->sarg && _old_256 != lh->targ && _old_256 != _src_1_245 && _old_256 != _keep_0_0_248 && _old_256 != _keep_0_1_249 && _old_256 != _keep_0_2_250 && _old_256 != _keep_1_0_251 && _old_256 != _keep_1_1_252 && _old_256 != _keep_1_2_253) {
                            LIR_unload_subject(_old_256);
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
                    lir_subject_t* _src_1_265 = lh->farg;
                    lir_operation_t _match_op_0_266 = lh->op;
                    lir_operation_t _match_op_1_267 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_268 = lh->farg;
                    lir_subject_t* _keep_0_1_269 = lh->sarg;
                    lir_subject_t* _keep_0_2_270 = lh->targ;
                    lir_subject_t* _keep_1_0_271 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_272 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_273 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_274 = lh->sarg;
                    if (_old_274 != _src_1_265) {
                        lh->sarg = _src_1_265;
                        optimized = 1;
                        if (_old_274 && _old_274 != lh->farg && _old_274 != lh->sarg && _old_274 != lh->targ && _old_274 != _src_1_265 && _old_274 != _keep_0_0_268 && _old_274 != _keep_0_1_269 && _old_274 != _keep_0_2_270 && _old_274 != _keep_1_0_271 && _old_274 != _keep_1_1_272 && _old_274 != _keep_1_2_273) {
                            LIR_unload_subject(_old_274);
                        }
                    }
                    lir_subject_t* _old_275 = lh->targ;
                    if (_old_275 != _src_1_265) {
                        lh->targ = _src_1_265;
                        optimized = 1;
                        if (_old_275 && _old_275 != lh->farg && _old_275 != lh->sarg && _old_275 != lh->targ && _old_275 != _src_1_265 && _old_275 != _keep_0_0_268 && _old_275 != _keep_0_1_269 && _old_275 != _keep_0_2_270 && _old_275 != _keep_1_0_271 && _old_275 != _keep_1_1_272 && _old_275 != _keep_1_2_273) {
                            LIR_unload_subject(_old_275);
                        }
                    }
                    lir_subject_t* _old_276 = lh->farg;
                    if (_old_276 != _src_1_265) {
                        lh->farg = _src_1_265;
                        optimized = 1;
                        if (_old_276 && _old_276 != lh->farg && _old_276 != lh->sarg && _old_276 != lh->targ && _old_276 != _src_1_265 && _old_276 != _keep_0_0_268 && _old_276 != _keep_0_1_269 && _old_276 != _keep_0_2_270 && _old_276 != _keep_1_0_271 && _old_276 != _keep_1_1_272 && _old_276 != _keep_1_2_273) {
                            LIR_unload_subject(_old_276);
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
                    lir_operation_t _match_op_0_203 = lh->op;
                    lir_subject_t* _keep_0_0_204 = lh->farg;
                    lir_subject_t* _keep_0_1_205 = lh->sarg;
                    lir_subject_t* _keep_0_2_206 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_207 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_207 && _old_207 != lh->farg && _old_207 != lh->sarg && _old_207 != lh->targ && _old_207 != _keep_0_0_204 && _old_207 != _keep_0_1_205 && _old_207 != _keep_0_2_206) {
                        LIR_unload_subject(_old_207);
                    }
                    lir_subject_t* _old_208 = lh->targ;
                    if (_old_208) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_208 && _old_208 != lh->farg && _old_208 != lh->sarg && _old_208 != lh->targ && _old_208 != _keep_0_0_204 && _old_208 != _keep_0_1_205 && _old_208 != _keep_0_2_206) {
                            LIR_unload_subject(_old_208);
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
                    lir_subject_t* _src_1_146 = lh->farg;
                    lir_operation_t _match_op_0_147 = lh->op;
                    lir_subject_t* _keep_0_0_148 = lh->farg;
                    lir_subject_t* _keep_0_1_149 = lh->sarg;
                    lir_subject_t* _keep_0_2_150 = lh->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_151 = lh->farg;
                    if (_old_151 != _src_1_146) {
                        lh->farg = _src_1_146;
                        optimized = 1;
                        if (_old_151 && _old_151 != lh->farg && _old_151 != lh->sarg && _old_151 != lh->targ && _old_151 != _src_1_146 && _old_151 != _keep_0_0_148 && _old_151 != _keep_0_1_149 && _old_151 != _keep_0_2_150) {
                            LIR_unload_subject(_old_151);
                        }
                    }
                    lir_subject_t* _old_152 = lh->sarg;
                    if (_old_152) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_152 && _old_152 != lh->farg && _old_152 != lh->sarg && _old_152 != lh->targ && _old_152 != _src_1_146 && _old_152 != _keep_0_0_148 && _old_152 != _keep_0_1_149 && _old_152 != _keep_0_2_150) {
                            LIR_unload_subject(_old_152);
                        }
                    }
                    lir_subject_t* _old_153 = lh->targ;
                    if (_old_153) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_153 && _old_153 != lh->farg && _old_153 != lh->sarg && _old_153 != lh->targ && _old_153 != _src_1_146 && _old_153 != _keep_0_0_148 && _old_153 != _keep_0_1_149 && _old_153 != _keep_0_2_150) {
                            LIR_unload_subject(_old_153);
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
                    lir_subject_t* _src_1_195 = lh->farg;
                    lir_operation_t _match_op_0_196 = lh->op;
                    lir_subject_t* _keep_0_0_197 = lh->farg;
                    lir_subject_t* _keep_0_1_198 = lh->sarg;
                    lir_subject_t* _keep_0_2_199 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_200 = lh->sarg;
                    if (_old_200 != _src_1_195) {
                        lh->sarg = _src_1_195;
                        optimized = 1;
                        if (_old_200 && _old_200 != lh->farg && _old_200 != lh->sarg && _old_200 != lh->targ && _old_200 != _src_1_195 && _old_200 != _keep_0_0_197 && _old_200 != _keep_0_1_198 && _old_200 != _keep_0_2_199) {
                            LIR_unload_subject(_old_200);
                        }
                    }
                    lir_subject_t* _old_201 = lh->targ;
                    if (_old_201 != _src_1_195) {
                        lh->targ = _src_1_195;
                        optimized = 1;
                        if (_old_201 && _old_201 != lh->farg && _old_201 != lh->sarg && _old_201 != lh->targ && _old_201 != _src_1_195 && _old_201 != _keep_0_0_197 && _old_201 != _keep_0_1_198 && _old_201 != _keep_0_2_199) {
                            LIR_unload_subject(_old_201);
                        }
                    }
                    lir_subject_t* _old_202 = lh->farg;
                    if (_old_202 != _src_1_195) {
                        lh->farg = _src_1_195;
                        optimized = 1;
                        if (_old_202 && _old_202 != lh->farg && _old_202 != lh->sarg && _old_202 != lh->targ && _old_202 != _src_1_195 && _old_202 != _keep_0_0_197 && _old_202 != _keep_0_1_198 && _old_202 != _keep_0_2_199) {
                            LIR_unload_subject(_old_202);
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