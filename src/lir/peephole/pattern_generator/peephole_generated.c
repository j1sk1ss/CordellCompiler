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
                    lir_subject_t* _src_1_137 = lh->farg;
                    lir_operation_t _match_op_0_138 = lh->op;
                    lir_subject_t* _keep_0_0_139 = lh->farg;
                    lir_subject_t* _keep_0_1_140 = lh->sarg;
                    lir_subject_t* _keep_0_2_141 = lh->targ;
                    if (lh->op != LIR_TST) {
                        lh->op = LIR_TST;
                        optimized = 1;
                    }
                    lir_subject_t* _old_142 = lh->farg;
                    if (_old_142 != _src_1_137) {
                        lh->farg = _src_1_137;
                        optimized = 1;
                        if (_old_142 && _old_142 != lh->farg && _old_142 != lh->sarg && _old_142 != lh->targ && _old_142 != _src_1_137 && _old_142 != _keep_0_0_139 && _old_142 != _keep_0_1_140 && _old_142 != _keep_0_2_141) {
                            LIR_unload_subject(_old_142);
                        }
                    }
                    lir_subject_t* _old_143 = lh->sarg;
                    if (_old_143 != _src_1_137) {
                        lh->sarg = _src_1_137;
                        optimized = 1;
                        if (_old_143 && _old_143 != lh->farg && _old_143 != lh->sarg && _old_143 != lh->targ && _old_143 != _src_1_137 && _old_143 != _keep_0_0_139 && _old_143 != _keep_0_1_140 && _old_143 != _keep_0_2_141) {
                            LIR_unload_subject(_old_143);
                        }
                    }
                    lir_subject_t* _old_144 = lh->targ;
                    if (_old_144) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_144 && _old_144 != lh->farg && _old_144 != lh->sarg && _old_144 != lh->targ && _old_144 != _src_1_137 && _old_144 != _keep_0_0_139 && _old_144 != _keep_0_1_140 && _old_144 != _keep_0_2_141) {
                            LIR_unload_subject(_old_144);
                        }
                    }
                }
                else if ((lh->op == LIR_iCMP || lh->op == LIR_CMP) &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
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
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_CVTSI2SD:
             {
                if (lh->op == LIR_CVTSI2SD &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_CVTSI2SS:
             {
                if (lh->op == LIR_CVTSI2SS &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_MOVSX:
             {
                if (lh->op == LIR_MOVSX &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_MOVSXD:
             {
                if (lh->op == LIR_MOVSXD &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_MOVZX:
             {
                if (lh->op == LIR_MOVZX &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
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

            case LIR_REF:
             {
                if (lh->op == LIR_REF &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
                        optimized = 1;
                    }
                }
                break;
            }

            case LIR_TST:
             {
                if (lh->op == LIR_TST &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
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
                LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_DEC &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER) &&
                LIR_get_next(lh, bb->lmap.exit, 2) &&
                (LIR_get_next(lh, bb->lmap.exit, 2)->op == LIR_iMOV || LIR_get_next(lh, bb->lmap.exit, 2)->op == LIR_fMOV || LIR_get_next(lh, bb->lmap.exit, 2)->op == LIR_aMOV) &&
                (LIR_get_next(lh, bb->lmap.exit, 2)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 2)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 2)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 2)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->farg, lh->farg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 2)->farg, lh->sarg) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 2)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_88 = lh->farg;
                    lir_subject_t* _src_2_89 = lh->sarg;
                    lir_operation_t _match_op_0_90 = lh->op;
                    lir_operation_t _match_op_1_91 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_operation_t _match_op_2_92 = LIR_get_next(lh, bb->lmap.exit, 2)->op;
                    lir_subject_t* _keep_0_0_93 = lh->farg;
                    lir_subject_t* _keep_0_1_94 = lh->sarg;
                    lir_subject_t* _keep_0_2_95 = lh->targ;
                    lir_subject_t* _keep_1_0_96 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_97 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_98 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    lir_subject_t* _keep_2_0_99 = LIR_get_next(lh, bb->lmap.exit, 2)->farg;
                    lir_subject_t* _keep_2_1_100 = LIR_get_next(lh, bb->lmap.exit, 2)->sarg;
                    lir_subject_t* _keep_2_2_101 = LIR_get_next(lh, bb->lmap.exit, 2)->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_102 = lh->farg;
                    if (_old_102 != _src_2_89) {
                        lh->farg = _src_2_89;
                        optimized = 1;
                        if (_old_102 && _old_102 != lh->farg && _old_102 != lh->sarg && _old_102 != lh->targ && _old_102 != _src_1_88 && _old_102 != _src_2_89 && _old_102 != _keep_0_0_93 && _old_102 != _keep_0_1_94 && _old_102 != _keep_0_2_95 && _old_102 != _keep_1_0_96 && _old_102 != _keep_1_1_97 && _old_102 != _keep_1_2_98 && _old_102 != _keep_2_0_99 && _old_102 != _keep_2_1_100 && _old_102 != _keep_2_2_101) {
                            LIR_unload_subject(_old_102);
                        }
                    }
                    lir_subject_t* _old_103 = lh->sarg;
                    if (_old_103) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_103 && _old_103 != lh->farg && _old_103 != lh->sarg && _old_103 != lh->targ && _old_103 != _src_1_88 && _old_103 != _src_2_89 && _old_103 != _keep_0_0_93 && _old_103 != _keep_0_1_94 && _old_103 != _keep_0_2_95 && _old_103 != _keep_1_0_96 && _old_103 != _keep_1_1_97 && _old_103 != _keep_1_2_98 && _old_103 != _keep_2_0_99 && _old_103 != _keep_2_1_100 && _old_103 != _keep_2_2_101) {
                            LIR_unload_subject(_old_103);
                        }
                    }
                    lir_subject_t* _old_104 = lh->targ;
                    if (_old_104) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_104 && _old_104 != lh->farg && _old_104 != lh->sarg && _old_104 != lh->targ && _old_104 != _src_1_88 && _old_104 != _src_2_89 && _old_104 != _keep_0_0_93 && _old_104 != _keep_0_1_94 && _old_104 != _keep_0_2_95 && _old_104 != _keep_1_0_96 && _old_104 != _keep_1_1_97 && _old_104 != _keep_1_2_98 && _old_104 != _keep_2_0_99 && _old_104 != _keep_2_1_100 && _old_104 != _keep_2_2_101) {
                            LIR_unload_subject(_old_104);
                        }
                    }
                    if (LIR_get_next(lh, bb->lmap.exit, 1)->op != _match_op_0_90) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->op = _match_op_0_90;
                        optimized = 1;
                    }
                    lir_subject_t* _old_105 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    if (_old_105 != _src_1_88) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->farg = _src_1_88;
                        optimized = 1;
                        if (_old_105 && _old_105 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_105 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_105 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_105 != _src_1_88 && _old_105 != _src_2_89 && _old_105 != _keep_0_0_93 && _old_105 != _keep_0_1_94 && _old_105 != _keep_0_2_95 && _old_105 != _keep_1_0_96 && _old_105 != _keep_1_1_97 && _old_105 != _keep_1_2_98 && _old_105 != _keep_2_0_99 && _old_105 != _keep_2_1_100 && _old_105 != _keep_2_2_101) {
                            LIR_unload_subject(_old_105);
                        }
                    }
                    lir_subject_t* _old_106 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_106 != _src_2_89) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->sarg = _src_2_89;
                        optimized = 1;
                        if (_old_106 && _old_106 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_106 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_106 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_106 != _src_1_88 && _old_106 != _src_2_89 && _old_106 != _keep_0_0_93 && _old_106 != _keep_0_1_94 && _old_106 != _keep_0_2_95 && _old_106 != _keep_1_0_96 && _old_106 != _keep_1_1_97 && _old_106 != _keep_1_2_98 && _old_106 != _keep_2_0_99 && _old_106 != _keep_2_1_100 && _old_106 != _keep_2_2_101) {
                            LIR_unload_subject(_old_106);
                        }
                    }
                    lir_subject_t* _old_107 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (_old_107) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->targ = NULL;
                        optimized = 1;
                        if (_old_107 && _old_107 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_107 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_107 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_107 != _src_1_88 && _old_107 != _src_2_89 && _old_107 != _keep_0_0_93 && _old_107 != _keep_0_1_94 && _old_107 != _keep_0_2_95 && _old_107 != _keep_1_0_96 && _old_107 != _keep_1_1_97 && _old_107 != _keep_1_2_98 && _old_107 != _keep_2_0_99 && _old_107 != _keep_2_1_100 && _old_107 != _keep_2_2_101) {
                            LIR_unload_subject(_old_107);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 2)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 2)->unused = 1;
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
                    lir_subject_t* _src_1_123 = lh->farg;
                    lir_subject_t* _src_2_124 = lh->sarg;
                    lir_subject_t* _src_3_125 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_operation_t _match_op_0_126 = lh->op;
                    lir_operation_t _match_op_1_127 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_128 = lh->farg;
                    lir_subject_t* _keep_0_1_129 = lh->sarg;
                    lir_subject_t* _keep_0_2_130 = lh->targ;
                    lir_subject_t* _keep_1_0_131 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_132 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_133 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != _match_op_1_127) {
                        lh->op = _match_op_1_127;
                        optimized = 1;
                    }
                    lir_subject_t* _old_134 = lh->farg;
                    if (_old_134 != _src_2_124) {
                        lh->farg = _src_2_124;
                        optimized = 1;
                        if (_old_134 && _old_134 != lh->farg && _old_134 != lh->sarg && _old_134 != lh->targ && _old_134 != _src_1_123 && _old_134 != _src_2_124 && _old_134 != _src_3_125 && _old_134 != _keep_0_0_128 && _old_134 != _keep_0_1_129 && _old_134 != _keep_0_2_130 && _old_134 != _keep_1_0_131 && _old_134 != _keep_1_1_132 && _old_134 != _keep_1_2_133) {
                            LIR_unload_subject(_old_134);
                        }
                    }
                    lir_subject_t* _old_135 = lh->sarg;
                    if (_old_135 != _src_3_125) {
                        lh->sarg = _src_3_125;
                        optimized = 1;
                        if (_old_135 && _old_135 != lh->farg && _old_135 != lh->sarg && _old_135 != lh->targ && _old_135 != _src_1_123 && _old_135 != _src_2_124 && _old_135 != _src_3_125 && _old_135 != _keep_0_0_128 && _old_135 != _keep_0_1_129 && _old_135 != _keep_0_2_130 && _old_135 != _keep_1_0_131 && _old_135 != _keep_1_1_132 && _old_135 != _keep_1_2_133) {
                            LIR_unload_subject(_old_135);
                        }
                    }
                    lir_subject_t* _old_136 = lh->targ;
                    if (_old_136) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_136 && _old_136 != lh->farg && _old_136 != lh->sarg && _old_136 != lh->targ && _old_136 != _src_1_123 && _old_136 != _src_2_124 && _old_136 != _src_3_125 && _old_136 != _keep_0_0_128 && _old_136 != _keep_0_1_129 && _old_136 != _keep_0_2_130 && _old_136 != _keep_1_0_131 && _old_136 != _keep_1_1_132 && _old_136 != _keep_1_2_133) {
                            LIR_unload_subject(_old_136);
                        }
                    }
                    if (!LIR_get_next(lh, bb->lmap.exit, 1)->unused) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->unused = 1;
                        optimized = 1;
                    }
                }
                else if ((lh->op == LIR_iMOV || lh->op == LIR_fMOV || lh->op == LIR_aMOV) &&
                (lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL))) {
                    if (!lh->unused) {
                        lh->unused = 1;
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
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_145 = lh->farg;
                    lir_operation_t _match_op_0_146 = lh->op;
                    lir_subject_t* _keep_0_0_147 = lh->farg;
                    lir_subject_t* _keep_0_1_148 = lh->sarg;
                    lir_subject_t* _keep_0_2_149 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_150 = lh->sarg;
                    if (_old_150 != _src_1_145) {
                        lh->sarg = _src_1_145;
                        optimized = 1;
                        if (_old_150 && _old_150 != lh->farg && _old_150 != lh->sarg && _old_150 != lh->targ && _old_150 != _src_1_145 && _old_150 != _keep_0_0_147 && _old_150 != _keep_0_1_148 && _old_150 != _keep_0_2_149) {
                            LIR_unload_subject(_old_150);
                        }
                    }
                    lir_subject_t* _old_151 = lh->targ;
                    if (_old_151 != _src_1_145) {
                        lh->targ = _src_1_145;
                        optimized = 1;
                        if (_old_151 && _old_151 != lh->farg && _old_151 != lh->sarg && _old_151 != lh->targ && _old_151 != _src_1_145 && _old_151 != _keep_0_0_147 && _old_151 != _keep_0_1_148 && _old_151 != _keep_0_2_149) {
                            LIR_unload_subject(_old_151);
                        }
                    }
                    lir_subject_t* _old_152 = lh->farg;
                    if (_old_152 != _src_1_145) {
                        lh->farg = _src_1_145;
                        optimized = 1;
                        if (_old_152 && _old_152 != lh->farg && _old_152 != lh->sarg && _old_152 != lh->targ && _old_152 != _src_1_145 && _old_152 != _keep_0_0_147 && _old_152 != _keep_0_1_148 && _old_152 != _keep_0_2_149) {
                            LIR_unload_subject(_old_152);
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
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 0) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
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
                LIR_subj_equals(lh->farg, lh->sarg)) &&
                LIR_get_next(lh, bb->lmap.exit, 1) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_iMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_fMOV || LIR_get_next(lh, bb->lmap.exit, 1)->op == LIR_aMOV) &&
                (LIR_get_next(lh, bb->lmap.exit, 1)->farg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->farg->t == LIR_REGISTER &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg &&
                LIR_get_next(lh, bb->lmap.exit, 1)->sarg->t == LIR_REGISTER) &&
                LIR_subj_equals(LIR_get_next(lh, bb->lmap.exit, 1)->sarg, lh->farg)) {
                    lir_subject_t* _src_1_22 = lh->farg;
                    lir_subject_t* _src_2_23 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_operation_t _match_op_0_24 = lh->op;
                    lir_operation_t _match_op_1_25 = LIR_get_next(lh, bb->lmap.exit, 1)->op;
                    lir_subject_t* _keep_0_0_26 = lh->farg;
                    lir_subject_t* _keep_0_1_27 = lh->sarg;
                    lir_subject_t* _keep_0_2_28 = lh->targ;
                    lir_subject_t* _keep_1_0_29 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    lir_subject_t* _keep_1_1_30 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    lir_subject_t* _keep_1_2_31 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_32 = lh->sarg;
                    if (_old_32 != _src_1_22) {
                        lh->sarg = _src_1_22;
                        optimized = 1;
                        if (_old_32 && _old_32 != lh->farg && _old_32 != lh->sarg && _old_32 != lh->targ && _old_32 != _src_1_22 && _old_32 != _src_2_23 && _old_32 != _keep_0_0_26 && _old_32 != _keep_0_1_27 && _old_32 != _keep_0_2_28 && _old_32 != _keep_1_0_29 && _old_32 != _keep_1_1_30 && _old_32 != _keep_1_2_31) {
                            LIR_unload_subject(_old_32);
                        }
                    }
                    lir_subject_t* _old_33 = lh->targ;
                    if (_old_33 != _src_1_22) {
                        lh->targ = _src_1_22;
                        optimized = 1;
                        if (_old_33 && _old_33 != lh->farg && _old_33 != lh->sarg && _old_33 != lh->targ && _old_33 != _src_1_22 && _old_33 != _src_2_23 && _old_33 != _keep_0_0_26 && _old_33 != _keep_0_1_27 && _old_33 != _keep_0_2_28 && _old_33 != _keep_1_0_29 && _old_33 != _keep_1_1_30 && _old_33 != _keep_1_2_31) {
                            LIR_unload_subject(_old_33);
                        }
                    }
                    lir_subject_t* _old_34 = lh->farg;
                    if (_old_34 != _src_1_22) {
                        lh->farg = _src_1_22;
                        optimized = 1;
                        if (_old_34 && _old_34 != lh->farg && _old_34 != lh->sarg && _old_34 != lh->targ && _old_34 != _src_1_22 && _old_34 != _src_2_23 && _old_34 != _keep_0_0_26 && _old_34 != _keep_0_1_27 && _old_34 != _keep_0_2_28 && _old_34 != _keep_1_0_29 && _old_34 != _keep_1_1_30 && _old_34 != _keep_1_2_31) {
                            LIR_unload_subject(_old_34);
                        }
                    }
                    if (LIR_get_next(lh, bb->lmap.exit, 1)->op != LIR_bXOR) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_35 = LIR_get_next(lh, bb->lmap.exit, 1)->sarg;
                    if (_old_35 != _src_2_23) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->sarg = _src_2_23;
                        optimized = 1;
                        if (_old_35 && _old_35 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_35 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_35 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_35 != _src_1_22 && _old_35 != _src_2_23 && _old_35 != _keep_0_0_26 && _old_35 != _keep_0_1_27 && _old_35 != _keep_0_2_28 && _old_35 != _keep_1_0_29 && _old_35 != _keep_1_1_30 && _old_35 != _keep_1_2_31) {
                            LIR_unload_subject(_old_35);
                        }
                    }
                    lir_subject_t* _old_36 = LIR_get_next(lh, bb->lmap.exit, 1)->targ;
                    if (_old_36 != _src_2_23) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->targ = _src_2_23;
                        optimized = 1;
                        if (_old_36 && _old_36 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_36 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_36 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_36 != _src_1_22 && _old_36 != _src_2_23 && _old_36 != _keep_0_0_26 && _old_36 != _keep_0_1_27 && _old_36 != _keep_0_2_28 && _old_36 != _keep_1_0_29 && _old_36 != _keep_1_1_30 && _old_36 != _keep_1_2_31) {
                            LIR_unload_subject(_old_36);
                        }
                    }
                    lir_subject_t* _old_37 = LIR_get_next(lh, bb->lmap.exit, 1)->farg;
                    if (_old_37 != _src_2_23) {
                        LIR_get_next(lh, bb->lmap.exit, 1)->farg = _src_2_23;
                        optimized = 1;
                        if (_old_37 && _old_37 != LIR_get_next(lh, bb->lmap.exit, 1)->farg && _old_37 != LIR_get_next(lh, bb->lmap.exit, 1)->sarg && _old_37 != LIR_get_next(lh, bb->lmap.exit, 1)->targ && _old_37 != _src_1_22 && _old_37 != _src_2_23 && _old_37 != _keep_0_0_26 && _old_37 != _keep_0_1_27 && _old_37 != _keep_0_2_28 && _old_37 != _keep_1_0_29 && _old_37 != _keep_1_1_30 && _old_37 != _keep_1_2_31) {
                            LIR_unload_subject(_old_37);
                        }
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
                else if (lh->op == LIR_iADD &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_72 = lh->farg;
                    lir_operation_t _match_op_0_73 = lh->op;
                    lir_subject_t* _keep_0_0_74 = lh->farg;
                    lir_subject_t* _keep_0_1_75 = lh->sarg;
                    lir_subject_t* _keep_0_2_76 = lh->targ;
                    if (lh->op != LIR_INC) {
                        lh->op = LIR_INC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_77 = lh->sarg;
                    if (_old_77) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_77 && _old_77 != lh->farg && _old_77 != lh->sarg && _old_77 != lh->targ && _old_77 != _src_1_72 && _old_77 != _keep_0_0_74 && _old_77 != _keep_0_1_75 && _old_77 != _keep_0_2_76) {
                            LIR_unload_subject(_old_77);
                        }
                    }
                    lir_subject_t* _old_78 = lh->targ;
                    if (_old_78) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_78 && _old_78 != lh->farg && _old_78 != lh->sarg && _old_78 != lh->targ && _old_78 != _src_1_72 && _old_78 != _keep_0_0_74 && _old_78 != _keep_0_1_75 && _old_78 != _keep_0_2_76) {
                            LIR_unload_subject(_old_78);
                        }
                    }
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
                    lir_subject_t* _src_1_79 = lh->farg;
                    lir_subject_t* _src_2_80 = lh->targ;
                    lir_operation_t _match_op_0_81 = lh->op;
                    lir_subject_t* _keep_0_0_82 = lh->farg;
                    lir_subject_t* _keep_0_1_83 = lh->sarg;
                    lir_subject_t* _keep_0_2_84 = lh->targ;
                    if (lh->op != LIR_bSHL) {
                        lh->op = LIR_bSHL;
                        optimized = 1;
                    }
                    lir_subject_t* _old_85 = lh->sarg;
                    if (_old_85 != _src_1_79) {
                        lh->sarg = _src_1_79;
                        optimized = 1;
                        if (_old_85 && _old_85 != lh->farg && _old_85 != lh->sarg && _old_85 != lh->targ && _old_85 != _src_1_79 && _old_85 != _src_2_80 && _old_85 != _keep_0_0_82 && _old_85 != _keep_0_1_83 && _old_85 != _keep_0_2_84) {
                            LIR_unload_subject(_old_85);
                        }
                    }
                    lir_subject_t* _old_86 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(1);
                    optimized = 1;
                    if (_old_86 && _old_86 != lh->farg && _old_86 != lh->sarg && _old_86 != lh->targ && _old_86 != _src_1_79 && _old_86 != _src_2_80 && _old_86 != _keep_0_0_82 && _old_86 != _keep_0_1_83 && _old_86 != _keep_0_2_84) {
                        LIR_unload_subject(_old_86);
                    }
                    lir_subject_t* _old_87 = lh->farg;
                    if (_old_87 != _src_1_79) {
                        lh->farg = _src_1_79;
                        optimized = 1;
                        if (_old_87 && _old_87 != lh->farg && _old_87 != lh->sarg && _old_87 != lh->targ && _old_87 != _src_1_79 && _old_87 != _src_2_80 && _old_87 != _keep_0_0_82 && _old_87 != _keep_0_1_83 && _old_87 != _keep_0_2_84) {
                            LIR_unload_subject(_old_87);
                        }
                    }
                }
                else if (lh->op == LIR_iADD &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
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
                    lir_subject_t* _src_1_38 = lh->farg;
                    lir_subject_t* _src_2_39 = lh->targ;
                    lir_operation_t _match_op_0_40 = lh->op;
                    lir_subject_t* _keep_0_0_41 = lh->farg;
                    lir_subject_t* _keep_0_1_42 = lh->sarg;
                    lir_subject_t* _keep_0_2_43 = lh->targ;
                    if (lh->op != LIR_bSHR) {
                        lh->op = LIR_bSHR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_44 = lh->sarg;
                    if (_old_44 != _src_1_38) {
                        lh->sarg = _src_1_38;
                        optimized = 1;
                        if (_old_44 && _old_44 != lh->farg && _old_44 != lh->sarg && _old_44 != lh->targ && _old_44 != _src_1_38 && _old_44 != _src_2_39 && _old_44 != _keep_0_0_41 && _old_44 != _keep_0_1_42 && _old_44 != _keep_0_2_43) {
                            LIR_unload_subject(_old_44);
                        }
                    }
                    lir_subject_t* _old_45 = lh->targ;
                    if (_old_45 != _src_2_39) {
                        lh->targ = _src_2_39;
                        optimized = 1;
                        if (_old_45 && _old_45 != lh->farg && _old_45 != lh->sarg && _old_45 != lh->targ && _old_45 != _src_1_38 && _old_45 != _src_2_39 && _old_45 != _keep_0_0_41 && _old_45 != _keep_0_1_42 && _old_45 != _keep_0_2_43) {
                            LIR_unload_subject(_old_45);
                        }
                    }
                    lir_subject_t* _old_46 = lh->farg;
                    if (_old_46 != _src_1_38) {
                        lh->farg = _src_1_38;
                        optimized = 1;
                        if (_old_46 && _old_46 != lh->farg && _old_46 != lh->sarg && _old_46 != lh->targ && _old_46 != _src_1_38 && _old_46 != _src_2_39 && _old_46 != _keep_0_0_41 && _old_46 != _keep_0_1_42 && _old_46 != _keep_0_2_43) {
                            LIR_unload_subject(_old_46);
                        }
                    }
                    optimized = 1;
                    lir_subject_t* _old_47 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(_old_47));
                    if (_old_47 && _old_47 != lh->farg && _old_47 != lh->sarg && _old_47 != lh->targ && _old_47 != _src_1_38 && _old_47 != _src_2_39 && _old_47 != _keep_0_0_41 && _old_47 != _keep_0_1_42 && _old_47 != _keep_0_2_43) {
                        LIR_unload_subject(_old_47);
                    }
                }
                else if (lh->op == LIR_iDIV &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
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
                    lir_operation_t _match_op_0_48 = lh->op;
                    lir_subject_t* _keep_0_0_49 = lh->farg;
                    lir_subject_t* _keep_0_1_50 = lh->sarg;
                    lir_subject_t* _keep_0_2_51 = lh->targ;
                    if (lh->op != LIR_iMOV) {
                        lh->op = LIR_iMOV;
                        optimized = 1;
                    }
                    lir_subject_t* _old_52 = lh->sarg;
                    lh->sarg = LIR_SUBJ_CONST(0);
                    optimized = 1;
                    if (_old_52 && _old_52 != lh->farg && _old_52 != lh->sarg && _old_52 != lh->targ && _old_52 != _keep_0_0_49 && _old_52 != _keep_0_1_50 && _old_52 != _keep_0_2_51) {
                        LIR_unload_subject(_old_52);
                    }
                    lir_subject_t* _old_53 = lh->targ;
                    if (_old_53) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_53 && _old_53 != lh->farg && _old_53 != lh->sarg && _old_53 != lh->targ && _old_53 != _keep_0_0_49 && _old_53 != _keep_0_1_50 && _old_53 != _keep_0_2_51) {
                            LIR_unload_subject(_old_53);
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
                    lir_subject_t* _src_1_54 = lh->farg;
                    lir_subject_t* _src_2_55 = lh->targ;
                    lir_operation_t _match_op_0_56 = lh->op;
                    lir_subject_t* _keep_0_0_57 = lh->farg;
                    lir_subject_t* _keep_0_1_58 = lh->sarg;
                    lir_subject_t* _keep_0_2_59 = lh->targ;
                    if (lh->op != LIR_bSHL) {
                        lh->op = LIR_bSHL;
                        optimized = 1;
                    }
                    lir_subject_t* _old_60 = lh->sarg;
                    if (_old_60 != _src_1_54) {
                        lh->sarg = _src_1_54;
                        optimized = 1;
                        if (_old_60 && _old_60 != lh->farg && _old_60 != lh->sarg && _old_60 != lh->targ && _old_60 != _src_1_54 && _old_60 != _src_2_55 && _old_60 != _keep_0_0_57 && _old_60 != _keep_0_1_58 && _old_60 != _keep_0_2_59) {
                            LIR_unload_subject(_old_60);
                        }
                    }
                    lir_subject_t* _old_61 = lh->targ;
                    if (_old_61 != _src_2_55) {
                        lh->targ = _src_2_55;
                        optimized = 1;
                        if (_old_61 && _old_61 != lh->farg && _old_61 != lh->sarg && _old_61 != lh->targ && _old_61 != _src_1_54 && _old_61 != _src_2_55 && _old_61 != _keep_0_0_57 && _old_61 != _keep_0_1_58 && _old_61 != _keep_0_2_59) {
                            LIR_unload_subject(_old_61);
                        }
                    }
                    lir_subject_t* _old_62 = lh->farg;
                    if (_old_62 != _src_1_54) {
                        lh->farg = _src_1_54;
                        optimized = 1;
                        if (_old_62 && _old_62 != lh->farg && _old_62 != lh->sarg && _old_62 != lh->targ && _old_62 != _src_1_54 && _old_62 != _src_2_55 && _old_62 != _keep_0_0_57 && _old_62 != _keep_0_1_58 && _old_62 != _keep_0_2_59) {
                            LIR_unload_subject(_old_62);
                        }
                    }
                    optimized = 1;
                    lir_subject_t* _old_63 = lh->targ;
                    lh->targ = LIR_SUBJ_CONST(LIR_peephole_get_log2_number(_old_63));
                    if (_old_63 && _old_63 != lh->farg && _old_63 != lh->sarg && _old_63 != lh->targ && _old_63 != _src_1_54 && _old_63 != _src_2_55 && _old_63 != _keep_0_0_57 && _old_63 != _keep_0_1_58 && _old_63 != _keep_0_2_59) {
                        LIR_unload_subject(_old_63);
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == -1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_64 = lh->farg;
                    lir_operation_t _match_op_0_65 = lh->op;
                    lir_subject_t* _keep_0_0_66 = lh->farg;
                    lir_subject_t* _keep_0_1_67 = lh->sarg;
                    lir_subject_t* _keep_0_2_68 = lh->targ;
                    if (lh->op != LIR_NOT) {
                        lh->op = LIR_NOT;
                        optimized = 1;
                    }
                    lir_subject_t* _old_69 = lh->farg;
                    if (_old_69 != _src_1_64) {
                        lh->farg = _src_1_64;
                        optimized = 1;
                        if (_old_69 && _old_69 != lh->farg && _old_69 != lh->sarg && _old_69 != lh->targ && _old_69 != _src_1_64 && _old_69 != _keep_0_0_66 && _old_69 != _keep_0_1_67 && _old_69 != _keep_0_2_68) {
                            LIR_unload_subject(_old_69);
                        }
                    }
                    lir_subject_t* _old_70 = lh->sarg;
                    if (_old_70) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_70 && _old_70 != lh->farg && _old_70 != lh->sarg && _old_70 != lh->targ && _old_70 != _src_1_64 && _old_70 != _keep_0_0_66 && _old_70 != _keep_0_1_67 && _old_70 != _keep_0_2_68) {
                            LIR_unload_subject(_old_70);
                        }
                    }
                    lir_subject_t* _old_71 = lh->targ;
                    if (_old_71) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_71 && _old_71 != lh->farg && _old_71 != lh->sarg && _old_71 != lh->targ && _old_71 != _src_1_64 && _old_71 != _keep_0_0_66 && _old_71 != _keep_0_1_67 && _old_71 != _keep_0_2_68) {
                            LIR_unload_subject(_old_71);
                        }
                    }
                }
                else if (lh->op == LIR_iMUL &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
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
                ((lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) && LIR_peephole_get_long_number(lh->targ) == 1) &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_108 = lh->farg;
                    lir_operation_t _match_op_0_109 = lh->op;
                    lir_subject_t* _keep_0_0_110 = lh->farg;
                    lir_subject_t* _keep_0_1_111 = lh->sarg;
                    lir_subject_t* _keep_0_2_112 = lh->targ;
                    if (lh->op != LIR_DEC) {
                        lh->op = LIR_DEC;
                        optimized = 1;
                    }
                    lir_subject_t* _old_113 = lh->sarg;
                    if (_old_113) {
                        lh->sarg = NULL;
                        optimized = 1;
                        if (_old_113 && _old_113 != lh->farg && _old_113 != lh->sarg && _old_113 != lh->targ && _old_113 != _src_1_108 && _old_113 != _keep_0_0_110 && _old_113 != _keep_0_1_111 && _old_113 != _keep_0_2_112) {
                            LIR_unload_subject(_old_113);
                        }
                    }
                    lir_subject_t* _old_114 = lh->targ;
                    if (_old_114) {
                        lh->targ = NULL;
                        optimized = 1;
                        if (_old_114 && _old_114 != lh->farg && _old_114 != lh->sarg && _old_114 != lh->targ && _old_114 != _src_1_108 && _old_114 != _keep_0_0_110 && _old_114 != _keep_0_1_111 && _old_114 != _keep_0_2_112) {
                            LIR_unload_subject(_old_114);
                        }
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                lh->sarg->t == LIR_REGISTER &&
                lh->targ &&
                lh->targ->t == LIR_REGISTER &&
                lh->farg &&
                lh->farg->t == LIR_REGISTER &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    lir_subject_t* _src_1_115 = lh->farg;
                    lir_operation_t _match_op_0_116 = lh->op;
                    lir_subject_t* _keep_0_0_117 = lh->farg;
                    lir_subject_t* _keep_0_1_118 = lh->sarg;
                    lir_subject_t* _keep_0_2_119 = lh->targ;
                    if (lh->op != LIR_bXOR) {
                        lh->op = LIR_bXOR;
                        optimized = 1;
                    }
                    lir_subject_t* _old_120 = lh->sarg;
                    if (_old_120 != _src_1_115) {
                        lh->sarg = _src_1_115;
                        optimized = 1;
                        if (_old_120 && _old_120 != lh->farg && _old_120 != lh->sarg && _old_120 != lh->targ && _old_120 != _src_1_115 && _old_120 != _keep_0_0_117 && _old_120 != _keep_0_1_118 && _old_120 != _keep_0_2_119) {
                            LIR_unload_subject(_old_120);
                        }
                    }
                    lir_subject_t* _old_121 = lh->targ;
                    if (_old_121 != _src_1_115) {
                        lh->targ = _src_1_115;
                        optimized = 1;
                        if (_old_121 && _old_121 != lh->farg && _old_121 != lh->sarg && _old_121 != lh->targ && _old_121 != _src_1_115 && _old_121 != _keep_0_0_117 && _old_121 != _keep_0_1_118 && _old_121 != _keep_0_2_119) {
                            LIR_unload_subject(_old_121);
                        }
                    }
                    lir_subject_t* _old_122 = lh->farg;
                    if (_old_122 != _src_1_115) {
                        lh->farg = _src_1_115;
                        optimized = 1;
                        if (_old_122 && _old_122 != lh->farg && _old_122 != lh->sarg && _old_122 != lh->targ && _old_122 != _src_1_115 && _old_122 != _keep_0_0_117 && _old_122 != _keep_0_1_118 && _old_122 != _keep_0_2_119) {
                            LIR_unload_subject(_old_122);
                        }
                    }
                }
                else if (lh->op == LIR_iSUB &&
                (lh->sarg &&
                (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) &&
                lh->farg &&
                (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) &&
                LIR_subj_equals(lh->farg, lh->sarg))) {
                    if (!lh->unused) {
                        lh->unused = 1;
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