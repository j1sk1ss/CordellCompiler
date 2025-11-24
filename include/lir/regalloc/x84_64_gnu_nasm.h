#ifndef REGALLOC_X86_64_GNU_NASM_
#define REGALLOC_X86_64_GNU_NASM_

#include <std/map.h>

#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

#include <lir/regalloc/ra.h>

int x86_64_regalloc_graph(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors);
int x86_64_regalloc_linear(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors);

#endif