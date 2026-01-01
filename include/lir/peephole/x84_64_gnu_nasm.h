#ifndef PEEPHOLE_X86_64_GNU_NASM_
#define PEEPHOLE_X86_64_GNU_NASM_

#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx);

#endif