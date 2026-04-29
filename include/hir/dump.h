#ifndef HIR_DUMP_H_
#define HIR_DUMP_H_

#include <stdio.h>
#include <ast/dump.h>
#include <hir/hir.h>
#include <hir/hir_types.h>

int DUMP_format_hirctx(hir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output);

#endif
