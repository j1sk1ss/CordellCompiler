#ifndef SAVEREG_H_
#define SAVEREG_H_

#include <std/map.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/func.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    int (*save_registers)(cfg_ctx_t*, call_graph_t*, sym_table_t*);
} register_saver_t;

int LIR_save_registers(cfg_ctx_t* cctx, call_graph_t* calls, sym_table_t* smt, register_saver_t* selector);

#endif