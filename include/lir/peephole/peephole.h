#ifndef PEEPHOLE_H_
#define PEEPHOLE_H_

#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

static inline long peephole_get_long_number(lir_subject_t* s) {
    switch (s->t) {
        case LIR_CONSTVAL: return s->storage.cnst.value;
        case LIR_NUMBER:   return s->storage.num.value->to_llong(s->storage.num.value);
        default: return 0;
    }
}

static inline long peephole_get_sqrt_number(lir_subject_t* s) {
    return 1;
}

typedef struct {
    int (*perform_peephole)(cfg_ctx_t*);
} peephole_t;

int peephole_first_pass(cfg_block_t* bb);
int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole);

#endif