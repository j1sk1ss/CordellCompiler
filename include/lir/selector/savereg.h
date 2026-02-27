#ifndef SAVEREG_H_
#define SAVEREG_H_

#include <std/map.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    int (*save_registers)(cfg_ctx_t*);
} register_saver_t;

int LIR_save_registers(cfg_ctx_t* cctx, register_saver_t* selector);

#endif