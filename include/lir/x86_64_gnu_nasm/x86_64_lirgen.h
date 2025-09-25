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

int LIR_allocate_var(hir_subject_t* v, stack_map_t* stk, sym_table_t* smt, alloc_info_t* i, long* off);
int LIR_deallocate_scope_heap(ast_node_t* t, lir_ctx_t* ctx);
int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif