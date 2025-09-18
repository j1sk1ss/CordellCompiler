#include <ir/ir.h>

ir_ctx_t* IR_create_ctx() {
    ir_ctx_t* ctx = mm_malloc(sizeof(ir_ctx_t));
    if (!ctx) return NULL;
    ctx->h = ctx->t = NULL;
    ctx->cid = 0;
    ctx->lid = 0;
    return ctx;
}

int IR_destroy_ctx(ir_ctx_t* ctx) {
    if (!ctx) return -1;
    ir_block_t* cur = ctx->h;
    while (cur) {
        ir_block_t* nxt = cur->next;
        mm_free(cur);
        cur = nxt;
    }

    mm_free(ctx);
    return 0;
}

ir_subject_t* IR_create_subject(int reg, int dref, int offset, const char* val1, int val2, int size) {
    ir_subject_t* subj = mm_malloc(sizeof(ir_subject_t));
    if (!subj) return NULL;
    subj->storage.rinfo.dref = dref;

    if (reg >= 0) {
        subj->isreg = 1;
        subj->storage.rinfo.reg_id = reg;
    } 
    else {
        subj->isreg = 0;
        subj->storage.vinfo.obj_id  = 0;
        subj->storage.vinfo.instack = 0;
        subj->storage.vinfo.size    = size;
        subj->storage.vinfo.cnstvl  = val2;
        subj->storage.vinfo.pos.offset = offset;
        if (val1) {
            str_strncpy(subj->storage.vinfo.pos.value, val1, IR_VAL_MSIZE);
        }
    }

    return subj;
}

ir_block_t* IR_create_block(ir_operation_t op, ir_subject_t* fa, ir_subject_t* sa, ir_subject_t* ta) {
    ir_block_t* blk = mm_malloc(sizeof(ir_block_t));
    if (!blk) return NULL;
    blk->op   = op;
    blk->farg = fa;
    blk->sarg = sa;
    blk->targ = ta;
    blk->next = NULL;
    if (fa) blk->args = 1;
    if (sa) blk->args = 2;
    if (ta) blk->args = 3;
    return blk;
}

int IR_insert_block(ir_block_t* block, ir_ctx_t* ctx) {
    if (!ctx || !block) return -1;
    if (!ctx->h) {
        ctx->h = ctx->t = block;
    } 
    else {
        ctx->t->next = block;
        ctx->t = block;
    }
    
    return 0;
}

int IR_unload_blocks(ir_block_t* block) {
    if (!block) return -1;
    while (block) {
        ir_block_t* nxt = block->next;
        mm_free(block);
        block = nxt;
    }

    return 0;
}
