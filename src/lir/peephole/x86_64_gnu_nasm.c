#include <lir/peephole/x84_64_gnu_nasm.h>

static inline long _get_long_number(lir_subject_t* s) {
    switch (s->t) {
        case LIR_CONSTVAL: return s->storage.cnst.value;
        case LIR_NUMBER:   return s->storage.num.value->to_llong(s->storage.num.value);
        default: return 0;
    }
}

static int _first_pass(cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        switch (lh->op) {
            case LIR_CVTSS2SD:
            case LIR_CVTSD2SS:
            case LIR_CVTTSS2SI:
            case LIR_CVTTSD2SI: if (LIR_subj_equals(lh->farg, lh->sarg)) lh->unused = 1;
            case LIR_bOR:
            case LIR_iLWR:
            case LIR_iLRE:
            case LIR_iLRG:
            case LIR_iLGE:
            case LIR_iCMP:
            case LIR_iNMP:
            case LIR_bXOR:
            case LIR_bAND:
            case LIR_iMOD: {
                if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) lh->unused = 1;
                break;
            }

            case LIR_fMOV:
            case LIR_aMOV:
            case LIR_iMOV: 
            case LIR_MOVSX:
            case LIR_MOVZX:
            case LIR_MOVSXD: {
                if (LIR_subj_equals(lh->farg, lh->sarg)) lh->unused = 1;
                if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) lh->unused = 1;
                if (lh->farg->t != LIR_REGISTER) break;
                if (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) {
                    if (!_get_long_number(lh->sarg)) {
                        lh->op = LIR_bXOR;
                        LIR_unload_subject(lh->sarg);
                        LIR_unload_subject(lh->targ);
                        lh->sarg = lh->farg;
                        lh->targ = lh->farg;
                    }
                }

                break;
            }

            case LIR_CMP: {
                if (lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) {
                    if (lh->farg->t != LIR_REGISTER) break;
                    if (!_get_long_number(lh->sarg)) {
                        lh->op = LIR_TST;
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = lh->farg;
                    }
                }

                break;
            }

            case LIR_iSUB:
            case LIR_iADD: {
                if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) lh->unused = 1;
                if (LIR_subj_equals(lh->sarg, lh->targ)) {
                    lh->op = lh->op == LIR_iSUB ? LIR_bXOR : LIR_bSHL;
                    LIR_unload_subject(lh->targ);
                    lh->targ = lh->op == LIR_iSUB ? lh->sarg : LIR_SUBJ_CONST(1);
                    break;
                }

                if (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) {
                    if (!_get_long_number(lh->targ)) lh->unused = 1;
                }

                break;
            }

            case LIR_iDIV:
            case LIR_iMUL: {
                if (lh->farg->t == LIR_NUMBER || lh->farg->t == LIR_CONSTVAL) lh->unused = 1;
                else if (lh->targ->t == LIR_NUMBER || lh->targ->t == LIR_CONSTVAL) {
                    int rval = _get_long_number(lh->targ);
                    if (rval == 1) lh->unused = 1;
                    else if (!(rval & (rval - 1))) {
                        int shift = 0;
                        while (rval >>= 1) {
                            shift++;
                        }
                        
                        lh->op = (lh->op == LIR_iMUL) ? LIR_bSHL : LIR_bSHR;
                        LIR_unload_subject(lh->targ);
                        lh->targ = LIR_SUBJ_CONST(shift);
                    }
                }
                
                break;
            }

            default: break;
        }

        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    return 1;
}

static inline int _check_home(lir_block_t* h, lir_subject_t* s) {
    lir_subject_t* args[] = { h->farg, h->sarg, h->targ };
    for (int i = 0; i < 3; i++) {
        if (!args[i]) continue;
        if (args[i] == s) return 1;
    }

    return 0;
}

static int _second_pass(cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        if (LIR_movop(lh->op)) {
            lir_subject_t* src = lh->sarg;
            lir_subject_t* dst = lh->farg;

            lir_block_t* currh = lh->next;
            while (currh) {
                if (LIR_writeop(lh->op) && (LIR_subj_equals(currh->farg, dst))) break; 
                if (currh->op == lh->op) {
                    if (LIR_subj_equals(currh->farg, src)) break;
                    if (
                        LIR_subj_equals(currh->sarg, dst) &&
                        (currh->farg->t != LIR_MEMORY || src->t != LIR_MEMORY)
                    ) {
                        if (!_check_home(currh->sarg->home, currh->sarg)) {
                            LIR_unload_subject(currh->sarg);
                        }

                        currh->sarg = src;
                        currh->op   = lh->op;
                    }
                }

                if (currh == bb->lmap.exit) break;
                currh = currh->next;
            }
        }

        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    return 1;
}

static unsigned int _visit_counter = 1;
static int _recursive_cleanup(
    lir_operation_t op, long pred, cfg_block_t* bbh, lir_subject_t* trg, lir_block_t* ign, lir_block_t* off
) {
    if (!bbh) return 0;
    if (bbh->visited != _visit_counter) {
        set_free(&bbh->visitors);
        set_init(&bbh->visitors, SET_NO_CMP);
    }
    
    if (set_has(&bbh->visitors, (void*)pred)) return 0;
    bbh->visited = _visit_counter;
    set_add(&bbh->visitors, (void*)pred);

    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if (lh->op == LIR_aMOV) return 0;
        if (LIR_readop(lh->op) && (LIR_subj_equals(lh->sarg, trg) || LIR_subj_equals(lh->targ, trg))) return 0;
        if (lh != ign && lh->op == op && LIR_subj_equals(lh->farg, trg)) return 1;
        if (lh == bbh->lmap.exit) break;
        lh = lh->next;
    }

    if (
        _recursive_cleanup(op, bbh->id, bbh->l, trg, ign, NULL) || 
        _recursive_cleanup(op, bbh->id, bbh->jmp, trg, ign, NULL)
    ) return 1;
    return 0;
}

static int _cleanup_pass(cfg_block_t* bb) {
    if (!bb) return 0;

    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        if (lh == bb->lmap.exit) break;
        if (LIR_writeop(lh->op)) {
            _visit_counter++;
            if (_recursive_cleanup(lh->op, -1, bb, lh->farg, lh, lh->next)) {
                lh->unused = 1;
            }
        }

        lh = lh->next;
    }

    return 1;
}

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach(cfg_block_t* bb, &fb->blocks) {
            _first_pass(bb);
            _second_pass(bb);
            _cleanup_pass(bb);
        }
    }

    return 1;
}
