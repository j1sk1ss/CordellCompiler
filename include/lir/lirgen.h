#ifndef IRGEN_H_
#define IRGEN_H_

#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgens/lirgens.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif