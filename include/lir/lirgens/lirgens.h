#ifndef X86_64_IRGEN_H_
#define X86_64_IRGEN_H_

#include <hir/hir.h>
#include <hir/hir_types.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <symtab/symtab.h>
#include <std/map.h>
#include <std/qsort.h>
#include <std/stack.h>
#include <std/stackmap.h>

/* x86_64_lirvar.c */
lir_subject_t* x86_64_format_variable(hir_subject_t* subj);
int x86_64_store_var2var(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* dst, hir_subject_t* src);

/* x86_64_asmgen.c */
int x86_64_generate_asmblock(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params);

/* x86_64_funcgen.c */
int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);

/* x86_64_lirgen.c */
int LIR_generate_block(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif