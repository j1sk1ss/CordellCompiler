#ifndef X86_64_IRGEN_H_
#define X86_64_IRGEN_H_

#include <hir/hir.h>
#include <hir/hir_types.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <std/qsort.h>
#include <symtab/symtab.h>

static inline int IR_deallocate_scope_heap(ast_node_t* t, lir_ctx_t* ctx) {
    if (scope_id_top(&ctx->heap) == t->sinfo.s_id) {
        scope_elem_t hinfo;
        scope_pop_top(&ctx->heap, &hinfo);
        print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", t->sinfo.s_id, hinfo.offset);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_OFF(hinfo.offset, 8));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(12));
        IR_BLOCK0(ctx, SYSC);
    }

    return 1;
}

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt);

/* asm_irgen.c */
int LIR_generate_asmblock(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* assign_irgen.c */
int LIR_generate_assignment_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* block_irgen.c */
int LIR_generate_elem_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* cond_irgen.c */
int LIR_generate_if_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_while_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_switch_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* decl_irgen.c */
int LIR_generate_declaration_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* func_irgen.c */
int LIR_generate_funcdef_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_return_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_funccall_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_function_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* load_irgen.c */
int LIR_generate_ptr_load_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_load_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* op_irgen.c */
int LIR_generate_operand_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* store_irgen.c */
int LIR_generate_store_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

/* syscl_irgen.c */
int LIR_generate_start_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_exit_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);
int LIR_generate_syscall_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx);

#endif