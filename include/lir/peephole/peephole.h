#ifndef PEEPHOLE_H_
#define PEEPHOLE_H_

#include <std/math.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>

/*
Convert the LIR subject to long number.
Note: This function works only with a const/number subject.
Params:
    - s - The LIR subject.

Return the long nummber represented by this subject.
*/
long LIR_peephole_get_long_number(lir_subject_t* s);

/*
Get the sqrt from the LIR subject.
Note: This function works only with a const/number subject.
Params:
    - s - The LIR subject.

Return the long nummber sqrt from this subject.
*/
long LIR_peephole_get_sqrt_number(lir_subject_t* s);

/*
Get the log2 from the LIR subject.
Note: This function works only with a const/number subject.
Params:
    - s - The LIR subject.

Return the long nummber log2 from this subject.
*/
long LIR_peephole_get_log2_number(lir_subject_t* s);

typedef struct {
    int (*perform_peephole)(cfg_ctx_t*);
} peephole_t;

/*
The first peephole optimization pass. This pass mainly focuses on pattern matching and basic cleanup.
Note: This phase must be generated with PTRN DSL application.
Params:
    - bb - Basic block.

Return 1 if phase optimizes something.
*/
int peephole_first_pass(cfg_block_t* bb);

/*
Proceed the LIR peephole optimization.
This optimization devided into a three steps:
- first pass (see peephole_first_pass)
- second pass - Cleaning phase.
                This phase will remove all redundant movs,
                that are rewritten with further movs.
                Example:
                    mov rax, 10
                    ... ; <- No RAX usage
                    mov rax, rcx

Params:
    - cctx - CFG with the HIR and the LIR.
    - peephole - Architecture dependent peephole second phase implementation.

Return 1 or 0.
*/
int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole);

#endif