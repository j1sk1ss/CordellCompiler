#ifndef REGALLOC_H_
#define REGALLOC_H_

#include <symtab/symtab.h>
#include <std/map.h>
#include <hir/cfg.h>

typedef struct {
    int (*regallocate)(cfg_ctx_t*, sym_table_t*, map_t*);
} regalloc_t;

/*
Allocate registers for variables with the provided strategy.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.
    - `colors` - Colors map.
    - `allocator` - Allocator strategy.

Returns 1 if succeeds.
*/
int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors, regalloc_t* allocator);

#endif