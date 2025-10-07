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
lir_subject_t* LIR_format_variable(lir_ctx_t* ctx, hir_subject_t* subj, sym_table_t* smt);
int LIR_store_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt);
int LIR_load_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt);
int LIR_reg_op(lir_ctx_t* ctx, int freg, int fs, int sreg, int ss, lir_operation_t op);

int LIR_deallocate_scope_heap(lir_ctx_t* ctx, int s_id, scope_stack_t* heap);
int LIR_allocate_arr(
    lir_ctx_t* ctx, hir_subject_t* v, hir_subject_t* size, scope_stack_t* heap, 
    scope_stack_t* scopes, sym_table_t* smt, array_info_t* ai, long* offset
);

/* x86_64_refgen.c */
int x86_64_generate_ref(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);

/* x86_64_funcgen.c */
int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params);

/* x86_64_declgen.c */
int x86_64_generate_declaration(
    lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params, scope_stack_t* scopes, scope_stack_t* heap, long* offset
);

/* x86_64_opgen.c */
int x86_64_generate_unary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_ifop(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_conv(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);

/* x86_64_lirgen.c */
int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif