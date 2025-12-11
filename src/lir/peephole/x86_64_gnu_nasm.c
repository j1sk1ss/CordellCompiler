#include <lir/peephole/x84_64_gnu_nasm.h>

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
        if (
            lh->op == LIR_aMOV ||
            (
                LIR_readop(lh->op) && (
                    LIR_subj_equals(lh->sarg, trg) || 
                    LIR_subj_equals(lh->targ, trg)
                )
            )
        ) goto _skip_instruction;
        if (lh != ign && lh->op == op && LIR_subj_equals(lh->farg, trg)) return 1;
_skip_instruction: {}
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
            _second_pass(bb);
            _cleanup_pass(bb);
        }
    }

    return 1;
}
