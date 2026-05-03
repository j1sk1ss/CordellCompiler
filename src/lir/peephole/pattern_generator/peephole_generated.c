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
                    lir_subject_t* _src_1_324 = lh->farg;
                    lir_subject_t* _src_2_325 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_326 = lh->op;
                    lir_operation_t _match_op_1_327 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_328 = lh->farg;
                    lir_subject_t* _keep_0_1_329 = lh->sarg;
                    lir_subject_t* _keep_0_2_330 = lh->targ;
                    lir_subject_t* _keep_1_0_331 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_332 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_333 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_334 = lh->farg;
                    if (_old_334 != _src_1_324) {
                        lh->farg = _src_1_324;
                        optimized = 1;
                        if (_old_334 && _old_334 != lh->farg && _old_334 != lh->sarg && _old_334 != lh->targ && _old_334 != _src_1_324 && _old_334 != _src_2_325 && _old_334 != _keep_0_0_328 && _old_334 != _keep_0_1_329 && _old_334 != _keep_0_2_330 && _old_334 != _keep_1_0_331 && _old_334 != _keep_1_1_332 && _old_334 != _keep_1_2_333) {
                            LIR_unload_subject(_old_334);
                        }
                    }
                    lir_subject_t* _old_335 = lh->sarg;
                    if (_old_335 != _src_1_324) {
                        lh->sarg = _src_1_324;
                        optimized = 1;
                        if (_old_335 && _old_335 != lh->farg && _old_335 != lh->sarg && _old_335 != lh->targ && _old_335 != _src_1_324 && _old_335 != _src_2_325 && _old_335 != _keep_0_0_328 && _old_335 != _keep_0_1_329 && _old_335 != _keep_0_2_330 && _old_335 != _keep_1_0_331 && _old_335 != _keep_1_1_332 && _old_335 != _keep_1_2_333) {
                            LIR_unload_subject(_old_335);
                        }
                    }
                    lir_subject_t* _old_336 = lh->targ;
                    if (_old_336) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_336 && _old_336 != lh->farg && _old_336 != lh->sarg && _old_336 != lh->targ && _old_336 != _src_1_324 && _old_336 != _src_2_325 && _old_336 != _keep_0_0_328 && _old_336 != _keep_0_1_329 && _old_336 != _keep_0_2_330 && _old_336 != _keep_1_0_331 && _old_336 != _keep_1_1_332 && _old_336 != _keep_1_2_333) {
                            LIR_unload_subject(_old_336);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_327) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_327;
                        optimized = 1;
                    }
                    lir_subject_t* _old_337 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_337 != _src_2_325) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_325;
                        optimized = 1;
                        if (_old_337 && _old_337 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_337 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_337 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_337 != _src_1_324 && _old_337 != _src_2_325 && _old_337 != _keep_0_0_328 && _old_337 != _keep_0_1_329 && _old_337 != _keep_0_2_330 && _old_337 != _keep_1_0_331 && _old_337 != _keep_1_1_332 && _old_337 != _keep_1_2_333) {
                            LIR_unload_subject(_old_337);
                        }
                    }
                    lir_subject_t* _old_338 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_338) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_338 && _old_338 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_338 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_338 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_338 != _src_1_324 && _old_338 != _src_2_325 && _old_338 != _keep_0_0_328 && _old_338 != _keep_0_1_329 && _old_338 != _keep_0_2_330 && _old_338 != _keep_1_0_331 && _old_338 != _keep_1_1_332 && _old_338 != _keep_1_2_333) {
                            LIR_unload_subject(_old_338);
                        }
                    }
                    lir_subject_t* _old_339 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_339) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_339 && _old_339 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_339 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_339 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_339 != _src_1_324 && _old_339 != _src_2_325 && _old_339 != _keep_0_0_328 && _old_339 != _keep_0_1_329 && _old_339 != _keep_0_2_330 && _old_339 != _keep_1_0_331 && _old_339 != _keep_1_1_332 && _old_339 != _keep_1_2_333) {
                            LIR_unload_subject(_old_339);
                        }
                    }
                }
                else if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lir_subject_t* _src_1_492 = lh->farg;
                    lir_operation_t _match_op_0_493 = lh->op;
                    lir_subject_t* _keep_0_0_494 = lh->farg;
                    lir_subject_t* _keep_0_1_495 = lh->sarg;
                    lir_subject_t* _keep_0_2_496 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_497 = lh->farg;
                    if (_old_497 != _src_1_492) {
                        lh->farg = _src_1_492;
                        optimized = 1;
                        if (_old_497 && _old_497 != lh->farg && _old_497 != lh->sarg && _old_497 != lh->targ && _old_497 != _src_1_492 && _old_497 != _keep_0_0_494 && _old_497 != _keep_0_1_495 && _old_497 != _keep_0_2_496) {
                            LIR_unload_subject(_old_497);
                        }
                    }
                    lir_subject_t* _old_498 = lh->sarg;
                    if (_old_498 != _src_1_492) {
                        lh->sarg = _src_1_492;
                        optimized = 1;
                        if (_old_498 && _old_498 != lh->farg && _old_498 != lh->sarg && _old_498 != lh->targ && _old_498 != _src_1_492 && _old_498 != _keep_0_0_494 && _old_498 != _keep_0_1_495 && _old_498 != _keep_0_2_496) {
                            LIR_unload_subject(_old_498);
                        }
                    }
                    lir_subject_t* _old_499 = lh->targ;
                    if (_old_499) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_499 && _old_499 != lh->farg && _old_499 != lh->sarg && _old_499 != lh->targ && _old_499 != _src_1_492 && _old_499 != _keep_0_0_494 && _old_499 != _keep_0_1_495 && _old_499 != _keep_0_2_496) {
                            LIR_unload_subject(_old_499);
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
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iADD &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg)) {
                    lir_subject_t* _src_1_167 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_2_168 = lh->sarg;
                    lir_subject_t* _src_3_169 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _src_4_170 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_171 = lh->op;
                    lir_operation_t _match_op_1_172 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_173 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_174 = lh->farg;
                    lir_subject_t* _keep_0_1_175 = lh->sarg;
                    lir_subject_t* _keep_0_2_176 = lh->targ;
                    lir_subject_t* _keep_1_0_177 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_178 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_179 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_180 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_181 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_182 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_171) {
                        lh->op = _match_op_0_171;
                        optimized = 1;
                    }
                    lir_subject_t* _old_183 = lh->farg;
                    if (_old_183 != _src_1_167) {
                        lh->farg = _src_1_167;
                        optimized = 1;
                        if (_old_183 && _old_183 != lh->farg && _old_183 != lh->sarg && _old_183 != lh->targ && _old_183 != _src_1_167 && _old_183 != _src_2_168 && _old_183 != _src_3_169 && _old_183 != _src_4_170 && _old_183 != _keep_0_0_174 && _old_183 != _keep_0_1_175 && _old_183 != _keep_0_2_176 && _old_183 != _keep_1_0_177 && _old_183 != _keep_1_1_178 && _old_183 != _keep_1_2_179 && _old_183 != _keep_2_0_180 && _old_183 != _keep_2_1_181 && _old_183 != _keep_2_2_182) {
                            LIR_unload_subject(_old_183);
                        }
                    }
                    lir_subject_t* _old_184 = lh->sarg;
                    if (_old_184 != _src_2_168) {
                        lh->sarg = _src_2_168;
                        optimized = 1;
                        if (_old_184 && _old_184 != lh->farg && _old_184 != lh->sarg && _old_184 != lh->targ && _old_184 != _src_1_167 && _old_184 != _src_2_168 && _old_184 != _src_3_169 && _old_184 != _src_4_170 && _old_184 != _keep_0_0_174 && _old_184 != _keep_0_1_175 && _old_184 != _keep_0_2_176 && _old_184 != _keep_1_0_177 && _old_184 != _keep_1_1_178 && _old_184 != _keep_1_2_179 && _old_184 != _keep_2_0_180 && _old_184 != _keep_2_1_181 && _old_184 != _keep_2_2_182) {
                            LIR_unload_subject(_old_184);
                        }
                    }
                    lir_subject_t* _old_185 = lh->targ;
                    if (_old_185) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_185 && _old_185 != lh->farg && _old_185 != lh->sarg && _old_185 != lh->targ && _old_185 != _src_1_167 && _old_185 != _src_2_168 && _old_185 != _src_3_169 && _old_185 != _src_4_170 && _old_185 != _keep_0_0_174 && _old_185 != _keep_0_1_175 && _old_185 != _keep_0_2_176 && _old_185 != _keep_1_0_177 && _old_185 != _keep_1_1_178 && _old_185 != _keep_1_2_179 && _old_185 != _keep_2_0_180 && _old_185 != _keep_2_1_181 && _old_185 != _keep_2_2_182) {
                            LIR_unload_subject(_old_185);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_172) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_172;
                        optimized = 1;
                    }
                    lir_subject_t* _old_186 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_186 != _src_3_169) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_169;
                        optimized = 1;
                        if (_old_186 && _old_186 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_186 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_186 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_186 != _src_1_167 && _old_186 != _src_2_168 && _old_186 != _src_3_169 && _old_186 != _src_4_170 && _old_186 != _keep_0_0_174 && _old_186 != _keep_0_1_175 && _old_186 != _keep_0_2_176 && _old_186 != _keep_1_0_177 && _old_186 != _keep_1_1_178 && _old_186 != _keep_1_2_179 && _old_186 != _keep_2_0_180 && _old_186 != _keep_2_1_181 && _old_186 != _keep_2_2_182) {
                            LIR_unload_subject(_old_186);
                        }
                    }
                    lir_subject_t* _old_187 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_187 != _src_4_170) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_4_170;
                        optimized = 1;
                        if (_old_187 && _old_187 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_187 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_187 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_187 != _src_1_167 && _old_187 != _src_2_168 && _old_187 != _src_3_169 && _old_187 != _src_4_170 && _old_187 != _keep_0_0_174 && _old_187 != _keep_0_1_175 && _old_187 != _keep_0_2_176 && _old_187 != _keep_1_0_177 && _old_187 != _keep_1_1_178 && _old_187 != _keep_1_2_179 && _old_187 != _keep_2_0_180 && _old_187 != _keep_2_1_181 && _old_187 != _keep_2_2_182) {
                            LIR_unload_subject(_old_187);
                        }
                    }
                    lir_subject_t* _old_188 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_188) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_188 && _old_188 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_188 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_188 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_188 != _src_1_167 && _old_188 != _src_2_168 && _old_188 != _src_3_169 && _old_188 != _src_4_170 && _old_188 != _keep_0_0_174 && _old_188 != _keep_0_1_175 && _old_188 != _keep_0_2_176 && _old_188 != _keep_1_0_177 && _old_188 != _keep_1_1_178 && _old_188 != _keep_1_2_179 && _old_188 != _keep_2_0_180 && _old_188 != _keep_2_1_181 && _old_188 != _keep_2_2_182) {
                            LIR_unload_subject(_old_188);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_189 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    if (_old_189 != _src_1_167) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg = _src_1_167;
                        optimized = 1;
                        if (_old_189 && _old_189 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_189 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_189 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_189 != _src_1_167 && _old_189 != _src_2_168 && _old_189 != _src_3_169 && _old_189 != _src_4_170 && _old_189 != _keep_0_0_174 && _old_189 != _keep_0_1_175 && _old_189 != _keep_0_2_176 && _old_189 != _keep_1_0_177 && _old_189 != _keep_1_1_178 && _old_189 != _keep_1_2_179 && _old_189 != _keep_2_0_180 && _old_189 != _keep_2_1_181 && _old_189 != _keep_2_2_182) {
                            LIR_unload_subject(_old_189);
                        }
                    }
                    lir_subject_t* _old_190 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (_old_190 != _src_4_170) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ = _src_4_170;
                        optimized = 1;
                        if (_old_190 && _old_190 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_190 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_190 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_190 != _src_1_167 && _old_190 != _src_2_168 && _old_190 != _src_3_169 && _old_190 != _src_4_170 && _old_190 != _keep_0_0_174 && _old_190 != _keep_0_1_175 && _old_190 != _keep_0_2_176 && _old_190 != _keep_1_0_177 && _old_190 != _keep_1_1_178 && _old_190 != _keep_1_2_179 && _old_190 != _keep_2_0_180 && _old_190 != _keep_2_1_181 && _old_190 != _keep_2_2_182) {
                            LIR_unload_subject(_old_190);
                        }
                    }
                    lir_subject_t* _old_191 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    if (_old_191 != _src_1_167) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg = _src_1_167;
                        optimized = 1;
                        if (_old_191 && _old_191 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_191 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_191 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_191 != _src_1_167 && _old_191 != _src_2_168 && _old_191 != _src_3_169 && _old_191 != _src_4_170 && _old_191 != _keep_0_0_174 && _old_191 != _keep_0_1_175 && _old_191 != _keep_0_2_176 && _old_191 != _keep_1_0_177 && _old_191 != _keep_1_1_178 && _old_191 != _keep_1_2_179 && _old_191 != _keep_2_0_180 && _old_191 != _keep_2_1_181 && _old_191 != _keep_2_2_182) {
                            LIR_unload_subject(_old_191);
                        }
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
                    lir_subject_t* _src_1_192 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_193 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_194 = lh->op;
                    lir_operation_t _match_op_1_195 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_196 = lh->farg;
                    lir_subject_t* _keep_0_1_197 = lh->sarg;
                    lir_subject_t* _keep_0_2_198 = lh->targ;
                    lir_subject_t* _keep_1_0_199 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_200 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_201 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_194) {
                        lh->op = _match_op_0_194;
                        optimized = 1;
                    }
                    lir_subject_t* _old_202 = lh->farg;
                    if (_old_202 != _src_1_192) {
                        lh->farg = _src_1_192;
                        optimized = 1;
                        if (_old_202 && _old_202 != lh->farg && _old_202 != lh->sarg && _old_202 != lh->targ && _old_202 != _src_1_192 && _old_202 != _src_2_193 && _old_202 != _keep_0_0_196 && _old_202 != _keep_0_1_197 && _old_202 != _keep_0_2_198 && _old_202 != _keep_1_0_199 && _old_202 != _keep_1_1_200 && _old_202 != _keep_1_2_201) {
                            LIR_unload_subject(_old_202);
                        }
                    }
                    lir_subject_t* _old_203 = lh->sarg;
                    if (_old_203 != _src_2_193) {
                        lh->sarg = _src_2_193;
                        optimized = 1;
                        if (_old_203 && _old_203 != lh->farg && _old_203 != lh->sarg && _old_203 != lh->targ && _old_203 != _src_1_192 && _old_203 != _src_2_193 && _old_203 != _keep_0_0_196 && _old_203 != _keep_0_1_197 && _old_203 != _keep_0_2_198 && _old_203 != _keep_1_0_199 && _old_203 != _keep_1_1_200 && _old_203 != _keep_1_2_201) {
                            LIR_unload_subject(_old_203);
                        }
                    }
                    lir_subject_t* _old_204 = lh->targ;
                    if (_old_204) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_204 && _old_204 != lh->farg && _old_204 != lh->sarg && _old_204 != lh->targ && _old_204 != _src_1_192 && _old_204 != _src_2_193 && _old_204 != _keep_0_0_196 && _old_204 != _keep_0_1_197 && _old_204 != _keep_0_2_198 && _old_204 != _keep_1_0_199 && _old_204 != _keep_1_1_200 && _old_204 != _keep_1_2_201) {
                            LIR_unload_subject(_old_204);
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
                    lir_subject_t* _src_1_205 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_206 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_3_207 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_208 = lh->op;
                    lir_operation_t _match_op_1_209 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_210 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_211 = lh->farg;
                    lir_subject_t* _keep_0_1_212 = lh->sarg;
                    lir_subject_t* _keep_0_2_213 = lh->targ;
                    lir_subject_t* _keep_1_0_214 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_215 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_216 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_217 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_218 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_219 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_208) {
                        lh->op = _match_op_0_208;
                        optimized = 1;
                    }
                    lir_subject_t* _old_220 = lh->farg;
                    if (_old_220 != _src_1_205) {
                        lh->farg = _src_1_205;
                        optimized = 1;
                        if (_old_220 && _old_220 != lh->farg && _old_220 != lh->sarg && _old_220 != lh->targ && _old_220 != _src_1_205 && _old_220 != _src_2_206 && _old_220 != _src_3_207 && _old_220 != _keep_0_0_211 && _old_220 != _keep_0_1_212 && _old_220 != _keep_0_2_213 && _old_220 != _keep_1_0_214 && _old_220 != _keep_1_1_215 && _old_220 != _keep_1_2_216 && _old_220 != _keep_2_0_217 && _old_220 != _keep_2_1_218 && _old_220 != _keep_2_2_219) {
                            LIR_unload_subject(_old_220);
                        }
                    }
                    lir_subject_t* _old_221 = lh->sarg;
                    if (_old_221 != _src_2_206) {
                        lh->sarg = _src_2_206;
                        optimized = 1;
                        if (_old_221 && _old_221 != lh->farg && _old_221 != lh->sarg && _old_221 != lh->targ && _old_221 != _src_1_205 && _old_221 != _src_2_206 && _old_221 != _src_3_207 && _old_221 != _keep_0_0_211 && _old_221 != _keep_0_1_212 && _old_221 != _keep_0_2_213 && _old_221 != _keep_1_0_214 && _old_221 != _keep_1_1_215 && _old_221 != _keep_1_2_216 && _old_221 != _keep_2_0_217 && _old_221 != _keep_2_1_218 && _old_221 != _keep_2_2_219) {
                            LIR_unload_subject(_old_221);
                        }
                    }
                    lir_subject_t* _old_222 = lh->targ;
                    if (_old_222) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_222 && _old_222 != lh->farg && _old_222 != lh->sarg && _old_222 != lh->targ && _old_222 != _src_1_205 && _old_222 != _src_2_206 && _old_222 != _src_3_207 && _old_222 != _keep_0_0_211 && _old_222 != _keep_0_1_212 && _old_222 != _keep_0_2_213 && _old_222 != _keep_1_0_214 && _old_222 != _keep_1_1_215 && _old_222 != _keep_1_2_216 && _old_222 != _keep_2_0_217 && _old_222 != _keep_2_1_218 && _old_222 != _keep_2_2_219) {
                            LIR_unload_subject(_old_222);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_223 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_223 != _src_2_206) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_206;
                        optimized = 1;
                        if (_old_223 && _old_223 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_223 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_223 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_223 != _src_1_205 && _old_223 != _src_2_206 && _old_223 != _src_3_207 && _old_223 != _keep_0_0_211 && _old_223 != _keep_0_1_212 && _old_223 != _keep_0_2_213 && _old_223 != _keep_1_0_214 && _old_223 != _keep_1_1_215 && _old_223 != _keep_1_2_216 && _old_223 != _keep_2_0_217 && _old_223 != _keep_2_1_218 && _old_223 != _keep_2_2_219) {
                            LIR_unload_subject(_old_223);
                        }
                    }
                    lir_subject_t* _old_224 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_224 != _src_3_207) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_3_207;
                        optimized = 1;
                        if (_old_224 && _old_224 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_224 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_224 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_224 != _src_1_205 && _old_224 != _src_2_206 && _old_224 != _src_3_207 && _old_224 != _keep_0_0_211 && _old_224 != _keep_0_1_212 && _old_224 != _keep_0_2_213 && _old_224 != _keep_1_0_214 && _old_224 != _keep_1_1_215 && _old_224 != _keep_1_2_216 && _old_224 != _keep_2_0_217 && _old_224 != _keep_2_1_218 && _old_224 != _keep_2_2_219) {
                            LIR_unload_subject(_old_224);
                        }
                    }
                    lir_subject_t* _old_225 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_225 != _src_2_206) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_206;
                        optimized = 1;
                        if (_old_225 && _old_225 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_225 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_225 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_225 != _src_1_205 && _old_225 != _src_2_206 && _old_225 != _src_3_207 && _old_225 != _keep_0_0_211 && _old_225 != _keep_0_1_212 && _old_225 != _keep_0_2_213 && _old_225 != _keep_1_0_214 && _old_225 != _keep_1_1_215 && _old_225 != _keep_1_2_216 && _old_225 != _keep_2_0_217 && _old_225 != _keep_2_1_218 && _old_225 != _keep_2_2_219) {
                            LIR_unload_subject(_old_225);
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
                    lir_subject_t* _src_1_226 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_227 = lh->sarg;
                    lir_subject_t* _src_3_228 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_229 = lh->op;
                    lir_operation_t _match_op_1_230 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_231 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_232 = lh->farg;
                    lir_subject_t* _keep_0_1_233 = lh->sarg;
                    lir_subject_t* _keep_0_2_234 = lh->targ;
                    lir_subject_t* _keep_1_0_235 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_236 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_237 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_238 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_239 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_240 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_241 = lh->sarg;
                    if (_old_241 != _src_2_227) {
                        lh->sarg = _src_2_227;
                        optimized = 1;
                        if (_old_241 && _old_241 != lh->farg && _old_241 != lh->sarg && _old_241 != lh->targ && _old_241 != _src_1_226 && _old_241 != _src_2_227 && _old_241 != _src_3_228 && _old_241 != _keep_0_0_232 && _old_241 != _keep_0_1_233 && _old_241 != _keep_0_2_234 && _old_241 != _keep_1_0_235 && _old_241 != _keep_1_1_236 && _old_241 != _keep_1_2_237 && _old_241 != _keep_2_0_238 && _old_241 != _keep_2_1_239 && _old_241 != _keep_2_2_240) {
                            LIR_unload_subject(_old_241);
                        }
                    }
                    lir_subject_t* _old_242 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_242 && _old_242 != lh->farg && _old_242 != lh->sarg && _old_242 != lh->targ && _old_242 != _src_1_226 && _old_242 != _src_2_227 && _old_242 != _src_3_228 && _old_242 != _keep_0_0_232 && _old_242 != _keep_0_1_233 && _old_242 != _keep_0_2_234 && _old_242 != _keep_1_0_235 && _old_242 != _keep_1_1_236 && _old_242 != _keep_1_2_237 && _old_242 != _keep_2_0_238 && _old_242 != _keep_2_1_239 && _old_242 != _keep_2_2_240) {
                        LIR_unload_subject(_old_242);
                    }
                    lir_subject_t* _old_243 = lh->farg;
                    if (_old_243 != _src_2_227) {
                        lh->farg = _src_2_227;
                        optimized = 1;
                        if (_old_243 && _old_243 != lh->farg && _old_243 != lh->sarg && _old_243 != lh->targ && _old_243 != _src_1_226 && _old_243 != _src_2_227 && _old_243 != _src_3_228 && _old_243 != _keep_0_0_232 && _old_243 != _keep_0_1_233 && _old_243 != _keep_0_2_234 && _old_243 != _keep_1_0_235 && _old_243 != _keep_1_1_236 && _old_243 != _keep_1_2_237 && _old_243 != _keep_2_0_238 && _old_243 != _keep_2_1_239 && _old_243 != _keep_2_2_240) {
                            LIR_unload_subject(_old_243);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_229) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_229;
                        optimized = 1;
                    }
                    lir_subject_t* _old_244 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_244 != _src_3_228) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_228;
                        optimized = 1;
                        if (_old_244 && _old_244 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_244 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_244 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_244 != _src_1_226 && _old_244 != _src_2_227 && _old_244 != _src_3_228 && _old_244 != _keep_0_0_232 && _old_244 != _keep_0_1_233 && _old_244 != _keep_0_2_234 && _old_244 != _keep_1_0_235 && _old_244 != _keep_1_1_236 && _old_244 != _keep_1_2_237 && _old_244 != _keep_2_0_238 && _old_244 != _keep_2_1_239 && _old_244 != _keep_2_2_240) {
                            LIR_unload_subject(_old_244);
                        }
                    }
                    lir_subject_t* _old_245 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_245 != _src_2_227) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_227;
                        optimized = 1;
                        if (_old_245 && _old_245 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_245 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_245 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_245 != _src_1_226 && _old_245 != _src_2_227 && _old_245 != _src_3_228 && _old_245 != _keep_0_0_232 && _old_245 != _keep_0_1_233 && _old_245 != _keep_0_2_234 && _old_245 != _keep_1_0_235 && _old_245 != _keep_1_1_236 && _old_245 != _keep_1_2_237 && _old_245 != _keep_2_0_238 && _old_245 != _keep_2_1_239 && _old_245 != _keep_2_2_240) {
                            LIR_unload_subject(_old_245);
                        }
                    }
                    lir_subject_t* _old_246 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_246) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_246 && _old_246 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_246 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_246 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_246 != _src_1_226 && _old_246 != _src_2_227 && _old_246 != _src_3_228 && _old_246 != _keep_0_0_232 && _old_246 != _keep_0_1_233 && _old_246 != _keep_0_2_234 && _old_246 != _keep_1_0_235 && _old_246 != _keep_1_1_236 && _old_246 != _keep_1_2_237 && _old_246 != _keep_2_0_238 && _old_246 != _keep_2_1_239 && _old_246 != _keep_2_2_240) {
                            LIR_unload_subject(_old_246);
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
                    lir_subject_t* _src_2_247 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_1_248 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_249 = lh->op;
                    lir_operation_t _match_op_1_250 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_251 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_252 = lh->farg;
                    lir_subject_t* _keep_0_1_253 = lh->sarg;
                    lir_subject_t* _keep_0_2_254 = lh->targ;
                    lir_subject_t* _keep_1_0_255 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_256 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_257 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_258 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_259 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_260 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_249) {
                        lh->op = _match_op_0_249;
                        optimized = 1;
                    }
                    lir_subject_t* _old_261 = lh->farg;
                    if (_old_261 != _src_2_247) {
                        lh->farg = _src_2_247;
                        optimized = 1;
                        if (_old_261 && _old_261 != lh->farg && _old_261 != lh->sarg && _old_261 != lh->targ && _old_261 != _src_2_247 && _old_261 != _src_1_248 && _old_261 != _keep_0_0_252 && _old_261 != _keep_0_1_253 && _old_261 != _keep_0_2_254 && _old_261 != _keep_1_0_255 && _old_261 != _keep_1_1_256 && _old_261 != _keep_1_2_257 && _old_261 != _keep_2_0_258 && _old_261 != _keep_2_1_259 && _old_261 != _keep_2_2_260) {
                            LIR_unload_subject(_old_261);
                        }
                    }
                    lir_subject_t* _old_262 = lh->sarg;
                    if (_old_262 != _src_1_248) {
                        lh->sarg = _src_1_248;
                        optimized = 1;
                        if (_old_262 && _old_262 != lh->farg && _old_262 != lh->sarg && _old_262 != lh->targ && _old_262 != _src_2_247 && _old_262 != _src_1_248 && _old_262 != _keep_0_0_252 && _old_262 != _keep_0_1_253 && _old_262 != _keep_0_2_254 && _old_262 != _keep_1_0_255 && _old_262 != _keep_1_1_256 && _old_262 != _keep_1_2_257 && _old_262 != _keep_2_0_258 && _old_262 != _keep_2_1_259 && _old_262 != _keep_2_2_260) {
                            LIR_unload_subject(_old_262);
                        }
                    }
                    lir_subject_t* _old_263 = lh->targ;
                    if (_old_263) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_263 && _old_263 != lh->farg && _old_263 != lh->sarg && _old_263 != lh->targ && _old_263 != _src_2_247 && _old_263 != _src_1_248 && _old_263 != _keep_0_0_252 && _old_263 != _keep_0_1_253 && _old_263 != _keep_0_2_254 && _old_263 != _keep_1_0_255 && _old_263 != _keep_1_1_256 && _old_263 != _keep_1_2_257 && _old_263 != _keep_2_0_258 && _old_263 != _keep_2_1_259 && _old_263 != _keep_2_2_260) {
                            LIR_unload_subject(_old_263);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iSUB) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_264 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_264 != _src_1_248) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_248;
                        optimized = 1;
                        if (_old_264 && _old_264 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_264 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_264 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_264 != _src_2_247 && _old_264 != _src_1_248 && _old_264 != _keep_0_0_252 && _old_264 != _keep_0_1_253 && _old_264 != _keep_0_2_254 && _old_264 != _keep_1_0_255 && _old_264 != _keep_1_1_256 && _old_264 != _keep_1_2_257 && _old_264 != _keep_2_0_258 && _old_264 != _keep_2_1_259 && _old_264 != _keep_2_2_260) {
                            LIR_unload_subject(_old_264);
                        }
                    }
                    lir_subject_t* _old_265 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_265 && _old_265 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_265 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_265 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_265 != _src_2_247 && _old_265 != _src_1_248 && _old_265 != _keep_0_0_252 && _old_265 != _keep_0_1_253 && _old_265 != _keep_0_2_254 && _old_265 != _keep_1_0_255 && _old_265 != _keep_1_1_256 && _old_265 != _keep_1_2_257 && _old_265 != _keep_2_0_258 && _old_265 != _keep_2_1_259 && _old_265 != _keep_2_2_260) {
                        LIR_unload_subject(_old_265);
                    }
                    lir_subject_t* _old_266 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_266 != _src_1_248) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_248;
                        optimized = 1;
                        if (_old_266 && _old_266 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_266 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_266 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_266 != _src_2_247 && _old_266 != _src_1_248 && _old_266 != _keep_0_0_252 && _old_266 != _keep_0_1_253 && _old_266 != _keep_0_2_254 && _old_266 != _keep_1_0_255 && _old_266 != _keep_1_1_256 && _old_266 != _keep_1_2_257 && _old_266 != _keep_2_0_258 && _old_266 != _keep_2_1_259 && _old_266 != _keep_2_2_260) {
                            LIR_unload_subject(_old_266);
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
                    lir_subject_t* _src_1_281 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_282 = lh->sarg;
                    lir_subject_t* _src_3_283 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_4_284 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_285 = lh->op;
                    lir_operation_t _match_op_1_286 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_287 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_288 = lh->farg;
                    lir_subject_t* _keep_0_1_289 = lh->sarg;
                    lir_subject_t* _keep_0_2_290 = lh->targ;
                    lir_subject_t* _keep_1_0_291 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_292 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_293 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_294 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_295 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_296 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_1_286) {
                        lh->op = _match_op_1_286;
                        optimized = 1;
                    }
                    lir_subject_t* _old_297 = lh->farg;
                    if (_old_297 != _src_2_282) {
                        lh->farg = _src_2_282;
                        optimized = 1;
                        if (_old_297 && _old_297 != lh->farg && _old_297 != lh->sarg && _old_297 != lh->targ && _old_297 != _src_1_281 && _old_297 != _src_2_282 && _old_297 != _src_3_283 && _old_297 != _src_4_284 && _old_297 != _keep_0_0_288 && _old_297 != _keep_0_1_289 && _old_297 != _keep_0_2_290 && _old_297 != _keep_1_0_291 && _old_297 != _keep_1_1_292 && _old_297 != _keep_1_2_293 && _old_297 != _keep_2_0_294 && _old_297 != _keep_2_1_295 && _old_297 != _keep_2_2_296) {
                            LIR_unload_subject(_old_297);
                        }
                    }
                    lir_subject_t* _old_298 = lh->sarg;
                    if (_old_298 != _src_3_283) {
                        lh->sarg = _src_3_283;
                        optimized = 1;
                        if (_old_298 && _old_298 != lh->farg && _old_298 != lh->sarg && _old_298 != lh->targ && _old_298 != _src_1_281 && _old_298 != _src_2_282 && _old_298 != _src_3_283 && _old_298 != _src_4_284 && _old_298 != _keep_0_0_288 && _old_298 != _keep_0_1_289 && _old_298 != _keep_0_2_290 && _old_298 != _keep_1_0_291 && _old_298 != _keep_1_1_292 && _old_298 != _keep_1_2_293 && _old_298 != _keep_2_0_294 && _old_298 != _keep_2_1_295 && _old_298 != _keep_2_2_296) {
                            LIR_unload_subject(_old_298);
                        }
                    }
                    lir_subject_t* _old_299 = lh->targ;
                    if (_old_299) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_299 && _old_299 != lh->farg && _old_299 != lh->sarg && _old_299 != lh->targ && _old_299 != _src_1_281 && _old_299 != _src_2_282 && _old_299 != _src_3_283 && _old_299 != _src_4_284 && _old_299 != _keep_0_0_288 && _old_299 != _keep_0_1_289 && _old_299 != _keep_0_2_290 && _old_299 != _keep_1_0_291 && _old_299 != _keep_1_1_292 && _old_299 != _keep_1_2_293 && _old_299 != _keep_2_0_294 && _old_299 != _keep_2_1_295 && _old_299 != _keep_2_2_296) {
                            LIR_unload_subject(_old_299);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_287) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_287;
                        optimized = 1;
                    }
                    lir_subject_t* _old_300 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_300 != _src_4_284) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_4_284;
                        optimized = 1;
                        if (_old_300 && _old_300 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_300 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_300 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_300 != _src_1_281 && _old_300 != _src_2_282 && _old_300 != _src_3_283 && _old_300 != _src_4_284 && _old_300 != _keep_0_0_288 && _old_300 != _keep_0_1_289 && _old_300 != _keep_0_2_290 && _old_300 != _keep_1_0_291 && _old_300 != _keep_1_1_292 && _old_300 != _keep_1_2_293 && _old_300 != _keep_2_0_294 && _old_300 != _keep_2_1_295 && _old_300 != _keep_2_2_296) {
                            LIR_unload_subject(_old_300);
                        }
                    }
                    lir_subject_t* _old_301 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_301) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_301 && _old_301 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_301 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_301 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_301 != _src_1_281 && _old_301 != _src_2_282 && _old_301 != _src_3_283 && _old_301 != _src_4_284 && _old_301 != _keep_0_0_288 && _old_301 != _keep_0_1_289 && _old_301 != _keep_0_2_290 && _old_301 != _keep_1_0_291 && _old_301 != _keep_1_1_292 && _old_301 != _keep_1_2_293 && _old_301 != _keep_2_0_294 && _old_301 != _keep_2_1_295 && _old_301 != _keep_2_2_296) {
                            LIR_unload_subject(_old_301);
                        }
                    }
                    lir_subject_t* _old_302 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_302) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_302 && _old_302 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_302 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_302 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_302 != _src_1_281 && _old_302 != _src_2_282 && _old_302 != _src_3_283 && _old_302 != _src_4_284 && _old_302 != _keep_0_0_288 && _old_302 != _keep_0_1_289 && _old_302 != _keep_0_2_290 && _old_302 != _keep_1_0_291 && _old_302 != _keep_1_1_292 && _old_302 != _keep_1_2_293 && _old_302 != _keep_2_0_294 && _old_302 != _keep_2_1_295 && _old_302 != _keep_2_2_296) {
                            LIR_unload_subject(_old_302);
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
                    lir_subject_t* _src_1_303 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_304 = lh->sarg;
                    lir_subject_t* _src_3_305 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_306 = lh->op;
                    lir_operation_t _match_op_1_307 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_308 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_309 = lh->farg;
                    lir_subject_t* _keep_0_1_310 = lh->sarg;
                    lir_subject_t* _keep_0_2_311 = lh->targ;
                    lir_subject_t* _keep_1_0_312 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_313 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_314 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_315 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_316 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_317 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_318 = lh->farg;
                    if (_old_318 != _src_2_304) {
                        lh->farg = _src_2_304;
                        optimized = 1;
                        if (_old_318 && _old_318 != lh->farg && _old_318 != lh->sarg && _old_318 != lh->targ && _old_318 != _src_1_303 && _old_318 != _src_2_304 && _old_318 != _src_3_305 && _old_318 != _keep_0_0_309 && _old_318 != _keep_0_1_310 && _old_318 != _keep_0_2_311 && _old_318 != _keep_1_0_312 && _old_318 != _keep_1_1_313 && _old_318 != _keep_1_2_314 && _old_318 != _keep_2_0_315 && _old_318 != _keep_2_1_316 && _old_318 != _keep_2_2_317) {
                            LIR_unload_subject(_old_318);
                        }
                    }
                    lir_subject_t* _old_319 = lh->sarg;
                    if (_old_319 != _src_2_304) {
                        lh->sarg = _src_2_304;
                        optimized = 1;
                        if (_old_319 && _old_319 != lh->farg && _old_319 != lh->sarg && _old_319 != lh->targ && _old_319 != _src_1_303 && _old_319 != _src_2_304 && _old_319 != _src_3_305 && _old_319 != _keep_0_0_309 && _old_319 != _keep_0_1_310 && _old_319 != _keep_0_2_311 && _old_319 != _keep_1_0_312 && _old_319 != _keep_1_1_313 && _old_319 != _keep_1_2_314 && _old_319 != _keep_2_0_315 && _old_319 != _keep_2_1_316 && _old_319 != _keep_2_2_317) {
                            LIR_unload_subject(_old_319);
                        }
                    }
                    lir_subject_t* _old_320 = lh->targ;
                    if (_old_320) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_320 && _old_320 != lh->farg && _old_320 != lh->sarg && _old_320 != lh->targ && _old_320 != _src_1_303 && _old_320 != _src_2_304 && _old_320 != _src_3_305 && _old_320 != _keep_0_0_309 && _old_320 != _keep_0_1_310 && _old_320 != _keep_0_2_311 && _old_320 != _keep_1_0_312 && _old_320 != _keep_1_1_313 && _old_320 != _keep_1_2_314 && _old_320 != _keep_2_0_315 && _old_320 != _keep_2_1_316 && _old_320 != _keep_2_2_317) {
                            LIR_unload_subject(_old_320);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_308) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_308;
                        optimized = 1;
                    }
                    lir_subject_t* _old_321 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_321 != _src_3_305) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_305;
                        optimized = 1;
                        if (_old_321 && _old_321 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_321 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_321 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_321 != _src_1_303 && _old_321 != _src_2_304 && _old_321 != _src_3_305 && _old_321 != _keep_0_0_309 && _old_321 != _keep_0_1_310 && _old_321 != _keep_0_2_311 && _old_321 != _keep_1_0_312 && _old_321 != _keep_1_1_313 && _old_321 != _keep_1_2_314 && _old_321 != _keep_2_0_315 && _old_321 != _keep_2_1_316 && _old_321 != _keep_2_2_317) {
                            LIR_unload_subject(_old_321);
                        }
                    }
                    lir_subject_t* _old_322 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_322) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_322 && _old_322 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_322 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_322 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_322 != _src_1_303 && _old_322 != _src_2_304 && _old_322 != _src_3_305 && _old_322 != _keep_0_0_309 && _old_322 != _keep_0_1_310 && _old_322 != _keep_0_2_311 && _old_322 != _keep_1_0_312 && _old_322 != _keep_1_1_313 && _old_322 != _keep_1_2_314 && _old_322 != _keep_2_0_315 && _old_322 != _keep_2_1_316 && _old_322 != _keep_2_2_317) {
                            LIR_unload_subject(_old_322);
                        }
                    }
                    lir_subject_t* _old_323 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_323) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_323 && _old_323 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_323 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_323 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_323 != _src_1_303 && _old_323 != _src_2_304 && _old_323 != _src_3_305 && _old_323 != _keep_0_0_309 && _old_323 != _keep_0_1_310 && _old_323 != _keep_0_2_311 && _old_323 != _keep_1_0_312 && _old_323 != _keep_1_1_313 && _old_323 != _keep_1_2_314 && _old_323 != _keep_2_0_315 && _old_323 != _keep_2_1_316 && _old_323 != _keep_2_2_317) {
                            LIR_unload_subject(_old_323);
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
                    lir_subject_t* _src_1_356 = lh->farg;
                    lir_subject_t* _src_2_357 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_358 = lh->op;
                    lir_operation_t _match_op_1_359 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_360 = lh->farg;
                    lir_subject_t* _keep_0_1_361 = lh->sarg;
                    lir_subject_t* _keep_0_2_362 = lh->targ;
                    lir_subject_t* _keep_1_0_363 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_364 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_365 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_366 = lh->farg;
                    if (_old_366 != _src_1_356) {
                        lh->farg = _src_1_356;
                        optimized = 1;
                        if (_old_366 && _old_366 != lh->farg && _old_366 != lh->sarg && _old_366 != lh->targ && _old_366 != _src_1_356 && _old_366 != _src_2_357 && _old_366 != _keep_0_0_360 && _old_366 != _keep_0_1_361 && _old_366 != _keep_0_2_362 && _old_366 != _keep_1_0_363 && _old_366 != _keep_1_1_364 && _old_366 != _keep_1_2_365) {
                            LIR_unload_subject(_old_366);
                        }
                    }
                    lir_subject_t* _old_367 = lh->sarg;
                    if (_old_367 != _src_1_356) {
                        lh->sarg = _src_1_356;
                        optimized = 1;
                        if (_old_367 && _old_367 != lh->farg && _old_367 != lh->sarg && _old_367 != lh->targ && _old_367 != _src_1_356 && _old_367 != _src_2_357 && _old_367 != _keep_0_0_360 && _old_367 != _keep_0_1_361 && _old_367 != _keep_0_2_362 && _old_367 != _keep_1_0_363 && _old_367 != _keep_1_1_364 && _old_367 != _keep_1_2_365) {
                            LIR_unload_subject(_old_367);
                        }
                    }
                    lir_subject_t* _old_368 = lh->targ;
                    if (_old_368) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_368 && _old_368 != lh->farg && _old_368 != lh->sarg && _old_368 != lh->targ && _old_368 != _src_1_356 && _old_368 != _src_2_357 && _old_368 != _keep_0_0_360 && _old_368 != _keep_0_1_361 && _old_368 != _keep_0_2_362 && _old_368 != _keep_1_0_363 && _old_368 != _keep_1_1_364 && _old_368 != _keep_1_2_365) {
                            LIR_unload_subject(_old_368);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_359) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_359;
                        optimized = 1;
                    }
                    lir_subject_t* _old_369 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_369 != _src_2_357) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_357;
                        optimized = 1;
                        if (_old_369 && _old_369 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_369 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_369 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_369 != _src_1_356 && _old_369 != _src_2_357 && _old_369 != _keep_0_0_360 && _old_369 != _keep_0_1_361 && _old_369 != _keep_0_2_362 && _old_369 != _keep_1_0_363 && _old_369 != _keep_1_1_364 && _old_369 != _keep_1_2_365) {
                            LIR_unload_subject(_old_369);
                        }
                    }
                    lir_subject_t* _old_370 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_370) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_370 && _old_370 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_370 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_370 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_370 != _src_1_356 && _old_370 != _src_2_357 && _old_370 != _keep_0_0_360 && _old_370 != _keep_0_1_361 && _old_370 != _keep_0_2_362 && _old_370 != _keep_1_0_363 && _old_370 != _keep_1_1_364 && _old_370 != _keep_1_2_365) {
                            LIR_unload_subject(_old_370);
                        }
                    }
                    lir_subject_t* _old_371 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_371) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_371 && _old_371 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_371 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_371 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_371 != _src_1_356 && _old_371 != _src_2_357 && _old_371 != _keep_0_0_360 && _old_371 != _keep_0_1_361 && _old_371 != _keep_0_2_362 && _old_371 != _keep_1_0_363 && _old_371 != _keep_1_1_364 && _old_371 != _keep_1_2_365) {
                            LIR_unload_subject(_old_371);
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
                    lir_subject_t* _src_1_508 = lh->farg;
                    lir_operation_t _match_op_0_509 = lh->op;
                    lir_subject_t* _keep_0_0_510 = lh->farg;
                    lir_subject_t* _keep_0_1_511 = lh->sarg;
                    lir_subject_t* _keep_0_2_512 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_513 = lh->farg;
                    if (_old_513 != _src_1_508) {
                        lh->farg = _src_1_508;
                        optimized = 1;
                        if (_old_513 && _old_513 != lh->farg && _old_513 != lh->sarg && _old_513 != lh->targ && _old_513 != _src_1_508 && _old_513 != _keep_0_0_510 && _old_513 != _keep_0_1_511 && _old_513 != _keep_0_2_512) {
                            LIR_unload_subject(_old_513);
                        }
                    }
                    lir_subject_t* _old_514 = lh->sarg;
                    if (_old_514 != _src_1_508) {
                        lh->sarg = _src_1_508;
                        optimized = 1;
                        if (_old_514 && _old_514 != lh->farg && _old_514 != lh->sarg && _old_514 != lh->targ && _old_514 != _src_1_508 && _old_514 != _keep_0_0_510 && _old_514 != _keep_0_1_511 && _old_514 != _keep_0_2_512) {
                            LIR_unload_subject(_old_514);
                        }
                    }
                    lir_subject_t* _old_515 = lh->targ;
                    if (_old_515) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_515 && _old_515 != lh->farg && _old_515 != lh->sarg && _old_515 != lh->targ && _old_515 != _src_1_508 && _old_515 != _keep_0_0_510 && _old_515 != _keep_0_1_511 && _old_515 != _keep_0_2_512) {
                            LIR_unload_subject(_old_515);
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
                    lir_subject_t* _src_1_540 = lh->farg;
                    lir_operation_t _match_op_0_541 = lh->op;
                    lir_subject_t* _keep_0_0_542 = lh->farg;
                    lir_subject_t* _keep_0_1_543 = lh->sarg;
                    lir_subject_t* _keep_0_2_544 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_545 = lh->sarg;
                    if (_old_545 != _src_1_540) {
                        lh->sarg = _src_1_540;
                        optimized = 1;
                        if (_old_545 && _old_545 != lh->farg && _old_545 != lh->sarg && _old_545 != lh->targ && _old_545 != _src_1_540 && _old_545 != _keep_0_0_542 && _old_545 != _keep_0_1_543 && _old_545 != _keep_0_2_544) {
                            LIR_unload_subject(_old_545);
                        }
                    }
                    lir_subject_t* _old_546 = lh->targ;
                    if (_old_546 != _src_1_540) {
                        lh->targ = _src_1_540;
                        optimized = 1;
                        if (_old_546 && _old_546 != lh->farg && _old_546 != lh->sarg && _old_546 != lh->targ && _old_546 != _src_1_540 && _old_546 != _keep_0_0_542 && _old_546 != _keep_0_1_543 && _old_546 != _keep_0_2_544) {
                            LIR_unload_subject(_old_546);
                        }
                    }
                    lir_subject_t* _old_547 = lh->farg;
                    if (_old_547 != _src_1_540) {
                        lh->farg = _src_1_540;
                        optimized = 1;
                        if (_old_547 && _old_547 != lh->farg && _old_547 != lh->sarg && _old_547 != lh->targ && _old_547 != _src_1_540 && _old_547 != _keep_0_0_542 && _old_547 != _keep_0_1_543 && _old_547 != _keep_0_2_544) {
                            LIR_unload_subject(_old_547);
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
                    lir_subject_t* _src_1_340 = lh->farg;
                    lir_subject_t* _src_2_341 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_342 = lh->op;
                    lir_operation_t _match_op_1_343 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_344 = lh->farg;
                    lir_subject_t* _keep_0_1_345 = lh->sarg;
                    lir_subject_t* _keep_0_2_346 = lh->targ;
                    lir_subject_t* _keep_1_0_347 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_348 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_349 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_350 = lh->farg;
                    if (_old_350 != _src_1_340) {
                        lh->farg = _src_1_340;
                        optimized = 1;
                        if (_old_350 && _old_350 != lh->farg && _old_350 != lh->sarg && _old_350 != lh->targ && _old_350 != _src_1_340 && _old_350 != _src_2_341 && _old_350 != _keep_0_0_344 && _old_350 != _keep_0_1_345 && _old_350 != _keep_0_2_346 && _old_350 != _keep_1_0_347 && _old_350 != _keep_1_1_348 && _old_350 != _keep_1_2_349) {
                            LIR_unload_subject(_old_350);
                        }
                    }
                    lir_subject_t* _old_351 = lh->sarg;
                    if (_old_351 != _src_1_340) {
                        lh->sarg = _src_1_340;
                        optimized = 1;
                        if (_old_351 && _old_351 != lh->farg && _old_351 != lh->sarg && _old_351 != lh->targ && _old_351 != _src_1_340 && _old_351 != _src_2_341 && _old_351 != _keep_0_0_344 && _old_351 != _keep_0_1_345 && _old_351 != _keep_0_2_346 && _old_351 != _keep_1_0_347 && _old_351 != _keep_1_1_348 && _old_351 != _keep_1_2_349) {
                            LIR_unload_subject(_old_351);
                        }
                    }
                    lir_subject_t* _old_352 = lh->targ;
                    if (_old_352) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_352 && _old_352 != lh->farg && _old_352 != lh->sarg && _old_352 != lh->targ && _old_352 != _src_1_340 && _old_352 != _src_2_341 && _old_352 != _keep_0_0_344 && _old_352 != _keep_0_1_345 && _old_352 != _keep_0_2_346 && _old_352 != _keep_1_0_347 && _old_352 != _keep_1_1_348 && _old_352 != _keep_1_2_349) {
                            LIR_unload_subject(_old_352);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_343) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_343;
                        optimized = 1;
                    }
                    lir_subject_t* _old_353 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_353 != _src_2_341) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_341;
                        optimized = 1;
                        if (_old_353 && _old_353 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_353 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_353 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_353 != _src_1_340 && _old_353 != _src_2_341 && _old_353 != _keep_0_0_344 && _old_353 != _keep_0_1_345 && _old_353 != _keep_0_2_346 && _old_353 != _keep_1_0_347 && _old_353 != _keep_1_1_348 && _old_353 != _keep_1_2_349) {
                            LIR_unload_subject(_old_353);
                        }
                    }
                    lir_subject_t* _old_354 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_354) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_354 && _old_354 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_354 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_354 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_354 != _src_1_340 && _old_354 != _src_2_341 && _old_354 != _keep_0_0_344 && _old_354 != _keep_0_1_345 && _old_354 != _keep_0_2_346 && _old_354 != _keep_1_0_347 && _old_354 != _keep_1_1_348 && _old_354 != _keep_1_2_349) {
                            LIR_unload_subject(_old_354);
                        }
                    }
                    lir_subject_t* _old_355 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_355) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_355 && _old_355 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_355 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_355 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_355 != _src_1_340 && _old_355 != _src_2_341 && _old_355 != _keep_0_0_344 && _old_355 != _keep_0_1_345 && _old_355 != _keep_0_2_346 && _old_355 != _keep_1_0_347 && _old_355 != _keep_1_1_348 && _old_355 != _keep_1_2_349) {
                            LIR_unload_subject(_old_355);
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
                    lir_subject_t* _src_1_500 = lh->farg;
                    lir_operation_t _match_op_0_501 = lh->op;
                    lir_subject_t* _keep_0_0_502 = lh->farg;
                    lir_subject_t* _keep_0_1_503 = lh->sarg;
                    lir_subject_t* _keep_0_2_504 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_505 = lh->farg;
                    if (_old_505 != _src_1_500) {
                        lh->farg = _src_1_500;
                        optimized = 1;
                        if (_old_505 && _old_505 != lh->farg && _old_505 != lh->sarg && _old_505 != lh->targ && _old_505 != _src_1_500 && _old_505 != _keep_0_0_502 && _old_505 != _keep_0_1_503 && _old_505 != _keep_0_2_504) {
                            LIR_unload_subject(_old_505);
                        }
                    }
                    lir_subject_t* _old_506 = lh->sarg;
                    if (_old_506 != _src_1_500) {
                        lh->sarg = _src_1_500;
                        optimized = 1;
                        if (_old_506 && _old_506 != lh->farg && _old_506 != lh->sarg && _old_506 != lh->targ && _old_506 != _src_1_500 && _old_506 != _keep_0_0_502 && _old_506 != _keep_0_1_503 && _old_506 != _keep_0_2_504) {
                            LIR_unload_subject(_old_506);
                        }
                    }
                    lir_subject_t* _old_507 = lh->targ;
                    if (_old_507) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_507 && _old_507 != lh->farg && _old_507 != lh->sarg && _old_507 != lh->targ && _old_507 != _src_1_500 && _old_507 != _keep_0_0_502 && _old_507 != _keep_0_1_503 && _old_507 != _keep_0_2_504) {
                            LIR_unload_subject(_old_507);
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
                    lir_subject_t* _src_1_516 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_517 = lh->op;
                    lir_operation_t _match_op_1_518 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_519 = lh->farg;
                    lir_subject_t* _keep_0_1_520 = lh->sarg;
                    lir_subject_t* _keep_0_2_521 = lh->targ;
                    lir_subject_t* _keep_1_0_522 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_523 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_524 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_525 = lh->sarg;
                    if (_old_525 != _src_1_516) {
                        lh->sarg = _src_1_516;
                        optimized = 1;
                        if (_old_525 && _old_525 != lh->farg && _old_525 != lh->sarg && _old_525 != lh->targ && _old_525 != _src_1_516 && _old_525 != _keep_0_0_519 && _old_525 != _keep_0_1_520 && _old_525 != _keep_0_2_521 && _old_525 != _keep_1_0_522 && _old_525 != _keep_1_1_523 && _old_525 != _keep_1_2_524) {
                            LIR_unload_subject(_old_525);
                        }
                    }
                    lir_subject_t* _old_526 = lh->targ;
                    if (_old_526 != _src_1_516) {
                        lh->targ = _src_1_516;
                        optimized = 1;
                        if (_old_526 && _old_526 != lh->farg && _old_526 != lh->sarg && _old_526 != lh->targ && _old_526 != _src_1_516 && _old_526 != _keep_0_0_519 && _old_526 != _keep_0_1_520 && _old_526 != _keep_0_2_521 && _old_526 != _keep_1_0_522 && _old_526 != _keep_1_1_523 && _old_526 != _keep_1_2_524) {
                            LIR_unload_subject(_old_526);
                        }
                    }
                    lir_subject_t* _old_527 = lh->farg;
                    if (_old_527 != _src_1_516) {
                        lh->farg = _src_1_516;
                        optimized = 1;
                        if (_old_527 && _old_527 != lh->farg && _old_527 != lh->sarg && _old_527 != lh->targ && _old_527 != _src_1_516 && _old_527 != _keep_0_0_519 && _old_527 != _keep_0_1_520 && _old_527 != _keep_0_2_521 && _old_527 != _keep_1_0_522 && _old_527 != _keep_1_1_523 && _old_527 != _keep_1_2_524) {
                            LIR_unload_subject(_old_527);
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
                    lir_subject_t* _src_1_528 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_529 = lh->op;
                    lir_operation_t _match_op_1_530 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_531 = lh->farg;
                    lir_subject_t* _keep_0_1_532 = lh->sarg;
                    lir_subject_t* _keep_0_2_533 = lh->targ;
                    lir_subject_t* _keep_1_0_534 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_535 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_536 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_537 = lh->sarg;
                    if (_old_537 != _src_1_528) {
                        lh->sarg = _src_1_528;
                        optimized = 1;
                        if (_old_537 && _old_537 != lh->farg && _old_537 != lh->sarg && _old_537 != lh->targ && _old_537 != _src_1_528 && _old_537 != _keep_0_0_531 && _old_537 != _keep_0_1_532 && _old_537 != _keep_0_2_533 && _old_537 != _keep_1_0_534 && _old_537 != _keep_1_1_535 && _old_537 != _keep_1_2_536) {
                            LIR_unload_subject(_old_537);
                        }
                    }
                    lir_subject_t* _old_538 = lh->targ;
                    if (_old_538 != _src_1_528) {
                        lh->targ = _src_1_528;
                        optimized = 1;
                        if (_old_538 && _old_538 != lh->farg && _old_538 != lh->sarg && _old_538 != lh->targ && _old_538 != _src_1_528 && _old_538 != _keep_0_0_531 && _old_538 != _keep_0_1_532 && _old_538 != _keep_0_2_533 && _old_538 != _keep_1_0_534 && _old_538 != _keep_1_1_535 && _old_538 != _keep_1_2_536) {
                            LIR_unload_subject(_old_538);
                        }
                    }
                    lir_subject_t* _old_539 = lh->farg;
                    if (_old_539 != _src_1_528) {
                        lh->farg = _src_1_528;
                        optimized = 1;
                        if (_old_539 && _old_539 != lh->farg && _old_539 != lh->sarg && _old_539 != lh->targ && _old_539 != _src_1_528 && _old_539 != _keep_0_0_531 && _old_539 != _keep_0_1_532 && _old_539 != _keep_0_2_533 && _old_539 != _keep_1_0_534 && _old_539 != _keep_1_1_535 && _old_539 != _keep_1_2_536) {
                            LIR_unload_subject(_old_539);
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
                    lir_subject_t* _src_1_548 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_549 = lh->op;
                    lir_operation_t _match_op_1_550 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_551 = lh->farg;
                    lir_subject_t* _keep_0_1_552 = lh->sarg;
                    lir_subject_t* _keep_0_2_553 = lh->targ;
                    lir_subject_t* _keep_1_0_554 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_555 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_556 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_557 = lh->sarg;
                    if (_old_557 != _src_1_548) {
                        lh->sarg = _src_1_548;
                        optimized = 1;
                        if (_old_557 && _old_557 != lh->farg && _old_557 != lh->sarg && _old_557 != lh->targ && _old_557 != _src_1_548 && _old_557 != _keep_0_0_551 && _old_557 != _keep_0_1_552 && _old_557 != _keep_0_2_553 && _old_557 != _keep_1_0_554 && _old_557 != _keep_1_1_555 && _old_557 != _keep_1_2_556) {
                            LIR_unload_subject(_old_557);
                        }
                    }
                    lir_subject_t* _old_558 = lh->targ;
                    if (_old_558 != _src_1_548) {
                        lh->targ = _src_1_548;
                        optimized = 1;
                        if (_old_558 && _old_558 != lh->farg && _old_558 != lh->sarg && _old_558 != lh->targ && _old_558 != _src_1_548 && _old_558 != _keep_0_0_551 && _old_558 != _keep_0_1_552 && _old_558 != _keep_0_2_553 && _old_558 != _keep_1_0_554 && _old_558 != _keep_1_1_555 && _old_558 != _keep_1_2_556) {
                            LIR_unload_subject(_old_558);
                        }
                    }
                    lir_subject_t* _old_559 = lh->farg;
                    if (_old_559 != _src_1_548) {
                        lh->farg = _src_1_548;
                        optimized = 1;
                        if (_old_559 && _old_559 != lh->farg && _old_559 != lh->sarg && _old_559 != lh->targ && _old_559 != _src_1_548 && _old_559 != _keep_0_0_551 && _old_559 != _keep_0_1_552 && _old_559 != _keep_0_2_553 && _old_559 != _keep_1_0_554 && _old_559 != _keep_1_1_555 && _old_559 != _keep_1_2_556) {
                            LIR_unload_subject(_old_559);
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
                    lir_operation_t _match_op_0_275 = lh->op;
                    lir_subject_t* _keep_0_0_276 = lh->farg;
                    lir_subject_t* _keep_0_1_277 = lh->sarg;
                    lir_subject_t* _keep_0_2_278 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_279 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_279 && _old_279 != lh->farg && _old_279 != lh->sarg && _old_279 != lh->targ && _old_279 != _keep_0_0_276 && _old_279 != _keep_0_1_277 && _old_279 != _keep_0_2_278) {
                        LIR_unload_subject(_old_279);
                    }
                    lir_subject_t* _old_280 = lh->targ;
                    if (_old_280) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_280 && _old_280 != lh->farg && _old_280 != lh->sarg && _old_280 != lh->targ && _old_280 != _keep_0_0_276 && _old_280 != _keep_0_1_277 && _old_280 != _keep_0_2_278) {
                            LIR_unload_subject(_old_280);
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
                    lir_subject_t* _src_1_267 = lh->farg;
                    lir_operation_t _match_op_0_268 = lh->op;
                    lir_subject_t* _keep_0_0_269 = lh->farg;
                    lir_subject_t* _keep_0_1_270 = lh->sarg;
                    lir_subject_t* _keep_0_2_271 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_272 = lh->sarg;
                    if (_old_272 != _src_1_267) {
                        lh->sarg = _src_1_267;
                        optimized = 1;
                        if (_old_272 && _old_272 != lh->farg && _old_272 != lh->sarg && _old_272 != lh->targ && _old_272 != _src_1_267 && _old_272 != _keep_0_0_269 && _old_272 != _keep_0_1_270 && _old_272 != _keep_0_2_271) {
                            LIR_unload_subject(_old_272);
                        }
                    }
                    lir_subject_t* _old_273 = lh->targ;
                    if (_old_273 != _src_1_267) {
                        lh->targ = _src_1_267;
                        optimized = 1;
                        if (_old_273 && _old_273 != lh->farg && _old_273 != lh->sarg && _old_273 != lh->targ && _old_273 != _src_1_267 && _old_273 != _keep_0_0_269 && _old_273 != _keep_0_1_270 && _old_273 != _keep_0_2_271) {
                            LIR_unload_subject(_old_273);
                        }
                    }
                    lir_subject_t* _old_274 = lh->farg;
                    if (_old_274 != _src_1_267) {
                        lh->farg = _src_1_267;
                        optimized = 1;
                        if (_old_274 && _old_274 != lh->farg && _old_274 != lh->sarg && _old_274 != lh->targ && _old_274 != _src_1_267 && _old_274 != _keep_0_0_269 && _old_274 != _keep_0_1_270 && _old_274 != _keep_0_2_271) {
                            LIR_unload_subject(_old_274);
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
                    lir_subject_t* _src_1_372 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_373 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_374 = lh->op;
                    lir_operation_t _match_op_1_375 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_376 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_377 = lh->farg;
                    lir_subject_t* _keep_0_1_378 = lh->sarg;
                    lir_subject_t* _keep_0_2_379 = lh->targ;
                    lir_subject_t* _keep_1_0_380 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_381 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_382 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_383 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_384 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_385 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_386 = lh->sarg;
                    if (_old_386 != _src_1_372) {
                        lh->sarg = _src_1_372;
                        optimized = 1;
                        if (_old_386 && _old_386 != lh->farg && _old_386 != lh->sarg && _old_386 != lh->targ && _old_386 != _src_1_372 && _old_386 != _src_2_373 && _old_386 != _keep_0_0_377 && _old_386 != _keep_0_1_378 && _old_386 != _keep_0_2_379 && _old_386 != _keep_1_0_380 && _old_386 != _keep_1_1_381 && _old_386 != _keep_1_2_382 && _old_386 != _keep_2_0_383 && _old_386 != _keep_2_1_384 && _old_386 != _keep_2_2_385) {
                            LIR_unload_subject(_old_386);
                        }
                    }
                    lir_subject_t* _old_387 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_387 && _old_387 != lh->farg && _old_387 != lh->sarg && _old_387 != lh->targ && _old_387 != _src_1_372 && _old_387 != _src_2_373 && _old_387 != _keep_0_0_377 && _old_387 != _keep_0_1_378 && _old_387 != _keep_0_2_379 && _old_387 != _keep_1_0_380 && _old_387 != _keep_1_1_381 && _old_387 != _keep_1_2_382 && _old_387 != _keep_2_0_383 && _old_387 != _keep_2_1_384 && _old_387 != _keep_2_2_385) {
                        LIR_unload_subject(_old_387);
                    }
                    lir_subject_t* _old_388 = lh->farg;
                    if (_old_388 != _src_1_372) {
                        lh->farg = _src_1_372;
                        optimized = 1;
                        if (_old_388 && _old_388 != lh->farg && _old_388 != lh->sarg && _old_388 != lh->targ && _old_388 != _src_1_372 && _old_388 != _src_2_373 && _old_388 != _keep_0_0_377 && _old_388 != _keep_0_1_378 && _old_388 != _keep_0_2_379 && _old_388 != _keep_1_0_380 && _old_388 != _keep_1_1_381 && _old_388 != _keep_1_2_382 && _old_388 != _keep_2_0_383 && _old_388 != _keep_2_1_384 && _old_388 != _keep_2_2_385) {
                            LIR_unload_subject(_old_388);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_389 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_389 != _src_2_373) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_373;
                        optimized = 1;
                        if (_old_389 && _old_389 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_389 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_389 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_389 != _src_1_372 && _old_389 != _src_2_373 && _old_389 != _keep_0_0_377 && _old_389 != _keep_0_1_378 && _old_389 != _keep_0_2_379 && _old_389 != _keep_1_0_380 && _old_389 != _keep_1_1_381 && _old_389 != _keep_1_2_382 && _old_389 != _keep_2_0_383 && _old_389 != _keep_2_1_384 && _old_389 != _keep_2_2_385) {
                            LIR_unload_subject(_old_389);
                        }
                    }
                    lir_subject_t* _old_390 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_390) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_390 && _old_390 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_390 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_390 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_390 != _src_1_372 && _old_390 != _src_2_373 && _old_390 != _keep_0_0_377 && _old_390 != _keep_0_1_378 && _old_390 != _keep_0_2_379 && _old_390 != _keep_1_0_380 && _old_390 != _keep_1_1_381 && _old_390 != _keep_1_2_382 && _old_390 != _keep_2_0_383 && _old_390 != _keep_2_1_384 && _old_390 != _keep_2_2_385) {
                            LIR_unload_subject(_old_390);
                        }
                    }
                    lir_subject_t* _old_391 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_391) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_391 && _old_391 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_391 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_391 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_391 != _src_1_372 && _old_391 != _src_2_373 && _old_391 != _keep_0_0_377 && _old_391 != _keep_0_1_378 && _old_391 != _keep_0_2_379 && _old_391 != _keep_1_0_380 && _old_391 != _keep_1_1_381 && _old_391 != _keep_1_2_382 && _old_391 != _keep_2_0_383 && _old_391 != _keep_2_1_384 && _old_391 != _keep_2_2_385) {
                            LIR_unload_subject(_old_391);
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
                    lir_subject_t* _src_1_392 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_393 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_394 = lh->op;
                    lir_operation_t _match_op_1_395 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_396 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_397 = lh->farg;
                    lir_subject_t* _keep_0_1_398 = lh->sarg;
                    lir_subject_t* _keep_0_2_399 = lh->targ;
                    lir_subject_t* _keep_1_0_400 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_401 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_402 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_403 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_404 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_405 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_406 = lh->sarg;
                    if (_old_406 != _src_1_392) {
                        lh->sarg = _src_1_392;
                        optimized = 1;
                        if (_old_406 && _old_406 != lh->farg && _old_406 != lh->sarg && _old_406 != lh->targ && _old_406 != _src_1_392 && _old_406 != _src_2_393 && _old_406 != _keep_0_0_397 && _old_406 != _keep_0_1_398 && _old_406 != _keep_0_2_399 && _old_406 != _keep_1_0_400 && _old_406 != _keep_1_1_401 && _old_406 != _keep_1_2_402 && _old_406 != _keep_2_0_403 && _old_406 != _keep_2_1_404 && _old_406 != _keep_2_2_405) {
                            LIR_unload_subject(_old_406);
                        }
                    }
                    lir_subject_t* _old_407 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_407 && _old_407 != lh->farg && _old_407 != lh->sarg && _old_407 != lh->targ && _old_407 != _src_1_392 && _old_407 != _src_2_393 && _old_407 != _keep_0_0_397 && _old_407 != _keep_0_1_398 && _old_407 != _keep_0_2_399 && _old_407 != _keep_1_0_400 && _old_407 != _keep_1_1_401 && _old_407 != _keep_1_2_402 && _old_407 != _keep_2_0_403 && _old_407 != _keep_2_1_404 && _old_407 != _keep_2_2_405) {
                        LIR_unload_subject(_old_407);
                    }
                    lir_subject_t* _old_408 = lh->farg;
                    if (_old_408 != _src_1_392) {
                        lh->farg = _src_1_392;
                        optimized = 1;
                        if (_old_408 && _old_408 != lh->farg && _old_408 != lh->sarg && _old_408 != lh->targ && _old_408 != _src_1_392 && _old_408 != _src_2_393 && _old_408 != _keep_0_0_397 && _old_408 != _keep_0_1_398 && _old_408 != _keep_0_2_399 && _old_408 != _keep_1_0_400 && _old_408 != _keep_1_1_401 && _old_408 != _keep_1_2_402 && _old_408 != _keep_2_0_403 && _old_408 != _keep_2_1_404 && _old_408 != _keep_2_2_405) {
                            LIR_unload_subject(_old_408);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_409 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_409 != _src_2_393) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_393;
                        optimized = 1;
                        if (_old_409 && _old_409 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_409 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_409 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_409 != _src_1_392 && _old_409 != _src_2_393 && _old_409 != _keep_0_0_397 && _old_409 != _keep_0_1_398 && _old_409 != _keep_0_2_399 && _old_409 != _keep_1_0_400 && _old_409 != _keep_1_1_401 && _old_409 != _keep_1_2_402 && _old_409 != _keep_2_0_403 && _old_409 != _keep_2_1_404 && _old_409 != _keep_2_2_405) {
                            LIR_unload_subject(_old_409);
                        }
                    }
                    lir_subject_t* _old_410 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_410) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_410 && _old_410 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_410 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_410 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_410 != _src_1_392 && _old_410 != _src_2_393 && _old_410 != _keep_0_0_397 && _old_410 != _keep_0_1_398 && _old_410 != _keep_0_2_399 && _old_410 != _keep_1_0_400 && _old_410 != _keep_1_1_401 && _old_410 != _keep_1_2_402 && _old_410 != _keep_2_0_403 && _old_410 != _keep_2_1_404 && _old_410 != _keep_2_2_405) {
                            LIR_unload_subject(_old_410);
                        }
                    }
                    lir_subject_t* _old_411 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_411) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_411 && _old_411 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_411 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_411 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_411 != _src_1_392 && _old_411 != _src_2_393 && _old_411 != _keep_0_0_397 && _old_411 != _keep_0_1_398 && _old_411 != _keep_0_2_399 && _old_411 != _keep_1_0_400 && _old_411 != _keep_1_1_401 && _old_411 != _keep_1_2_402 && _old_411 != _keep_2_0_403 && _old_411 != _keep_2_1_404 && _old_411 != _keep_2_2_405) {
                            LIR_unload_subject(_old_411);
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
                    lir_subject_t* _src_1_412 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_413 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_414 = lh->op;
                    lir_operation_t _match_op_1_415 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_416 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_417 = lh->farg;
                    lir_subject_t* _keep_0_1_418 = lh->sarg;
                    lir_subject_t* _keep_0_2_419 = lh->targ;
                    lir_subject_t* _keep_1_0_420 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_421 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_422 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_423 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_424 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_425 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_426 = lh->sarg;
                    if (_old_426 != _src_1_412) {
                        lh->sarg = _src_1_412;
                        optimized = 1;
                        if (_old_426 && _old_426 != lh->farg && _old_426 != lh->sarg && _old_426 != lh->targ && _old_426 != _src_1_412 && _old_426 != _src_2_413 && _old_426 != _keep_0_0_417 && _old_426 != _keep_0_1_418 && _old_426 != _keep_0_2_419 && _old_426 != _keep_1_0_420 && _old_426 != _keep_1_1_421 && _old_426 != _keep_1_2_422 && _old_426 != _keep_2_0_423 && _old_426 != _keep_2_1_424 && _old_426 != _keep_2_2_425) {
                            LIR_unload_subject(_old_426);
                        }
                    }
                    lir_subject_t* _old_427 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_427 && _old_427 != lh->farg && _old_427 != lh->sarg && _old_427 != lh->targ && _old_427 != _src_1_412 && _old_427 != _src_2_413 && _old_427 != _keep_0_0_417 && _old_427 != _keep_0_1_418 && _old_427 != _keep_0_2_419 && _old_427 != _keep_1_0_420 && _old_427 != _keep_1_1_421 && _old_427 != _keep_1_2_422 && _old_427 != _keep_2_0_423 && _old_427 != _keep_2_1_424 && _old_427 != _keep_2_2_425) {
                        LIR_unload_subject(_old_427);
                    }
                    lir_subject_t* _old_428 = lh->farg;
                    if (_old_428 != _src_1_412) {
                        lh->farg = _src_1_412;
                        optimized = 1;
                        if (_old_428 && _old_428 != lh->farg && _old_428 != lh->sarg && _old_428 != lh->targ && _old_428 != _src_1_412 && _old_428 != _src_2_413 && _old_428 != _keep_0_0_417 && _old_428 != _keep_0_1_418 && _old_428 != _keep_0_2_419 && _old_428 != _keep_1_0_420 && _old_428 != _keep_1_1_421 && _old_428 != _keep_1_2_422 && _old_428 != _keep_2_0_423 && _old_428 != _keep_2_1_424 && _old_428 != _keep_2_2_425) {
                            LIR_unload_subject(_old_428);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_429 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_429 != _src_2_413) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_413;
                        optimized = 1;
                        if (_old_429 && _old_429 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_429 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_429 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_429 != _src_1_412 && _old_429 != _src_2_413 && _old_429 != _keep_0_0_417 && _old_429 != _keep_0_1_418 && _old_429 != _keep_0_2_419 && _old_429 != _keep_1_0_420 && _old_429 != _keep_1_1_421 && _old_429 != _keep_1_2_422 && _old_429 != _keep_2_0_423 && _old_429 != _keep_2_1_424 && _old_429 != _keep_2_2_425) {
                            LIR_unload_subject(_old_429);
                        }
                    }
                    lir_subject_t* _old_430 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_430) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_430 && _old_430 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_430 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_430 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_430 != _src_1_412 && _old_430 != _src_2_413 && _old_430 != _keep_0_0_417 && _old_430 != _keep_0_1_418 && _old_430 != _keep_0_2_419 && _old_430 != _keep_1_0_420 && _old_430 != _keep_1_1_421 && _old_430 != _keep_1_2_422 && _old_430 != _keep_2_0_423 && _old_430 != _keep_2_1_424 && _old_430 != _keep_2_2_425) {
                            LIR_unload_subject(_old_430);
                        }
                    }
                    lir_subject_t* _old_431 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_431) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_431 && _old_431 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_431 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_431 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_431 != _src_1_412 && _old_431 != _src_2_413 && _old_431 != _keep_0_0_417 && _old_431 != _keep_0_1_418 && _old_431 != _keep_0_2_419 && _old_431 != _keep_1_0_420 && _old_431 != _keep_1_1_421 && _old_431 != _keep_1_2_422 && _old_431 != _keep_2_0_423 && _old_431 != _keep_2_1_424 && _old_431 != _keep_2_2_425) {
                            LIR_unload_subject(_old_431);
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
                    lir_subject_t* _src_1_432 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_433 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_434 = lh->op;
                    lir_operation_t _match_op_1_435 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_436 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_437 = lh->farg;
                    lir_subject_t* _keep_0_1_438 = lh->sarg;
                    lir_subject_t* _keep_0_2_439 = lh->targ;
                    lir_subject_t* _keep_1_0_440 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_441 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_442 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_443 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_444 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_445 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_446 = lh->sarg;
                    if (_old_446 != _src_1_432) {
                        lh->sarg = _src_1_432;
                        optimized = 1;
                        if (_old_446 && _old_446 != lh->farg && _old_446 != lh->sarg && _old_446 != lh->targ && _old_446 != _src_1_432 && _old_446 != _src_2_433 && _old_446 != _keep_0_0_437 && _old_446 != _keep_0_1_438 && _old_446 != _keep_0_2_439 && _old_446 != _keep_1_0_440 && _old_446 != _keep_1_1_441 && _old_446 != _keep_1_2_442 && _old_446 != _keep_2_0_443 && _old_446 != _keep_2_1_444 && _old_446 != _keep_2_2_445) {
                            LIR_unload_subject(_old_446);
                        }
                    }
                    lir_subject_t* _old_447 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_447 && _old_447 != lh->farg && _old_447 != lh->sarg && _old_447 != lh->targ && _old_447 != _src_1_432 && _old_447 != _src_2_433 && _old_447 != _keep_0_0_437 && _old_447 != _keep_0_1_438 && _old_447 != _keep_0_2_439 && _old_447 != _keep_1_0_440 && _old_447 != _keep_1_1_441 && _old_447 != _keep_1_2_442 && _old_447 != _keep_2_0_443 && _old_447 != _keep_2_1_444 && _old_447 != _keep_2_2_445) {
                        LIR_unload_subject(_old_447);
                    }
                    lir_subject_t* _old_448 = lh->farg;
                    if (_old_448 != _src_1_432) {
                        lh->farg = _src_1_432;
                        optimized = 1;
                        if (_old_448 && _old_448 != lh->farg && _old_448 != lh->sarg && _old_448 != lh->targ && _old_448 != _src_1_432 && _old_448 != _src_2_433 && _old_448 != _keep_0_0_437 && _old_448 != _keep_0_1_438 && _old_448 != _keep_0_2_439 && _old_448 != _keep_1_0_440 && _old_448 != _keep_1_1_441 && _old_448 != _keep_1_2_442 && _old_448 != _keep_2_0_443 && _old_448 != _keep_2_1_444 && _old_448 != _keep_2_2_445) {
                            LIR_unload_subject(_old_448);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_449 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_449 != _src_2_433) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_433;
                        optimized = 1;
                        if (_old_449 && _old_449 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_449 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_449 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_449 != _src_1_432 && _old_449 != _src_2_433 && _old_449 != _keep_0_0_437 && _old_449 != _keep_0_1_438 && _old_449 != _keep_0_2_439 && _old_449 != _keep_1_0_440 && _old_449 != _keep_1_1_441 && _old_449 != _keep_1_2_442 && _old_449 != _keep_2_0_443 && _old_449 != _keep_2_1_444 && _old_449 != _keep_2_2_445) {
                            LIR_unload_subject(_old_449);
                        }
                    }
                    lir_subject_t* _old_450 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_450) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_450 && _old_450 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_450 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_450 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_450 != _src_1_432 && _old_450 != _src_2_433 && _old_450 != _keep_0_0_437 && _old_450 != _keep_0_1_438 && _old_450 != _keep_0_2_439 && _old_450 != _keep_1_0_440 && _old_450 != _keep_1_1_441 && _old_450 != _keep_1_2_442 && _old_450 != _keep_2_0_443 && _old_450 != _keep_2_1_444 && _old_450 != _keep_2_2_445) {
                            LIR_unload_subject(_old_450);
                        }
                    }
                    lir_subject_t* _old_451 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_451) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_451 && _old_451 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_451 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_451 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_451 != _src_1_432 && _old_451 != _src_2_433 && _old_451 != _keep_0_0_437 && _old_451 != _keep_0_1_438 && _old_451 != _keep_0_2_439 && _old_451 != _keep_1_0_440 && _old_451 != _keep_1_1_441 && _old_451 != _keep_1_2_442 && _old_451 != _keep_2_0_443 && _old_451 != _keep_2_1_444 && _old_451 != _keep_2_2_445) {
                            LIR_unload_subject(_old_451);
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
                    lir_subject_t* _src_1_452 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_453 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_454 = lh->op;
                    lir_operation_t _match_op_1_455 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_456 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_457 = lh->farg;
                    lir_subject_t* _keep_0_1_458 = lh->sarg;
                    lir_subject_t* _keep_0_2_459 = lh->targ;
                    lir_subject_t* _keep_1_0_460 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_461 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_462 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_463 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_464 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_465 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_466 = lh->sarg;
                    if (_old_466 != _src_1_452) {
                        lh->sarg = _src_1_452;
                        optimized = 1;
                        if (_old_466 && _old_466 != lh->farg && _old_466 != lh->sarg && _old_466 != lh->targ && _old_466 != _src_1_452 && _old_466 != _src_2_453 && _old_466 != _keep_0_0_457 && _old_466 != _keep_0_1_458 && _old_466 != _keep_0_2_459 && _old_466 != _keep_1_0_460 && _old_466 != _keep_1_1_461 && _old_466 != _keep_1_2_462 && _old_466 != _keep_2_0_463 && _old_466 != _keep_2_1_464 && _old_466 != _keep_2_2_465) {
                            LIR_unload_subject(_old_466);
                        }
                    }
                    lir_subject_t* _old_467 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_467 && _old_467 != lh->farg && _old_467 != lh->sarg && _old_467 != lh->targ && _old_467 != _src_1_452 && _old_467 != _src_2_453 && _old_467 != _keep_0_0_457 && _old_467 != _keep_0_1_458 && _old_467 != _keep_0_2_459 && _old_467 != _keep_1_0_460 && _old_467 != _keep_1_1_461 && _old_467 != _keep_1_2_462 && _old_467 != _keep_2_0_463 && _old_467 != _keep_2_1_464 && _old_467 != _keep_2_2_465) {
                        LIR_unload_subject(_old_467);
                    }
                    lir_subject_t* _old_468 = lh->farg;
                    if (_old_468 != _src_1_452) {
                        lh->farg = _src_1_452;
                        optimized = 1;
                        if (_old_468 && _old_468 != lh->farg && _old_468 != lh->sarg && _old_468 != lh->targ && _old_468 != _src_1_452 && _old_468 != _src_2_453 && _old_468 != _keep_0_0_457 && _old_468 != _keep_0_1_458 && _old_468 != _keep_0_2_459 && _old_468 != _keep_1_0_460 && _old_468 != _keep_1_1_461 && _old_468 != _keep_1_2_462 && _old_468 != _keep_2_0_463 && _old_468 != _keep_2_1_464 && _old_468 != _keep_2_2_465) {
                            LIR_unload_subject(_old_468);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_SETE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_SETE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_469 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_469 != _src_2_453) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_453;
                        optimized = 1;
                        if (_old_469 && _old_469 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_469 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_469 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_469 != _src_1_452 && _old_469 != _src_2_453 && _old_469 != _keep_0_0_457 && _old_469 != _keep_0_1_458 && _old_469 != _keep_0_2_459 && _old_469 != _keep_1_0_460 && _old_469 != _keep_1_1_461 && _old_469 != _keep_1_2_462 && _old_469 != _keep_2_0_463 && _old_469 != _keep_2_1_464 && _old_469 != _keep_2_2_465) {
                            LIR_unload_subject(_old_469);
                        }
                    }
                    lir_subject_t* _old_470 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_470) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_470 && _old_470 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_470 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_470 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_470 != _src_1_452 && _old_470 != _src_2_453 && _old_470 != _keep_0_0_457 && _old_470 != _keep_0_1_458 && _old_470 != _keep_0_2_459 && _old_470 != _keep_1_0_460 && _old_470 != _keep_1_1_461 && _old_470 != _keep_1_2_462 && _old_470 != _keep_2_0_463 && _old_470 != _keep_2_1_464 && _old_470 != _keep_2_2_465) {
                            LIR_unload_subject(_old_470);
                        }
                    }
                    lir_subject_t* _old_471 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_471) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_471 && _old_471 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_471 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_471 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_471 != _src_1_452 && _old_471 != _src_2_453 && _old_471 != _keep_0_0_457 && _old_471 != _keep_0_1_458 && _old_471 != _keep_0_2_459 && _old_471 != _keep_1_0_460 && _old_471 != _keep_1_1_461 && _old_471 != _keep_1_2_462 && _old_471 != _keep_2_0_463 && _old_471 != _keep_2_1_464 && _old_471 != _keep_2_2_465) {
                            LIR_unload_subject(_old_471);
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
                    lir_subject_t* _src_1_472 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_473 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_474 = lh->op;
                    lir_operation_t _match_op_1_475 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_476 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_477 = lh->farg;
                    lir_subject_t* _keep_0_1_478 = lh->sarg;
                    lir_subject_t* _keep_0_2_479 = lh->targ;
                    lir_subject_t* _keep_1_0_480 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_481 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_482 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_483 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_484 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_485 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_486 = lh->sarg;
                    if (_old_486 != _src_1_472) {
                        lh->sarg = _src_1_472;
                        optimized = 1;
                        if (_old_486 && _old_486 != lh->farg && _old_486 != lh->sarg && _old_486 != lh->targ && _old_486 != _src_1_472 && _old_486 != _src_2_473 && _old_486 != _keep_0_0_477 && _old_486 != _keep_0_1_478 && _old_486 != _keep_0_2_479 && _old_486 != _keep_1_0_480 && _old_486 != _keep_1_1_481 && _old_486 != _keep_1_2_482 && _old_486 != _keep_2_0_483 && _old_486 != _keep_2_1_484 && _old_486 != _keep_2_2_485) {
                            LIR_unload_subject(_old_486);
                        }
                    }
                    lir_subject_t* _old_487 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_487 && _old_487 != lh->farg && _old_487 != lh->sarg && _old_487 != lh->targ && _old_487 != _src_1_472 && _old_487 != _src_2_473 && _old_487 != _keep_0_0_477 && _old_487 != _keep_0_1_478 && _old_487 != _keep_0_2_479 && _old_487 != _keep_1_0_480 && _old_487 != _keep_1_1_481 && _old_487 != _keep_1_2_482 && _old_487 != _keep_2_0_483 && _old_487 != _keep_2_1_484 && _old_487 != _keep_2_2_485) {
                        LIR_unload_subject(_old_487);
                    }
                    lir_subject_t* _old_488 = lh->farg;
                    if (_old_488 != _src_1_472) {
                        lh->farg = _src_1_472;
                        optimized = 1;
                        if (_old_488 && _old_488 != lh->farg && _old_488 != lh->sarg && _old_488 != lh->targ && _old_488 != _src_1_472 && _old_488 != _src_2_473 && _old_488 != _keep_0_0_477 && _old_488 != _keep_0_1_478 && _old_488 != _keep_0_2_479 && _old_488 != _keep_1_0_480 && _old_488 != _keep_1_1_481 && _old_488 != _keep_1_2_482 && _old_488 != _keep_2_0_483 && _old_488 != _keep_2_1_484 && _old_488 != _keep_2_2_485) {
                            LIR_unload_subject(_old_488);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_STNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_STNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_489 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_489 != _src_2_473) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_473;
                        optimized = 1;
                        if (_old_489 && _old_489 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_489 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_489 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_489 != _src_1_472 && _old_489 != _src_2_473 && _old_489 != _keep_0_0_477 && _old_489 != _keep_0_1_478 && _old_489 != _keep_0_2_479 && _old_489 != _keep_1_0_480 && _old_489 != _keep_1_1_481 && _old_489 != _keep_1_2_482 && _old_489 != _keep_2_0_483 && _old_489 != _keep_2_1_484 && _old_489 != _keep_2_2_485) {
                            LIR_unload_subject(_old_489);
                        }
                    }
                    lir_subject_t* _old_490 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_490) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_490 && _old_490 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_490 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_490 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_490 != _src_1_472 && _old_490 != _src_2_473 && _old_490 != _keep_0_0_477 && _old_490 != _keep_0_1_478 && _old_490 != _keep_0_2_479 && _old_490 != _keep_1_0_480 && _old_490 != _keep_1_1_481 && _old_490 != _keep_1_2_482 && _old_490 != _keep_2_0_483 && _old_490 != _keep_2_1_484 && _old_490 != _keep_2_2_485) {
                            LIR_unload_subject(_old_490);
                        }
                    }
                    lir_subject_t* _old_491 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_491) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_491 && _old_491 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_491 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_491 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_491 != _src_1_472 && _old_491 != _src_2_473 && _old_491 != _keep_0_0_477 && _old_491 != _keep_0_1_478 && _old_491 != _keep_0_2_479 && _old_491 != _keep_1_0_480 && _old_491 != _keep_1_1_481 && _old_491 != _keep_1_2_482 && _old_491 != _keep_2_0_483 && _old_491 != _keep_2_1_484 && _old_491 != _keep_2_2_485) {
                            LIR_unload_subject(_old_491);
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