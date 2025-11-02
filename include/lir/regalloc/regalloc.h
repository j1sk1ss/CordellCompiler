#ifndef REGALLOC_H_
#define REGALLOC_H_

#include <hir/cfg.h>

typedef struct {
    int (*regallocate)(cfg_ctx_t*, sym_table_t*);
} regalloc_t;

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, regalloc_t* allocator);

#endif