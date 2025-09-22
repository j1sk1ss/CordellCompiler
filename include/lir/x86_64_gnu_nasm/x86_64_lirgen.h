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

static inline int IR_deallocate_scope_heap(ast_node_t* t, lir_ctx_t* ctx) {
    // if (scope_id_top(&ctx->heap) == t->sinfo.s_id) {
    //     scope_elem_t hinfo;
    //     scope_pop_top(&ctx->heap, &hinfo);
    //     print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", t->sinfo.s_id, hinfo.offset);
    //     IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_OFF(hinfo.offset, 8));
    //     IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(12));
    //     IR_BLOCK0(ctx, SYSC);
    // }

    return 1;
}

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif