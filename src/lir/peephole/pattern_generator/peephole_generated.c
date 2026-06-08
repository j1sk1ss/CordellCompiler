/* This is a generated code. Don't change it, use the main.py instead. */
#include <lir/peephole/peephole.h>

static unsigned long long _peephole_visit_counter = 100;

static int _peephole_subject_is_read(lir_block_t* lh, lir_subject_t* subj) {
    return LIR_is_readop(lh->op) && (
        LIR_subj_equals(lh->farg, subj) ||
        LIR_subj_equals(lh->sarg, subj) ||
        LIR_subj_equals(lh->targ, subj)
    );
}

static int _peephole_subject_is_overwritten(lir_block_t* lh, lir_subject_t* subj) {
    return LIR_is_movop(lh->op) &&
        LIR_subj_equals(lh->farg, subj) &&
        !LIR_subj_equals(lh->sarg, subj) &&
        !LIR_subj_equals(lh->targ, subj);
}

static int _peephole_subject_dead_after_rec(long pred, cfg_block_t* bb, lir_subject_t* subj, lir_block_t* start) {
    if (!bb) return 1;
    if (bb->visited != _peephole_visit_counter) {
        set_free(&bb->visitors);
        set_init(&bb->visitors, SET_NO_CMP);
    }

    if (set_has(&bb->visitors, (void*)pred)) return 1;
    bb->visited = _peephole_visit_counter;
    set_add(&bb->visitors, (void*)pred);

    lir_block_t* lh = start ? start : bb->lmap.entry;
    while (lh && lh != bb->lmap.exit) {
        if (!lh->unused) {
            if (_peephole_subject_is_overwritten(lh, subj)) return 1;
            if (_peephole_subject_is_read(lh, subj)) return 0;
        }
        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return _peephole_subject_dead_after_rec(bb->id, bb->l, subj, NULL) &&
           _peephole_subject_dead_after_rec(bb->id, bb->jmp, subj, NULL);
}

static int _peephole_subject_dead_after(cfg_block_t* bb, lir_subject_t* subj, lir_block_t* start) {
    if (!subj) return 1;
    _peephole_visit_counter++;
    return _peephole_subject_dead_after_rec(-1, bb, subj, start);
}

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
                    lir_subject_t* _src_1_349 = lh->farg;
                    lir_subject_t* _src_2_350 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_1_352 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_353 = lh->farg;
                    lir_subject_t* _keep_0_1_354 = lh->sarg;
                    lir_subject_t* _keep_0_2_355 = lh->targ;
                    lir_subject_t* _keep_1_0_356 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_357 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_358 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_359 = lh->farg;
                    if (_old_359 != _src_1_349) {
                        lh->farg = _src_1_349;
                        optimized = 1;
                        if (_old_359 && _old_359 != lh->farg && _old_359 != lh->sarg && _old_359 != lh->targ && _old_359 != _src_1_349 && _old_359 != _src_2_350 && _old_359 != _keep_0_0_353 && _old_359 != _keep_0_1_354 && _old_359 != _keep_0_2_355 && _old_359 != _keep_1_0_356 && _old_359 != _keep_1_1_357 && _old_359 != _keep_1_2_358) {
                            LIR_unload_subject(_old_359);
                        }
                    }
                    lir_subject_t* _old_360 = lh->sarg;
                    if (_old_360 != _src_1_349) {
                        lh->sarg = _src_1_349;
                        optimized = 1;
                        if (_old_360 && _old_360 != lh->farg && _old_360 != lh->sarg && _old_360 != lh->targ && _old_360 != _src_1_349 && _old_360 != _src_2_350 && _old_360 != _keep_0_0_353 && _old_360 != _keep_0_1_354 && _old_360 != _keep_0_2_355 && _old_360 != _keep_1_0_356 && _old_360 != _keep_1_1_357 && _old_360 != _keep_1_2_358) {
                            LIR_unload_subject(_old_360);
                        }
                    }
                    lir_subject_t* _old_361 = lh->targ;
                    if (_old_361) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_361 && _old_361 != lh->farg && _old_361 != lh->sarg && _old_361 != lh->targ && _old_361 != _src_1_349 && _old_361 != _src_2_350 && _old_361 != _keep_0_0_353 && _old_361 != _keep_0_1_354 && _old_361 != _keep_0_2_355 && _old_361 != _keep_1_0_356 && _old_361 != _keep_1_1_357 && _old_361 != _keep_1_2_358) {
                            LIR_unload_subject(_old_361);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_352) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_352;
                        optimized = 1;
                    }
                    lir_subject_t* _old_362 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_362 != _src_2_350) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_350;
                        optimized = 1;
                        if (_old_362 && _old_362 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_362 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_362 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_362 != _src_1_349 && _old_362 != _src_2_350 && _old_362 != _keep_0_0_353 && _old_362 != _keep_0_1_354 && _old_362 != _keep_0_2_355 && _old_362 != _keep_1_0_356 && _old_362 != _keep_1_1_357 && _old_362 != _keep_1_2_358) {
                            LIR_unload_subject(_old_362);
                        }
                    }
                    lir_subject_t* _old_363 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_363) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_363 && _old_363 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_363 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_363 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_363 != _src_1_349 && _old_363 != _src_2_350 && _old_363 != _keep_0_0_353 && _old_363 != _keep_0_1_354 && _old_363 != _keep_0_2_355 && _old_363 != _keep_1_0_356 && _old_363 != _keep_1_1_357 && _old_363 != _keep_1_2_358) {
                            LIR_unload_subject(_old_363);
                        }
                    }
                    lir_subject_t* _old_364 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_364) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_364 && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_364 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_364 != _src_1_349 && _old_364 != _src_2_350 && _old_364 != _keep_0_0_353 && _old_364 != _keep_0_1_354 && _old_364 != _keep_0_2_355 && _old_364 != _keep_1_0_356 && _old_364 != _keep_1_1_357 && _old_364 != _keep_1_2_358) {
                            LIR_unload_subject(_old_364);
                        }
                    }
                }
                else if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                lh->sarg &&
                ((lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->sarg) == 0))) {
                    lir_subject_t* _src_1_517 = lh->farg;
                    lir_subject_t* _keep_0_0_519 = lh->farg;
                    lir_subject_t* _keep_0_1_520 = lh->sarg;
                    lir_subject_t* _keep_0_2_521 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_522 = lh->farg;
                    if (_old_522 != _src_1_517) {
                        lh->farg = _src_1_517;
                        optimized = 1;
                        if (_old_522 && _old_522 != lh->farg && _old_522 != lh->sarg && _old_522 != lh->targ && _old_522 != _src_1_517 && _old_522 != _keep_0_0_519 && _old_522 != _keep_0_1_520 && _old_522 != _keep_0_2_521) {
                            LIR_unload_subject(_old_522);
                        }
                    }
                    lir_subject_t* _old_523 = lh->sarg;
                    if (_old_523 != _src_1_517) {
                        lh->sarg = _src_1_517;
                        optimized = 1;
                        if (_old_523 && _old_523 != lh->farg && _old_523 != lh->sarg && _old_523 != lh->targ && _old_523 != _src_1_517 && _old_523 != _keep_0_0_519 && _old_523 != _keep_0_1_520 && _old_523 != _keep_0_2_521) {
                            LIR_unload_subject(_old_523);
                        }
                    }
                    lir_subject_t* _old_524 = lh->targ;
                    if (_old_524) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_524 && _old_524 != lh->farg && _old_524 != lh->sarg && _old_524 != lh->targ && _old_524 != _src_1_517 && _old_524 != _keep_0_0_519 && _old_524 != _keep_0_1_520 && _old_524 != _keep_0_2_521) {
                            LIR_unload_subject(_old_524);
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
                _peephole_subject_dead_after(bb, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg)) {
                    lir_subject_t* _src_1_13 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_2_14 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_55 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_56 = lh->sarg;
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_113 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_114 = lh->sarg;
                    lir_subject_t* _src_3_115 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
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
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_NUMBER || LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg->t == LIR_CONSTVAL)) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2) &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op == LIR_iSUB &&
                (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ->t == LIR_REGISTER &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ, LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg)) {
                    lir_subject_t* _src_1_192 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_2_193 = lh->sarg;
                    lir_subject_t* _src_3_194 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    lir_subject_t* _src_4_195 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_196 = lh->op;
                    lir_operation_t _match_op_1_197 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_199 = lh->farg;
                    lir_subject_t* _keep_0_1_200 = lh->sarg;
                    lir_subject_t* _keep_0_2_201 = lh->targ;
                    lir_subject_t* _keep_1_0_202 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_203 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_204 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_205 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_206 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_207 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_196) {
                        lh->op = _match_op_0_196;
                        optimized = 1;
                    }
                    lir_subject_t* _old_208 = lh->farg;
                    if (_old_208 != _src_1_192) {
                        lh->farg = _src_1_192;
                        optimized = 1;
                        if (_old_208 && _old_208 != lh->farg && _old_208 != lh->sarg && _old_208 != lh->targ && _old_208 != _src_1_192 && _old_208 != _src_2_193 && _old_208 != _src_3_194 && _old_208 != _src_4_195 && _old_208 != _keep_0_0_199 && _old_208 != _keep_0_1_200 && _old_208 != _keep_0_2_201 && _old_208 != _keep_1_0_202 && _old_208 != _keep_1_1_203 && _old_208 != _keep_1_2_204 && _old_208 != _keep_2_0_205 && _old_208 != _keep_2_1_206 && _old_208 != _keep_2_2_207) {
                            LIR_unload_subject(_old_208);
                        }
                    }
                    lir_subject_t* _old_209 = lh->sarg;
                    if (_old_209 != _src_2_193) {
                        lh->sarg = _src_2_193;
                        optimized = 1;
                        if (_old_209 && _old_209 != lh->farg && _old_209 != lh->sarg && _old_209 != lh->targ && _old_209 != _src_1_192 && _old_209 != _src_2_193 && _old_209 != _src_3_194 && _old_209 != _src_4_195 && _old_209 != _keep_0_0_199 && _old_209 != _keep_0_1_200 && _old_209 != _keep_0_2_201 && _old_209 != _keep_1_0_202 && _old_209 != _keep_1_1_203 && _old_209 != _keep_1_2_204 && _old_209 != _keep_2_0_205 && _old_209 != _keep_2_1_206 && _old_209 != _keep_2_2_207) {
                            LIR_unload_subject(_old_209);
                        }
                    }
                    lir_subject_t* _old_210 = lh->targ;
                    if (_old_210) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_210 && _old_210 != lh->farg && _old_210 != lh->sarg && _old_210 != lh->targ && _old_210 != _src_1_192 && _old_210 != _src_2_193 && _old_210 != _src_3_194 && _old_210 != _src_4_195 && _old_210 != _keep_0_0_199 && _old_210 != _keep_0_1_200 && _old_210 != _keep_0_2_201 && _old_210 != _keep_1_0_202 && _old_210 != _keep_1_1_203 && _old_210 != _keep_1_2_204 && _old_210 != _keep_2_0_205 && _old_210 != _keep_2_1_206 && _old_210 != _keep_2_2_207) {
                            LIR_unload_subject(_old_210);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_197) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_197;
                        optimized = 1;
                    }
                    lir_subject_t* _old_211 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_211 != _src_3_194) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_194;
                        optimized = 1;
                        if (_old_211 && _old_211 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_211 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_211 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_211 != _src_1_192 && _old_211 != _src_2_193 && _old_211 != _src_3_194 && _old_211 != _src_4_195 && _old_211 != _keep_0_0_199 && _old_211 != _keep_0_1_200 && _old_211 != _keep_0_2_201 && _old_211 != _keep_1_0_202 && _old_211 != _keep_1_1_203 && _old_211 != _keep_1_2_204 && _old_211 != _keep_2_0_205 && _old_211 != _keep_2_1_206 && _old_211 != _keep_2_2_207) {
                            LIR_unload_subject(_old_211);
                        }
                    }
                    lir_subject_t* _old_212 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_212 != _src_4_195) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_4_195;
                        optimized = 1;
                        if (_old_212 && _old_212 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_212 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_212 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_212 != _src_1_192 && _old_212 != _src_2_193 && _old_212 != _src_3_194 && _old_212 != _src_4_195 && _old_212 != _keep_0_0_199 && _old_212 != _keep_0_1_200 && _old_212 != _keep_0_2_201 && _old_212 != _keep_1_0_202 && _old_212 != _keep_1_1_203 && _old_212 != _keep_1_2_204 && _old_212 != _keep_2_0_205 && _old_212 != _keep_2_1_206 && _old_212 != _keep_2_2_207) {
                            LIR_unload_subject(_old_212);
                        }
                    }
                    lir_subject_t* _old_213 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_213) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_213 && _old_213 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_213 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_213 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_213 != _src_1_192 && _old_213 != _src_2_193 && _old_213 != _src_3_194 && _old_213 != _src_4_195 && _old_213 != _keep_0_0_199 && _old_213 != _keep_0_1_200 && _old_213 != _keep_0_2_201 && _old_213 != _keep_1_0_202 && _old_213 != _keep_1_1_203 && _old_213 != _keep_1_2_204 && _old_213 != _keep_2_0_205 && _old_213 != _keep_2_1_206 && _old_213 != _keep_2_2_207) {
                            LIR_unload_subject(_old_213);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op != LIR_iSUB) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_214 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    if (_old_214 != _src_1_192) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg = _src_1_192;
                        optimized = 1;
                        if (_old_214 && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_214 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_214 != _src_1_192 && _old_214 != _src_2_193 && _old_214 != _src_3_194 && _old_214 != _src_4_195 && _old_214 != _keep_0_0_199 && _old_214 != _keep_0_1_200 && _old_214 != _keep_0_2_201 && _old_214 != _keep_1_0_202 && _old_214 != _keep_1_1_203 && _old_214 != _keep_1_2_204 && _old_214 != _keep_2_0_205 && _old_214 != _keep_2_1_206 && _old_214 != _keep_2_2_207) {
                            LIR_unload_subject(_old_214);
                        }
                    }
                    lir_subject_t* _old_215 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (_old_215 != _src_4_195) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ = _src_4_195;
                        optimized = 1;
                        if (_old_215 && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_215 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_215 != _src_1_192 && _old_215 != _src_2_193 && _old_215 != _src_3_194 && _old_215 != _src_4_195 && _old_215 != _keep_0_0_199 && _old_215 != _keep_0_1_200 && _old_215 != _keep_0_2_201 && _old_215 != _keep_1_0_202 && _old_215 != _keep_1_1_203 && _old_215 != _keep_1_2_204 && _old_215 != _keep_2_0_205 && _old_215 != _keep_2_1_206 && _old_215 != _keep_2_2_207) {
                            LIR_unload_subject(_old_215);
                        }
                    }
                    lir_subject_t* _old_216 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    if (_old_216 != _src_1_192) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg = _src_1_192;
                        optimized = 1;
                        if (_old_216 && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg && _old_216 != LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ && _old_216 != _src_1_192 && _old_216 != _src_2_193 && _old_216 != _src_3_194 && _old_216 != _src_4_195 && _old_216 != _keep_0_0_199 && _old_216 != _keep_0_1_200 && _old_216 != _keep_0_2_201 && _old_216 != _keep_1_0_202 && _old_216 != _keep_1_1_203 && _old_216 != _keep_1_2_204 && _old_216 != _keep_2_0_205 && _old_216 != _keep_2_1_206 && _old_216 != _keep_2_2_207) {
                            LIR_unload_subject(_old_216);
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
                    lir_subject_t* _src_1_217 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_218 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_219 = lh->op;
                    lir_subject_t* _keep_0_0_221 = lh->farg;
                    lir_subject_t* _keep_0_1_222 = lh->sarg;
                    lir_subject_t* _keep_0_2_223 = lh->targ;
                    lir_subject_t* _keep_1_0_224 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_225 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_226 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_0_219) {
                        lh->op = _match_op_0_219;
                        optimized = 1;
                    }
                    lir_subject_t* _old_227 = lh->farg;
                    if (_old_227 != _src_1_217) {
                        lh->farg = _src_1_217;
                        optimized = 1;
                        if (_old_227 && _old_227 != lh->farg && _old_227 != lh->sarg && _old_227 != lh->targ && _old_227 != _src_1_217 && _old_227 != _src_2_218 && _old_227 != _keep_0_0_221 && _old_227 != _keep_0_1_222 && _old_227 != _keep_0_2_223 && _old_227 != _keep_1_0_224 && _old_227 != _keep_1_1_225 && _old_227 != _keep_1_2_226) {
                            LIR_unload_subject(_old_227);
                        }
                    }
                    lir_subject_t* _old_228 = lh->sarg;
                    if (_old_228 != _src_2_218) {
                        lh->sarg = _src_2_218;
                        optimized = 1;
                        if (_old_228 && _old_228 != lh->farg && _old_228 != lh->sarg && _old_228 != lh->targ && _old_228 != _src_1_217 && _old_228 != _src_2_218 && _old_228 != _keep_0_0_221 && _old_228 != _keep_0_1_222 && _old_228 != _keep_0_2_223 && _old_228 != _keep_1_0_224 && _old_228 != _keep_1_1_225 && _old_228 != _keep_1_2_226) {
                            LIR_unload_subject(_old_228);
                        }
                    }
                    lir_subject_t* _old_229 = lh->targ;
                    if (_old_229) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_229 && _old_229 != lh->farg && _old_229 != lh->sarg && _old_229 != lh->targ && _old_229 != _src_1_217 && _old_229 != _src_2_218 && _old_229 != _keep_0_0_221 && _old_229 != _keep_0_1_222 && _old_229 != _keep_0_2_223 && _old_229 != _keep_1_0_224 && _old_229 != _keep_1_1_225 && _old_229 != _keep_1_2_226) {
                            LIR_unload_subject(_old_229);
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_230 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_231 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _src_3_232 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_operation_t _match_op_0_233 = lh->op;
                    lir_subject_t* _keep_0_0_236 = lh->farg;
                    lir_subject_t* _keep_0_1_237 = lh->sarg;
                    lir_subject_t* _keep_0_2_238 = lh->targ;
                    lir_subject_t* _keep_1_0_239 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_240 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_241 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_242 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_243 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_244 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_233) {
                        lh->op = _match_op_0_233;
                        optimized = 1;
                    }
                    lir_subject_t* _old_245 = lh->farg;
                    if (_old_245 != _src_1_230) {
                        lh->farg = _src_1_230;
                        optimized = 1;
                        if (_old_245 && _old_245 != lh->farg && _old_245 != lh->sarg && _old_245 != lh->targ && _old_245 != _src_1_230 && _old_245 != _src_2_231 && _old_245 != _src_3_232 && _old_245 != _keep_0_0_236 && _old_245 != _keep_0_1_237 && _old_245 != _keep_0_2_238 && _old_245 != _keep_1_0_239 && _old_245 != _keep_1_1_240 && _old_245 != _keep_1_2_241 && _old_245 != _keep_2_0_242 && _old_245 != _keep_2_1_243 && _old_245 != _keep_2_2_244) {
                            LIR_unload_subject(_old_245);
                        }
                    }
                    lir_subject_t* _old_246 = lh->sarg;
                    if (_old_246 != _src_2_231) {
                        lh->sarg = _src_2_231;
                        optimized = 1;
                        if (_old_246 && _old_246 != lh->farg && _old_246 != lh->sarg && _old_246 != lh->targ && _old_246 != _src_1_230 && _old_246 != _src_2_231 && _old_246 != _src_3_232 && _old_246 != _keep_0_0_236 && _old_246 != _keep_0_1_237 && _old_246 != _keep_0_2_238 && _old_246 != _keep_1_0_239 && _old_246 != _keep_1_1_240 && _old_246 != _keep_1_2_241 && _old_246 != _keep_2_0_242 && _old_246 != _keep_2_1_243 && _old_246 != _keep_2_2_244) {
                            LIR_unload_subject(_old_246);
                        }
                    }
                    lir_subject_t* _old_247 = lh->targ;
                    if (_old_247) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_247 && _old_247 != lh->farg && _old_247 != lh->sarg && _old_247 != lh->targ && _old_247 != _src_1_230 && _old_247 != _src_2_231 && _old_247 != _src_3_232 && _old_247 != _keep_0_0_236 && _old_247 != _keep_0_1_237 && _old_247 != _keep_0_2_238 && _old_247 != _keep_1_0_239 && _old_247 != _keep_1_1_240 && _old_247 != _keep_1_2_241 && _old_247 != _keep_2_0_242 && _old_247 != _keep_2_1_243 && _old_247 != _keep_2_2_244) {
                            LIR_unload_subject(_old_247);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iADD) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iADD;
                        optimized = 1;
                    }
                    lir_subject_t* _old_248 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_248 != _src_2_231) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_231;
                        optimized = 1;
                        if (_old_248 && _old_248 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_248 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_248 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_248 != _src_1_230 && _old_248 != _src_2_231 && _old_248 != _src_3_232 && _old_248 != _keep_0_0_236 && _old_248 != _keep_0_1_237 && _old_248 != _keep_0_2_238 && _old_248 != _keep_1_0_239 && _old_248 != _keep_1_1_240 && _old_248 != _keep_1_2_241 && _old_248 != _keep_2_0_242 && _old_248 != _keep_2_1_243 && _old_248 != _keep_2_2_244) {
                            LIR_unload_subject(_old_248);
                        }
                    }
                    lir_subject_t* _old_249 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_249 != _src_3_232) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = _src_3_232;
                        optimized = 1;
                        if (_old_249 && _old_249 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_249 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_249 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_249 != _src_1_230 && _old_249 != _src_2_231 && _old_249 != _src_3_232 && _old_249 != _keep_0_0_236 && _old_249 != _keep_0_1_237 && _old_249 != _keep_0_2_238 && _old_249 != _keep_1_0_239 && _old_249 != _keep_1_1_240 && _old_249 != _keep_1_2_241 && _old_249 != _keep_2_0_242 && _old_249 != _keep_2_1_243 && _old_249 != _keep_2_2_244) {
                            LIR_unload_subject(_old_249);
                        }
                    }
                    lir_subject_t* _old_250 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_250 != _src_2_231) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_231;
                        optimized = 1;
                        if (_old_250 && _old_250 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_250 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_250 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_250 != _src_1_230 && _old_250 != _src_2_231 && _old_250 != _src_3_232 && _old_250 != _keep_0_0_236 && _old_250 != _keep_0_1_237 && _old_250 != _keep_0_2_238 && _old_250 != _keep_1_0_239 && _old_250 != _keep_1_1_240 && _old_250 != _keep_1_2_241 && _old_250 != _keep_2_0_242 && _old_250 != _keep_2_1_243 && _old_250 != _keep_2_2_244) {
                            LIR_unload_subject(_old_250);
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                _peephole_subject_dead_after(bb, lh->sarg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_251 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_2_252 = lh->sarg;
                    lir_subject_t* _src_3_253 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_254 = lh->op;
                    lir_subject_t* _keep_0_0_257 = lh->farg;
                    lir_subject_t* _keep_0_1_258 = lh->sarg;
                    lir_subject_t* _keep_0_2_259 = lh->targ;
                    lir_subject_t* _keep_1_0_260 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_261 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_262 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_263 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_264 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_265 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_266 = lh->sarg;
                    if (_old_266 != _src_2_252) {
                        lh->sarg = _src_2_252;
                        optimized = 1;
                        if (_old_266 && _old_266 != lh->farg && _old_266 != lh->sarg && _old_266 != lh->targ && _old_266 != _src_1_251 && _old_266 != _src_2_252 && _old_266 != _src_3_253 && _old_266 != _keep_0_0_257 && _old_266 != _keep_0_1_258 && _old_266 != _keep_0_2_259 && _old_266 != _keep_1_0_260 && _old_266 != _keep_1_1_261 && _old_266 != _keep_1_2_262 && _old_266 != _keep_2_0_263 && _old_266 != _keep_2_1_264 && _old_266 != _keep_2_2_265) {
                            LIR_unload_subject(_old_266);
                        }
                    }
                    lir_subject_t* _old_267 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_267 && _old_267 != lh->farg && _old_267 != lh->sarg && _old_267 != lh->targ && _old_267 != _src_1_251 && _old_267 != _src_2_252 && _old_267 != _src_3_253 && _old_267 != _keep_0_0_257 && _old_267 != _keep_0_1_258 && _old_267 != _keep_0_2_259 && _old_267 != _keep_1_0_260 && _old_267 != _keep_1_1_261 && _old_267 != _keep_1_2_262 && _old_267 != _keep_2_0_263 && _old_267 != _keep_2_1_264 && _old_267 != _keep_2_2_265) {
                        LIR_unload_subject(_old_267);
                    }
                    lir_subject_t* _old_268 = lh->farg;
                    if (_old_268 != _src_2_252) {
                        lh->farg = _src_2_252;
                        optimized = 1;
                        if (_old_268 && _old_268 != lh->farg && _old_268 != lh->sarg && _old_268 != lh->targ && _old_268 != _src_1_251 && _old_268 != _src_2_252 && _old_268 != _src_3_253 && _old_268 != _keep_0_0_257 && _old_268 != _keep_0_1_258 && _old_268 != _keep_0_2_259 && _old_268 != _keep_1_0_260 && _old_268 != _keep_1_1_261 && _old_268 != _keep_1_2_262 && _old_268 != _keep_2_0_263 && _old_268 != _keep_2_1_264 && _old_268 != _keep_2_2_265) {
                            LIR_unload_subject(_old_268);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_0_254) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_0_254;
                        optimized = 1;
                    }
                    lir_subject_t* _old_269 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_269 != _src_3_253) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_253;
                        optimized = 1;
                        if (_old_269 && _old_269 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_269 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_269 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_269 != _src_1_251 && _old_269 != _src_2_252 && _old_269 != _src_3_253 && _old_269 != _keep_0_0_257 && _old_269 != _keep_0_1_258 && _old_269 != _keep_0_2_259 && _old_269 != _keep_1_0_260 && _old_269 != _keep_1_1_261 && _old_269 != _keep_1_2_262 && _old_269 != _keep_2_0_263 && _old_269 != _keep_2_1_264 && _old_269 != _keep_2_2_265) {
                            LIR_unload_subject(_old_269);
                        }
                    }
                    lir_subject_t* _old_270 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_270 != _src_2_252) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_2_252;
                        optimized = 1;
                        if (_old_270 && _old_270 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_270 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_270 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_270 != _src_1_251 && _old_270 != _src_2_252 && _old_270 != _src_3_253 && _old_270 != _keep_0_0_257 && _old_270 != _keep_0_1_258 && _old_270 != _keep_0_2_259 && _old_270 != _keep_1_0_260 && _old_270 != _keep_1_1_261 && _old_270 != _keep_1_2_262 && _old_270 != _keep_2_0_263 && _old_270 != _keep_2_1_264 && _old_270 != _keep_2_2_265) {
                            LIR_unload_subject(_old_270);
                        }
                    }
                    lir_subject_t* _old_271 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_271) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_271 && _old_271 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_271 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_271 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_271 != _src_1_251 && _old_271 != _src_2_252 && _old_271 != _src_3_253 && _old_271 != _keep_0_0_257 && _old_271 != _keep_0_1_258 && _old_271 != _keep_0_2_259 && _old_271 != _keep_1_0_260 && _old_271 != _keep_1_1_261 && _old_271 != _keep_1_2_262 && _old_271 != _keep_2_0_263 && _old_271 != _keep_2_1_264 && _old_271 != _keep_2_2_265) {
                            LIR_unload_subject(_old_271);
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_2_272 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _src_1_273 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_0_274 = lh->op;
                    lir_subject_t* _keep_0_0_277 = lh->farg;
                    lir_subject_t* _keep_0_1_278 = lh->sarg;
                    lir_subject_t* _keep_0_2_279 = lh->targ;
                    lir_subject_t* _keep_1_0_280 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_281 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_282 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_283 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_284 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_285 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_0_274) {
                        lh->op = _match_op_0_274;
                        optimized = 1;
                    }
                    lir_subject_t* _old_286 = lh->farg;
                    if (_old_286 != _src_2_272) {
                        lh->farg = _src_2_272;
                        optimized = 1;
                        if (_old_286 && _old_286 != lh->farg && _old_286 != lh->sarg && _old_286 != lh->targ && _old_286 != _src_2_272 && _old_286 != _src_1_273 && _old_286 != _keep_0_0_277 && _old_286 != _keep_0_1_278 && _old_286 != _keep_0_2_279 && _old_286 != _keep_1_0_280 && _old_286 != _keep_1_1_281 && _old_286 != _keep_1_2_282 && _old_286 != _keep_2_0_283 && _old_286 != _keep_2_1_284 && _old_286 != _keep_2_2_285) {
                            LIR_unload_subject(_old_286);
                        }
                    }
                    lir_subject_t* _old_287 = lh->sarg;
                    if (_old_287 != _src_1_273) {
                        lh->sarg = _src_1_273;
                        optimized = 1;
                        if (_old_287 && _old_287 != lh->farg && _old_287 != lh->sarg && _old_287 != lh->targ && _old_287 != _src_2_272 && _old_287 != _src_1_273 && _old_287 != _keep_0_0_277 && _old_287 != _keep_0_1_278 && _old_287 != _keep_0_2_279 && _old_287 != _keep_1_0_280 && _old_287 != _keep_1_1_281 && _old_287 != _keep_1_2_282 && _old_287 != _keep_2_0_283 && _old_287 != _keep_2_1_284 && _old_287 != _keep_2_2_285) {
                            LIR_unload_subject(_old_287);
                        }
                    }
                    lir_subject_t* _old_288 = lh->targ;
                    if (_old_288) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_288 && _old_288 != lh->farg && _old_288 != lh->sarg && _old_288 != lh->targ && _old_288 != _src_2_272 && _old_288 != _src_1_273 && _old_288 != _keep_0_0_277 && _old_288 != _keep_0_1_278 && _old_288 != _keep_0_2_279 && _old_288 != _keep_1_0_280 && _old_288 != _keep_1_1_281 && _old_288 != _keep_1_2_282 && _old_288 != _keep_2_0_283 && _old_288 != _keep_2_1_284 && _old_288 != _keep_2_2_285) {
                            LIR_unload_subject(_old_288);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_iSUB) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_289 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_289 != _src_1_273) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = _src_1_273;
                        optimized = 1;
                        if (_old_289 && _old_289 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_289 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_289 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_289 != _src_2_272 && _old_289 != _src_1_273 && _old_289 != _keep_0_0_277 && _old_289 != _keep_0_1_278 && _old_289 != _keep_0_2_279 && _old_289 != _keep_1_0_280 && _old_289 != _keep_1_1_281 && _old_289 != _keep_1_2_282 && _old_289 != _keep_2_0_283 && _old_289 != _keep_2_1_284 && _old_289 != _keep_2_2_285) {
                            LIR_unload_subject(_old_289);
                        }
                    }
                    lir_subject_t* _old_290 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_290 && _old_290 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_290 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_290 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_290 != _src_2_272 && _old_290 != _src_1_273 && _old_290 != _keep_0_0_277 && _old_290 != _keep_0_1_278 && _old_290 != _keep_0_2_279 && _old_290 != _keep_1_0_280 && _old_290 != _keep_1_1_281 && _old_290 != _keep_1_2_282 && _old_290 != _keep_2_0_283 && _old_290 != _keep_2_1_284 && _old_290 != _keep_2_2_285) {
                        LIR_unload_subject(_old_290);
                    }
                    lir_subject_t* _old_291 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_291 != _src_1_273) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_1_273;
                        optimized = 1;
                        if (_old_291 && _old_291 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_291 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_291 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_291 != _src_2_272 && _old_291 != _src_1_273 && _old_291 != _keep_0_0_277 && _old_291 != _keep_0_1_278 && _old_291 != _keep_0_2_279 && _old_291 != _keep_1_0_280 && _old_291 != _keep_1_1_281 && _old_291 != _keep_1_2_282 && _old_291 != _keep_2_0_283 && _old_291 != _keep_2_1_284 && _old_291 != _keep_2_2_285) {
                            LIR_unload_subject(_old_291);
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_306 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_307 = lh->sarg;
                    lir_subject_t* _src_3_308 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _src_4_309 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_1_311 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_312 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_313 = lh->farg;
                    lir_subject_t* _keep_0_1_314 = lh->sarg;
                    lir_subject_t* _keep_0_2_315 = lh->targ;
                    lir_subject_t* _keep_1_0_316 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_317 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_318 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_319 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_320 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_321 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != _match_op_1_311) {
                        lh->op = _match_op_1_311;
                        optimized = 1;
                    }
                    lir_subject_t* _old_322 = lh->farg;
                    if (_old_322 != _src_2_307) {
                        lh->farg = _src_2_307;
                        optimized = 1;
                        if (_old_322 && _old_322 != lh->farg && _old_322 != lh->sarg && _old_322 != lh->targ && _old_322 != _src_1_306 && _old_322 != _src_2_307 && _old_322 != _src_3_308 && _old_322 != _src_4_309 && _old_322 != _keep_0_0_313 && _old_322 != _keep_0_1_314 && _old_322 != _keep_0_2_315 && _old_322 != _keep_1_0_316 && _old_322 != _keep_1_1_317 && _old_322 != _keep_1_2_318 && _old_322 != _keep_2_0_319 && _old_322 != _keep_2_1_320 && _old_322 != _keep_2_2_321) {
                            LIR_unload_subject(_old_322);
                        }
                    }
                    lir_subject_t* _old_323 = lh->sarg;
                    if (_old_323 != _src_3_308) {
                        lh->sarg = _src_3_308;
                        optimized = 1;
                        if (_old_323 && _old_323 != lh->farg && _old_323 != lh->sarg && _old_323 != lh->targ && _old_323 != _src_1_306 && _old_323 != _src_2_307 && _old_323 != _src_3_308 && _old_323 != _src_4_309 && _old_323 != _keep_0_0_313 && _old_323 != _keep_0_1_314 && _old_323 != _keep_0_2_315 && _old_323 != _keep_1_0_316 && _old_323 != _keep_1_1_317 && _old_323 != _keep_1_2_318 && _old_323 != _keep_2_0_319 && _old_323 != _keep_2_1_320 && _old_323 != _keep_2_2_321) {
                            LIR_unload_subject(_old_323);
                        }
                    }
                    lir_subject_t* _old_324 = lh->targ;
                    if (_old_324) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_324 && _old_324 != lh->farg && _old_324 != lh->sarg && _old_324 != lh->targ && _old_324 != _src_1_306 && _old_324 != _src_2_307 && _old_324 != _src_3_308 && _old_324 != _src_4_309 && _old_324 != _keep_0_0_313 && _old_324 != _keep_0_1_314 && _old_324 != _keep_0_2_315 && _old_324 != _keep_1_0_316 && _old_324 != _keep_1_1_317 && _old_324 != _keep_1_2_318 && _old_324 != _keep_2_0_319 && _old_324 != _keep_2_1_320 && _old_324 != _keep_2_2_321) {
                            LIR_unload_subject(_old_324);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_312) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_312;
                        optimized = 1;
                    }
                    lir_subject_t* _old_325 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_325 != _src_4_309) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_4_309;
                        optimized = 1;
                        if (_old_325 && _old_325 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_325 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_325 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_325 != _src_1_306 && _old_325 != _src_2_307 && _old_325 != _src_3_308 && _old_325 != _src_4_309 && _old_325 != _keep_0_0_313 && _old_325 != _keep_0_1_314 && _old_325 != _keep_0_2_315 && _old_325 != _keep_1_0_316 && _old_325 != _keep_1_1_317 && _old_325 != _keep_1_2_318 && _old_325 != _keep_2_0_319 && _old_325 != _keep_2_1_320 && _old_325 != _keep_2_2_321) {
                            LIR_unload_subject(_old_325);
                        }
                    }
                    lir_subject_t* _old_326 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_326) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_326 && _old_326 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_326 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_326 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_326 != _src_1_306 && _old_326 != _src_2_307 && _old_326 != _src_3_308 && _old_326 != _src_4_309 && _old_326 != _keep_0_0_313 && _old_326 != _keep_0_1_314 && _old_326 != _keep_0_2_315 && _old_326 != _keep_1_0_316 && _old_326 != _keep_1_1_317 && _old_326 != _keep_1_2_318 && _old_326 != _keep_2_0_319 && _old_326 != _keep_2_1_320 && _old_326 != _keep_2_2_321) {
                            LIR_unload_subject(_old_326);
                        }
                    }
                    lir_subject_t* _old_327 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_327) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_327 && _old_327 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_327 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_327 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_327 != _src_1_306 && _old_327 != _src_2_307 && _old_327 != _src_3_308 && _old_327 != _src_4_309 && _old_327 != _keep_0_0_313 && _old_327 != _keep_0_1_314 && _old_327 != _keep_0_2_315 && _old_327 != _keep_1_0_316 && _old_327 != _keep_1_1_317 && _old_327 != _keep_1_2_318 && _old_327 != _keep_2_0_319 && _old_327 != _keep_2_1_320 && _old_327 != _keep_2_2_321) {
                            LIR_unload_subject(_old_327);
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
                _peephole_subject_dead_after(bb, lh->farg, LIR_get_near_instruction(lh, bb->lmap.exit, 3)) &&
                LIR_subj_equals(LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg, lh->farg)) {
                    lir_subject_t* _src_1_328 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_329 = lh->sarg;
                    lir_subject_t* _src_3_330 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_operation_t _match_op_2_333 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_334 = lh->farg;
                    lir_subject_t* _keep_0_1_335 = lh->sarg;
                    lir_subject_t* _keep_0_2_336 = lh->targ;
                    lir_subject_t* _keep_1_0_337 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_338 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_339 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_340 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_341 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_342 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_343 = lh->farg;
                    if (_old_343 != _src_2_329) {
                        lh->farg = _src_2_329;
                        optimized = 1;
                        if (_old_343 && _old_343 != lh->farg && _old_343 != lh->sarg && _old_343 != lh->targ && _old_343 != _src_1_328 && _old_343 != _src_2_329 && _old_343 != _src_3_330 && _old_343 != _keep_0_0_334 && _old_343 != _keep_0_1_335 && _old_343 != _keep_0_2_336 && _old_343 != _keep_1_0_337 && _old_343 != _keep_1_1_338 && _old_343 != _keep_1_2_339 && _old_343 != _keep_2_0_340 && _old_343 != _keep_2_1_341 && _old_343 != _keep_2_2_342) {
                            LIR_unload_subject(_old_343);
                        }
                    }
                    lir_subject_t* _old_344 = lh->sarg;
                    if (_old_344 != _src_2_329) {
                        lh->sarg = _src_2_329;
                        optimized = 1;
                        if (_old_344 && _old_344 != lh->farg && _old_344 != lh->sarg && _old_344 != lh->targ && _old_344 != _src_1_328 && _old_344 != _src_2_329 && _old_344 != _src_3_330 && _old_344 != _keep_0_0_334 && _old_344 != _keep_0_1_335 && _old_344 != _keep_0_2_336 && _old_344 != _keep_1_0_337 && _old_344 != _keep_1_1_338 && _old_344 != _keep_1_2_339 && _old_344 != _keep_2_0_340 && _old_344 != _keep_2_1_341 && _old_344 != _keep_2_2_342) {
                            LIR_unload_subject(_old_344);
                        }
                    }
                    lir_subject_t* _old_345 = lh->targ;
                    if (_old_345) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_345 && _old_345 != lh->farg && _old_345 != lh->sarg && _old_345 != lh->targ && _old_345 != _src_1_328 && _old_345 != _src_2_329 && _old_345 != _src_3_330 && _old_345 != _keep_0_0_334 && _old_345 != _keep_0_1_335 && _old_345 != _keep_0_2_336 && _old_345 != _keep_1_0_337 && _old_345 != _keep_1_1_338 && _old_345 != _keep_1_2_339 && _old_345 != _keep_2_0_340 && _old_345 != _keep_2_1_341 && _old_345 != _keep_2_2_342) {
                            LIR_unload_subject(_old_345);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_2_333) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_2_333;
                        optimized = 1;
                    }
                    lir_subject_t* _old_346 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_346 != _src_3_330) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_3_330;
                        optimized = 1;
                        if (_old_346 && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_346 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_346 != _src_1_328 && _old_346 != _src_2_329 && _old_346 != _src_3_330 && _old_346 != _keep_0_0_334 && _old_346 != _keep_0_1_335 && _old_346 != _keep_0_2_336 && _old_346 != _keep_1_0_337 && _old_346 != _keep_1_1_338 && _old_346 != _keep_1_2_339 && _old_346 != _keep_2_0_340 && _old_346 != _keep_2_1_341 && _old_346 != _keep_2_2_342) {
                            LIR_unload_subject(_old_346);
                        }
                    }
                    lir_subject_t* _old_347 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_347) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_347 && _old_347 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_347 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_347 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_347 != _src_1_328 && _old_347 != _src_2_329 && _old_347 != _src_3_330 && _old_347 != _keep_0_0_334 && _old_347 != _keep_0_1_335 && _old_347 != _keep_0_2_336 && _old_347 != _keep_1_0_337 && _old_347 != _keep_1_1_338 && _old_347 != _keep_1_2_339 && _old_347 != _keep_2_0_340 && _old_347 != _keep_2_1_341 && _old_347 != _keep_2_2_342) {
                            LIR_unload_subject(_old_347);
                        }
                    }
                    lir_subject_t* _old_348 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_348) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_348 && _old_348 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_348 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_348 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_348 != _src_1_328 && _old_348 != _src_2_329 && _old_348 != _src_3_330 && _old_348 != _keep_0_0_334 && _old_348 != _keep_0_1_335 && _old_348 != _keep_0_2_336 && _old_348 != _keep_1_0_337 && _old_348 != _keep_1_1_338 && _old_348 != _keep_1_2_339 && _old_348 != _keep_2_0_340 && _old_348 != _keep_2_1_341 && _old_348 != _keep_2_2_342) {
                            LIR_unload_subject(_old_348);
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
                    lir_subject_t* _src_1_381 = lh->farg;
                    lir_subject_t* _src_2_382 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_1_384 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_385 = lh->farg;
                    lir_subject_t* _keep_0_1_386 = lh->sarg;
                    lir_subject_t* _keep_0_2_387 = lh->targ;
                    lir_subject_t* _keep_1_0_388 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_389 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_390 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_391 = lh->farg;
                    if (_old_391 != _src_1_381) {
                        lh->farg = _src_1_381;
                        optimized = 1;
                        if (_old_391 && _old_391 != lh->farg && _old_391 != lh->sarg && _old_391 != lh->targ && _old_391 != _src_1_381 && _old_391 != _src_2_382 && _old_391 != _keep_0_0_385 && _old_391 != _keep_0_1_386 && _old_391 != _keep_0_2_387 && _old_391 != _keep_1_0_388 && _old_391 != _keep_1_1_389 && _old_391 != _keep_1_2_390) {
                            LIR_unload_subject(_old_391);
                        }
                    }
                    lir_subject_t* _old_392 = lh->sarg;
                    if (_old_392 != _src_1_381) {
                        lh->sarg = _src_1_381;
                        optimized = 1;
                        if (_old_392 && _old_392 != lh->farg && _old_392 != lh->sarg && _old_392 != lh->targ && _old_392 != _src_1_381 && _old_392 != _src_2_382 && _old_392 != _keep_0_0_385 && _old_392 != _keep_0_1_386 && _old_392 != _keep_0_2_387 && _old_392 != _keep_1_0_388 && _old_392 != _keep_1_1_389 && _old_392 != _keep_1_2_390) {
                            LIR_unload_subject(_old_392);
                        }
                    }
                    lir_subject_t* _old_393 = lh->targ;
                    if (_old_393) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_393 && _old_393 != lh->farg && _old_393 != lh->sarg && _old_393 != lh->targ && _old_393 != _src_1_381 && _old_393 != _src_2_382 && _old_393 != _keep_0_0_385 && _old_393 != _keep_0_1_386 && _old_393 != _keep_0_2_387 && _old_393 != _keep_1_0_388 && _old_393 != _keep_1_1_389 && _old_393 != _keep_1_2_390) {
                            LIR_unload_subject(_old_393);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_384) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_384;
                        optimized = 1;
                    }
                    lir_subject_t* _old_394 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_394 != _src_2_382) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_382;
                        optimized = 1;
                        if (_old_394 && _old_394 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_394 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_394 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_394 != _src_1_381 && _old_394 != _src_2_382 && _old_394 != _keep_0_0_385 && _old_394 != _keep_0_1_386 && _old_394 != _keep_0_2_387 && _old_394 != _keep_1_0_388 && _old_394 != _keep_1_1_389 && _old_394 != _keep_1_2_390) {
                            LIR_unload_subject(_old_394);
                        }
                    }
                    lir_subject_t* _old_395 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_395) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_395 && _old_395 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_395 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_395 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_395 != _src_1_381 && _old_395 != _src_2_382 && _old_395 != _keep_0_0_385 && _old_395 != _keep_0_1_386 && _old_395 != _keep_0_2_387 && _old_395 != _keep_1_0_388 && _old_395 != _keep_1_1_389 && _old_395 != _keep_1_2_390) {
                            LIR_unload_subject(_old_395);
                        }
                    }
                    lir_subject_t* _old_396 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_396) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_396 && _old_396 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_396 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_396 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_396 != _src_1_381 && _old_396 != _src_2_382 && _old_396 != _keep_0_0_385 && _old_396 != _keep_0_1_386 && _old_396 != _keep_0_2_387 && _old_396 != _keep_1_0_388 && _old_396 != _keep_1_1_389 && _old_396 != _keep_1_2_390) {
                            LIR_unload_subject(_old_396);
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
                    lir_subject_t* _src_1_533 = lh->farg;
                    lir_subject_t* _keep_0_0_535 = lh->farg;
                    lir_subject_t* _keep_0_1_536 = lh->sarg;
                    lir_subject_t* _keep_0_2_537 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_538 = lh->farg;
                    if (_old_538 != _src_1_533) {
                        lh->farg = _src_1_533;
                        optimized = 1;
                        if (_old_538 && _old_538 != lh->farg && _old_538 != lh->sarg && _old_538 != lh->targ && _old_538 != _src_1_533 && _old_538 != _keep_0_0_535 && _old_538 != _keep_0_1_536 && _old_538 != _keep_0_2_537) {
                            LIR_unload_subject(_old_538);
                        }
                    }
                    lir_subject_t* _old_539 = lh->sarg;
                    if (_old_539 != _src_1_533) {
                        lh->sarg = _src_1_533;
                        optimized = 1;
                        if (_old_539 && _old_539 != lh->farg && _old_539 != lh->sarg && _old_539 != lh->targ && _old_539 != _src_1_533 && _old_539 != _keep_0_0_535 && _old_539 != _keep_0_1_536 && _old_539 != _keep_0_2_537) {
                            LIR_unload_subject(_old_539);
                        }
                    }
                    lir_subject_t* _old_540 = lh->targ;
                    if (_old_540) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_540 && _old_540 != lh->farg && _old_540 != lh->sarg && _old_540 != lh->targ && _old_540 != _src_1_533 && _old_540 != _keep_0_0_535 && _old_540 != _keep_0_1_536 && _old_540 != _keep_0_2_537) {
                            LIR_unload_subject(_old_540);
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
                    lir_subject_t* _src_1_565 = lh->farg;
                    lir_subject_t* _keep_0_0_567 = lh->farg;
                    lir_subject_t* _keep_0_1_568 = lh->sarg;
                    lir_subject_t* _keep_0_2_569 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_570 = lh->sarg;
                    if (_old_570 != _src_1_565) {
                        lh->sarg = _src_1_565;
                        optimized = 1;
                        if (_old_570 && _old_570 != lh->farg && _old_570 != lh->sarg && _old_570 != lh->targ && _old_570 != _src_1_565 && _old_570 != _keep_0_0_567 && _old_570 != _keep_0_1_568 && _old_570 != _keep_0_2_569) {
                            LIR_unload_subject(_old_570);
                        }
                    }
                    lir_subject_t* _old_571 = lh->targ;
                    if (_old_571 != _src_1_565) {
                        lh->targ = _src_1_565;
                        optimized = 1;
                        if (_old_571 && _old_571 != lh->farg && _old_571 != lh->sarg && _old_571 != lh->targ && _old_571 != _src_1_565 && _old_571 != _keep_0_0_567 && _old_571 != _keep_0_1_568 && _old_571 != _keep_0_2_569) {
                            LIR_unload_subject(_old_571);
                        }
                    }
                    lir_subject_t* _old_572 = lh->farg;
                    if (_old_572 != _src_1_565) {
                        lh->farg = _src_1_565;
                        optimized = 1;
                        if (_old_572 && _old_572 != lh->farg && _old_572 != lh->sarg && _old_572 != lh->targ && _old_572 != _src_1_565 && _old_572 != _keep_0_0_567 && _old_572 != _keep_0_1_568 && _old_572 != _keep_0_2_569) {
                            LIR_unload_subject(_old_572);
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
                    lir_subject_t* _src_1_365 = lh->farg;
                    lir_subject_t* _src_2_366 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_1_368 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_369 = lh->farg;
                    lir_subject_t* _keep_0_1_370 = lh->sarg;
                    lir_subject_t* _keep_0_2_371 = lh->targ;
                    lir_subject_t* _keep_1_0_372 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_373 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_374 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_375 = lh->farg;
                    if (_old_375 != _src_1_365) {
                        lh->farg = _src_1_365;
                        optimized = 1;
                        if (_old_375 && _old_375 != lh->farg && _old_375 != lh->sarg && _old_375 != lh->targ && _old_375 != _src_1_365 && _old_375 != _src_2_366 && _old_375 != _keep_0_0_369 && _old_375 != _keep_0_1_370 && _old_375 != _keep_0_2_371 && _old_375 != _keep_1_0_372 && _old_375 != _keep_1_1_373 && _old_375 != _keep_1_2_374) {
                            LIR_unload_subject(_old_375);
                        }
                    }
                    lir_subject_t* _old_376 = lh->sarg;
                    if (_old_376 != _src_1_365) {
                        lh->sarg = _src_1_365;
                        optimized = 1;
                        if (_old_376 && _old_376 != lh->farg && _old_376 != lh->sarg && _old_376 != lh->targ && _old_376 != _src_1_365 && _old_376 != _src_2_366 && _old_376 != _keep_0_0_369 && _old_376 != _keep_0_1_370 && _old_376 != _keep_0_2_371 && _old_376 != _keep_1_0_372 && _old_376 != _keep_1_1_373 && _old_376 != _keep_1_2_374) {
                            LIR_unload_subject(_old_376);
                        }
                    }
                    lir_subject_t* _old_377 = lh->targ;
                    if (_old_377) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_377 && _old_377 != lh->farg && _old_377 != lh->sarg && _old_377 != lh->targ && _old_377 != _src_1_365 && _old_377 != _src_2_366 && _old_377 != _keep_0_0_369 && _old_377 != _keep_0_1_370 && _old_377 != _keep_0_2_371 && _old_377 != _keep_1_0_372 && _old_377 != _keep_1_1_373 && _old_377 != _keep_1_2_374) {
                            LIR_unload_subject(_old_377);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != _match_op_1_368) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = _match_op_1_368;
                        optimized = 1;
                    }
                    lir_subject_t* _old_378 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_378 != _src_2_366) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_366;
                        optimized = 1;
                        if (_old_378 && _old_378 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_378 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_378 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_378 != _src_1_365 && _old_378 != _src_2_366 && _old_378 != _keep_0_0_369 && _old_378 != _keep_0_1_370 && _old_378 != _keep_0_2_371 && _old_378 != _keep_1_0_372 && _old_378 != _keep_1_1_373 && _old_378 != _keep_1_2_374) {
                            LIR_unload_subject(_old_378);
                        }
                    }
                    lir_subject_t* _old_379 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_379) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_379 && _old_379 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_379 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_379 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_379 != _src_1_365 && _old_379 != _src_2_366 && _old_379 != _keep_0_0_369 && _old_379 != _keep_0_1_370 && _old_379 != _keep_0_2_371 && _old_379 != _keep_1_0_372 && _old_379 != _keep_1_1_373 && _old_379 != _keep_1_2_374) {
                            LIR_unload_subject(_old_379);
                        }
                    }
                    lir_subject_t* _old_380 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_380) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_380 && _old_380 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_380 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_380 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_380 != _src_1_365 && _old_380 != _src_2_366 && _old_380 != _keep_0_0_369 && _old_380 != _keep_0_1_370 && _old_380 != _keep_0_2_371 && _old_380 != _keep_1_0_372 && _old_380 != _keep_1_1_373 && _old_380 != _keep_1_2_374) {
                            LIR_unload_subject(_old_380);
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
                    lir_subject_t* _src_1_525 = lh->farg;
                    lir_subject_t* _keep_0_0_527 = lh->farg;
                    lir_subject_t* _keep_0_1_528 = lh->sarg;
                    lir_subject_t* _keep_0_2_529 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_530 = lh->farg;
                    if (_old_530 != _src_1_525) {
                        lh->farg = _src_1_525;
                        optimized = 1;
                        if (_old_530 && _old_530 != lh->farg && _old_530 != lh->sarg && _old_530 != lh->targ && _old_530 != _src_1_525 && _old_530 != _keep_0_0_527 && _old_530 != _keep_0_1_528 && _old_530 != _keep_0_2_529) {
                            LIR_unload_subject(_old_530);
                        }
                    }
                    lir_subject_t* _old_531 = lh->sarg;
                    if (_old_531 != _src_1_525) {
                        lh->sarg = _src_1_525;
                        optimized = 1;
                        if (_old_531 && _old_531 != lh->farg && _old_531 != lh->sarg && _old_531 != lh->targ && _old_531 != _src_1_525 && _old_531 != _keep_0_0_527 && _old_531 != _keep_0_1_528 && _old_531 != _keep_0_2_529) {
                            LIR_unload_subject(_old_531);
                        }
                    }
                    lir_subject_t* _old_532 = lh->targ;
                    if (_old_532) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_532 && _old_532 != lh->farg && _old_532 != lh->sarg && _old_532 != lh->targ && _old_532 != _src_1_525 && _old_532 != _keep_0_0_527 && _old_532 != _keep_0_1_528 && _old_532 != _keep_0_2_529) {
                            LIR_unload_subject(_old_532);
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
                    lir_subject_t* _src_1_541 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_0_0_544 = lh->farg;
                    lir_subject_t* _keep_0_1_545 = lh->sarg;
                    lir_subject_t* _keep_0_2_546 = lh->targ;
                    lir_subject_t* _keep_1_0_547 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_548 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_549 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_550 = lh->sarg;
                    if (_old_550 != _src_1_541) {
                        lh->sarg = _src_1_541;
                        optimized = 1;
                        if (_old_550 && _old_550 != lh->farg && _old_550 != lh->sarg && _old_550 != lh->targ && _old_550 != _src_1_541 && _old_550 != _keep_0_0_544 && _old_550 != _keep_0_1_545 && _old_550 != _keep_0_2_546 && _old_550 != _keep_1_0_547 && _old_550 != _keep_1_1_548 && _old_550 != _keep_1_2_549) {
                            LIR_unload_subject(_old_550);
                        }
                    }
                    lir_subject_t* _old_551 = lh->targ;
                    if (_old_551 != _src_1_541) {
                        lh->targ = _src_1_541;
                        optimized = 1;
                        if (_old_551 && _old_551 != lh->farg && _old_551 != lh->sarg && _old_551 != lh->targ && _old_551 != _src_1_541 && _old_551 != _keep_0_0_544 && _old_551 != _keep_0_1_545 && _old_551 != _keep_0_2_546 && _old_551 != _keep_1_0_547 && _old_551 != _keep_1_1_548 && _old_551 != _keep_1_2_549) {
                            LIR_unload_subject(_old_551);
                        }
                    }
                    lir_subject_t* _old_552 = lh->farg;
                    if (_old_552 != _src_1_541) {
                        lh->farg = _src_1_541;
                        optimized = 1;
                        if (_old_552 && _old_552 != lh->farg && _old_552 != lh->sarg && _old_552 != lh->targ && _old_552 != _src_1_541 && _old_552 != _keep_0_0_544 && _old_552 != _keep_0_1_545 && _old_552 != _keep_0_2_546 && _old_552 != _keep_1_0_547 && _old_552 != _keep_1_1_548 && _old_552 != _keep_1_2_549) {
                            LIR_unload_subject(_old_552);
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
                    lir_subject_t* _src_1_553 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_0_0_556 = lh->farg;
                    lir_subject_t* _keep_0_1_557 = lh->sarg;
                    lir_subject_t* _keep_0_2_558 = lh->targ;
                    lir_subject_t* _keep_1_0_559 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_560 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_561 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_562 = lh->sarg;
                    if (_old_562 != _src_1_553) {
                        lh->sarg = _src_1_553;
                        optimized = 1;
                        if (_old_562 && _old_562 != lh->farg && _old_562 != lh->sarg && _old_562 != lh->targ && _old_562 != _src_1_553 && _old_562 != _keep_0_0_556 && _old_562 != _keep_0_1_557 && _old_562 != _keep_0_2_558 && _old_562 != _keep_1_0_559 && _old_562 != _keep_1_1_560 && _old_562 != _keep_1_2_561) {
                            LIR_unload_subject(_old_562);
                        }
                    }
                    lir_subject_t* _old_563 = lh->targ;
                    if (_old_563 != _src_1_553) {
                        lh->targ = _src_1_553;
                        optimized = 1;
                        if (_old_563 && _old_563 != lh->farg && _old_563 != lh->sarg && _old_563 != lh->targ && _old_563 != _src_1_553 && _old_563 != _keep_0_0_556 && _old_563 != _keep_0_1_557 && _old_563 != _keep_0_2_558 && _old_563 != _keep_1_0_559 && _old_563 != _keep_1_1_560 && _old_563 != _keep_1_2_561) {
                            LIR_unload_subject(_old_563);
                        }
                    }
                    lir_subject_t* _old_564 = lh->farg;
                    if (_old_564 != _src_1_553) {
                        lh->farg = _src_1_553;
                        optimized = 1;
                        if (_old_564 && _old_564 != lh->farg && _old_564 != lh->sarg && _old_564 != lh->targ && _old_564 != _src_1_553 && _old_564 != _keep_0_0_556 && _old_564 != _keep_0_1_557 && _old_564 != _keep_0_2_558 && _old_564 != _keep_1_0_559 && _old_564 != _keep_1_1_560 && _old_564 != _keep_1_2_561) {
                            LIR_unload_subject(_old_564);
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
                    lir_subject_t* _src_1_573 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_0_0_576 = lh->farg;
                    lir_subject_t* _keep_0_1_577 = lh->sarg;
                    lir_subject_t* _keep_0_2_578 = lh->targ;
                    lir_subject_t* _keep_1_0_579 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_580 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_581 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_582 = lh->sarg;
                    if (_old_582 != _src_1_573) {
                        lh->sarg = _src_1_573;
                        optimized = 1;
                        if (_old_582 && _old_582 != lh->farg && _old_582 != lh->sarg && _old_582 != lh->targ && _old_582 != _src_1_573 && _old_582 != _keep_0_0_576 && _old_582 != _keep_0_1_577 && _old_582 != _keep_0_2_578 && _old_582 != _keep_1_0_579 && _old_582 != _keep_1_1_580 && _old_582 != _keep_1_2_581) {
                            LIR_unload_subject(_old_582);
                        }
                    }
                    lir_subject_t* _old_583 = lh->targ;
                    if (_old_583 != _src_1_573) {
                        lh->targ = _src_1_573;
                        optimized = 1;
                        if (_old_583 && _old_583 != lh->farg && _old_583 != lh->sarg && _old_583 != lh->targ && _old_583 != _src_1_573 && _old_583 != _keep_0_0_576 && _old_583 != _keep_0_1_577 && _old_583 != _keep_0_2_578 && _old_583 != _keep_1_0_579 && _old_583 != _keep_1_1_580 && _old_583 != _keep_1_2_581) {
                            LIR_unload_subject(_old_583);
                        }
                    }
                    lir_subject_t* _old_584 = lh->farg;
                    if (_old_584 != _src_1_573) {
                        lh->farg = _src_1_573;
                        optimized = 1;
                        if (_old_584 && _old_584 != lh->farg && _old_584 != lh->sarg && _old_584 != lh->targ && _old_584 != _src_1_573 && _old_584 != _keep_0_0_576 && _old_584 != _keep_0_1_577 && _old_584 != _keep_0_2_578 && _old_584 != _keep_1_0_579 && _old_584 != _keep_1_1_580 && _old_584 != _keep_1_2_581) {
                            LIR_unload_subject(_old_584);
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
                    lir_subject_t* _keep_0_0_301 = lh->farg;
                    lir_subject_t* _keep_0_1_302 = lh->sarg;
                    lir_subject_t* _keep_0_2_303 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_304 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_304 && _old_304 != lh->farg && _old_304 != lh->sarg && _old_304 != lh->targ && _old_304 != _keep_0_0_301 && _old_304 != _keep_0_1_302 && _old_304 != _keep_0_2_303) {
                        LIR_unload_subject(_old_304);
                    }
                    lir_subject_t* _old_305 = lh->targ;
                    if (_old_305) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_305 && _old_305 != lh->farg && _old_305 != lh->sarg && _old_305 != lh->targ && _old_305 != _keep_0_0_301 && _old_305 != _keep_0_1_302 && _old_305 != _keep_0_2_303) {
                            LIR_unload_subject(_old_305);
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
                    lir_subject_t* _src_1_292 = lh->farg;
                    lir_subject_t* _keep_0_0_294 = lh->farg;
                    lir_subject_t* _keep_0_1_295 = lh->sarg;
                    lir_subject_t* _keep_0_2_296 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_297 = lh->sarg;
                    if (_old_297 != _src_1_292) {
                        lh->sarg = _src_1_292;
                        optimized = 1;
                        if (_old_297 && _old_297 != lh->farg && _old_297 != lh->sarg && _old_297 != lh->targ && _old_297 != _src_1_292 && _old_297 != _keep_0_0_294 && _old_297 != _keep_0_1_295 && _old_297 != _keep_0_2_296) {
                            LIR_unload_subject(_old_297);
                        }
                    }
                    lir_subject_t* _old_298 = lh->targ;
                    if (_old_298 != _src_1_292) {
                        lh->targ = _src_1_292;
                        optimized = 1;
                        if (_old_298 && _old_298 != lh->farg && _old_298 != lh->sarg && _old_298 != lh->targ && _old_298 != _src_1_292 && _old_298 != _keep_0_0_294 && _old_298 != _keep_0_1_295 && _old_298 != _keep_0_2_296) {
                            LIR_unload_subject(_old_298);
                        }
                    }
                    lir_subject_t* _old_299 = lh->farg;
                    if (_old_299 != _src_1_292) {
                        lh->farg = _src_1_292;
                        optimized = 1;
                        if (_old_299 && _old_299 != lh->farg && _old_299 != lh->sarg && _old_299 != lh->targ && _old_299 != _src_1_292 && _old_299 != _keep_0_0_294 && _old_299 != _keep_0_1_295 && _old_299 != _keep_0_2_296) {
                            LIR_unload_subject(_old_299);
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
                    lir_subject_t* _src_1_397 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_398 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_402 = lh->farg;
                    lir_subject_t* _keep_0_1_403 = lh->sarg;
                    lir_subject_t* _keep_0_2_404 = lh->targ;
                    lir_subject_t* _keep_1_0_405 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_406 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_407 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_408 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_409 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_410 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_411 = lh->sarg;
                    if (_old_411 != _src_1_397) {
                        lh->sarg = _src_1_397;
                        optimized = 1;
                        if (_old_411 && _old_411 != lh->farg && _old_411 != lh->sarg && _old_411 != lh->targ && _old_411 != _src_1_397 && _old_411 != _src_2_398 && _old_411 != _keep_0_0_402 && _old_411 != _keep_0_1_403 && _old_411 != _keep_0_2_404 && _old_411 != _keep_1_0_405 && _old_411 != _keep_1_1_406 && _old_411 != _keep_1_2_407 && _old_411 != _keep_2_0_408 && _old_411 != _keep_2_1_409 && _old_411 != _keep_2_2_410) {
                            LIR_unload_subject(_old_411);
                        }
                    }
                    lir_subject_t* _old_412 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_412 && _old_412 != lh->farg && _old_412 != lh->sarg && _old_412 != lh->targ && _old_412 != _src_1_397 && _old_412 != _src_2_398 && _old_412 != _keep_0_0_402 && _old_412 != _keep_0_1_403 && _old_412 != _keep_0_2_404 && _old_412 != _keep_1_0_405 && _old_412 != _keep_1_1_406 && _old_412 != _keep_1_2_407 && _old_412 != _keep_2_0_408 && _old_412 != _keep_2_1_409 && _old_412 != _keep_2_2_410) {
                        LIR_unload_subject(_old_412);
                    }
                    lir_subject_t* _old_413 = lh->farg;
                    if (_old_413 != _src_1_397) {
                        lh->farg = _src_1_397;
                        optimized = 1;
                        if (_old_413 && _old_413 != lh->farg && _old_413 != lh->sarg && _old_413 != lh->targ && _old_413 != _src_1_397 && _old_413 != _src_2_398 && _old_413 != _keep_0_0_402 && _old_413 != _keep_0_1_403 && _old_413 != _keep_0_2_404 && _old_413 != _keep_1_0_405 && _old_413 != _keep_1_1_406 && _old_413 != _keep_1_2_407 && _old_413 != _keep_2_0_408 && _old_413 != _keep_2_1_409 && _old_413 != _keep_2_2_410) {
                            LIR_unload_subject(_old_413);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_414 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_414 != _src_2_398) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_398;
                        optimized = 1;
                        if (_old_414 && _old_414 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_414 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_414 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_414 != _src_1_397 && _old_414 != _src_2_398 && _old_414 != _keep_0_0_402 && _old_414 != _keep_0_1_403 && _old_414 != _keep_0_2_404 && _old_414 != _keep_1_0_405 && _old_414 != _keep_1_1_406 && _old_414 != _keep_1_2_407 && _old_414 != _keep_2_0_408 && _old_414 != _keep_2_1_409 && _old_414 != _keep_2_2_410) {
                            LIR_unload_subject(_old_414);
                        }
                    }
                    lir_subject_t* _old_415 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_415) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_415 && _old_415 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_415 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_415 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_415 != _src_1_397 && _old_415 != _src_2_398 && _old_415 != _keep_0_0_402 && _old_415 != _keep_0_1_403 && _old_415 != _keep_0_2_404 && _old_415 != _keep_1_0_405 && _old_415 != _keep_1_1_406 && _old_415 != _keep_1_2_407 && _old_415 != _keep_2_0_408 && _old_415 != _keep_2_1_409 && _old_415 != _keep_2_2_410) {
                            LIR_unload_subject(_old_415);
                        }
                    }
                    lir_subject_t* _old_416 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_416) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_416 && _old_416 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_416 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_416 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_416 != _src_1_397 && _old_416 != _src_2_398 && _old_416 != _keep_0_0_402 && _old_416 != _keep_0_1_403 && _old_416 != _keep_0_2_404 && _old_416 != _keep_1_0_405 && _old_416 != _keep_1_1_406 && _old_416 != _keep_1_2_407 && _old_416 != _keep_2_0_408 && _old_416 != _keep_2_1_409 && _old_416 != _keep_2_2_410) {
                            LIR_unload_subject(_old_416);
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
                    lir_subject_t* _src_1_417 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_418 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_422 = lh->farg;
                    lir_subject_t* _keep_0_1_423 = lh->sarg;
                    lir_subject_t* _keep_0_2_424 = lh->targ;
                    lir_subject_t* _keep_1_0_425 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_426 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_427 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_428 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_429 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_430 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_431 = lh->sarg;
                    if (_old_431 != _src_1_417) {
                        lh->sarg = _src_1_417;
                        optimized = 1;
                        if (_old_431 && _old_431 != lh->farg && _old_431 != lh->sarg && _old_431 != lh->targ && _old_431 != _src_1_417 && _old_431 != _src_2_418 && _old_431 != _keep_0_0_422 && _old_431 != _keep_0_1_423 && _old_431 != _keep_0_2_424 && _old_431 != _keep_1_0_425 && _old_431 != _keep_1_1_426 && _old_431 != _keep_1_2_427 && _old_431 != _keep_2_0_428 && _old_431 != _keep_2_1_429 && _old_431 != _keep_2_2_430) {
                            LIR_unload_subject(_old_431);
                        }
                    }
                    lir_subject_t* _old_432 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_432 && _old_432 != lh->farg && _old_432 != lh->sarg && _old_432 != lh->targ && _old_432 != _src_1_417 && _old_432 != _src_2_418 && _old_432 != _keep_0_0_422 && _old_432 != _keep_0_1_423 && _old_432 != _keep_0_2_424 && _old_432 != _keep_1_0_425 && _old_432 != _keep_1_1_426 && _old_432 != _keep_1_2_427 && _old_432 != _keep_2_0_428 && _old_432 != _keep_2_1_429 && _old_432 != _keep_2_2_430) {
                        LIR_unload_subject(_old_432);
                    }
                    lir_subject_t* _old_433 = lh->farg;
                    if (_old_433 != _src_1_417) {
                        lh->farg = _src_1_417;
                        optimized = 1;
                        if (_old_433 && _old_433 != lh->farg && _old_433 != lh->sarg && _old_433 != lh->targ && _old_433 != _src_1_417 && _old_433 != _src_2_418 && _old_433 != _keep_0_0_422 && _old_433 != _keep_0_1_423 && _old_433 != _keep_0_2_424 && _old_433 != _keep_1_0_425 && _old_433 != _keep_1_1_426 && _old_433 != _keep_1_2_427 && _old_433 != _keep_2_0_428 && _old_433 != _keep_2_1_429 && _old_433 != _keep_2_2_430) {
                            LIR_unload_subject(_old_433);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_434 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_434 != _src_2_418) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_418;
                        optimized = 1;
                        if (_old_434 && _old_434 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_434 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_434 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_434 != _src_1_417 && _old_434 != _src_2_418 && _old_434 != _keep_0_0_422 && _old_434 != _keep_0_1_423 && _old_434 != _keep_0_2_424 && _old_434 != _keep_1_0_425 && _old_434 != _keep_1_1_426 && _old_434 != _keep_1_2_427 && _old_434 != _keep_2_0_428 && _old_434 != _keep_2_1_429 && _old_434 != _keep_2_2_430) {
                            LIR_unload_subject(_old_434);
                        }
                    }
                    lir_subject_t* _old_435 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_435) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_435 && _old_435 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_435 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_435 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_435 != _src_1_417 && _old_435 != _src_2_418 && _old_435 != _keep_0_0_422 && _old_435 != _keep_0_1_423 && _old_435 != _keep_0_2_424 && _old_435 != _keep_1_0_425 && _old_435 != _keep_1_1_426 && _old_435 != _keep_1_2_427 && _old_435 != _keep_2_0_428 && _old_435 != _keep_2_1_429 && _old_435 != _keep_2_2_430) {
                            LIR_unload_subject(_old_435);
                        }
                    }
                    lir_subject_t* _old_436 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_436) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_436 && _old_436 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_436 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_436 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_436 != _src_1_417 && _old_436 != _src_2_418 && _old_436 != _keep_0_0_422 && _old_436 != _keep_0_1_423 && _old_436 != _keep_0_2_424 && _old_436 != _keep_1_0_425 && _old_436 != _keep_1_1_426 && _old_436 != _keep_1_2_427 && _old_436 != _keep_2_0_428 && _old_436 != _keep_2_1_429 && _old_436 != _keep_2_2_430) {
                            LIR_unload_subject(_old_436);
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
                    lir_subject_t* _src_1_437 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_438 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_442 = lh->farg;
                    lir_subject_t* _keep_0_1_443 = lh->sarg;
                    lir_subject_t* _keep_0_2_444 = lh->targ;
                    lir_subject_t* _keep_1_0_445 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_446 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_447 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_448 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_449 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_450 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_451 = lh->sarg;
                    if (_old_451 != _src_1_437) {
                        lh->sarg = _src_1_437;
                        optimized = 1;
                        if (_old_451 && _old_451 != lh->farg && _old_451 != lh->sarg && _old_451 != lh->targ && _old_451 != _src_1_437 && _old_451 != _src_2_438 && _old_451 != _keep_0_0_442 && _old_451 != _keep_0_1_443 && _old_451 != _keep_0_2_444 && _old_451 != _keep_1_0_445 && _old_451 != _keep_1_1_446 && _old_451 != _keep_1_2_447 && _old_451 != _keep_2_0_448 && _old_451 != _keep_2_1_449 && _old_451 != _keep_2_2_450) {
                            LIR_unload_subject(_old_451);
                        }
                    }
                    lir_subject_t* _old_452 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_452 && _old_452 != lh->farg && _old_452 != lh->sarg && _old_452 != lh->targ && _old_452 != _src_1_437 && _old_452 != _src_2_438 && _old_452 != _keep_0_0_442 && _old_452 != _keep_0_1_443 && _old_452 != _keep_0_2_444 && _old_452 != _keep_1_0_445 && _old_452 != _keep_1_1_446 && _old_452 != _keep_1_2_447 && _old_452 != _keep_2_0_448 && _old_452 != _keep_2_1_449 && _old_452 != _keep_2_2_450) {
                        LIR_unload_subject(_old_452);
                    }
                    lir_subject_t* _old_453 = lh->farg;
                    if (_old_453 != _src_1_437) {
                        lh->farg = _src_1_437;
                        optimized = 1;
                        if (_old_453 && _old_453 != lh->farg && _old_453 != lh->sarg && _old_453 != lh->targ && _old_453 != _src_1_437 && _old_453 != _src_2_438 && _old_453 != _keep_0_0_442 && _old_453 != _keep_0_1_443 && _old_453 != _keep_0_2_444 && _old_453 != _keep_1_0_445 && _old_453 != _keep_1_1_446 && _old_453 != _keep_1_2_447 && _old_453 != _keep_2_0_448 && _old_453 != _keep_2_1_449 && _old_453 != _keep_2_2_450) {
                            LIR_unload_subject(_old_453);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_454 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_454 != _src_2_438) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_438;
                        optimized = 1;
                        if (_old_454 && _old_454 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_454 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_454 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_454 != _src_1_437 && _old_454 != _src_2_438 && _old_454 != _keep_0_0_442 && _old_454 != _keep_0_1_443 && _old_454 != _keep_0_2_444 && _old_454 != _keep_1_0_445 && _old_454 != _keep_1_1_446 && _old_454 != _keep_1_2_447 && _old_454 != _keep_2_0_448 && _old_454 != _keep_2_1_449 && _old_454 != _keep_2_2_450) {
                            LIR_unload_subject(_old_454);
                        }
                    }
                    lir_subject_t* _old_455 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_455) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_455 && _old_455 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_455 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_455 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_455 != _src_1_437 && _old_455 != _src_2_438 && _old_455 != _keep_0_0_442 && _old_455 != _keep_0_1_443 && _old_455 != _keep_0_2_444 && _old_455 != _keep_1_0_445 && _old_455 != _keep_1_1_446 && _old_455 != _keep_1_2_447 && _old_455 != _keep_2_0_448 && _old_455 != _keep_2_1_449 && _old_455 != _keep_2_2_450) {
                            LIR_unload_subject(_old_455);
                        }
                    }
                    lir_subject_t* _old_456 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_456) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_456 && _old_456 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_456 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_456 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_456 != _src_1_437 && _old_456 != _src_2_438 && _old_456 != _keep_0_0_442 && _old_456 != _keep_0_1_443 && _old_456 != _keep_0_2_444 && _old_456 != _keep_1_0_445 && _old_456 != _keep_1_1_446 && _old_456 != _keep_1_2_447 && _old_456 != _keep_2_0_448 && _old_456 != _keep_2_1_449 && _old_456 != _keep_2_2_450) {
                            LIR_unload_subject(_old_456);
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
                    lir_subject_t* _src_1_457 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_458 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_462 = lh->farg;
                    lir_subject_t* _keep_0_1_463 = lh->sarg;
                    lir_subject_t* _keep_0_2_464 = lh->targ;
                    lir_subject_t* _keep_1_0_465 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_466 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_467 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_468 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_469 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_470 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_471 = lh->sarg;
                    if (_old_471 != _src_1_457) {
                        lh->sarg = _src_1_457;
                        optimized = 1;
                        if (_old_471 && _old_471 != lh->farg && _old_471 != lh->sarg && _old_471 != lh->targ && _old_471 != _src_1_457 && _old_471 != _src_2_458 && _old_471 != _keep_0_0_462 && _old_471 != _keep_0_1_463 && _old_471 != _keep_0_2_464 && _old_471 != _keep_1_0_465 && _old_471 != _keep_1_1_466 && _old_471 != _keep_1_2_467 && _old_471 != _keep_2_0_468 && _old_471 != _keep_2_1_469 && _old_471 != _keep_2_2_470) {
                            LIR_unload_subject(_old_471);
                        }
                    }
                    lir_subject_t* _old_472 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_472 && _old_472 != lh->farg && _old_472 != lh->sarg && _old_472 != lh->targ && _old_472 != _src_1_457 && _old_472 != _src_2_458 && _old_472 != _keep_0_0_462 && _old_472 != _keep_0_1_463 && _old_472 != _keep_0_2_464 && _old_472 != _keep_1_0_465 && _old_472 != _keep_1_1_466 && _old_472 != _keep_1_2_467 && _old_472 != _keep_2_0_468 && _old_472 != _keep_2_1_469 && _old_472 != _keep_2_2_470) {
                        LIR_unload_subject(_old_472);
                    }
                    lir_subject_t* _old_473 = lh->farg;
                    if (_old_473 != _src_1_457) {
                        lh->farg = _src_1_457;
                        optimized = 1;
                        if (_old_473 && _old_473 != lh->farg && _old_473 != lh->sarg && _old_473 != lh->targ && _old_473 != _src_1_457 && _old_473 != _src_2_458 && _old_473 != _keep_0_0_462 && _old_473 != _keep_0_1_463 && _old_473 != _keep_0_2_464 && _old_473 != _keep_1_0_465 && _old_473 != _keep_1_1_466 && _old_473 != _keep_1_2_467 && _old_473 != _keep_2_0_468 && _old_473 != _keep_2_1_469 && _old_473 != _keep_2_2_470) {
                            LIR_unload_subject(_old_473);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_JNZ) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_JNZ;
                        optimized = 1;
                    }
                    lir_subject_t* _old_474 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_474 != _src_2_458) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_458;
                        optimized = 1;
                        if (_old_474 && _old_474 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_474 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_474 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_474 != _src_1_457 && _old_474 != _src_2_458 && _old_474 != _keep_0_0_462 && _old_474 != _keep_0_1_463 && _old_474 != _keep_0_2_464 && _old_474 != _keep_1_0_465 && _old_474 != _keep_1_1_466 && _old_474 != _keep_1_2_467 && _old_474 != _keep_2_0_468 && _old_474 != _keep_2_1_469 && _old_474 != _keep_2_2_470) {
                            LIR_unload_subject(_old_474);
                        }
                    }
                    lir_subject_t* _old_475 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_475) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_475 && _old_475 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_475 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_475 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_475 != _src_1_457 && _old_475 != _src_2_458 && _old_475 != _keep_0_0_462 && _old_475 != _keep_0_1_463 && _old_475 != _keep_0_2_464 && _old_475 != _keep_1_0_465 && _old_475 != _keep_1_1_466 && _old_475 != _keep_1_2_467 && _old_475 != _keep_2_0_468 && _old_475 != _keep_2_1_469 && _old_475 != _keep_2_2_470) {
                            LIR_unload_subject(_old_475);
                        }
                    }
                    lir_subject_t* _old_476 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_476) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_476 && _old_476 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_476 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_476 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_476 != _src_1_457 && _old_476 != _src_2_458 && _old_476 != _keep_0_0_462 && _old_476 != _keep_0_1_463 && _old_476 != _keep_0_2_464 && _old_476 != _keep_1_0_465 && _old_476 != _keep_1_1_466 && _old_476 != _keep_1_2_467 && _old_476 != _keep_2_0_468 && _old_476 != _keep_2_1_469 && _old_476 != _keep_2_2_470) {
                            LIR_unload_subject(_old_476);
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
                    lir_subject_t* _src_1_477 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_478 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_482 = lh->farg;
                    lir_subject_t* _keep_0_1_483 = lh->sarg;
                    lir_subject_t* _keep_0_2_484 = lh->targ;
                    lir_subject_t* _keep_1_0_485 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_486 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_487 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_488 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_489 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_490 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_491 = lh->sarg;
                    if (_old_491 != _src_1_477) {
                        lh->sarg = _src_1_477;
                        optimized = 1;
                        if (_old_491 && _old_491 != lh->farg && _old_491 != lh->sarg && _old_491 != lh->targ && _old_491 != _src_1_477 && _old_491 != _src_2_478 && _old_491 != _keep_0_0_482 && _old_491 != _keep_0_1_483 && _old_491 != _keep_0_2_484 && _old_491 != _keep_1_0_485 && _old_491 != _keep_1_1_486 && _old_491 != _keep_1_2_487 && _old_491 != _keep_2_0_488 && _old_491 != _keep_2_1_489 && _old_491 != _keep_2_2_490) {
                            LIR_unload_subject(_old_491);
                        }
                    }
                    lir_subject_t* _old_492 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_492 && _old_492 != lh->farg && _old_492 != lh->sarg && _old_492 != lh->targ && _old_492 != _src_1_477 && _old_492 != _src_2_478 && _old_492 != _keep_0_0_482 && _old_492 != _keep_0_1_483 && _old_492 != _keep_0_2_484 && _old_492 != _keep_1_0_485 && _old_492 != _keep_1_1_486 && _old_492 != _keep_1_2_487 && _old_492 != _keep_2_0_488 && _old_492 != _keep_2_1_489 && _old_492 != _keep_2_2_490) {
                        LIR_unload_subject(_old_492);
                    }
                    lir_subject_t* _old_493 = lh->farg;
                    if (_old_493 != _src_1_477) {
                        lh->farg = _src_1_477;
                        optimized = 1;
                        if (_old_493 && _old_493 != lh->farg && _old_493 != lh->sarg && _old_493 != lh->targ && _old_493 != _src_1_477 && _old_493 != _src_2_478 && _old_493 != _keep_0_0_482 && _old_493 != _keep_0_1_483 && _old_493 != _keep_0_2_484 && _old_493 != _keep_1_0_485 && _old_493 != _keep_1_1_486 && _old_493 != _keep_1_2_487 && _old_493 != _keep_2_0_488 && _old_493 != _keep_2_1_489 && _old_493 != _keep_2_2_490) {
                            LIR_unload_subject(_old_493);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_SETE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_SETE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_494 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_494 != _src_2_478) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_478;
                        optimized = 1;
                        if (_old_494 && _old_494 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_494 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_494 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_494 != _src_1_477 && _old_494 != _src_2_478 && _old_494 != _keep_0_0_482 && _old_494 != _keep_0_1_483 && _old_494 != _keep_0_2_484 && _old_494 != _keep_1_0_485 && _old_494 != _keep_1_1_486 && _old_494 != _keep_1_2_487 && _old_494 != _keep_2_0_488 && _old_494 != _keep_2_1_489 && _old_494 != _keep_2_2_490) {
                            LIR_unload_subject(_old_494);
                        }
                    }
                    lir_subject_t* _old_495 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_495) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_495 && _old_495 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_495 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_495 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_495 != _src_1_477 && _old_495 != _src_2_478 && _old_495 != _keep_0_0_482 && _old_495 != _keep_0_1_483 && _old_495 != _keep_0_2_484 && _old_495 != _keep_1_0_485 && _old_495 != _keep_1_1_486 && _old_495 != _keep_1_2_487 && _old_495 != _keep_2_0_488 && _old_495 != _keep_2_1_489 && _old_495 != _keep_2_2_490) {
                            LIR_unload_subject(_old_495);
                        }
                    }
                    lir_subject_t* _old_496 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_496) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_496 && _old_496 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_496 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_496 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_496 != _src_1_477 && _old_496 != _src_2_478 && _old_496 != _keep_0_0_482 && _old_496 != _keep_0_1_483 && _old_496 != _keep_0_2_484 && _old_496 != _keep_1_0_485 && _old_496 != _keep_1_1_486 && _old_496 != _keep_1_2_487 && _old_496 != _keep_2_0_488 && _old_496 != _keep_2_1_489 && _old_496 != _keep_2_2_490) {
                            LIR_unload_subject(_old_496);
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
                    lir_subject_t* _src_1_497 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _src_2_498 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_0_0_502 = lh->farg;
                    lir_subject_t* _keep_0_1_503 = lh->sarg;
                    lir_subject_t* _keep_0_2_504 = lh->targ;
                    lir_subject_t* _keep_1_0_505 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_506 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_507 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_508 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_509 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_510 = LIR_get_near_instruction(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_iSUB) {
                        lh->op = LIR_iSUB;
                        optimized = 1;
                    }
                    lir_subject_t* _old_511 = lh->sarg;
                    if (_old_511 != _src_1_497) {
                        lh->sarg = _src_1_497;
                        optimized = 1;
                        if (_old_511 && _old_511 != lh->farg && _old_511 != lh->sarg && _old_511 != lh->targ && _old_511 != _src_1_497 && _old_511 != _src_2_498 && _old_511 != _keep_0_0_502 && _old_511 != _keep_0_1_503 && _old_511 != _keep_0_2_504 && _old_511 != _keep_1_0_505 && _old_511 != _keep_1_1_506 && _old_511 != _keep_1_2_507 && _old_511 != _keep_2_0_508 && _old_511 != _keep_2_1_509 && _old_511 != _keep_2_2_510) {
                            LIR_unload_subject(_old_511);
                        }
                    }
                    lir_subject_t* _old_512 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_512 && _old_512 != lh->farg && _old_512 != lh->sarg && _old_512 != lh->targ && _old_512 != _src_1_497 && _old_512 != _src_2_498 && _old_512 != _keep_0_0_502 && _old_512 != _keep_0_1_503 && _old_512 != _keep_0_2_504 && _old_512 != _keep_1_0_505 && _old_512 != _keep_1_1_506 && _old_512 != _keep_1_2_507 && _old_512 != _keep_2_0_508 && _old_512 != _keep_2_1_509 && _old_512 != _keep_2_2_510) {
                        LIR_unload_subject(_old_512);
                    }
                    lir_subject_t* _old_513 = lh->farg;
                    if (_old_513 != _src_1_497) {
                        lh->farg = _src_1_497;
                        optimized = 1;
                        if (_old_513 && _old_513 != lh->farg && _old_513 != lh->sarg && _old_513 != lh->targ && _old_513 != _src_1_497 && _old_513 != _src_2_498 && _old_513 != _keep_0_0_502 && _old_513 != _keep_0_1_503 && _old_513 != _keep_0_2_504 && _old_513 != _keep_1_0_505 && _old_513 != _keep_1_1_506 && _old_513 != _keep_1_2_507 && _old_513 != _keep_2_0_508 && _old_513 != _keep_2_1_509 && _old_513 != _keep_2_2_510) {
                            LIR_unload_subject(_old_513);
                        }
                    }
                    if (LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op != LIR_STNE) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->op = LIR_STNE;
                        optimized = 1;
                    }
                    lir_subject_t* _old_514 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg;
                    if (_old_514 != _src_2_498) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg = _src_2_498;
                        optimized = 1;
                        if (_old_514 && _old_514 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_514 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_514 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_514 != _src_1_497 && _old_514 != _src_2_498 && _old_514 != _keep_0_0_502 && _old_514 != _keep_0_1_503 && _old_514 != _keep_0_2_504 && _old_514 != _keep_1_0_505 && _old_514 != _keep_1_1_506 && _old_514 != _keep_1_2_507 && _old_514 != _keep_2_0_508 && _old_514 != _keep_2_1_509 && _old_514 != _keep_2_2_510) {
                            LIR_unload_subject(_old_514);
                        }
                    }
                    lir_subject_t* _old_515 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_515) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg = NULL;
                        optimized = 1;
                        if (_old_515 && _old_515 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_515 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_515 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_515 != _src_1_497 && _old_515 != _src_2_498 && _old_515 != _keep_0_0_502 && _old_515 != _keep_0_1_503 && _old_515 != _keep_0_2_504 && _old_515 != _keep_1_0_505 && _old_515 != _keep_1_1_506 && _old_515 != _keep_1_2_507 && _old_515 != _keep_2_0_508 && _old_515 != _keep_2_1_509 && _old_515 != _keep_2_2_510) {
                            LIR_unload_subject(_old_515);
                        }
                    }
                    lir_subject_t* _old_516 = LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ;
                    if (_old_516) {
                        LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_516 && _old_516 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->farg && _old_516 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->sarg && _old_516 != LIR_get_near_instruction(lh, bb->lmap.exit, 1)->targ && _old_516 != _src_1_497 && _old_516 != _src_2_498 && _old_516 != _keep_0_0_502 && _old_516 != _keep_0_1_503 && _old_516 != _keep_0_2_504 && _old_516 != _keep_1_0_505 && _old_516 != _keep_1_1_506 && _old_516 != _keep_1_2_507 && _old_516 != _keep_2_0_508 && _old_516 != _keep_2_1_509 && _old_516 != _keep_2_2_510) {
                            LIR_unload_subject(_old_516);
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