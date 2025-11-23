#ifndef MEMSEL_H_
#define MEMSEL_H_

#include <std/map.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    int (*select_memory)(cfg_ctx_t*, map_t*, sym_table_t*);
} mem_selector_h;

int LIR_select_memory(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt, mem_selector_h* selector);

#endif