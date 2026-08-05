#ifndef HIR_DUMP_H_
#define HIR_DUMP_H_

#include <stdio.h>
#include <stdarg.h>
#include <ast/dump.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/hir_types.h>

int DUMP_format_hirctx(hir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output);
int DUMP_format_hir_cfg(cfg_ctx_t* cctx, sym_table_t* smt, const char* name, FILE* output);

#endif
