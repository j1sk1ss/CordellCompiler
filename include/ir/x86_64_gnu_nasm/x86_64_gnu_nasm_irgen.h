#ifndef X86_64_GNU_NASM_IRGEN_H_
#define X86_64_GNU_NASM_IRGEN_H_

#include <ir/ir.h>
#include <ir/irctx.h>
#include <std/qsort.h>

static inline int IR_deallocate_scope_heap(ast_node_t* t, ir_ctx_t* ctx) {
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

/* asm_irgen.c */
int IR_generate_asmblock(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* assign_irgen.c */
int IR_generate_assignment_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* block_irgen.c */
int IR_generate_elem_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* cond_irgen.c */
int IR_generate_if_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_while_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_switch_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* data_irgen.c */
int get_stack_size(ast_node_t* root);
int IR_generate_data_block(ast_node_t* node, int section, int bss, ir_gen_t* g, ir_ctx_t* ctx);

/* decl_irgen.c */
int IR_generate_declaration_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* func_irgen.c */
int IR_generate_funcdef_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_return_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_funccall_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_function_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* load_irgen.c */
int IR_generate_ptr_load_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_load_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* op_irgen.c */
int IR_generate_operand_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* store_irgen.c */
int IR_generate_store_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

/* syscl_irgen.c */
int IR_generate_start_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_exit_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);
int IR_generate_syscall_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx);

#endif