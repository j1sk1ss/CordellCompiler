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
                    lir_subject_t* _src_1_296 = lh->farg;
                    lir_operation_t _match_op_0_297 = lh->op;
                    lir_subject_t* _keep_0_0_298 = lh->farg;
                    lir_subject_t* _keep_0_1_299 = lh->sarg;
                    lir_subject_t* _keep_0_2_300 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_301 = lh->farg;
                    if (_old_301 != _src_1_296) {
                        lh->farg = _src_1_296;
                        optimized = 1;
                        if (_old_301 && _old_301 != lh->farg && _old_301 != lh->sarg && _old_301 != lh->targ && _old_301 != _src_1_296 && _old_301 != _keep_0_0_298 && _old_301 != _keep_0_1_299 && _old_301 != _keep_0_2_300) {
                            LIR_unload_subject(_old_301);
                        }
                    }
                    lir_subject_t* _old_302 = lh->sarg;
                    if (_old_302 != _src_1_296) {
                        lh->sarg = _src_1_296;
                        optimized = 1;
                        if (_old_302 && _old_302 != lh->farg && _old_302 != lh->sarg && _old_302 != lh->targ && _old_302 != _src_1_296 && _old_302 != _keep_0_0_298 && _old_302 != _keep_0_1_299 && _old_302 != _keep_0_2_300) {
                            LIR_unload_subject(_old_302);
                        }
                    }
                    lir_subject_t* _old_303 = lh->targ;
                    if (_old_303) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_303 && _old_303 != lh->farg && _old_303 != lh->sarg && _old_303 != lh->targ && _old_303 != _src_1_296 && _old_303 != _keep_0_0_298 && _old_303 != _keep_0_1_299 && _old_303 != _keep_0_2_300) {
                            LIR_unload_subject(_old_303);
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

            case LIR_SETL:
             {
                if (lh->op == LIR_SETL &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_MOVSX &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg)) {
                    lir_subject_t* _src_1_13 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_14 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_15 = lh->op;
                    lir_operation_t _match_op_1_16 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_17 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_18 = lh->farg;
                    lir_subject_t* _keep_0_1_19 = lh->sarg;
                    lir_subject_t* _keep_0_2_20 = lh->targ;
                    lir_subject_t* _keep_1_0_21 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_22 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_23 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_24 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_25 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_26 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_SETL) {
                        lh->op = LIR_SETL;
                        optimized = 1;
                    }
                    lir_subject_t* _old_27 = lh->farg;
                    if (_old_27 != _src_1_13) {
                        lh->farg = _src_1_13;
                        optimized = 1;
                        if (_old_27 && _old_27 != lh->farg && _old_27 != lh->sarg && _old_27 != lh->targ && _old_27 != _src_1_13 && _old_27 != _src_2_14 && _old_27 != _keep_0_0_18 && _old_27 != _keep_0_1_19 && _old_27 != _keep_0_2_20 && _old_27 != _keep_1_0_21 && _old_27 != _keep_1_1_22 && _old_27 != _keep_1_2_23 && _old_27 != _keep_2_0_24 && _old_27 != _keep_2_1_25 && _old_27 != _keep_2_2_26) {
                            LIR_unload_subject(_old_27);
                        }
                    }
                    lir_subject_t* _old_28 = lh->sarg;
                    if (_old_28) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_28 && _old_28 != lh->farg && _old_28 != lh->sarg && _old_28 != lh->targ && _old_28 != _src_1_13 && _old_28 != _src_2_14 && _old_28 != _keep_0_0_18 && _old_28 != _keep_0_1_19 && _old_28 != _keep_0_2_20 && _old_28 != _keep_1_0_21 && _old_28 != _keep_1_1_22 && _old_28 != _keep_1_2_23 && _old_28 != _keep_2_0_24 && _old_28 != _keep_2_1_25 && _old_28 != _keep_2_2_26) {
                            LIR_unload_subject(_old_28);
                        }
                    }
                    lir_subject_t* _old_29 = lh->targ;
                    if (_old_29) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_29 && _old_29 != lh->farg && _old_29 != lh->sarg && _old_29 != lh->targ && _old_29 != _src_1_13 && _old_29 != _src_2_14 && _old_29 != _keep_0_0_18 && _old_29 != _keep_0_1_19 && _old_29 != _keep_0_2_20 && _old_29 != _keep_1_0_21 && _old_29 != _keep_1_1_22 && _old_29 != _keep_1_2_23 && _old_29 != _keep_2_0_24 && _old_29 != _keep_2_1_25 && _old_29 != _keep_2_2_26) {
                            LIR_unload_subject(_old_29);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_TST) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_30 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_30 != _src_1_13) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_13;
                        optimized = 1;
                        if (_old_30 && _old_30 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_30 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_30 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_30 != _src_1_13 && _old_30 != _src_2_14 && _old_30 != _keep_0_0_18 && _old_30 != _keep_0_1_19 && _old_30 != _keep_0_2_20 && _old_30 != _keep_1_0_21 && _old_30 != _keep_1_1_22 && _old_30 != _keep_1_2_23 && _old_30 != _keep_2_0_24 && _old_30 != _keep_2_1_25 && _old_30 != _keep_2_2_26) {
                            LIR_unload_subject(_old_30);
                        }
                    }
                    lir_subject_t* _old_31 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_31 != _src_1_13) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_13;
                        optimized = 1;
                        if (_old_31 && _old_31 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_31 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_31 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_31 != _src_1_13 && _old_31 != _src_2_14 && _old_31 != _keep_0_0_18 && _old_31 != _keep_0_1_19 && _old_31 != _keep_0_2_20 && _old_31 != _keep_1_0_21 && _old_31 != _keep_1_1_22 && _old_31 != _keep_1_2_23 && _old_31 != _keep_2_0_24 && _old_31 != _keep_2_1_25 && _old_31 != _keep_2_2_26) {
                            LIR_unload_subject(_old_31);
                        }
                    }
                    lir_subject_t* _old_32 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_32) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_32 && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_32 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_32 != _src_1_13 && _old_32 != _src_2_14 && _old_32 != _keep_0_0_18 && _old_32 != _keep_0_1_19 && _old_32 != _keep_0_2_20 && _old_32 != _keep_1_0_21 && _old_32 != _keep_1_1_22 && _old_32 != _keep_1_2_23 && _old_32 != _keep_2_0_24 && _old_32 != _keep_2_1_25 && _old_32 != _keep_2_2_26) {
                            LIR_unload_subject(_old_32);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL)) &&
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
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->farg)) {
                    lir_subject_t* _src_1_33 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_2_34 = lh->sarg;
                    lir_subject_t* _src_3_35 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_4_36 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_operation_t _match_op_0_37 = lh->op;
                    lir_operation_t _match_op_1_38 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_39 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_40 = lh->farg;
                    lir_subject_t* _keep_0_1_41 = lh->sarg;
                    lir_subject_t* _keep_0_2_42 = lh->targ;
                    lir_subject_t* _keep_1_0_43 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_44 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_45 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_46 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_47 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_48 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_37) {
                        lh->op = _match_op_0_37;
                        optimized = 1;
                    }
                    lir_subject_t* _old_49 = lh->farg;
                    if (_old_49 != _src_3_35) {
                        lh->farg = _src_3_35;
                        optimized = 1;
                        if (_old_49 && _old_49 != lh->farg && _old_49 != lh->sarg && _old_49 != lh->targ && _old_49 != _src_1_33 && _old_49 != _src_2_34 && _old_49 != _src_3_35 && _old_49 != _src_4_36 && _old_49 != _keep_0_0_40 && _old_49 != _keep_0_1_41 && _old_49 != _keep_0_2_42 && _old_49 != _keep_1_0_43 && _old_49 != _keep_1_1_44 && _old_49 != _keep_1_2_45 && _old_49 != _keep_2_0_46 && _old_49 != _keep_2_1_47 && _old_49 != _keep_2_2_48) {
                            LIR_unload_subject(_old_49);
                        }
                    }
                    lir_subject_t* _old_50 = lh->sarg;
                    if (_old_50 != _src_2_34) {
                        lh->sarg = _src_2_34;
                        optimized = 1;
                        if (_old_50 && _old_50 != lh->farg && _old_50 != lh->sarg && _old_50 != lh->targ && _old_50 != _src_1_33 && _old_50 != _src_2_34 && _old_50 != _src_3_35 && _old_50 != _src_4_36 && _old_50 != _keep_0_0_40 && _old_50 != _keep_0_1_41 && _old_50 != _keep_0_2_42 && _old_50 != _keep_1_0_43 && _old_50 != _keep_1_1_44 && _old_50 != _keep_1_2_45 && _old_50 != _keep_2_0_46 && _old_50 != _keep_2_1_47 && _old_50 != _keep_2_2_48) {
                            LIR_unload_subject(_old_50);
                        }
                    }
                    lir_subject_t* _old_51 = lh->targ;
                    if (_old_51) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_51 && _old_51 != lh->farg && _old_51 != lh->sarg && _old_51 != lh->targ && _old_51 != _src_1_33 && _old_51 != _src_2_34 && _old_51 != _src_3_35 && _old_51 != _src_4_36 && _old_51 != _keep_0_0_40 && _old_51 != _keep_0_1_41 && _old_51 != _keep_0_2_42 && _old_51 != _keep_1_0_43 && _old_51 != _keep_1_1_44 && _old_51 != _keep_1_2_45 && _old_51 != _keep_2_0_46 && _old_51 != _keep_2_1_47 && _old_51 != _keep_2_2_48) {
                            LIR_unload_subject(_old_51);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_38) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_38;
                        optimized = 1;
                    }
                    lir_subject_t* _old_52 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_52 != _src_1_33) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_33;
                        optimized = 1;
                        if (_old_52 && _old_52 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_52 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_52 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_52 != _src_1_33 && _old_52 != _src_2_34 && _old_52 != _src_3_35 && _old_52 != _src_4_36 && _old_52 != _keep_0_0_40 && _old_52 != _keep_0_1_41 && _old_52 != _keep_0_2_42 && _old_52 != _keep_1_0_43 && _old_52 != _keep_1_1_44 && _old_52 != _keep_1_2_45 && _old_52 != _keep_2_0_46 && _old_52 != _keep_2_1_47 && _old_52 != _keep_2_2_48) {
                            LIR_unload_subject(_old_52);
                        }
                    }
                    lir_subject_t* _old_53 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_53 != _src_4_36) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_4_36;
                        optimized = 1;
                        if (_old_53 && _old_53 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_53 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_53 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_53 != _src_1_33 && _old_53 != _src_2_34 && _old_53 != _src_3_35 && _old_53 != _src_4_36 && _old_53 != _keep_0_0_40 && _old_53 != _keep_0_1_41 && _old_53 != _keep_0_2_42 && _old_53 != _keep_1_0_43 && _old_53 != _keep_1_1_44 && _old_53 != _keep_1_2_45 && _old_53 != _keep_2_0_46 && _old_53 != _keep_2_1_47 && _old_53 != _keep_2_2_48) {
                            LIR_unload_subject(_old_53);
                        }
                    }
                    lir_subject_t* _old_54 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_54) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_54 && _old_54 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_54 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_54 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_54 != _src_1_33 && _old_54 != _src_2_34 && _old_54 != _src_3_35 && _old_54 != _src_4_36 && _old_54 != _keep_0_0_40 && _old_54 != _keep_0_1_41 && _old_54 != _keep_0_2_42 && _old_54 != _keep_1_0_43 && _old_54 != _keep_1_1_44 && _old_54 != _keep_1_2_45 && _old_54 != _keep_2_0_46 && _old_54 != _keep_2_1_47 && _old_54 != _keep_2_2_48) {
                            LIR_unload_subject(_old_54);
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
                    lir_subject_t* _src_1_55 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_56 = lh->sarg;
                    lir_operation_t _match_op_0_57 = lh->op;
                    lir_operation_t _match_op_1_58 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_59 = lh->farg;
                    lir_subject_t* _keep_0_1_60 = lh->sarg;
                    lir_subject_t* _keep_0_2_61 = lh->targ;
                    lir_subject_t* _keep_1_0_62 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_63 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_64 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_65 = lh->farg;
                    if (_old_65 != _src_2_56) {
                        lh->farg = _src_2_56;
                        optimized = 1;
                        if (_old_65 && _old_65 != lh->farg && _old_65 != lh->sarg && _old_65 != lh->targ && _old_65 != _src_1_55 && _old_65 != _src_2_56 && _old_65 != _keep_0_0_59 && _old_65 != _keep_0_1_60 && _old_65 != _keep_0_2_61 && _old_65 != _keep_1_0_62 && _old_65 != _keep_1_1_63 && _old_65 != _keep_1_2_64) {
                            LIR_unload_subject(_old_65);
                        }
                    }
                    lir_subject_t* _old_66 = lh->sarg;
                    if (_old_66 != _src_2_56) {
                        lh->sarg = _src_2_56;
                        optimized = 1;
                        if (_old_66 && _old_66 != lh->farg && _old_66 != lh->sarg && _old_66 != lh->targ && _old_66 != _src_1_55 && _old_66 != _src_2_56 && _old_66 != _keep_0_0_59 && _old_66 != _keep_0_1_60 && _old_66 != _keep_0_2_61 && _old_66 != _keep_1_0_62 && _old_66 != _keep_1_1_63 && _old_66 != _keep_1_2_64) {
                            LIR_unload_subject(_old_66);
                        }
                    }
                    lir_subject_t* _old_67 = lh->targ;
                    if (_old_67) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_67 && _old_67 != lh->farg && _old_67 != lh->sarg && _old_67 != lh->targ && _old_67 != _src_1_55 && _old_67 != _src_2_56 && _old_67 != _keep_0_0_59 && _old_67 != _keep_0_1_60 && _old_67 != _keep_0_2_61 && _old_67 != _keep_1_0_62 && _old_67 != _keep_1_1_63 && _old_67 != _keep_1_2_64) {
                            LIR_unload_subject(_old_67);
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
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_LABEL) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL) &&
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
                    lir_subject_t* _src_1_68 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _src_2_69 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_operation_t _match_op_0_70 = lh->op;
                    lir_operation_t _match_op_1_71 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_72 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_operation_t _match_op_3_73 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->op;
                    lir_subject_t* _keep_0_0_74 = lh->farg;
                    lir_subject_t* _keep_0_1_75 = lh->sarg;
                    lir_subject_t* _keep_0_2_76 = lh->targ;
                    lir_subject_t* _keep_1_0_77 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_78 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_79 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_80 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_81 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_82 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _keep_3_0_83 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->farg;
                    lir_subject_t* _keep_3_1_84 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->sarg;
                    lir_subject_t* _keep_3_2_85 = LIR_get_near_instruction(lh, bb->lmap.exit, 3)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_86 = lh->sarg;
                    if (_old_86 != _src_2_69) {
                        lh->sarg = _src_2_69;
                        optimized = 1;
                        if (_old_86 && _old_86 != lh->farg && _old_86 != lh->sarg && _old_86 != lh->targ && _old_86 != _src_1_68 && _old_86 != _src_2_69 && _old_86 != _keep_0_0_74 && _old_86 != _keep_0_1_75 && _old_86 != _keep_0_2_76 && _old_86 != _keep_1_0_77 && _old_86 != _keep_1_1_78 && _old_86 != _keep_1_2_79 && _old_86 != _keep_2_0_80 && _old_86 != _keep_2_1_81 && _old_86 != _keep_2_2_82 && _old_86 != _keep_3_0_83 && _old_86 != _keep_3_1_84 && _old_86 != _keep_3_2_85) {
                            LIR_unload_subject(_old_86);
                        }
                    }
                    lir_subject_t* _old_87 = lh->targ;
                    if (_old_87 != _src_2_69) {
                        lh->targ = _src_2_69;
                        optimized = 1;
                        if (_old_87 && _old_87 != lh->farg && _old_87 != lh->sarg && _old_87 != lh->targ && _old_87 != _src_1_68 && _old_87 != _src_2_69 && _old_87 != _keep_0_0_74 && _old_87 != _keep_0_1_75 && _old_87 != _keep_0_2_76 && _old_87 != _keep_1_0_77 && _old_87 != _keep_1_1_78 && _old_87 != _keep_1_2_79 && _old_87 != _keep_2_0_80 && _old_87 != _keep_2_1_81 && _old_87 != _keep_2_2_82 && _old_87 != _keep_3_0_83 && _old_87 != _keep_3_1_84 && _old_87 != _keep_3_2_85) {
                            LIR_unload_subject(_old_87);
                        }
                    }
                    lir_subject_t* _old_88 = lh->farg;
                    if (_old_88 != _src_2_69) {
                        lh->farg = _src_2_69;
                        optimized = 1;
                        if (_old_88 && _old_88 != lh->farg && _old_88 != lh->sarg && _old_88 != lh->targ && _old_88 != _src_1_68 && _old_88 != _src_2_69 && _old_88 != _keep_0_0_74 && _old_88 != _keep_0_1_75 && _old_88 != _keep_0_2_76 && _old_88 != _keep_1_0_77 && _old_88 != _keep_1_1_78 && _old_88 != _keep_1_2_79 && _old_88 != _keep_2_0_80 && _old_88 != _keep_2_1_81 && _old_88 != _keep_2_2_82 && _old_88 != _keep_3_0_83 && _old_88 != _keep_3_1_84 && _old_88 != _keep_3_2_85) {
                            LIR_unload_subject(_old_88);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_70) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_70;
                        optimized = 1;
                    }
                    lir_subject_t* _old_89 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_89 != _src_1_68) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_68;
                        optimized = 1;
                        if (_old_89 && _old_89 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_89 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_89 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_89 != _src_1_68 && _old_89 != _src_2_69 && _old_89 != _keep_0_0_74 && _old_89 != _keep_0_1_75 && _old_89 != _keep_0_2_76 && _old_89 != _keep_1_0_77 && _old_89 != _keep_1_1_78 && _old_89 != _keep_1_2_79 && _old_89 != _keep_2_0_80 && _old_89 != _keep_2_1_81 && _old_89 != _keep_2_2_82 && _old_89 != _keep_3_0_83 && _old_89 != _keep_3_1_84 && _old_89 != _keep_3_2_85) {
                            LIR_unload_subject(_old_89);
                        }
                    }
                    lir_subject_t* _old_90 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_90 != _src_1_68) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_68;
                        optimized = 1;
                        if (_old_90 && _old_90 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_90 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_90 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_90 != _src_1_68 && _old_90 != _src_2_69 && _old_90 != _keep_0_0_74 && _old_90 != _keep_0_1_75 && _old_90 != _keep_0_2_76 && _old_90 != _keep_1_0_77 && _old_90 != _keep_1_1_78 && _old_90 != _keep_1_2_79 && _old_90 != _keep_2_0_80 && _old_90 != _keep_2_1_81 && _old_90 != _keep_2_2_82 && _old_90 != _keep_3_0_83 && _old_90 != _keep_3_1_84 && _old_90 != _keep_3_2_85) {
                            LIR_unload_subject(_old_90);
                        }
                    }
                    lir_subject_t* _old_91 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_91) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_91 && _old_91 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_91 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_91 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_91 != _src_1_68 && _old_91 != _src_2_69 && _old_91 != _keep_0_0_74 && _old_91 != _keep_0_1_75 && _old_91 != _keep_0_2_76 && _old_91 != _keep_1_0_77 && _old_91 != _keep_1_1_78 && _old_91 != _keep_1_2_79 && _old_91 != _keep_2_0_80 && _old_91 != _keep_2_1_81 && _old_91 != _keep_2_2_82 && _old_91 != _keep_3_0_83 && _old_91 != _keep_3_1_84 && _old_91 != _keep_3_2_85) {
                            LIR_unload_subject(_old_91);
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
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
                lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL)) &&
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
                    lir_subject_t* _src_1_92 = lh->farg;
                    lir_operation_t _match_op_0_93 = lh->op;
                    lir_subject_t* _keep_0_0_94 = lh->farg;
                    lir_subject_t* _keep_0_1_95 = lh->sarg;
                    lir_subject_t* _keep_0_2_96 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_97 = lh->sarg;
                    if (_old_97 != _src_1_92) {
                        lh->sarg = _src_1_92;
                        optimized = 1;
                        if (_old_97 && _old_97 != lh->farg && _old_97 != lh->sarg && _old_97 != lh->targ && _old_97 != _src_1_92 && _old_97 != _keep_0_0_94 && _old_97 != _keep_0_1_95 && _old_97 != _keep_0_2_96) {
                            LIR_unload_subject(_old_97);
                        }
                    }
                    lir_subject_t* _old_98 = lh->targ;
                    if (_old_98 != _src_1_92) {
                        lh->targ = _src_1_92;
                        optimized = 1;
                        if (_old_98 && _old_98 != lh->farg && _old_98 != lh->sarg && _old_98 != lh->targ && _old_98 != _src_1_92 && _old_98 != _keep_0_0_94 && _old_98 != _keep_0_1_95 && _old_98 != _keep_0_2_96) {
                            LIR_unload_subject(_old_98);
                        }
                    }
                    lir_subject_t* _old_99 = lh->farg;
                    if (_old_99 != _src_1_92) {
                        lh->farg = _src_1_92;
                        optimized = 1;
                        if (_old_99 && _old_99 != lh->farg && _old_99 != lh->sarg && _old_99 != lh->targ && _old_99 != _src_1_92 && _old_99 != _keep_0_0_94 && _old_99 != _keep_0_1_95 && _old_99 != _keep_0_2_96) {
                            LIR_unload_subject(_old_99);
                        }
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_LABEL) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_100 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_101 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_102 = lh->op;
                    lir_operation_t _match_op_1_103 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_104 = lh->farg;
                    lir_subject_t* _keep_0_1_105 = lh->sarg;
                    lir_subject_t* _keep_0_2_106 = lh->targ;
                    lir_subject_t* _keep_1_0_107 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_108 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_109 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_102) {
                        lh->op = _match_op_0_102;
                        optimized = 1;
                    }
                    lir_subject_t* _old_110 = lh->farg;
                    if (_old_110 != _src_1_100) {
                        lh->farg = _src_1_100;
                        optimized = 1;
                        if (_old_110 && _old_110 != lh->farg && _old_110 != lh->sarg && _old_110 != lh->targ && _old_110 != _src_1_100 && _old_110 != _src_2_101 && _old_110 != _keep_0_0_104 && _old_110 != _keep_0_1_105 && _old_110 != _keep_0_2_106 && _old_110 != _keep_1_0_107 && _old_110 != _keep_1_1_108 && _old_110 != _keep_1_2_109) {
                            LIR_unload_subject(_old_110);
                        }
                    }
                    lir_subject_t* _old_111 = lh->sarg;
                    if (_old_111 != _src_2_101) {
                        lh->sarg = _src_2_101;
                        optimized = 1;
                        if (_old_111 && _old_111 != lh->farg && _old_111 != lh->sarg && _old_111 != lh->targ && _old_111 != _src_1_100 && _old_111 != _src_2_101 && _old_111 != _keep_0_0_104 && _old_111 != _keep_0_1_105 && _old_111 != _keep_0_2_106 && _old_111 != _keep_1_0_107 && _old_111 != _keep_1_1_108 && _old_111 != _keep_1_2_109) {
                            LIR_unload_subject(_old_111);
                        }
                    }
                    lir_subject_t* _old_112 = lh->targ;
                    if (_old_112) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_112 && _old_112 != lh->farg && _old_112 != lh->sarg && _old_112 != lh->targ && _old_112 != _src_1_100 && _old_112 != _src_2_101 && _old_112 != _keep_0_0_104 && _old_112 != _keep_0_1_105 && _old_112 != _keep_0_2_106 && _old_112 != _keep_1_0_107 && _old_112 != _keep_1_1_108 && _old_112 != _keep_1_2_109) {
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
                lh->sarg->t == LIR_REGISTER) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iCMP || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_CMP) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_113 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_114 = lh->sarg;
                    lir_subject_t* _src_3_115 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_116 = lh->op;
                    lir_operation_t _match_op_1_117 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_118 = lh->farg;
                    lir_subject_t* _keep_0_1_119 = lh->sarg;
                    lir_subject_t* _keep_0_2_120 = lh->targ;
                    lir_subject_t* _keep_1_0_121 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_122 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_123 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_117) {
                        lh->op = _match_op_1_117;
                        optimized = 1;
                    }
                    lir_subject_t* _old_124 = lh->farg;
                    if (_old_124 != _src_2_114) {
                        lh->farg = _src_2_114;
                        optimized = 1;
                        if (_old_124 && _old_124 != lh->farg && _old_124 != lh->sarg && _old_124 != lh->targ && _old_124 != _src_1_113 && _old_124 != _src_2_114 && _old_124 != _src_3_115 && _old_124 != _keep_0_0_118 && _old_124 != _keep_0_1_119 && _old_124 != _keep_0_2_120 && _old_124 != _keep_1_0_121 && _old_124 != _keep_1_1_122 && _old_124 != _keep_1_2_123) {
                            LIR_unload_subject(_old_124);
                        }
                    }
                    lir_subject_t* _old_125 = lh->sarg;
                    if (_old_125 != _src_3_115) {
                        lh->sarg = _src_3_115;
                        optimized = 1;
                        if (_old_125 && _old_125 != lh->farg && _old_125 != lh->sarg && _old_125 != lh->targ && _old_125 != _src_1_113 && _old_125 != _src_2_114 && _old_125 != _src_3_115 && _old_125 != _keep_0_0_118 && _old_125 != _keep_0_1_119 && _old_125 != _keep_0_2_120 && _old_125 != _keep_1_0_121 && _old_125 != _keep_1_1_122 && _old_125 != _keep_1_2_123) {
                            LIR_unload_subject(_old_125);
                        }
                    }
                    lir_subject_t* _old_126 = lh->targ;
                    if (_old_126) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_126 && _old_126 != lh->farg && _old_126 != lh->sarg && _old_126 != lh->targ && _old_126 != _src_1_113 && _old_126 != _src_2_114 && _old_126 != _src_3_115 && _old_126 != _keep_0_0_118 && _old_126 != _keep_0_1_119 && _old_126 != _keep_0_2_120 && _old_126 != _keep_1_0_121 && _old_126 != _keep_1_1_122 && _old_126 != _keep_1_2_123) {
                            LIR_unload_subject(_old_126);
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
                    lir_subject_t* _src_1_143 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_144 = lh->op;
                    lir_operation_t _match_op_1_145 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_146 = lh->farg;
                    lir_subject_t* _keep_0_1_147 = lh->sarg;
                    lir_subject_t* _keep_0_2_148 = lh->targ;
                    lir_subject_t* _keep_1_0_149 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_150 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_151 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_152 = lh->sarg;
                    if (_old_152 != _src_1_143) {
                        lh->sarg = _src_1_143;
                        optimized = 1;
                        if (_old_152 && _old_152 != lh->farg && _old_152 != lh->sarg && _old_152 != lh->targ && _old_152 != _src_1_143 && _old_152 != _keep_0_0_146 && _old_152 != _keep_0_1_147 && _old_152 != _keep_0_2_148 && _old_152 != _keep_1_0_149 && _old_152 != _keep_1_1_150 && _old_152 != _keep_1_2_151) {
                            LIR_unload_subject(_old_152);
                        }
                    }
                    lir_subject_t* _old_153 = lh->targ;
                    if (_old_153 != _src_1_143) {
                        lh->targ = _src_1_143;
                        optimized = 1;
                        if (_old_153 && _old_153 != lh->farg && _old_153 != lh->sarg && _old_153 != lh->targ && _old_153 != _src_1_143 && _old_153 != _keep_0_0_146 && _old_153 != _keep_0_1_147 && _old_153 != _keep_0_2_148 && _old_153 != _keep_1_0_149 && _old_153 != _keep_1_1_150 && _old_153 != _keep_1_2_151) {
                            LIR_unload_subject(_old_153);
                        }
                    }
                    lir_subject_t* _old_154 = lh->farg;
                    if (_old_154 != _src_1_143) {
                        lh->farg = _src_1_143;
                        optimized = 1;
                        if (_old_154 && _old_154 != lh->farg && _old_154 != lh->sarg && _old_154 != lh->targ && _old_154 != _src_1_143 && _old_154 != _keep_0_0_146 && _old_154 != _keep_0_1_147 && _old_154 != _keep_0_2_148 && _old_154 != _keep_1_0_149 && _old_154 != _keep_1_1_150 && _old_154 != _keep_1_2_151) {
                            LIR_unload_subject(_old_154);
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
                    lir_subject_t* _src_1_155 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_156 = lh->op;
                    lir_operation_t _match_op_1_157 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_158 = lh->farg;
                    lir_subject_t* _keep_0_1_159 = lh->sarg;
                    lir_subject_t* _keep_0_2_160 = lh->targ;
                    lir_subject_t* _keep_1_0_161 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_162 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_163 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_164 = lh->sarg;
                    if (_old_164 != _src_1_155) {
                        lh->sarg = _src_1_155;
                        optimized = 1;
                        if (_old_164 && _old_164 != lh->farg && _old_164 != lh->sarg && _old_164 != lh->targ && _old_164 != _src_1_155 && _old_164 != _keep_0_0_158 && _old_164 != _keep_0_1_159 && _old_164 != _keep_0_2_160 && _old_164 != _keep_1_0_161 && _old_164 != _keep_1_1_162 && _old_164 != _keep_1_2_163) {
                            LIR_unload_subject(_old_164);
                        }
                    }
                    lir_subject_t* _old_165 = lh->targ;
                    if (_old_165 != _src_1_155) {
                        lh->targ = _src_1_155;
                        optimized = 1;
                        if (_old_165 && _old_165 != lh->farg && _old_165 != lh->sarg && _old_165 != lh->targ && _old_165 != _src_1_155 && _old_165 != _keep_0_0_158 && _old_165 != _keep_0_1_159 && _old_165 != _keep_0_2_160 && _old_165 != _keep_1_0_161 && _old_165 != _keep_1_1_162 && _old_165 != _keep_1_2_163) {
                            LIR_unload_subject(_old_165);
                        }
                    }
                    lir_subject_t* _old_166 = lh->farg;
                    if (_old_166 != _src_1_155) {
                        lh->farg = _src_1_155;
                        optimized = 1;
                        if (_old_166 && _old_166 != lh->farg && _old_166 != lh->sarg && _old_166 != lh->targ && _old_166 != _src_1_155 && _old_166 != _keep_0_0_158 && _old_166 != _keep_0_1_159 && _old_166 != _keep_0_2_160 && _old_166 != _keep_1_0_161 && _old_166 != _keep_1_1_162 && _old_166 != _keep_1_2_163) {
                            LIR_unload_subject(_old_166);
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
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 1)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMUL &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_CONSTVAL) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_167 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_168 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_169 = lh->op;
                    lir_operation_t _match_op_1_170 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_171 = lh->farg;
                    lir_subject_t* _keep_0_1_172 = lh->sarg;
                    lir_subject_t* _keep_0_2_173 = lh->targ;
                    lir_subject_t* _keep_1_0_174 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_175 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_176 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_169) {
                        lh->op = _match_op_0_169;
                        optimized = 1;
                    }
                    lir_subject_t* _old_177 = lh->farg;
                    if (_old_177 != _src_1_167) {
                        lh->farg = _src_1_167;
                        optimized = 1;
                        if (_old_177 && _old_177 != lh->farg && _old_177 != lh->sarg && _old_177 != lh->targ && _old_177 != _src_1_167 && _old_177 != _src_2_168 && _old_177 != _keep_0_0_171 && _old_177 != _keep_0_1_172 && _old_177 != _keep_0_2_173 && _old_177 != _keep_1_0_174 && _old_177 != _keep_1_1_175 && _old_177 != _keep_1_2_176) {
                            LIR_unload_subject(_old_177);
                        }
                    }
                    lir_subject_t* _old_178 = lh->sarg;
                    if (_old_178 != _src_2_168) {
                        lh->sarg = _src_2_168;
                        optimized = 1;
                        if (_old_178 && _old_178 != lh->farg && _old_178 != lh->sarg && _old_178 != lh->targ && _old_178 != _src_1_167 && _old_178 != _src_2_168 && _old_178 != _keep_0_0_171 && _old_178 != _keep_0_1_172 && _old_178 != _keep_0_2_173 && _old_178 != _keep_1_0_174 && _old_178 != _keep_1_1_175 && _old_178 != _keep_1_2_176) {
                            LIR_unload_subject(_old_178);
                        }
                    }
                    lir_subject_t* _old_179 = lh->targ;
                    if (_old_179) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_179 && _old_179 != lh->farg && _old_179 != lh->sarg && _old_179 != lh->targ && _old_179 != _src_1_167 && _old_179 != _src_2_168 && _old_179 != _keep_0_0_171 && _old_179 != _keep_0_1_172 && _old_179 != _keep_0_2_173 && _old_179 != _keep_1_0_174 && _old_179 != _keep_1_1_175 && _old_179 != _keep_1_2_176) {
                            LIR_unload_subject(_old_179);
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
                    lir_subject_t* _src_1_180 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_181 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_3_182 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_183 = lh->op;
                    lir_operation_t _match_op_1_184 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_185 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_186 = lh->farg;
                    lir_subject_t* _keep_0_1_187 = lh->sarg;
                    lir_subject_t* _keep_0_2_188 = lh->targ;
                    lir_subject_t* _keep_1_0_189 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_190 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_191 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_192 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_193 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_194 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_183) {
                        lh->op = _match_op_0_183;
                        optimized = 1;
                    }
                    lir_subject_t* _old_195 = lh->farg;
                    if (_old_195 != _src_1_180) {
                        lh->farg = _src_1_180;
                        optimized = 1;
                        if (_old_195 && _old_195 != lh->farg && _old_195 != lh->sarg && _old_195 != lh->targ && _old_195 != _src_1_180 && _old_195 != _src_2_181 && _old_195 != _src_3_182 && _old_195 != _keep_0_0_186 && _old_195 != _keep_0_1_187 && _old_195 != _keep_0_2_188 && _old_195 != _keep_1_0_189 && _old_195 != _keep_1_1_190 && _old_195 != _keep_1_2_191 && _old_195 != _keep_2_0_192 && _old_195 != _keep_2_1_193 && _old_195 != _keep_2_2_194) {
                            LIR_unload_subject(_old_195);
                        }
                    }
                    lir_subject_t* _old_196 = lh->sarg;
                    if (_old_196 != _src_2_181) {
                        lh->sarg = _src_2_181;
                        optimized = 1;
                        if (_old_196 && _old_196 != lh->farg && _old_196 != lh->sarg && _old_196 != lh->targ && _old_196 != _src_1_180 && _old_196 != _src_2_181 && _old_196 != _src_3_182 && _old_196 != _keep_0_0_186 && _old_196 != _keep_0_1_187 && _old_196 != _keep_0_2_188 && _old_196 != _keep_1_0_189 && _old_196 != _keep_1_1_190 && _old_196 != _keep_1_2_191 && _old_196 != _keep_2_0_192 && _old_196 != _keep_2_1_193 && _old_196 != _keep_2_2_194) {
                            LIR_unload_subject(_old_196);
                        }
                    }
                    lir_subject_t* _old_197 = lh->targ;
                    if (_old_197) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_197 && _old_197 != lh->farg && _old_197 != lh->sarg && _old_197 != lh->targ && _old_197 != _src_1_180 && _old_197 != _src_2_181 && _old_197 != _src_3_182 && _old_197 != _keep_0_0_186 && _old_197 != _keep_0_1_187 && _old_197 != _keep_0_2_188 && _old_197 != _keep_1_0_189 && _old_197 != _keep_1_1_190 && _old_197 != _keep_1_2_191 && _old_197 != _keep_2_0_192 && _old_197 != _keep_2_1_193 && _old_197 != _keep_2_2_194) {
                            LIR_unload_subject(_old_197);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_198 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_198 != _src_2_181) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_181;
                        optimized = 1;
                        if (_old_198 && _old_198 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_198 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_198 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_198 != _src_1_180 && _old_198 != _src_2_181 && _old_198 != _src_3_182 && _old_198 != _keep_0_0_186 && _old_198 != _keep_0_1_187 && _old_198 != _keep_0_2_188 && _old_198 != _keep_1_0_189 && _old_198 != _keep_1_1_190 && _old_198 != _keep_1_2_191 && _old_198 != _keep_2_0_192 && _old_198 != _keep_2_1_193 && _old_198 != _keep_2_2_194) {
                            LIR_unload_subject(_old_198);
                        }
                    }
                    lir_subject_t* _old_199 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_199 != _src_3_182) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_3_182;
                        optimized = 1;
                        if (_old_199 && _old_199 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_199 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_199 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_199 != _src_1_180 && _old_199 != _src_2_181 && _old_199 != _src_3_182 && _old_199 != _keep_0_0_186 && _old_199 != _keep_0_1_187 && _old_199 != _keep_0_2_188 && _old_199 != _keep_1_0_189 && _old_199 != _keep_1_1_190 && _old_199 != _keep_1_2_191 && _old_199 != _keep_2_0_192 && _old_199 != _keep_2_1_193 && _old_199 != _keep_2_2_194) {
                            LIR_unload_subject(_old_199);
                        }
                    }
                    lir_subject_t* _old_200 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_200 != _src_2_181) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_181;
                        optimized = 1;
                        if (_old_200 && _old_200 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_200 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_200 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_200 != _src_1_180 && _old_200 != _src_2_181 && _old_200 != _src_3_182 && _old_200 != _keep_0_0_186 && _old_200 != _keep_0_1_187 && _old_200 != _keep_0_2_188 && _old_200 != _keep_1_0_189 && _old_200 != _keep_1_1_190 && _old_200 != _keep_1_2_191 && _old_200 != _keep_2_0_192 && _old_200 != _keep_2_1_193 && _old_200 != _keep_2_2_194) {
                            LIR_unload_subject(_old_200);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iSUB &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                ((LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ) == 1) &&
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
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_201 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_202 = lh->sarg;
                    lir_subject_t* _src_3_203 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_204 = lh->op;
                    lir_operation_t _match_op_1_205 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_206 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_207 = lh->farg;
                    lir_subject_t* _keep_0_1_208 = lh->sarg;
                    lir_subject_t* _keep_0_2_209 = lh->targ;
                    lir_subject_t* _keep_1_0_210 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_211 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_212 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_213 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_214 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_215 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_216 = lh->sarg;
                    if (_old_216 != _src_2_202) {
                        lh->sarg = _src_2_202;
                        optimized = 1;
                        if (_old_216 && _old_216 != lh->farg && _old_216 != lh->sarg && _old_216 != lh->targ && _old_216 != _src_1_201 && _old_216 != _src_2_202 && _old_216 != _src_3_203 && _old_216 != _keep_0_0_207 && _old_216 != _keep_0_1_208 && _old_216 != _keep_0_2_209 && _old_216 != _keep_1_0_210 && _old_216 != _keep_1_1_211 && _old_216 != _keep_1_2_212 && _old_216 != _keep_2_0_213 && _old_216 != _keep_2_1_214 && _old_216 != _keep_2_2_215) {
                            LIR_unload_subject(_old_216);
                        }
                    }
                    lir_subject_t* _old_217 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_217 && _old_217 != lh->farg && _old_217 != lh->sarg && _old_217 != lh->targ && _old_217 != _src_1_201 && _old_217 != _src_2_202 && _old_217 != _src_3_203 && _old_217 != _keep_0_0_207 && _old_217 != _keep_0_1_208 && _old_217 != _keep_0_2_209 && _old_217 != _keep_1_0_210 && _old_217 != _keep_1_1_211 && _old_217 != _keep_1_2_212 && _old_217 != _keep_2_0_213 && _old_217 != _keep_2_1_214 && _old_217 != _keep_2_2_215) {
                        LIR_unload_subject(_old_217);
                    }
                    lir_subject_t* _old_218 = lh->farg;
                    if (_old_218 != _src_2_202) {
                        lh->farg = _src_2_202;
                        optimized = 1;
                        if (_old_218 && _old_218 != lh->farg && _old_218 != lh->sarg && _old_218 != lh->targ && _old_218 != _src_1_201 && _old_218 != _src_2_202 && _old_218 != _src_3_203 && _old_218 != _keep_0_0_207 && _old_218 != _keep_0_1_208 && _old_218 != _keep_0_2_209 && _old_218 != _keep_1_0_210 && _old_218 != _keep_1_1_211 && _old_218 != _keep_1_2_212 && _old_218 != _keep_2_0_213 && _old_218 != _keep_2_1_214 && _old_218 != _keep_2_2_215) {
                            LIR_unload_subject(_old_218);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_204) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_204;
                        optimized = 1;
                    }
                    lir_subject_t* _old_219 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_219 != _src_3_203) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_203;
                        optimized = 1;
                        if (_old_219 && _old_219 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_219 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_219 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_219 != _src_1_201 && _old_219 != _src_2_202 && _old_219 != _src_3_203 && _old_219 != _keep_0_0_207 && _old_219 != _keep_0_1_208 && _old_219 != _keep_0_2_209 && _old_219 != _keep_1_0_210 && _old_219 != _keep_1_1_211 && _old_219 != _keep_1_2_212 && _old_219 != _keep_2_0_213 && _old_219 != _keep_2_1_214 && _old_219 != _keep_2_2_215) {
                            LIR_unload_subject(_old_219);
                        }
                    }
                    lir_subject_t* _old_220 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_220 != _src_2_202) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_202;
                        optimized = 1;
                        if (_old_220 && _old_220 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_220 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_220 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_220 != _src_1_201 && _old_220 != _src_2_202 && _old_220 != _src_3_203 && _old_220 != _keep_0_0_207 && _old_220 != _keep_0_1_208 && _old_220 != _keep_0_2_209 && _old_220 != _keep_1_0_210 && _old_220 != _keep_1_1_211 && _old_220 != _keep_1_2_212 && _old_220 != _keep_2_0_213 && _old_220 != _keep_2_1_214 && _old_220 != _keep_2_2_215) {
                            LIR_unload_subject(_old_220);
                        }
                    }
                    lir_subject_t* _old_221 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_221) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_221 && _old_221 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_221 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_221 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_221 != _src_1_201 && _old_221 != _src_2_202 && _old_221 != _src_3_203 && _old_221 != _keep_0_0_207 && _old_221 != _keep_0_1_208 && _old_221 != _keep_0_2_209 && _old_221 != _keep_1_0_210 && _old_221 != _keep_1_1_211 && _old_221 != _keep_1_2_212 && _old_221 != _keep_2_0_213 && _old_221 != _keep_2_1_214 && _old_221 != _keep_2_2_215) {
                            LIR_unload_subject(_old_221);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iSUB &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ &&
                ((LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ) == 1) &&
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
                    lir_subject_t* _src_2_222 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_1_223 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_224 = lh->op;
                    lir_operation_t _match_op_1_225 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_226 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_227 = lh->farg;
                    lir_subject_t* _keep_0_1_228 = lh->sarg;
                    lir_subject_t* _keep_0_2_229 = lh->targ;
                    lir_subject_t* _keep_1_0_230 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_231 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_232 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_233 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_234 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_235 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_224) {
                        lh->op = _match_op_0_224;
                        optimized = 1;
                    }
                    lir_subject_t* _old_236 = lh->farg;
                    if (_old_236 != _src_2_222) {
                        lh->farg = _src_2_222;
                        optimized = 1;
                        if (_old_236 && _old_236 != lh->farg && _old_236 != lh->sarg && _old_236 != lh->targ && _old_236 != _src_2_222 && _old_236 != _src_1_223 && _old_236 != _keep_0_0_227 && _old_236 != _keep_0_1_228 && _old_236 != _keep_0_2_229 && _old_236 != _keep_1_0_230 && _old_236 != _keep_1_1_231 && _old_236 != _keep_1_2_232 && _old_236 != _keep_2_0_233 && _old_236 != _keep_2_1_234 && _old_236 != _keep_2_2_235) {
                            LIR_unload_subject(_old_236);
                        }
                    }
                    lir_subject_t* _old_237 = lh->sarg;
                    if (_old_237 != _src_1_223) {
                        lh->sarg = _src_1_223;
                        optimized = 1;
                        if (_old_237 && _old_237 != lh->farg && _old_237 != lh->sarg && _old_237 != lh->targ && _old_237 != _src_2_222 && _old_237 != _src_1_223 && _old_237 != _keep_0_0_227 && _old_237 != _keep_0_1_228 && _old_237 != _keep_0_2_229 && _old_237 != _keep_1_0_230 && _old_237 != _keep_1_1_231 && _old_237 != _keep_1_2_232 && _old_237 != _keep_2_0_233 && _old_237 != _keep_2_1_234 && _old_237 != _keep_2_2_235) {
                            LIR_unload_subject(_old_237);
                        }
                    }
                    lir_subject_t* _old_238 = lh->targ;
                    if (_old_238) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_238 && _old_238 != lh->farg && _old_238 != lh->sarg && _old_238 != lh->targ && _old_238 != _src_2_222 && _old_238 != _src_1_223 && _old_238 != _keep_0_0_227 && _old_238 != _keep_0_1_228 && _old_238 != _keep_0_2_229 && _old_238 != _keep_1_0_230 && _old_238 != _keep_1_1_231 && _old_238 != _keep_1_2_232 && _old_238 != _keep_2_0_233 && _old_238 != _keep_2_1_234 && _old_238 != _keep_2_2_235) {
                            LIR_unload_subject(_old_238);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iSUB) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_239 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_239 != _src_1_223) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_223;
                        optimized = 1;
                        if (_old_239 && _old_239 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_239 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_239 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_239 != _src_2_222 && _old_239 != _src_1_223 && _old_239 != _keep_0_0_227 && _old_239 != _keep_0_1_228 && _old_239 != _keep_0_2_229 && _old_239 != _keep_1_0_230 && _old_239 != _keep_1_1_231 && _old_239 != _keep_1_2_232 && _old_239 != _keep_2_0_233 && _old_239 != _keep_2_1_234 && _old_239 != _keep_2_2_235) {
                            LIR_unload_subject(_old_239);
                        }
                    }
                    lir_subject_t* _old_240 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_240 && _old_240 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_240 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_240 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_240 != _src_2_222 && _old_240 != _src_1_223 && _old_240 != _keep_0_0_227 && _old_240 != _keep_0_1_228 && _old_240 != _keep_0_2_229 && _old_240 != _keep_1_0_230 && _old_240 != _keep_1_1_231 && _old_240 != _keep_1_2_232 && _old_240 != _keep_2_0_233 && _old_240 != _keep_2_1_234 && _old_240 != _keep_2_2_235) {
                        LIR_unload_subject(_old_240);
                    }
                    lir_subject_t* _old_241 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_241 != _src_1_223) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_223;
                        optimized = 1;
                        if (_old_241 && _old_241 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_241 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_241 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_241 != _src_2_222 && _old_241 != _src_1_223 && _old_241 != _keep_0_0_227 && _old_241 != _keep_0_1_228 && _old_241 != _keep_0_2_229 && _old_241 != _keep_1_0_230 && _old_241 != _keep_1_1_231 && _old_241 != _keep_1_2_232 && _old_241 != _keep_2_0_233 && _old_241 != _keep_2_1_234 && _old_241 != _keep_2_2_235) {
                            LIR_unload_subject(_old_241);
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
                    lir_subject_t* _src_1_312 = lh->farg;
                    lir_operation_t _match_op_0_313 = lh->op;
                    lir_subject_t* _keep_0_0_314 = lh->farg;
                    lir_subject_t* _keep_0_1_315 = lh->sarg;
                    lir_subject_t* _keep_0_2_316 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_317 = lh->farg;
                    if (_old_317 != _src_1_312) {
                        lh->farg = _src_1_312;
                        optimized = 1;
                        if (_old_317 && _old_317 != lh->farg && _old_317 != lh->sarg && _old_317 != lh->targ && _old_317 != _src_1_312 && _old_317 != _keep_0_0_314 && _old_317 != _keep_0_1_315 && _old_317 != _keep_0_2_316) {
                            LIR_unload_subject(_old_317);
                        }
                    }
                    lir_subject_t* _old_318 = lh->sarg;
                    if (_old_318 != _src_1_312) {
                        lh->sarg = _src_1_312;
                        optimized = 1;
                        if (_old_318 && _old_318 != lh->farg && _old_318 != lh->sarg && _old_318 != lh->targ && _old_318 != _src_1_312 && _old_318 != _keep_0_0_314 && _old_318 != _keep_0_1_315 && _old_318 != _keep_0_2_316) {
                            LIR_unload_subject(_old_318);
                        }
                    }
                    lir_subject_t* _old_319 = lh->targ;
                    if (_old_319) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_319 && _old_319 != lh->farg && _old_319 != lh->sarg && _old_319 != lh->targ && _old_319 != _src_1_312 && _old_319 != _keep_0_0_314 && _old_319 != _keep_0_1_315 && _old_319 != _keep_0_2_316) {
                            LIR_unload_subject(_old_319);
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
                    lir_subject_t* _src_1_344 = lh->farg;
                    lir_operation_t _match_op_0_345 = lh->op;
                    lir_subject_t* _keep_0_0_346 = lh->farg;
                    lir_subject_t* _keep_0_1_347 = lh->sarg;
                    lir_subject_t* _keep_0_2_348 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_349 = lh->sarg;
                    if (_old_349 != _src_1_344) {
                        lh->sarg = _src_1_344;
                        optimized = 1;
                        if (_old_349 && _old_349 != lh->farg && _old_349 != lh->sarg && _old_349 != lh->targ && _old_349 != _src_1_344 && _old_349 != _keep_0_0_346 && _old_349 != _keep_0_1_347 && _old_349 != _keep_0_2_348) {
                            LIR_unload_subject(_old_349);
                        }
                    }
                    lir_subject_t* _old_350 = lh->targ;
                    if (_old_350 != _src_1_344) {
                        lh->targ = _src_1_344;
                        optimized = 1;
                        if (_old_350 && _old_350 != lh->farg && _old_350 != lh->sarg && _old_350 != lh->targ && _old_350 != _src_1_344 && _old_350 != _keep_0_0_346 && _old_350 != _keep_0_1_347 && _old_350 != _keep_0_2_348) {
                            LIR_unload_subject(_old_350);
                        }
                    }
                    lir_subject_t* _old_351 = lh->farg;
                    if (_old_351 != _src_1_344) {
                        lh->farg = _src_1_344;
                        optimized = 1;
                        if (_old_351 && _old_351 != lh->farg && _old_351 != lh->sarg && _old_351 != lh->targ && _old_351 != _src_1_344 && _old_351 != _keep_0_0_346 && _old_351 != _keep_0_1_347 && _old_351 != _keep_0_2_348) {
                            LIR_unload_subject(_old_351);
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
                    lir_subject_t* _src_1_304 = lh->farg;
                    lir_operation_t _match_op_0_305 = lh->op;
                    lir_subject_t* _keep_0_0_306 = lh->farg;
                    lir_subject_t* _keep_0_1_307 = lh->sarg;
                    lir_subject_t* _keep_0_2_308 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_309 = lh->farg;
                    if (_old_309 != _src_1_304) {
                        lh->farg = _src_1_304;
                        optimized = 1;
                        if (_old_309 && _old_309 != lh->farg && _old_309 != lh->sarg && _old_309 != lh->targ && _old_309 != _src_1_304 && _old_309 != _keep_0_0_306 && _old_309 != _keep_0_1_307 && _old_309 != _keep_0_2_308) {
                            LIR_unload_subject(_old_309);
                        }
                    }
                    lir_subject_t* _old_310 = lh->sarg;
                    if (_old_310 != _src_1_304) {
                        lh->sarg = _src_1_304;
                        optimized = 1;
                        if (_old_310 && _old_310 != lh->farg && _old_310 != lh->sarg && _old_310 != lh->targ && _old_310 != _src_1_304 && _old_310 != _keep_0_0_306 && _old_310 != _keep_0_1_307 && _old_310 != _keep_0_2_308) {
                            LIR_unload_subject(_old_310);
                        }
                    }
                    lir_subject_t* _old_311 = lh->targ;
                    if (_old_311) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_311 && _old_311 != lh->farg && _old_311 != lh->sarg && _old_311 != lh->targ && _old_311 != _src_1_304 && _old_311 != _keep_0_0_306 && _old_311 != _keep_0_1_307 && _old_311 != _keep_0_2_308) {
                            LIR_unload_subject(_old_311);
                        }
                    }
                }
                break;
            }

            case LIR_bSHL:
             {
                if (lh->op == LIR_bSHL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                    lir_subject_t* _src_1_127 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_128 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_129 = lh->op;
                    lir_operation_t _match_op_1_130 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_131 = lh->farg;
                    lir_subject_t* _keep_0_1_132 = lh->sarg;
                    lir_subject_t* _keep_0_2_133 = lh->targ;
                    lir_subject_t* _keep_1_0_134 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_135 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_136 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_137 = lh->sarg;
                    if (_old_137 != _src_1_127) {
                        lh->sarg = _src_1_127;
                        optimized = 1;
                        if (_old_137 && _old_137 != lh->farg && _old_137 != lh->sarg && _old_137 != lh->targ && _old_137 != _src_1_127 && _old_137 != _src_2_128 && _old_137 != _keep_0_0_131 && _old_137 != _keep_0_1_132 && _old_137 != _keep_0_2_133 && _old_137 != _keep_1_0_134 && _old_137 != _keep_1_1_135 && _old_137 != _keep_1_2_136) {
                            LIR_unload_subject(_old_137);
                        }
                    }
                    lir_subject_t* _old_138 = lh->targ;
                    if (_old_138 != _src_1_127) {
                        lh->targ = _src_1_127;
                        optimized = 1;
                        if (_old_138 && _old_138 != lh->farg && _old_138 != lh->sarg && _old_138 != lh->targ && _old_138 != _src_1_127 && _old_138 != _src_2_128 && _old_138 != _keep_0_0_131 && _old_138 != _keep_0_1_132 && _old_138 != _keep_0_2_133 && _old_138 != _keep_1_0_134 && _old_138 != _keep_1_1_135 && _old_138 != _keep_1_2_136) {
                            LIR_unload_subject(_old_138);
                        }
                    }
                    lir_subject_t* _old_139 = lh->farg;
                    if (_old_139 != _src_1_127) {
                        lh->farg = _src_1_127;
                        optimized = 1;
                        if (_old_139 && _old_139 != lh->farg && _old_139 != lh->sarg && _old_139 != lh->targ && _old_139 != _src_1_127 && _old_139 != _src_2_128 && _old_139 != _keep_0_0_131 && _old_139 != _keep_0_1_132 && _old_139 != _keep_0_2_133 && _old_139 != _keep_1_0_134 && _old_139 != _keep_1_1_135 && _old_139 != _keep_1_2_136) {
                            LIR_unload_subject(_old_139);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_140 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_140 != _src_2_128) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_128;
                        optimized = 1;
                        if (_old_140 && _old_140 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_140 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_140 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_140 != _src_1_127 && _old_140 != _src_2_128 && _old_140 != _keep_0_0_131 && _old_140 != _keep_0_1_132 && _old_140 != _keep_0_2_133 && _old_140 != _keep_1_0_134 && _old_140 != _keep_1_1_135 && _old_140 != _keep_1_2_136) {
                            LIR_unload_subject(_old_140);
                        }
                    }
                    lir_subject_t* _old_141 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_141 != _src_2_128) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_2_128;
                        optimized = 1;
                        if (_old_141 && _old_141 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_141 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_141 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_141 != _src_1_127 && _old_141 != _src_2_128 && _old_141 != _keep_0_0_131 && _old_141 != _keep_0_1_132 && _old_141 != _keep_0_2_133 && _old_141 != _keep_1_0_134 && _old_141 != _keep_1_1_135 && _old_141 != _keep_1_2_136) {
                            LIR_unload_subject(_old_141);
                        }
                    }
                    lir_subject_t* _old_142 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_142 != _src_2_128) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_128;
                        optimized = 1;
                        if (_old_142 && _old_142 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_142 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_142 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_142 != _src_1_127 && _old_142 != _src_2_128 && _old_142 != _keep_0_0_131 && _old_142 != _keep_0_1_132 && _old_142 != _keep_0_2_133 && _old_142 != _keep_1_0_134 && _old_142 != _keep_1_1_135 && _old_142 != _keep_1_2_136) {
                            LIR_unload_subject(_old_142);
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
                    lir_subject_t* _src_1_320 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_321 = lh->op;
                    lir_operation_t _match_op_1_322 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_323 = lh->farg;
                    lir_subject_t* _keep_0_1_324 = lh->sarg;
                    lir_subject_t* _keep_0_2_325 = lh->targ;
                    lir_subject_t* _keep_1_0_326 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_327 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_328 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_329 = lh->sarg;
                    if (_old_329 != _src_1_320) {
                        lh->sarg = _src_1_320;
                        optimized = 1;
                        if (_old_329 && _old_329 != lh->farg && _old_329 != lh->sarg && _old_329 != lh->targ && _old_329 != _src_1_320 && _old_329 != _keep_0_0_323 && _old_329 != _keep_0_1_324 && _old_329 != _keep_0_2_325 && _old_329 != _keep_1_0_326 && _old_329 != _keep_1_1_327 && _old_329 != _keep_1_2_328) {
                            LIR_unload_subject(_old_329);
                        }
                    }
                    lir_subject_t* _old_330 = lh->targ;
                    if (_old_330 != _src_1_320) {
                        lh->targ = _src_1_320;
                        optimized = 1;
                        if (_old_330 && _old_330 != lh->farg && _old_330 != lh->sarg && _old_330 != lh->targ && _old_330 != _src_1_320 && _old_330 != _keep_0_0_323 && _old_330 != _keep_0_1_324 && _old_330 != _keep_0_2_325 && _old_330 != _keep_1_0_326 && _old_330 != _keep_1_1_327 && _old_330 != _keep_1_2_328) {
                            LIR_unload_subject(_old_330);
                        }
                    }
                    lir_subject_t* _old_331 = lh->farg;
                    if (_old_331 != _src_1_320) {
                        lh->farg = _src_1_320;
                        optimized = 1;
                        if (_old_331 && _old_331 != lh->farg && _old_331 != lh->sarg && _old_331 != lh->targ && _old_331 != _src_1_320 && _old_331 != _keep_0_0_323 && _old_331 != _keep_0_1_324 && _old_331 != _keep_0_2_325 && _old_331 != _keep_1_0_326 && _old_331 != _keep_1_1_327 && _old_331 != _keep_1_2_328) {
                            LIR_unload_subject(_old_331);
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
                    lir_subject_t* _src_1_332 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_333 = lh->op;
                    lir_operation_t _match_op_1_334 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_335 = lh->farg;
                    lir_subject_t* _keep_0_1_336 = lh->sarg;
                    lir_subject_t* _keep_0_2_337 = lh->targ;
                    lir_subject_t* _keep_1_0_338 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_339 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_340 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_341 = lh->sarg;
                    if (_old_341 != _src_1_332) {
                        lh->sarg = _src_1_332;
                        optimized = 1;
                        if (_old_341 && _old_341 != lh->farg && _old_341 != lh->sarg && _old_341 != lh->targ && _old_341 != _src_1_332 && _old_341 != _keep_0_0_335 && _old_341 != _keep_0_1_336 && _old_341 != _keep_0_2_337 && _old_341 != _keep_1_0_338 && _old_341 != _keep_1_1_339 && _old_341 != _keep_1_2_340) {
                            LIR_unload_subject(_old_341);
                        }
                    }
                    lir_subject_t* _old_342 = lh->targ;
                    if (_old_342 != _src_1_332) {
                        lh->targ = _src_1_332;
                        optimized = 1;
                        if (_old_342 && _old_342 != lh->farg && _old_342 != lh->sarg && _old_342 != lh->targ && _old_342 != _src_1_332 && _old_342 != _keep_0_0_335 && _old_342 != _keep_0_1_336 && _old_342 != _keep_0_2_337 && _old_342 != _keep_1_0_338 && _old_342 != _keep_1_1_339 && _old_342 != _keep_1_2_340) {
                            LIR_unload_subject(_old_342);
                        }
                    }
                    lir_subject_t* _old_343 = lh->farg;
                    if (_old_343 != _src_1_332) {
                        lh->farg = _src_1_332;
                        optimized = 1;
                        if (_old_343 && _old_343 != lh->farg && _old_343 != lh->sarg && _old_343 != lh->targ && _old_343 != _src_1_332 && _old_343 != _keep_0_0_335 && _old_343 != _keep_0_1_336 && _old_343 != _keep_0_2_337 && _old_343 != _keep_1_0_338 && _old_343 != _keep_1_1_339 && _old_343 != _keep_1_2_340) {
                            LIR_unload_subject(_old_343);
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
                    lir_subject_t* _src_1_352 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_353 = lh->op;
                    lir_operation_t _match_op_1_354 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_355 = lh->farg;
                    lir_subject_t* _keep_0_1_356 = lh->sarg;
                    lir_subject_t* _keep_0_2_357 = lh->targ;
                    lir_subject_t* _keep_1_0_358 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_359 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_360 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_361 = lh->sarg;
                    if (_old_361 != _src_1_352) {
                        lh->sarg = _src_1_352;
                        optimized = 1;
                        if (_old_361 && _old_361 != lh->farg && _old_361 != lh->sarg && _old_361 != lh->targ && _old_361 != _src_1_352 && _old_361 != _keep_0_0_355 && _old_361 != _keep_0_1_356 && _old_361 != _keep_0_2_357 && _old_361 != _keep_1_0_358 && _old_361 != _keep_1_1_359 && _old_361 != _keep_1_2_360) {
                            LIR_unload_subject(_old_361);
                        }
                    }
                    lir_subject_t* _old_362 = lh->targ;
                    if (_old_362 != _src_1_352) {
                        lh->targ = _src_1_352;
                        optimized = 1;
                        if (_old_362 && _old_362 != lh->farg && _old_362 != lh->sarg && _old_362 != lh->targ && _old_362 != _src_1_352 && _old_362 != _keep_0_0_355 && _old_362 != _keep_0_1_356 && _old_362 != _keep_0_2_357 && _old_362 != _keep_1_0_358 && _old_362 != _keep_1_1_359 && _old_362 != _keep_1_2_360) {
                            LIR_unload_subject(_old_362);
                        }
                    }
                    lir_subject_t* _old_363 = lh->farg;
                    if (_old_363 != _src_1_352) {
                        lh->farg = _src_1_352;
                        optimized = 1;
                        if (_old_363 && _old_363 != lh->farg && _old_363 != lh->sarg && _old_363 != lh->targ && _old_363 != _src_1_352 && _old_363 != _keep_0_0_355 && _old_363 != _keep_0_1_356 && _old_363 != _keep_0_2_357 && _old_363 != _keep_1_0_358 && _old_363 != _keep_1_1_359 && _old_363 != _keep_1_2_360) {
                            LIR_unload_subject(_old_363);
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_operation_t _match_op_0_250 = lh->op;
                    lir_subject_t* _keep_0_0_251 = lh->farg;
                    lir_subject_t* _keep_0_1_252 = lh->sarg;
                    lir_subject_t* _keep_0_2_253 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_254 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_254 && _old_254 != lh->farg && _old_254 != lh->sarg && _old_254 != lh->targ && _old_254 != _keep_0_0_251 && _old_254 != _keep_0_1_252 && _old_254 != _keep_0_2_253) {
                        LIR_unload_subject(_old_254);
                    }
                    lir_subject_t* _old_255 = lh->targ;
                    if (_old_255) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_255 && _old_255 != lh->farg && _old_255 != lh->sarg && _old_255 != lh->targ && _old_255 != _keep_0_0_251 && _old_255 != _keep_0_1_252 && _old_255 != _keep_0_2_253) {
                            LIR_unload_subject(_old_255);
                        }
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                (lh->sarg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL || lh->sarg->t == LIR_MEMORY || lh->sarg->t == LIR_LABEL) &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                (lh->farg->t == LIR_REGISTER || lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL || lh->farg->t == LIR_MEMORY || lh->farg->t == LIR_LABEL) &&
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
                    lir_subject_t* _src_1_242 = lh->farg;
                    lir_operation_t _match_op_0_243 = lh->op;
                    lir_subject_t* _keep_0_0_244 = lh->farg;
                    lir_subject_t* _keep_0_1_245 = lh->sarg;
                    lir_subject_t* _keep_0_2_246 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_247 = lh->sarg;
                    if (_old_247 != _src_1_242) {
                        lh->sarg = _src_1_242;
                        optimized = 1;
                        if (_old_247 && _old_247 != lh->farg && _old_247 != lh->sarg && _old_247 != lh->targ && _old_247 != _src_1_242 && _old_247 != _keep_0_0_244 && _old_247 != _keep_0_1_245 && _old_247 != _keep_0_2_246) {
                            LIR_unload_subject(_old_247);
                        }
                    }
                    lir_subject_t* _old_248 = lh->targ;
                    if (_old_248 != _src_1_242) {
                        lh->targ = _src_1_242;
                        optimized = 1;
                        if (_old_248 && _old_248 != lh->farg && _old_248 != lh->sarg && _old_248 != lh->targ && _old_248 != _src_1_242 && _old_248 != _keep_0_0_244 && _old_248 != _keep_0_1_245 && _old_248 != _keep_0_2_246) {
                            LIR_unload_subject(_old_248);
                        }
                    }
                    lir_subject_t* _old_249 = lh->farg;
                    if (_old_249 != _src_1_242) {
                        lh->farg = _src_1_242;
                        optimized = 1;
                        if (_old_249 && _old_249 != lh->farg && _old_249 != lh->sarg && _old_249 != lh->targ && _old_249 != _src_1_242 && _old_249 != _keep_0_0_244 && _old_249 != _keep_0_1_245 && _old_249 != _keep_0_2_246) {
                            LIR_unload_subject(_old_249);
                        }
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_SETE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_STNE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_SETG || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_STGE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_SETL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_STLE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_256 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_257 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_258 = lh->op;
                    lir_operation_t _match_op_1_259 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_260 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_261 = lh->farg;
                    lir_subject_t* _keep_0_1_262 = lh->sarg;
                    lir_subject_t* _keep_0_2_263 = lh->targ;
                    lir_subject_t* _keep_1_0_264 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_265 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_266 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_267 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_268 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_269 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_270 = lh->sarg;
                    if (_old_270 != _src_1_256) {
                        lh->sarg = _src_1_256;
                        optimized = 1;
                        if (_old_270 && _old_270 != lh->farg && _old_270 != lh->sarg && _old_270 != lh->targ && _old_270 != _src_1_256 && _old_270 != _src_2_257 && _old_270 != _keep_0_0_261 && _old_270 != _keep_0_1_262 && _old_270 != _keep_0_2_263 && _old_270 != _keep_1_0_264 && _old_270 != _keep_1_1_265 && _old_270 != _keep_1_2_266 && _old_270 != _keep_2_0_267 && _old_270 != _keep_2_1_268 && _old_270 != _keep_2_2_269) {
                            LIR_unload_subject(_old_270);
                        }
                    }
                    lir_subject_t* _old_271 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_271 && _old_271 != lh->farg && _old_271 != lh->sarg && _old_271 != lh->targ && _old_271 != _src_1_256 && _old_271 != _src_2_257 && _old_271 != _keep_0_0_261 && _old_271 != _keep_0_1_262 && _old_271 != _keep_0_2_263 && _old_271 != _keep_1_0_264 && _old_271 != _keep_1_1_265 && _old_271 != _keep_1_2_266 && _old_271 != _keep_2_0_267 && _old_271 != _keep_2_1_268 && _old_271 != _keep_2_2_269) {
                        LIR_unload_subject(_old_271);
                    }
                    lir_subject_t* _old_272 = lh->farg;
                    if (_old_272 != _src_1_256) {
                        lh->farg = _src_1_256;
                        optimized = 1;
                        if (_old_272 && _old_272 != lh->farg && _old_272 != lh->sarg && _old_272 != lh->targ && _old_272 != _src_1_256 && _old_272 != _src_2_257 && _old_272 != _keep_0_0_261 && _old_272 != _keep_0_1_262 && _old_272 != _keep_0_2_263 && _old_272 != _keep_1_0_264 && _old_272 != _keep_1_1_265 && _old_272 != _keep_1_2_266 && _old_272 != _keep_2_0_267 && _old_272 != _keep_2_1_268 && _old_272 != _keep_2_2_269) {
                            LIR_unload_subject(_old_272);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_260) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_260;
                        optimized = 1;
                    }
                    lir_subject_t* _old_273 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_273 != _src_2_257) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_257;
                        optimized = 1;
                        if (_old_273 && _old_273 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_273 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_273 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_273 != _src_1_256 && _old_273 != _src_2_257 && _old_273 != _keep_0_0_261 && _old_273 != _keep_0_1_262 && _old_273 != _keep_0_2_263 && _old_273 != _keep_1_0_264 && _old_273 != _keep_1_1_265 && _old_273 != _keep_1_2_266 && _old_273 != _keep_2_0_267 && _old_273 != _keep_2_1_268 && _old_273 != _keep_2_2_269) {
                            LIR_unload_subject(_old_273);
                        }
                    }
                    lir_subject_t* _old_274 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_274) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_274 && _old_274 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_274 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_274 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_274 != _src_1_256 && _old_274 != _src_2_257 && _old_274 != _keep_0_0_261 && _old_274 != _keep_0_1_262 && _old_274 != _keep_0_2_263 && _old_274 != _keep_1_0_264 && _old_274 != _keep_1_1_265 && _old_274 != _keep_1_2_266 && _old_274 != _keep_2_0_267 && _old_274 != _keep_2_1_268 && _old_274 != _keep_2_2_269) {
                            LIR_unload_subject(_old_274);
                        }
                    }
                    lir_subject_t* _old_275 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_275) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_275 && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_275 != _src_1_256 && _old_275 != _src_2_257 && _old_275 != _keep_0_0_261 && _old_275 != _keep_0_1_262 && _old_275 != _keep_0_2_263 && _old_275 != _keep_1_0_264 && _old_275 != _keep_1_1_265 && _old_275 != _keep_1_2_266 && _old_275 != _keep_2_0_267 && _old_275 != _keep_2_1_268 && _old_275 != _keep_2_2_269) {
                            LIR_unload_subject(_old_275);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
                        optimized = 1;
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_TST &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_276 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_277 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_278 = lh->op;
                    lir_operation_t _match_op_1_279 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_280 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_281 = lh->farg;
                    lir_subject_t* _keep_0_1_282 = lh->sarg;
                    lir_subject_t* _keep_0_2_283 = lh->targ;
                    lir_subject_t* _keep_1_0_284 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_285 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_286 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_287 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_288 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_289 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_290 = lh->sarg;
                    if (_old_290 != _src_1_276) {
                        lh->sarg = _src_1_276;
                        optimized = 1;
                        if (_old_290 && _old_290 != lh->farg && _old_290 != lh->sarg && _old_290 != lh->targ && _old_290 != _src_1_276 && _old_290 != _src_2_277 && _old_290 != _keep_0_0_281 && _old_290 != _keep_0_1_282 && _old_290 != _keep_0_2_283 && _old_290 != _keep_1_0_284 && _old_290 != _keep_1_1_285 && _old_290 != _keep_1_2_286 && _old_290 != _keep_2_0_287 && _old_290 != _keep_2_1_288 && _old_290 != _keep_2_2_289) {
                            LIR_unload_subject(_old_290);
                        }
                    }
                    lir_subject_t* _old_291 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_291 && _old_291 != lh->farg && _old_291 != lh->sarg && _old_291 != lh->targ && _old_291 != _src_1_276 && _old_291 != _src_2_277 && _old_291 != _keep_0_0_281 && _old_291 != _keep_0_1_282 && _old_291 != _keep_0_2_283 && _old_291 != _keep_1_0_284 && _old_291 != _keep_1_1_285 && _old_291 != _keep_1_2_286 && _old_291 != _keep_2_0_287 && _old_291 != _keep_2_1_288 && _old_291 != _keep_2_2_289) {
                        LIR_unload_subject(_old_291);
                    }
                    lir_subject_t* _old_292 = lh->farg;
                    if (_old_292 != _src_1_276) {
                        lh->farg = _src_1_276;
                        optimized = 1;
                        if (_old_292 && _old_292 != lh->farg && _old_292 != lh->sarg && _old_292 != lh->targ && _old_292 != _src_1_276 && _old_292 != _src_2_277 && _old_292 != _keep_0_0_281 && _old_292 != _keep_0_1_282 && _old_292 != _keep_0_2_283 && _old_292 != _keep_1_0_284 && _old_292 != _keep_1_1_285 && _old_292 != _keep_1_2_286 && _old_292 != _keep_2_0_287 && _old_292 != _keep_2_1_288 && _old_292 != _keep_2_2_289) {
                            LIR_unload_subject(_old_292);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_280) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_280;
                        optimized = 1;
                    }
                    lir_subject_t* _old_293 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_293 != _src_2_277) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_277;
                        optimized = 1;
                        if (_old_293 && _old_293 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_293 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_293 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_293 != _src_1_276 && _old_293 != _src_2_277 && _old_293 != _keep_0_0_281 && _old_293 != _keep_0_1_282 && _old_293 != _keep_0_2_283 && _old_293 != _keep_1_0_284 && _old_293 != _keep_1_1_285 && _old_293 != _keep_1_2_286 && _old_293 != _keep_2_0_287 && _old_293 != _keep_2_1_288 && _old_293 != _keep_2_2_289) {
                            LIR_unload_subject(_old_293);
                        }
                    }
                    lir_subject_t* _old_294 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_294) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_294 && _old_294 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_294 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_294 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_294 != _src_1_276 && _old_294 != _src_2_277 && _old_294 != _keep_0_0_281 && _old_294 != _keep_0_1_282 && _old_294 != _keep_0_2_283 && _old_294 != _keep_1_0_284 && _old_294 != _keep_1_1_285 && _old_294 != _keep_1_2_286 && _old_294 != _keep_2_0_287 && _old_294 != _keep_2_1_288 && _old_294 != _keep_2_2_289) {
                            LIR_unload_subject(_old_294);
                        }
                    }
                    lir_subject_t* _old_295 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_295) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_295 && _old_295 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_295 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_295 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_295 != _src_1_276 && _old_295 != _src_2_277 && _old_295 != _keep_0_0_281 && _old_295 != _keep_0_1_282 && _old_295 != _keep_0_2_283 && _old_295 != _keep_1_0_284 && _old_295 != _keep_1_1_285 && _old_295 != _keep_1_2_286 && _old_295 != _keep_2_0_287 && _old_295 != _keep_2_1_288 && _old_295 != _keep_2_2_289) {
                            LIR_unload_subject(_old_295);
                        }
                    }
                    if (!LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->unused = 1;
                        optimized = 1;
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