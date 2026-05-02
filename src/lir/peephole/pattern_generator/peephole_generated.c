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
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_LABEL))) {
                    lir_subject_t* _src_1_299 = lh->farg;
                    lir_subject_t* _src_2_300 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_301 = lh->op;
                    lir_operation_t _match_op_1_302 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_303 = lh->farg;
                    lir_subject_t* _keep_0_1_304 = lh->sarg;
                    lir_subject_t* _keep_0_2_305 = lh->targ;
                    lir_subject_t* _keep_1_0_306 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_307 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_308 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_309 = lh->farg;
                    if (_old_309 != _src_1_299) {
                        lh->farg = _src_1_299;
                        optimized = 1;
                        if (_old_309 && _old_309 != lh->farg && _old_309 != lh->sarg && _old_309 != lh->targ && _old_309 != _src_1_299 && _old_309 != _src_2_300 && _old_309 != _keep_0_0_303 && _old_309 != _keep_0_1_304 && _old_309 != _keep_0_2_305 && _old_309 != _keep_1_0_306 && _old_309 != _keep_1_1_307 && _old_309 != _keep_1_2_308) {
                            LIR_unload_subject(_old_309);
                        }
                    }
                    lir_subject_t* _old_310 = lh->sarg;
                    if (_old_310 != _src_1_299) {
                        lh->sarg = _src_1_299;
                        optimized = 1;
                        if (_old_310 && _old_310 != lh->farg && _old_310 != lh->sarg && _old_310 != lh->targ && _old_310 != _src_1_299 && _old_310 != _src_2_300 && _old_310 != _keep_0_0_303 && _old_310 != _keep_0_1_304 && _old_310 != _keep_0_2_305 && _old_310 != _keep_1_0_306 && _old_310 != _keep_1_1_307 && _old_310 != _keep_1_2_308) {
                            LIR_unload_subject(_old_310);
                        }
                    }
                    lir_subject_t* _old_311 = lh->targ;
                    if (_old_311) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_311 && _old_311 != lh->farg && _old_311 != lh->sarg && _old_311 != lh->targ && _old_311 != _src_1_299 && _old_311 != _src_2_300 && _old_311 != _keep_0_0_303 && _old_311 != _keep_0_1_304 && _old_311 != _keep_0_2_305 && _old_311 != _keep_1_0_306 && _old_311 != _keep_1_1_307 && _old_311 != _keep_1_2_308) {
                            LIR_unload_subject(_old_311);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_302) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_302;
                        optimized = 1;
                    }
                    lir_subject_t* _old_312 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_312 != _src_2_300) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_300;
                        optimized = 1;
                        if (_old_312 && _old_312 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_312 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_312 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_312 != _src_1_299 && _old_312 != _src_2_300 && _old_312 != _keep_0_0_303 && _old_312 != _keep_0_1_304 && _old_312 != _keep_0_2_305 && _old_312 != _keep_1_0_306 && _old_312 != _keep_1_1_307 && _old_312 != _keep_1_2_308) {
                            LIR_unload_subject(_old_312);
                        }
                    }
                    lir_subject_t* _old_313 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_313) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_313 && _old_313 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_313 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_313 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_313 != _src_1_299 && _old_313 != _src_2_300 && _old_313 != _keep_0_0_303 && _old_313 != _keep_0_1_304 && _old_313 != _keep_0_2_305 && _old_313 != _keep_1_0_306 && _old_313 != _keep_1_1_307 && _old_313 != _keep_1_2_308) {
                            LIR_unload_subject(_old_313);
                        }
                    }
                    lir_subject_t* _old_314 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_314) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_314 && _old_314 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_314 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_314 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_314 != _src_1_299 && _old_314 != _src_2_300 && _old_314 != _keep_0_0_303 && _old_314 != _keep_0_1_304 && _old_314 != _keep_0_2_305 && _old_314 != _keep_1_0_306 && _old_314 != _keep_1_1_307 && _old_314 != _keep_1_2_308) {
                            LIR_unload_subject(_old_314);
                        }
                    }
                }
                else if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lir_subject_t* _src_1_467 = lh->farg;
                    lir_operation_t _match_op_0_468 = lh->op;
                    lir_subject_t* _keep_0_0_469 = lh->farg;
                    lir_subject_t* _keep_0_1_470 = lh->sarg;
                    lir_subject_t* _keep_0_2_471 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_472 = lh->farg;
                    if (_old_472 != _src_1_467) {
                        lh->farg = _src_1_467;
                        optimized = 1;
                        if (_old_472 && _old_472 != lh->farg && _old_472 != lh->sarg && _old_472 != lh->targ && _old_472 != _src_1_467 && _old_472 != _keep_0_0_469 && _old_472 != _keep_0_1_470 && _old_472 != _keep_0_2_471) {
                            LIR_unload_subject(_old_472);
                        }
                    }
                    lir_subject_t* _old_473 = lh->sarg;
                    if (_old_473 != _src_1_467) {
                        lh->sarg = _src_1_467;
                        optimized = 1;
                        if (_old_473 && _old_473 != lh->farg && _old_473 != lh->sarg && _old_473 != lh->targ && _old_473 != _src_1_467 && _old_473 != _keep_0_0_469 && _old_473 != _keep_0_1_470 && _old_473 != _keep_0_2_471) {
                            LIR_unload_subject(_old_473);
                        }
                    }
                    lir_subject_t* _old_474 = lh->targ;
                    if (_old_474) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_474 && _old_474 != lh->farg && _old_474 != lh->sarg && _old_474 != lh->targ && _old_474 != _src_1_467 && _old_474 != _keep_0_0_469 && _old_474 != _keep_0_1_470 && _old_474 != _keep_0_2_471) {
                            LIR_unload_subject(_old_474);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_256 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_257 = lh->sarg;
                    lir_subject_t* _src_3_258 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_4_259 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_260 = lh->op;
                    lir_operation_t _match_op_1_261 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_262 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_263 = lh->farg;
                    lir_subject_t* _keep_0_1_264 = lh->sarg;
                    lir_subject_t* _keep_0_2_265 = lh->targ;
                    lir_subject_t* _keep_1_0_266 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_267 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_268 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_269 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_270 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_271 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_1_261) {
                        lh->op = _match_op_1_261;
                        optimized = 1;
                    }
                    lir_subject_t* _old_272 = lh->farg;
                    if (_old_272 != _src_2_257) {
                        lh->farg = _src_2_257;
                        optimized = 1;
                        if (_old_272 && _old_272 != lh->farg && _old_272 != lh->sarg && _old_272 != lh->targ && _old_272 != _src_1_256 && _old_272 != _src_2_257 && _old_272 != _src_3_258 && _old_272 != _src_4_259 && _old_272 != _keep_0_0_263 && _old_272 != _keep_0_1_264 && _old_272 != _keep_0_2_265 && _old_272 != _keep_1_0_266 && _old_272 != _keep_1_1_267 && _old_272 != _keep_1_2_268 && _old_272 != _keep_2_0_269 && _old_272 != _keep_2_1_270 && _old_272 != _keep_2_2_271) {
                            LIR_unload_subject(_old_272);
                        }
                    }
                    lir_subject_t* _old_273 = lh->sarg;
                    if (_old_273 != _src_3_258) {
                        lh->sarg = _src_3_258;
                        optimized = 1;
                        if (_old_273 && _old_273 != lh->farg && _old_273 != lh->sarg && _old_273 != lh->targ && _old_273 != _src_1_256 && _old_273 != _src_2_257 && _old_273 != _src_3_258 && _old_273 != _src_4_259 && _old_273 != _keep_0_0_263 && _old_273 != _keep_0_1_264 && _old_273 != _keep_0_2_265 && _old_273 != _keep_1_0_266 && _old_273 != _keep_1_1_267 && _old_273 != _keep_1_2_268 && _old_273 != _keep_2_0_269 && _old_273 != _keep_2_1_270 && _old_273 != _keep_2_2_271) {
                            LIR_unload_subject(_old_273);
                        }
                    }
                    lir_subject_t* _old_274 = lh->targ;
                    if (_old_274) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_274 && _old_274 != lh->farg && _old_274 != lh->sarg && _old_274 != lh->targ && _old_274 != _src_1_256 && _old_274 != _src_2_257 && _old_274 != _src_3_258 && _old_274 != _src_4_259 && _old_274 != _keep_0_0_263 && _old_274 != _keep_0_1_264 && _old_274 != _keep_0_2_265 && _old_274 != _keep_1_0_266 && _old_274 != _keep_1_1_267 && _old_274 != _keep_1_2_268 && _old_274 != _keep_2_0_269 && _old_274 != _keep_2_1_270 && _old_274 != _keep_2_2_271) {
                            LIR_unload_subject(_old_274);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_262) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_262;
                        optimized = 1;
                    }
                    lir_subject_t* _old_275 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_275 != _src_4_259) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_4_259;
                        optimized = 1;
                        if (_old_275 && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_275 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_275 != _src_1_256 && _old_275 != _src_2_257 && _old_275 != _src_3_258 && _old_275 != _src_4_259 && _old_275 != _keep_0_0_263 && _old_275 != _keep_0_1_264 && _old_275 != _keep_0_2_265 && _old_275 != _keep_1_0_266 && _old_275 != _keep_1_1_267 && _old_275 != _keep_1_2_268 && _old_275 != _keep_2_0_269 && _old_275 != _keep_2_1_270 && _old_275 != _keep_2_2_271) {
                            LIR_unload_subject(_old_275);
                        }
                    }
                    lir_subject_t* _old_276 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_276) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_276 && _old_276 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_276 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_276 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_276 != _src_1_256 && _old_276 != _src_2_257 && _old_276 != _src_3_258 && _old_276 != _src_4_259 && _old_276 != _keep_0_0_263 && _old_276 != _keep_0_1_264 && _old_276 != _keep_0_2_265 && _old_276 != _keep_1_0_266 && _old_276 != _keep_1_1_267 && _old_276 != _keep_1_2_268 && _old_276 != _keep_2_0_269 && _old_276 != _keep_2_1_270 && _old_276 != _keep_2_2_271) {
                            LIR_unload_subject(_old_276);
                        }
                    }
                    lir_subject_t* _old_277 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_277) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_277 && _old_277 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_277 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_277 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_277 != _src_1_256 && _old_277 != _src_2_257 && _old_277 != _src_3_258 && _old_277 != _src_4_259 && _old_277 != _keep_0_0_263 && _old_277 != _keep_0_1_264 && _old_277 != _keep_0_2_265 && _old_277 != _keep_1_0_266 && _old_277 != _keep_1_1_267 && _old_277 != _keep_1_2_268 && _old_277 != _keep_2_0_269 && _old_277 != _keep_2_1_270 && _old_277 != _keep_2_2_271) {
                            LIR_unload_subject(_old_277);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_278 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_279 = lh->sarg;
                    lir_subject_t* _src_3_280 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_281 = lh->op;
                    lir_operation_t _match_op_1_282 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_283 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_284 = lh->farg;
                    lir_subject_t* _keep_0_1_285 = lh->sarg;
                    lir_subject_t* _keep_0_2_286 = lh->targ;
                    lir_subject_t* _keep_1_0_287 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_288 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_289 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_290 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_291 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_292 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_293 = lh->farg;
                    if (_old_293 != _src_2_279) {
                        lh->farg = _src_2_279;
                        optimized = 1;
                        if (_old_293 && _old_293 != lh->farg && _old_293 != lh->sarg && _old_293 != lh->targ && _old_293 != _src_1_278 && _old_293 != _src_2_279 && _old_293 != _src_3_280 && _old_293 != _keep_0_0_284 && _old_293 != _keep_0_1_285 && _old_293 != _keep_0_2_286 && _old_293 != _keep_1_0_287 && _old_293 != _keep_1_1_288 && _old_293 != _keep_1_2_289 && _old_293 != _keep_2_0_290 && _old_293 != _keep_2_1_291 && _old_293 != _keep_2_2_292) {
                            LIR_unload_subject(_old_293);
                        }
                    }
                    lir_subject_t* _old_294 = lh->sarg;
                    if (_old_294 != _src_2_279) {
                        lh->sarg = _src_2_279;
                        optimized = 1;
                        if (_old_294 && _old_294 != lh->farg && _old_294 != lh->sarg && _old_294 != lh->targ && _old_294 != _src_1_278 && _old_294 != _src_2_279 && _old_294 != _src_3_280 && _old_294 != _keep_0_0_284 && _old_294 != _keep_0_1_285 && _old_294 != _keep_0_2_286 && _old_294 != _keep_1_0_287 && _old_294 != _keep_1_1_288 && _old_294 != _keep_1_2_289 && _old_294 != _keep_2_0_290 && _old_294 != _keep_2_1_291 && _old_294 != _keep_2_2_292) {
                            LIR_unload_subject(_old_294);
                        }
                    }
                    lir_subject_t* _old_295 = lh->targ;
                    if (_old_295) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_295 && _old_295 != lh->farg && _old_295 != lh->sarg && _old_295 != lh->targ && _old_295 != _src_1_278 && _old_295 != _src_2_279 && _old_295 != _src_3_280 && _old_295 != _keep_0_0_284 && _old_295 != _keep_0_1_285 && _old_295 != _keep_0_2_286 && _old_295 != _keep_1_0_287 && _old_295 != _keep_1_1_288 && _old_295 != _keep_1_2_289 && _old_295 != _keep_2_0_290 && _old_295 != _keep_2_1_291 && _old_295 != _keep_2_2_292) {
                            LIR_unload_subject(_old_295);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_283) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_283;
                        optimized = 1;
                    }
                    lir_subject_t* _old_296 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_296 != _src_3_280) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_280;
                        optimized = 1;
                        if (_old_296 && _old_296 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_296 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_296 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_296 != _src_1_278 && _old_296 != _src_2_279 && _old_296 != _src_3_280 && _old_296 != _keep_0_0_284 && _old_296 != _keep_0_1_285 && _old_296 != _keep_0_2_286 && _old_296 != _keep_1_0_287 && _old_296 != _keep_1_1_288 && _old_296 != _keep_1_2_289 && _old_296 != _keep_2_0_290 && _old_296 != _keep_2_1_291 && _old_296 != _keep_2_2_292) {
                            LIR_unload_subject(_old_296);
                        }
                    }
                    lir_subject_t* _old_297 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_297) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_297 && _old_297 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_297 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_297 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_297 != _src_1_278 && _old_297 != _src_2_279 && _old_297 != _src_3_280 && _old_297 != _keep_0_0_284 && _old_297 != _keep_0_1_285 && _old_297 != _keep_0_2_286 && _old_297 != _keep_1_0_287 && _old_297 != _keep_1_1_288 && _old_297 != _keep_1_2_289 && _old_297 != _keep_2_0_290 && _old_297 != _keep_2_1_291 && _old_297 != _keep_2_2_292) {
                            LIR_unload_subject(_old_297);
                        }
                    }
                    lir_subject_t* _old_298 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_298) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_298 && _old_298 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_298 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_298 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_298 != _src_1_278 && _old_298 != _src_2_279 && _old_298 != _src_3_280 && _old_298 != _keep_0_0_284 && _old_298 != _keep_0_1_285 && _old_298 != _keep_0_2_286 && _old_298 != _keep_1_0_287 && _old_298 != _keep_1_1_288 && _old_298 != _keep_1_2_289 && _old_298 != _keep_2_0_290 && _old_298 != _keep_2_1_291 && _old_298 != _keep_2_2_292) {
                            LIR_unload_subject(_old_298);
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
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_LABEL))) {
                    lir_subject_t* _src_1_331 = lh->farg;
                    lir_subject_t* _src_2_332 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_333 = lh->op;
                    lir_operation_t _match_op_1_334 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_335 = lh->farg;
                    lir_subject_t* _keep_0_1_336 = lh->sarg;
                    lir_subject_t* _keep_0_2_337 = lh->targ;
                    lir_subject_t* _keep_1_0_338 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_339 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_340 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_341 = lh->farg;
                    if (_old_341 != _src_1_331) {
                        lh->farg = _src_1_331;
                        optimized = 1;
                        if (_old_341 && _old_341 != lh->farg && _old_341 != lh->sarg && _old_341 != lh->targ && _old_341 != _src_1_331 && _old_341 != _src_2_332 && _old_341 != _keep_0_0_335 && _old_341 != _keep_0_1_336 && _old_341 != _keep_0_2_337 && _old_341 != _keep_1_0_338 && _old_341 != _keep_1_1_339 && _old_341 != _keep_1_2_340) {
                            LIR_unload_subject(_old_341);
                        }
                    }
                    lir_subject_t* _old_342 = lh->sarg;
                    if (_old_342 != _src_1_331) {
                        lh->sarg = _src_1_331;
                        optimized = 1;
                        if (_old_342 && _old_342 != lh->farg && _old_342 != lh->sarg && _old_342 != lh->targ && _old_342 != _src_1_331 && _old_342 != _src_2_332 && _old_342 != _keep_0_0_335 && _old_342 != _keep_0_1_336 && _old_342 != _keep_0_2_337 && _old_342 != _keep_1_0_338 && _old_342 != _keep_1_1_339 && _old_342 != _keep_1_2_340) {
                            LIR_unload_subject(_old_342);
                        }
                    }
                    lir_subject_t* _old_343 = lh->targ;
                    if (_old_343) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_343 && _old_343 != lh->farg && _old_343 != lh->sarg && _old_343 != lh->targ && _old_343 != _src_1_331 && _old_343 != _src_2_332 && _old_343 != _keep_0_0_335 && _old_343 != _keep_0_1_336 && _old_343 != _keep_0_2_337 && _old_343 != _keep_1_0_338 && _old_343 != _keep_1_1_339 && _old_343 != _keep_1_2_340) {
                            LIR_unload_subject(_old_343);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_334) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_334;
                        optimized = 1;
                    }
                    lir_subject_t* _old_344 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_344 != _src_2_332) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_332;
                        optimized = 1;
                        if (_old_344 && _old_344 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_344 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_344 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_344 != _src_1_331 && _old_344 != _src_2_332 && _old_344 != _keep_0_0_335 && _old_344 != _keep_0_1_336 && _old_344 != _keep_0_2_337 && _old_344 != _keep_1_0_338 && _old_344 != _keep_1_1_339 && _old_344 != _keep_1_2_340) {
                            LIR_unload_subject(_old_344);
                        }
                    }
                    lir_subject_t* _old_345 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_345) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_345 && _old_345 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_345 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_345 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_345 != _src_1_331 && _old_345 != _src_2_332 && _old_345 != _keep_0_0_335 && _old_345 != _keep_0_1_336 && _old_345 != _keep_0_2_337 && _old_345 != _keep_1_0_338 && _old_345 != _keep_1_1_339 && _old_345 != _keep_1_2_340) {
                            LIR_unload_subject(_old_345);
                        }
                    }
                    lir_subject_t* _old_346 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_346) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_346 && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_346 != _src_1_331 && _old_346 != _src_2_332 && _old_346 != _keep_0_0_335 && _old_346 != _keep_0_1_336 && _old_346 != _keep_0_2_337 && _old_346 != _keep_1_0_338 && _old_346 != _keep_1_1_339 && _old_346 != _keep_1_2_340) {
                            LIR_unload_subject(_old_346);
                        }
                    }
                }
                else if (lh->op == LIR_bAND &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ))) {
                    lir_subject_t* _src_1_483 = lh->farg;
                    lir_operation_t _match_op_0_484 = lh->op;
                    lir_subject_t* _keep_0_0_485 = lh->farg;
                    lir_subject_t* _keep_0_1_486 = lh->sarg;
                    lir_subject_t* _keep_0_2_487 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_488 = lh->farg;
                    if (_old_488 != _src_1_483) {
                        lh->farg = _src_1_483;
                        optimized = 1;
                        if (_old_488 && _old_488 != lh->farg && _old_488 != lh->sarg && _old_488 != lh->targ && _old_488 != _src_1_483 && _old_488 != _keep_0_0_485 && _old_488 != _keep_0_1_486 && _old_488 != _keep_0_2_487) {
                            LIR_unload_subject(_old_488);
                        }
                    }
                    lir_subject_t* _old_489 = lh->sarg;
                    if (_old_489 != _src_1_483) {
                        lh->sarg = _src_1_483;
                        optimized = 1;
                        if (_old_489 && _old_489 != lh->farg && _old_489 != lh->sarg && _old_489 != lh->targ && _old_489 != _src_1_483 && _old_489 != _keep_0_0_485 && _old_489 != _keep_0_1_486 && _old_489 != _keep_0_2_487) {
                            LIR_unload_subject(_old_489);
                        }
                    }
                    lir_subject_t* _old_490 = lh->targ;
                    if (_old_490) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_490 && _old_490 != lh->farg && _old_490 != lh->sarg && _old_490 != lh->targ && _old_490 != _src_1_483 && _old_490 != _keep_0_0_485 && _old_490 != _keep_0_1_486 && _old_490 != _keep_0_2_487) {
                            LIR_unload_subject(_old_490);
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
                    lir_subject_t* _src_1_515 = lh->farg;
                    lir_operation_t _match_op_0_516 = lh->op;
                    lir_subject_t* _keep_0_0_517 = lh->farg;
                    lir_subject_t* _keep_0_1_518 = lh->sarg;
                    lir_subject_t* _keep_0_2_519 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_520 = lh->sarg;
                    if (_old_520 != _src_1_515) {
                        lh->sarg = _src_1_515;
                        optimized = 1;
                        if (_old_520 && _old_520 != lh->farg && _old_520 != lh->sarg && _old_520 != lh->targ && _old_520 != _src_1_515 && _old_520 != _keep_0_0_517 && _old_520 != _keep_0_1_518 && _old_520 != _keep_0_2_519) {
                            LIR_unload_subject(_old_520);
                        }
                    }
                    lir_subject_t* _old_521 = lh->targ;
                    if (_old_521 != _src_1_515) {
                        lh->targ = _src_1_515;
                        optimized = 1;
                        if (_old_521 && _old_521 != lh->farg && _old_521 != lh->sarg && _old_521 != lh->targ && _old_521 != _src_1_515 && _old_521 != _keep_0_0_517 && _old_521 != _keep_0_1_518 && _old_521 != _keep_0_2_519) {
                            LIR_unload_subject(_old_521);
                        }
                    }
                    lir_subject_t* _old_522 = lh->farg;
                    if (_old_522 != _src_1_515) {
                        lh->farg = _src_1_515;
                        optimized = 1;
                        if (_old_522 && _old_522 != lh->farg && _old_522 != lh->sarg && _old_522 != lh->targ && _old_522 != _src_1_515 && _old_522 != _keep_0_0_517 && _old_522 != _keep_0_1_518 && _old_522 != _keep_0_2_519) {
                            LIR_unload_subject(_old_522);
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
                LIR_subj_equals(lh->sarg, lh->targ)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JE || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_JNE) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_LABEL))) {
                    lir_subject_t* _src_1_315 = lh->farg;
                    lir_subject_t* _src_2_316 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_317 = lh->op;
                    lir_operation_t _match_op_1_318 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_319 = lh->farg;
                    lir_subject_t* _keep_0_1_320 = lh->sarg;
                    lir_subject_t* _keep_0_2_321 = lh->targ;
                    lir_subject_t* _keep_1_0_322 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_323 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_324 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_325 = lh->farg;
                    if (_old_325 != _src_1_315) {
                        lh->farg = _src_1_315;
                        optimized = 1;
                        if (_old_325 && _old_325 != lh->farg && _old_325 != lh->sarg && _old_325 != lh->targ && _old_325 != _src_1_315 && _old_325 != _src_2_316 && _old_325 != _keep_0_0_319 && _old_325 != _keep_0_1_320 && _old_325 != _keep_0_2_321 && _old_325 != _keep_1_0_322 && _old_325 != _keep_1_1_323 && _old_325 != _keep_1_2_324) {
                            LIR_unload_subject(_old_325);
                        }
                    }
                    lir_subject_t* _old_326 = lh->sarg;
                    if (_old_326 != _src_1_315) {
                        lh->sarg = _src_1_315;
                        optimized = 1;
                        if (_old_326 && _old_326 != lh->farg && _old_326 != lh->sarg && _old_326 != lh->targ && _old_326 != _src_1_315 && _old_326 != _src_2_316 && _old_326 != _keep_0_0_319 && _old_326 != _keep_0_1_320 && _old_326 != _keep_0_2_321 && _old_326 != _keep_1_0_322 && _old_326 != _keep_1_1_323 && _old_326 != _keep_1_2_324) {
                            LIR_unload_subject(_old_326);
                        }
                    }
                    lir_subject_t* _old_327 = lh->targ;
                    if (_old_327) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_327 && _old_327 != lh->farg && _old_327 != lh->sarg && _old_327 != lh->targ && _old_327 != _src_1_315 && _old_327 != _src_2_316 && _old_327 != _keep_0_0_319 && _old_327 != _keep_0_1_320 && _old_327 != _keep_0_2_321 && _old_327 != _keep_1_0_322 && _old_327 != _keep_1_1_323 && _old_327 != _keep_1_2_324) {
                            LIR_unload_subject(_old_327);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_318) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_318;
                        optimized = 1;
                    }
                    lir_subject_t* _old_328 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_328 != _src_2_316) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_316;
                        optimized = 1;
                        if (_old_328 && _old_328 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_328 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_328 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_328 != _src_1_315 && _old_328 != _src_2_316 && _old_328 != _keep_0_0_319 && _old_328 != _keep_0_1_320 && _old_328 != _keep_0_2_321 && _old_328 != _keep_1_0_322 && _old_328 != _keep_1_1_323 && _old_328 != _keep_1_2_324) {
                            LIR_unload_subject(_old_328);
                        }
                    }
                    lir_subject_t* _old_329 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_329) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_329 && _old_329 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_329 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_329 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_329 != _src_1_315 && _old_329 != _src_2_316 && _old_329 != _keep_0_0_319 && _old_329 != _keep_0_1_320 && _old_329 != _keep_0_2_321 && _old_329 != _keep_1_0_322 && _old_329 != _keep_1_1_323 && _old_329 != _keep_1_2_324) {
                            LIR_unload_subject(_old_329);
                        }
                    }
                    lir_subject_t* _old_330 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_330) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_330 && _old_330 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_330 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_330 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_330 != _src_1_315 && _old_330 != _src_2_316 && _old_330 != _keep_0_0_319 && _old_330 != _keep_0_1_320 && _old_330 != _keep_0_2_321 && _old_330 != _keep_1_0_322 && _old_330 != _keep_1_1_323 && _old_330 != _keep_1_2_324) {
                            LIR_unload_subject(_old_330);
                        }
                    }
                }
                else if (lh->op == LIR_bOR &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg) &&
                LIR_subj_equals(lh->sarg, lh->targ))) {
                    lir_subject_t* _src_1_475 = lh->farg;
                    lir_operation_t _match_op_0_476 = lh->op;
                    lir_subject_t* _keep_0_0_477 = lh->farg;
                    lir_subject_t* _keep_0_1_478 = lh->sarg;
                    lir_subject_t* _keep_0_2_479 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_480 = lh->farg;
                    if (_old_480 != _src_1_475) {
                        lh->farg = _src_1_475;
                        optimized = 1;
                        if (_old_480 && _old_480 != lh->farg && _old_480 != lh->sarg && _old_480 != lh->targ && _old_480 != _src_1_475 && _old_480 != _keep_0_0_477 && _old_480 != _keep_0_1_478 && _old_480 != _keep_0_2_479) {
                            LIR_unload_subject(_old_480);
                        }
                    }
                    lir_subject_t* _old_481 = lh->sarg;
                    if (_old_481 != _src_1_475) {
                        lh->sarg = _src_1_475;
                        optimized = 1;
                        if (_old_481 && _old_481 != lh->farg && _old_481 != lh->sarg && _old_481 != lh->targ && _old_481 != _src_1_475 && _old_481 != _keep_0_0_477 && _old_481 != _keep_0_1_478 && _old_481 != _keep_0_2_479) {
                            LIR_unload_subject(_old_481);
                        }
                    }
                    lir_subject_t* _old_482 = lh->targ;
                    if (_old_482) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_482 && _old_482 != lh->farg && _old_482 != lh->sarg && _old_482 != lh->targ && _old_482 != _src_1_475 && _old_482 != _keep_0_0_477 && _old_482 != _keep_0_1_478 && _old_482 != _keep_0_2_479) {
                            LIR_unload_subject(_old_482);
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
                    lir_subject_t* _src_1_491 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_492 = lh->op;
                    lir_operation_t _match_op_1_493 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_494 = lh->farg;
                    lir_subject_t* _keep_0_1_495 = lh->sarg;
                    lir_subject_t* _keep_0_2_496 = lh->targ;
                    lir_subject_t* _keep_1_0_497 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_498 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_499 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_500 = lh->sarg;
                    if (_old_500 != _src_1_491) {
                        lh->sarg = _src_1_491;
                        optimized = 1;
                        if (_old_500 && _old_500 != lh->farg && _old_500 != lh->sarg && _old_500 != lh->targ && _old_500 != _src_1_491 && _old_500 != _keep_0_0_494 && _old_500 != _keep_0_1_495 && _old_500 != _keep_0_2_496 && _old_500 != _keep_1_0_497 && _old_500 != _keep_1_1_498 && _old_500 != _keep_1_2_499) {
                            LIR_unload_subject(_old_500);
                        }
                    }
                    lir_subject_t* _old_501 = lh->targ;
                    if (_old_501 != _src_1_491) {
                        lh->targ = _src_1_491;
                        optimized = 1;
                        if (_old_501 && _old_501 != lh->farg && _old_501 != lh->sarg && _old_501 != lh->targ && _old_501 != _src_1_491 && _old_501 != _keep_0_0_494 && _old_501 != _keep_0_1_495 && _old_501 != _keep_0_2_496 && _old_501 != _keep_1_0_497 && _old_501 != _keep_1_1_498 && _old_501 != _keep_1_2_499) {
                            LIR_unload_subject(_old_501);
                        }
                    }
                    lir_subject_t* _old_502 = lh->farg;
                    if (_old_502 != _src_1_491) {
                        lh->farg = _src_1_491;
                        optimized = 1;
                        if (_old_502 && _old_502 != lh->farg && _old_502 != lh->sarg && _old_502 != lh->targ && _old_502 != _src_1_491 && _old_502 != _keep_0_0_494 && _old_502 != _keep_0_1_495 && _old_502 != _keep_0_2_496 && _old_502 != _keep_1_0_497 && _old_502 != _keep_1_1_498 && _old_502 != _keep_1_2_499) {
                            LIR_unload_subject(_old_502);
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
                    lir_subject_t* _src_1_503 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_504 = lh->op;
                    lir_operation_t _match_op_1_505 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_506 = lh->farg;
                    lir_subject_t* _keep_0_1_507 = lh->sarg;
                    lir_subject_t* _keep_0_2_508 = lh->targ;
                    lir_subject_t* _keep_1_0_509 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_510 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_511 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_512 = lh->sarg;
                    if (_old_512 != _src_1_503) {
                        lh->sarg = _src_1_503;
                        optimized = 1;
                        if (_old_512 && _old_512 != lh->farg && _old_512 != lh->sarg && _old_512 != lh->targ && _old_512 != _src_1_503 && _old_512 != _keep_0_0_506 && _old_512 != _keep_0_1_507 && _old_512 != _keep_0_2_508 && _old_512 != _keep_1_0_509 && _old_512 != _keep_1_1_510 && _old_512 != _keep_1_2_511) {
                            LIR_unload_subject(_old_512);
                        }
                    }
                    lir_subject_t* _old_513 = lh->targ;
                    if (_old_513 != _src_1_503) {
                        lh->targ = _src_1_503;
                        optimized = 1;
                        if (_old_513 && _old_513 != lh->farg && _old_513 != lh->sarg && _old_513 != lh->targ && _old_513 != _src_1_503 && _old_513 != _keep_0_0_506 && _old_513 != _keep_0_1_507 && _old_513 != _keep_0_2_508 && _old_513 != _keep_1_0_509 && _old_513 != _keep_1_1_510 && _old_513 != _keep_1_2_511) {
                            LIR_unload_subject(_old_513);
                        }
                    }
                    lir_subject_t* _old_514 = lh->farg;
                    if (_old_514 != _src_1_503) {
                        lh->farg = _src_1_503;
                        optimized = 1;
                        if (_old_514 && _old_514 != lh->farg && _old_514 != lh->sarg && _old_514 != lh->targ && _old_514 != _src_1_503 && _old_514 != _keep_0_0_506 && _old_514 != _keep_0_1_507 && _old_514 != _keep_0_2_508 && _old_514 != _keep_1_0_509 && _old_514 != _keep_1_1_510 && _old_514 != _keep_1_2_511) {
                            LIR_unload_subject(_old_514);
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
                    lir_subject_t* _src_1_523 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_524 = lh->op;
                    lir_operation_t _match_op_1_525 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_526 = lh->farg;
                    lir_subject_t* _keep_0_1_527 = lh->sarg;
                    lir_subject_t* _keep_0_2_528 = lh->targ;
                    lir_subject_t* _keep_1_0_529 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_530 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_531 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_532 = lh->sarg;
                    if (_old_532 != _src_1_523) {
                        lh->sarg = _src_1_523;
                        optimized = 1;
                        if (_old_532 && _old_532 != lh->farg && _old_532 != lh->sarg && _old_532 != lh->targ && _old_532 != _src_1_523 && _old_532 != _keep_0_0_526 && _old_532 != _keep_0_1_527 && _old_532 != _keep_0_2_528 && _old_532 != _keep_1_0_529 && _old_532 != _keep_1_1_530 && _old_532 != _keep_1_2_531) {
                            LIR_unload_subject(_old_532);
                        }
                    }
                    lir_subject_t* _old_533 = lh->targ;
                    if (_old_533 != _src_1_523) {
                        lh->targ = _src_1_523;
                        optimized = 1;
                        if (_old_533 && _old_533 != lh->farg && _old_533 != lh->sarg && _old_533 != lh->targ && _old_533 != _src_1_523 && _old_533 != _keep_0_0_526 && _old_533 != _keep_0_1_527 && _old_533 != _keep_0_2_528 && _old_533 != _keep_1_0_529 && _old_533 != _keep_1_1_530 && _old_533 != _keep_1_2_531) {
                            LIR_unload_subject(_old_533);
                        }
                    }
                    lir_subject_t* _old_534 = lh->farg;
                    if (_old_534 != _src_1_523) {
                        lh->farg = _src_1_523;
                        optimized = 1;
                        if (_old_534 && _old_534 != lh->farg && _old_534 != lh->sarg && _old_534 != lh->targ && _old_534 != _src_1_523 && _old_534 != _keep_0_0_526 && _old_534 != _keep_0_1_527 && _old_534 != _keep_0_2_528 && _old_534 != _keep_1_0_529 && _old_534 != _keep_1_1_530 && _old_534 != _keep_1_2_531) {
                            LIR_unload_subject(_old_534);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JE &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_347 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_348 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_349 = lh->op;
                    lir_operation_t _match_op_1_350 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_351 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_352 = lh->farg;
                    lir_subject_t* _keep_0_1_353 = lh->sarg;
                    lir_subject_t* _keep_0_2_354 = lh->targ;
                    lir_subject_t* _keep_1_0_355 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_356 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_357 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_358 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_359 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_360 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_361 = lh->sarg;
                    if (_old_361 != _src_1_347) {
                        lh->sarg = _src_1_347;
                        optimized = 1;
                        if (_old_361 && _old_361 != lh->farg && _old_361 != lh->sarg && _old_361 != lh->targ && _old_361 != _src_1_347 && _old_361 != _src_2_348 && _old_361 != _keep_0_0_352 && _old_361 != _keep_0_1_353 && _old_361 != _keep_0_2_354 && _old_361 != _keep_1_0_355 && _old_361 != _keep_1_1_356 && _old_361 != _keep_1_2_357 && _old_361 != _keep_2_0_358 && _old_361 != _keep_2_1_359 && _old_361 != _keep_2_2_360) {
                            LIR_unload_subject(_old_361);
                        }
                    }
                    lir_subject_t* _old_362 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_362 && _old_362 != lh->farg && _old_362 != lh->sarg && _old_362 != lh->targ && _old_362 != _src_1_347 && _old_362 != _src_2_348 && _old_362 != _keep_0_0_352 && _old_362 != _keep_0_1_353 && _old_362 != _keep_0_2_354 && _old_362 != _keep_1_0_355 && _old_362 != _keep_1_1_356 && _old_362 != _keep_1_2_357 && _old_362 != _keep_2_0_358 && _old_362 != _keep_2_1_359 && _old_362 != _keep_2_2_360) {
                        LIR_unload_subject(_old_362);
                    }
                    lir_subject_t* _old_363 = lh->farg;
                    if (_old_363 != _src_1_347) {
                        lh->farg = _src_1_347;
                        optimized = 1;
                        if (_old_363 && _old_363 != lh->farg && _old_363 != lh->sarg && _old_363 != lh->targ && _old_363 != _src_1_347 && _old_363 != _src_2_348 && _old_363 != _keep_0_0_352 && _old_363 != _keep_0_1_353 && _old_363 != _keep_0_2_354 && _old_363 != _keep_1_0_355 && _old_363 != _keep_1_1_356 && _old_363 != _keep_1_2_357 && _old_363 != _keep_2_0_358 && _old_363 != _keep_2_1_359 && _old_363 != _keep_2_2_360) {
                            LIR_unload_subject(_old_363);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_364 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_364 != _src_2_348) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_348;
                        optimized = 1;
                        if (_old_364 && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_364 != _src_1_347 && _old_364 != _src_2_348 && _old_364 != _keep_0_0_352 && _old_364 != _keep_0_1_353 && _old_364 != _keep_0_2_354 && _old_364 != _keep_1_0_355 && _old_364 != _keep_1_1_356 && _old_364 != _keep_1_2_357 && _old_364 != _keep_2_0_358 && _old_364 != _keep_2_1_359 && _old_364 != _keep_2_2_360) {
                            LIR_unload_subject(_old_364);
                        }
                    }
                    lir_subject_t* _old_365 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_365) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_365 && _old_365 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_365 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_365 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_365 != _src_1_347 && _old_365 != _src_2_348 && _old_365 != _keep_0_0_352 && _old_365 != _keep_0_1_353 && _old_365 != _keep_0_2_354 && _old_365 != _keep_1_0_355 && _old_365 != _keep_1_1_356 && _old_365 != _keep_1_2_357 && _old_365 != _keep_2_0_358 && _old_365 != _keep_2_1_359 && _old_365 != _keep_2_2_360) {
                            LIR_unload_subject(_old_365);
                        }
                    }
                    lir_subject_t* _old_366 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_366) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_366 && _old_366 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_366 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_366 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_366 != _src_1_347 && _old_366 != _src_2_348 && _old_366 != _keep_0_0_352 && _old_366 != _keep_0_1_353 && _old_366 != _keep_0_2_354 && _old_366 != _keep_1_0_355 && _old_366 != _keep_1_1_356 && _old_366 != _keep_1_2_357 && _old_366 != _keep_2_0_358 && _old_366 != _keep_2_1_359 && _old_366 != _keep_2_2_360) {
                            LIR_unload_subject(_old_366);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JZ &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_367 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_368 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_369 = lh->op;
                    lir_operation_t _match_op_1_370 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_371 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_372 = lh->farg;
                    lir_subject_t* _keep_0_1_373 = lh->sarg;
                    lir_subject_t* _keep_0_2_374 = lh->targ;
                    lir_subject_t* _keep_1_0_375 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_376 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_377 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_378 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_379 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_380 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_381 = lh->sarg;
                    if (_old_381 != _src_1_367) {
                        lh->sarg = _src_1_367;
                        optimized = 1;
                        if (_old_381 && _old_381 != lh->farg && _old_381 != lh->sarg && _old_381 != lh->targ && _old_381 != _src_1_367 && _old_381 != _src_2_368 && _old_381 != _keep_0_0_372 && _old_381 != _keep_0_1_373 && _old_381 != _keep_0_2_374 && _old_381 != _keep_1_0_375 && _old_381 != _keep_1_1_376 && _old_381 != _keep_1_2_377 && _old_381 != _keep_2_0_378 && _old_381 != _keep_2_1_379 && _old_381 != _keep_2_2_380) {
                            LIR_unload_subject(_old_381);
                        }
                    }
                    lir_subject_t* _old_382 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_382 && _old_382 != lh->farg && _old_382 != lh->sarg && _old_382 != lh->targ && _old_382 != _src_1_367 && _old_382 != _src_2_368 && _old_382 != _keep_0_0_372 && _old_382 != _keep_0_1_373 && _old_382 != _keep_0_2_374 && _old_382 != _keep_1_0_375 && _old_382 != _keep_1_1_376 && _old_382 != _keep_1_2_377 && _old_382 != _keep_2_0_378 && _old_382 != _keep_2_1_379 && _old_382 != _keep_2_2_380) {
                        LIR_unload_subject(_old_382);
                    }
                    lir_subject_t* _old_383 = lh->farg;
                    if (_old_383 != _src_1_367) {
                        lh->farg = _src_1_367;
                        optimized = 1;
                        if (_old_383 && _old_383 != lh->farg && _old_383 != lh->sarg && _old_383 != lh->targ && _old_383 != _src_1_367 && _old_383 != _src_2_368 && _old_383 != _keep_0_0_372 && _old_383 != _keep_0_1_373 && _old_383 != _keep_0_2_374 && _old_383 != _keep_1_0_375 && _old_383 != _keep_1_1_376 && _old_383 != _keep_1_2_377 && _old_383 != _keep_2_0_378 && _old_383 != _keep_2_1_379 && _old_383 != _keep_2_2_380) {
                            LIR_unload_subject(_old_383);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_384 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_384 != _src_2_368) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_368;
                        optimized = 1;
                        if (_old_384 && _old_384 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_384 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_384 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_384 != _src_1_367 && _old_384 != _src_2_368 && _old_384 != _keep_0_0_372 && _old_384 != _keep_0_1_373 && _old_384 != _keep_0_2_374 && _old_384 != _keep_1_0_375 && _old_384 != _keep_1_1_376 && _old_384 != _keep_1_2_377 && _old_384 != _keep_2_0_378 && _old_384 != _keep_2_1_379 && _old_384 != _keep_2_2_380) {
                            LIR_unload_subject(_old_384);
                        }
                    }
                    lir_subject_t* _old_385 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_385) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_385 && _old_385 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_385 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_385 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_385 != _src_1_367 && _old_385 != _src_2_368 && _old_385 != _keep_0_0_372 && _old_385 != _keep_0_1_373 && _old_385 != _keep_0_2_374 && _old_385 != _keep_1_0_375 && _old_385 != _keep_1_1_376 && _old_385 != _keep_1_2_377 && _old_385 != _keep_2_0_378 && _old_385 != _keep_2_1_379 && _old_385 != _keep_2_2_380) {
                            LIR_unload_subject(_old_385);
                        }
                    }
                    lir_subject_t* _old_386 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_386) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_386 && _old_386 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_386 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_386 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_386 != _src_1_367 && _old_386 != _src_2_368 && _old_386 != _keep_0_0_372 && _old_386 != _keep_0_1_373 && _old_386 != _keep_0_2_374 && _old_386 != _keep_1_0_375 && _old_386 != _keep_1_1_376 && _old_386 != _keep_1_2_377 && _old_386 != _keep_2_0_378 && _old_386 != _keep_2_1_379 && _old_386 != _keep_2_2_380) {
                            LIR_unload_subject(_old_386);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JNE &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_387 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_388 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_389 = lh->op;
                    lir_operation_t _match_op_1_390 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_391 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_392 = lh->farg;
                    lir_subject_t* _keep_0_1_393 = lh->sarg;
                    lir_subject_t* _keep_0_2_394 = lh->targ;
                    lir_subject_t* _keep_1_0_395 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_396 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_397 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_398 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_399 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_400 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_401 = lh->sarg;
                    if (_old_401 != _src_1_387) {
                        lh->sarg = _src_1_387;
                        optimized = 1;
                        if (_old_401 && _old_401 != lh->farg && _old_401 != lh->sarg && _old_401 != lh->targ && _old_401 != _src_1_387 && _old_401 != _src_2_388 && _old_401 != _keep_0_0_392 && _old_401 != _keep_0_1_393 && _old_401 != _keep_0_2_394 && _old_401 != _keep_1_0_395 && _old_401 != _keep_1_1_396 && _old_401 != _keep_1_2_397 && _old_401 != _keep_2_0_398 && _old_401 != _keep_2_1_399 && _old_401 != _keep_2_2_400) {
                            LIR_unload_subject(_old_401);
                        }
                    }
                    lir_subject_t* _old_402 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_402 && _old_402 != lh->farg && _old_402 != lh->sarg && _old_402 != lh->targ && _old_402 != _src_1_387 && _old_402 != _src_2_388 && _old_402 != _keep_0_0_392 && _old_402 != _keep_0_1_393 && _old_402 != _keep_0_2_394 && _old_402 != _keep_1_0_395 && _old_402 != _keep_1_1_396 && _old_402 != _keep_1_2_397 && _old_402 != _keep_2_0_398 && _old_402 != _keep_2_1_399 && _old_402 != _keep_2_2_400) {
                        LIR_unload_subject(_old_402);
                    }
                    lir_subject_t* _old_403 = lh->farg;
                    if (_old_403 != _src_1_387) {
                        lh->farg = _src_1_387;
                        optimized = 1;
                        if (_old_403 && _old_403 != lh->farg && _old_403 != lh->sarg && _old_403 != lh->targ && _old_403 != _src_1_387 && _old_403 != _src_2_388 && _old_403 != _keep_0_0_392 && _old_403 != _keep_0_1_393 && _old_403 != _keep_0_2_394 && _old_403 != _keep_1_0_395 && _old_403 != _keep_1_1_396 && _old_403 != _keep_1_2_397 && _old_403 != _keep_2_0_398 && _old_403 != _keep_2_1_399 && _old_403 != _keep_2_2_400) {
                            LIR_unload_subject(_old_403);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_404 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_404 != _src_2_388) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_388;
                        optimized = 1;
                        if (_old_404 && _old_404 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_404 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_404 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_404 != _src_1_387 && _old_404 != _src_2_388 && _old_404 != _keep_0_0_392 && _old_404 != _keep_0_1_393 && _old_404 != _keep_0_2_394 && _old_404 != _keep_1_0_395 && _old_404 != _keep_1_1_396 && _old_404 != _keep_1_2_397 && _old_404 != _keep_2_0_398 && _old_404 != _keep_2_1_399 && _old_404 != _keep_2_2_400) {
                            LIR_unload_subject(_old_404);
                        }
                    }
                    lir_subject_t* _old_405 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_405) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_405 && _old_405 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_405 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_405 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_405 != _src_1_387 && _old_405 != _src_2_388 && _old_405 != _keep_0_0_392 && _old_405 != _keep_0_1_393 && _old_405 != _keep_0_2_394 && _old_405 != _keep_1_0_395 && _old_405 != _keep_1_1_396 && _old_405 != _keep_1_2_397 && _old_405 != _keep_2_0_398 && _old_405 != _keep_2_1_399 && _old_405 != _keep_2_2_400) {
                            LIR_unload_subject(_old_405);
                        }
                    }
                    lir_subject_t* _old_406 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_406) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_406 && _old_406 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_406 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_406 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_406 != _src_1_387 && _old_406 != _src_2_388 && _old_406 != _keep_0_0_392 && _old_406 != _keep_0_1_393 && _old_406 != _keep_0_2_394 && _old_406 != _keep_1_0_395 && _old_406 != _keep_1_1_396 && _old_406 != _keep_1_2_397 && _old_406 != _keep_2_0_398 && _old_406 != _keep_2_1_399 && _old_406 != _keep_2_2_400) {
                            LIR_unload_subject(_old_406);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_JNZ &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_407 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_408 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_409 = lh->op;
                    lir_operation_t _match_op_1_410 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_411 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_412 = lh->farg;
                    lir_subject_t* _keep_0_1_413 = lh->sarg;
                    lir_subject_t* _keep_0_2_414 = lh->targ;
                    lir_subject_t* _keep_1_0_415 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_416 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_417 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_418 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_419 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_420 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_421 = lh->sarg;
                    if (_old_421 != _src_1_407) {
                        lh->sarg = _src_1_407;
                        optimized = 1;
                        if (_old_421 && _old_421 != lh->farg && _old_421 != lh->sarg && _old_421 != lh->targ && _old_421 != _src_1_407 && _old_421 != _src_2_408 && _old_421 != _keep_0_0_412 && _old_421 != _keep_0_1_413 && _old_421 != _keep_0_2_414 && _old_421 != _keep_1_0_415 && _old_421 != _keep_1_1_416 && _old_421 != _keep_1_2_417 && _old_421 != _keep_2_0_418 && _old_421 != _keep_2_1_419 && _old_421 != _keep_2_2_420) {
                            LIR_unload_subject(_old_421);
                        }
                    }
                    lir_subject_t* _old_422 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_422 && _old_422 != lh->farg && _old_422 != lh->sarg && _old_422 != lh->targ && _old_422 != _src_1_407 && _old_422 != _src_2_408 && _old_422 != _keep_0_0_412 && _old_422 != _keep_0_1_413 && _old_422 != _keep_0_2_414 && _old_422 != _keep_1_0_415 && _old_422 != _keep_1_1_416 && _old_422 != _keep_1_2_417 && _old_422 != _keep_2_0_418 && _old_422 != _keep_2_1_419 && _old_422 != _keep_2_2_420) {
                        LIR_unload_subject(_old_422);
                    }
                    lir_subject_t* _old_423 = lh->farg;
                    if (_old_423 != _src_1_407) {
                        lh->farg = _src_1_407;
                        optimized = 1;
                        if (_old_423 && _old_423 != lh->farg && _old_423 != lh->sarg && _old_423 != lh->targ && _old_423 != _src_1_407 && _old_423 != _src_2_408 && _old_423 != _keep_0_0_412 && _old_423 != _keep_0_1_413 && _old_423 != _keep_0_2_414 && _old_423 != _keep_1_0_415 && _old_423 != _keep_1_1_416 && _old_423 != _keep_1_2_417 && _old_423 != _keep_2_0_418 && _old_423 != _keep_2_1_419 && _old_423 != _keep_2_2_420) {
                            LIR_unload_subject(_old_423);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_424 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_424 != _src_2_408) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_408;
                        optimized = 1;
                        if (_old_424 && _old_424 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_424 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_424 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_424 != _src_1_407 && _old_424 != _src_2_408 && _old_424 != _keep_0_0_412 && _old_424 != _keep_0_1_413 && _old_424 != _keep_0_2_414 && _old_424 != _keep_1_0_415 && _old_424 != _keep_1_1_416 && _old_424 != _keep_1_2_417 && _old_424 != _keep_2_0_418 && _old_424 != _keep_2_1_419 && _old_424 != _keep_2_2_420) {
                            LIR_unload_subject(_old_424);
                        }
                    }
                    lir_subject_t* _old_425 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_425) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_425 && _old_425 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_425 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_425 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_425 != _src_1_407 && _old_425 != _src_2_408 && _old_425 != _keep_0_0_412 && _old_425 != _keep_0_1_413 && _old_425 != _keep_0_2_414 && _old_425 != _keep_1_0_415 && _old_425 != _keep_1_1_416 && _old_425 != _keep_1_2_417 && _old_425 != _keep_2_0_418 && _old_425 != _keep_2_1_419 && _old_425 != _keep_2_2_420) {
                            LIR_unload_subject(_old_425);
                        }
                    }
                    lir_subject_t* _old_426 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_426) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_426 && _old_426 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_426 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_426 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_426 != _src_1_407 && _old_426 != _src_2_408 && _old_426 != _keep_0_0_412 && _old_426 != _keep_0_1_413 && _old_426 != _keep_0_2_414 && _old_426 != _keep_1_0_415 && _old_426 != _keep_1_1_416 && _old_426 != _keep_1_2_417 && _old_426 != _keep_2_0_418 && _old_426 != _keep_2_1_419 && _old_426 != _keep_2_2_420) {
                            LIR_unload_subject(_old_426);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_SETE &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_427 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_428 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_429 = lh->op;
                    lir_operation_t _match_op_1_430 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_431 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_432 = lh->farg;
                    lir_subject_t* _keep_0_1_433 = lh->sarg;
                    lir_subject_t* _keep_0_2_434 = lh->targ;
                    lir_subject_t* _keep_1_0_435 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_436 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_437 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_438 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_439 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_440 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_441 = lh->sarg;
                    if (_old_441 != _src_1_427) {
                        lh->sarg = _src_1_427;
                        optimized = 1;
                        if (_old_441 && _old_441 != lh->farg && _old_441 != lh->sarg && _old_441 != lh->targ && _old_441 != _src_1_427 && _old_441 != _src_2_428 && _old_441 != _keep_0_0_432 && _old_441 != _keep_0_1_433 && _old_441 != _keep_0_2_434 && _old_441 != _keep_1_0_435 && _old_441 != _keep_1_1_436 && _old_441 != _keep_1_2_437 && _old_441 != _keep_2_0_438 && _old_441 != _keep_2_1_439 && _old_441 != _keep_2_2_440) {
                            LIR_unload_subject(_old_441);
                        }
                    }
                    lir_subject_t* _old_442 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_442 && _old_442 != lh->farg && _old_442 != lh->sarg && _old_442 != lh->targ && _old_442 != _src_1_427 && _old_442 != _src_2_428 && _old_442 != _keep_0_0_432 && _old_442 != _keep_0_1_433 && _old_442 != _keep_0_2_434 && _old_442 != _keep_1_0_435 && _old_442 != _keep_1_1_436 && _old_442 != _keep_1_2_437 && _old_442 != _keep_2_0_438 && _old_442 != _keep_2_1_439 && _old_442 != _keep_2_2_440) {
                        LIR_unload_subject(_old_442);
                    }
                    lir_subject_t* _old_443 = lh->farg;
                    if (_old_443 != _src_1_427) {
                        lh->farg = _src_1_427;
                        optimized = 1;
                        if (_old_443 && _old_443 != lh->farg && _old_443 != lh->sarg && _old_443 != lh->targ && _old_443 != _src_1_427 && _old_443 != _src_2_428 && _old_443 != _keep_0_0_432 && _old_443 != _keep_0_1_433 && _old_443 != _keep_0_2_434 && _old_443 != _keep_1_0_435 && _old_443 != _keep_1_1_436 && _old_443 != _keep_1_2_437 && _old_443 != _keep_2_0_438 && _old_443 != _keep_2_1_439 && _old_443 != _keep_2_2_440) {
                            LIR_unload_subject(_old_443);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_SETE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_SETE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_444 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_444 != _src_2_428) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_428;
                        optimized = 1;
                        if (_old_444 && _old_444 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_444 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_444 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_444 != _src_1_427 && _old_444 != _src_2_428 && _old_444 != _keep_0_0_432 && _old_444 != _keep_0_1_433 && _old_444 != _keep_0_2_434 && _old_444 != _keep_1_0_435 && _old_444 != _keep_1_1_436 && _old_444 != _keep_1_2_437 && _old_444 != _keep_2_0_438 && _old_444 != _keep_2_1_439 && _old_444 != _keep_2_2_440) {
                            LIR_unload_subject(_old_444);
                        }
                    }
                    lir_subject_t* _old_445 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_445) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_445 && _old_445 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_445 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_445 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_445 != _src_1_427 && _old_445 != _src_2_428 && _old_445 != _keep_0_0_432 && _old_445 != _keep_0_1_433 && _old_445 != _keep_0_2_434 && _old_445 != _keep_1_0_435 && _old_445 != _keep_1_1_436 && _old_445 != _keep_1_2_437 && _old_445 != _keep_2_0_438 && _old_445 != _keep_2_1_439 && _old_445 != _keep_2_2_440) {
                            LIR_unload_subject(_old_445);
                        }
                    }
                    lir_subject_t* _old_446 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_446) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_446 && _old_446 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_446 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_446 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_446 != _src_1_427 && _old_446 != _src_2_428 && _old_446 != _keep_0_0_432 && _old_446 != _keep_0_1_433 && _old_446 != _keep_0_2_434 && _old_446 != _keep_1_0_435 && _old_446 != _keep_1_1_436 && _old_446 != _keep_1_2_437 && _old_446 != _keep_2_0_438 && _old_446 != _keep_2_1_439 && _old_446 != _keep_2_2_440) {
                            LIR_unload_subject(_old_446);
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
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_STNE &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_CONSTVAL || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_MEMORY || LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_LABEL)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_447 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_448 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_449 = lh->op;
                    lir_operation_t _match_op_1_450 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_451 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_452 = lh->farg;
                    lir_subject_t* _keep_0_1_453 = lh->sarg;
                    lir_subject_t* _keep_0_2_454 = lh->targ;
                    lir_subject_t* _keep_1_0_455 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_456 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_457 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_458 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_459 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_460 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_461 = lh->sarg;
                    if (_old_461 != _src_1_447) {
                        lh->sarg = _src_1_447;
                        optimized = 1;
                        if (_old_461 && _old_461 != lh->farg && _old_461 != lh->sarg && _old_461 != lh->targ && _old_461 != _src_1_447 && _old_461 != _src_2_448 && _old_461 != _keep_0_0_452 && _old_461 != _keep_0_1_453 && _old_461 != _keep_0_2_454 && _old_461 != _keep_1_0_455 && _old_461 != _keep_1_1_456 && _old_461 != _keep_1_2_457 && _old_461 != _keep_2_0_458 && _old_461 != _keep_2_1_459 && _old_461 != _keep_2_2_460) {
                            LIR_unload_subject(_old_461);
                        }
                    }
                    lir_subject_t* _old_462 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_462 && _old_462 != lh->farg && _old_462 != lh->sarg && _old_462 != lh->targ && _old_462 != _src_1_447 && _old_462 != _src_2_448 && _old_462 != _keep_0_0_452 && _old_462 != _keep_0_1_453 && _old_462 != _keep_0_2_454 && _old_462 != _keep_1_0_455 && _old_462 != _keep_1_1_456 && _old_462 != _keep_1_2_457 && _old_462 != _keep_2_0_458 && _old_462 != _keep_2_1_459 && _old_462 != _keep_2_2_460) {
                        LIR_unload_subject(_old_462);
                    }
                    lir_subject_t* _old_463 = lh->farg;
                    if (_old_463 != _src_1_447) {
                        lh->farg = _src_1_447;
                        optimized = 1;
                        if (_old_463 && _old_463 != lh->farg && _old_463 != lh->sarg && _old_463 != lh->targ && _old_463 != _src_1_447 && _old_463 != _src_2_448 && _old_463 != _keep_0_0_452 && _old_463 != _keep_0_1_453 && _old_463 != _keep_0_2_454 && _old_463 != _keep_1_0_455 && _old_463 != _keep_1_1_456 && _old_463 != _keep_1_2_457 && _old_463 != _keep_2_0_458 && _old_463 != _keep_2_1_459 && _old_463 != _keep_2_2_460) {
                            LIR_unload_subject(_old_463);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_STNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_STNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_464 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_464 != _src_2_448) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_448;
                        optimized = 1;
                        if (_old_464 && _old_464 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_464 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_464 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_464 != _src_1_447 && _old_464 != _src_2_448 && _old_464 != _keep_0_0_452 && _old_464 != _keep_0_1_453 && _old_464 != _keep_0_2_454 && _old_464 != _keep_1_0_455 && _old_464 != _keep_1_1_456 && _old_464 != _keep_1_2_457 && _old_464 != _keep_2_0_458 && _old_464 != _keep_2_1_459 && _old_464 != _keep_2_2_460) {
                            LIR_unload_subject(_old_464);
                        }
                    }
                    lir_subject_t* _old_465 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_465) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_465 && _old_465 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_465 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_465 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_465 != _src_1_447 && _old_465 != _src_2_448 && _old_465 != _keep_0_0_452 && _old_465 != _keep_0_1_453 && _old_465 != _keep_0_2_454 && _old_465 != _keep_1_0_455 && _old_465 != _keep_1_1_456 && _old_465 != _keep_1_2_457 && _old_465 != _keep_2_0_458 && _old_465 != _keep_2_1_459 && _old_465 != _keep_2_2_460) {
                            LIR_unload_subject(_old_465);
                        }
                    }
                    lir_subject_t* _old_466 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_466) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_466 && _old_466 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_466 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_466 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_466 != _src_1_447 && _old_466 != _src_2_448 && _old_466 != _keep_0_0_452 && _old_466 != _keep_0_1_453 && _old_466 != _keep_0_2_454 && _old_466 != _keep_1_0_455 && _old_466 != _keep_1_1_456 && _old_466 != _keep_1_2_457 && _old_466 != _keep_2_0_458 && _old_466 != _keep_2_1_459 && _old_466 != _keep_2_2_460) {
                            LIR_unload_subject(_old_466);
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