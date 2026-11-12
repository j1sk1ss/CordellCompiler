#ifndef LIR_DUMP_H_
#define LIR_DUMP_H_

#include <stdio.h>
#include <utils.h>

#include <ast/dump.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>

#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>

const char*   DUMP_registers_to_string(lir_registers_t reg) ;
status_code_t DUMP_format_lirctx(lir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output);
status_code_t DUMP_format_lir_cfg(cfg_ctx_t* cctx, sym_table_t* smt, const char* name, FILE* output);

#endif
