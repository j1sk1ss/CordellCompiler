#ifndef COPY_PROP_H_
#define COPY_PROP_H_

#include <std/map.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>

int LIR_drop_unused_variables(cfg_ctx_t* cctx);
int LIR_copy_propagation(cfg_ctx_t* cctx);

#endif
