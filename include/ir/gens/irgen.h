#ifndef IRGEN_H_
#define IRGEN_H_

#include <ir/ir.h>

typedef struct ir_get {
    int (*datagen)(ast_node_t*, int, int, struct ir_get*, ir_ctx_t*);
    int (*funcdef)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*funcret)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*funccall)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*function)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*blockgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*elemegen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*operand)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*store)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*ptrload)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*load)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*assign)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*decl)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*start)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*asmer)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*exit)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*syscall)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*ifgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*whilegen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*switchgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
} ir_get_t;

static inline int IR_deallocate_scope_heap(ast_node_t* t, ir_ctx_t* ctx) {
    if (scope_id_top(&ctx->heap) == t->sinfo.s_id) {
        scope_elem_t hinfo;
        scope_pop_top(&ctx->heap, &hinfo);
        print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", t->sinfo.s_id, hinfo.offset);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI), IR_SUBJ_OFF(hinfo.offset));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_CNST(12));
        IR_BLOCK0(ctx, SYSC);
    }

    return 1;
}

#endif