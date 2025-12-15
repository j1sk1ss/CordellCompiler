#include <lir/peephole/x84_64_gnu_nasm.h>

/*
Simple function that helps us with a 'home verification' proccess.
Params:
    - h - Expected home of the subject.
    - s - Considering subject.

Return 1 if the 'h' is a home for the 's'.
*/
static int _check_home(lir_block_t* h, lir_subject_t* s) {
    lir_subject_t* args[] = { h->farg, h->sarg, h->targ };
    for (int i = 0; i < 3; i++) {
        if (!args[i]) continue;
        if (args[i] == s) return 1;
    }

    return 0;
}

/*
Second peephole optimization pass propagates mov operations.
The main idea is to solve a 'multiple mov' issue:
```asm
mov rax, rbx
mov rdx, rax
mov rcx, rdx
mov r12, rcx
push r12
```

This code just can't be optimized with a pattern matcher or third phase. That's why
we should propagate mov operations:
```asm
mov rax, rbx
mov rdx, rbx
mov rcx, rbx
mov r12, rbx
push r12
```

Params:
    - bb - Current basic block.

Return 1 if operation succeed, otherwise it will return 0.
*/
static int _second_pass(cfg_block_t* bb) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
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

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}

static unsigned int _visit_counter = 1;

/*
Recursive cleanup will clean each block from the CFG with one simple rule:
If there is a WRITE operation, it may be eliminated, if:
    - Either further code doesn't use it's value or /
    - Further code rewrites it's value

Params:
    - op - Write operation type.
           Note: We don't use a general term of WRITE operation 
                 given a different nature of some operations.
    - pred - Service argument. For initial call use '-1'.
    - bbh - Head Basic Block.
    - trg - Target WRITE location in the considering lir block.
    - ign - Service argument. For initial call use a parent of the 'trg' argument.
    - off - Service argument. For initial call use 'ign->next'.

Return 1 if the considering lir block can be marked as unused.
Retrun 0 if the considering lir block can't be marked as unused.
*/
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
            lh->op == LIR_aMOV ||                     /* Skip reserved instruction                                    */
            (
                LIR_readop(lh->op) &&                 /* If this instruction reads second and third arguments         */
                (
                    LIR_subj_equals(lh->sarg, trg) || /* And either second argument is equal to target                */
                    LIR_subj_equals(lh->targ, trg)    /* or third argument is equal to target                         */
                )
            )
        ) return 0;                                   /* That means, we should mark the target write command as valid */
        
        if (                                          
            lh != ign &&                              /* If this isn't ignored (likely the source) command            */
            lh->op == op &&                           /* With the same operation such as mov, add, etc.               */
            LIR_subj_equals(lh->farg, trg)            /* And similar destination of the write operation               */
        ) return 1;                                   /* That means we can safely mark the target write command       */
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    if (
        _recursive_cleanup(op, bbh->id, bbh->l, trg, ign, NULL) && 
        _recursive_cleanup(op, bbh->id, bbh->jmp, trg, ign, NULL)
    ) return 1;
    return 0;
}

static int _cleanup_pass(cfg_block_t* bb) {
    if (!bb) return 0;
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        if (LIR_writeop(lh->op)) {
            _visit_counter++;
            if (_recursive_cleanup(lh->op, -1, bb, lh->farg, lh, lh->next)) {
                lh->unused = 1;
            }
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            _second_pass(bb);
            _cleanup_pass(bb);
        }
    }

    return 1;
}
