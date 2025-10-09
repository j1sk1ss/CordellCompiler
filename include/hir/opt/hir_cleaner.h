#ifndef HIR_CLEANER_H_
#define HIR_CLEANER_H_

#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/hir_types.h>

int HIR_CLN_remove_unused_variables(cfg_ctx_t* cctx);

#endif