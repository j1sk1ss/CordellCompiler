#ifndef INSTSEL_H_
#define INSTSEL_H_

#include <hir/hir.h>
#include <hir/cfg.h>

#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    int (*select_instructions)(cfg_ctx_t*, sym_table_t*);
} inst_selector_h;

int LIR_select_instructions(cfg_ctx_t* cctx, sym_table_t* smt, inst_selector_h* selector);

#endif