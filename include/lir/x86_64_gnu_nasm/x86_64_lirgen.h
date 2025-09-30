#ifndef X86_64_IRGEN_H_
#define X86_64_IRGEN_H_

#include <hir/hir.h>
#include <hir/hir_types.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <symtab/symtab.h>
#include <std/qsort.h>
#include <std/stack.h>
#include <std/stackmap.h>

typedef struct {
    long offset;
    int  size;
} alloc_info_t;

lir_subject_t* LIR_format_variable(hir_subject_t* subj, sym_table_t* smt);
int LIR_store_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt);
int LIR_load_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt);
int LIR_reg_op(lir_ctx_t* ctx, int freg, int sreg, lir_operation_t op);

int LIR_allocate_var(hir_subject_t* v, stack_map_t* stk, sym_table_t* smt, alloc_info_t* i, long* off);
int LIR_deallocate_scope_heap(lir_ctx_t* ctx, int s_id, scope_stack_t* heap);

/* x86_64_funcgen.c */
int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params);

/* x86_64_declgen.c */
int x86_64_generate_declaration(
    lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params,
    scope_stack_t* scopes, scope_stack_t* heap, stack_map_t* stk, long* offset
);

/* x86_64_opgen.c */
int x86_64_generate_simd_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_ifop(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);
int x86_64_generate_conv(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt);

/* x86_64_lirgen.c */
int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif