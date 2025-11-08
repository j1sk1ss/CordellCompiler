#ifndef PEEPHOLE_H_
#define PEEPHOLE_H_

#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    int (*perform_peephole)(cfg_ctx_t*, sym_table_t*);
} peephole_t;

int LIR_peephole_optimization(cfg_ctx_t* cctx, sym_table_t* smt, peephole_t* peephole);

#endif