#include <lir/lir.h>

lir_ctx_t* LIR_create_ctx() {
    lir_ctx_t* ctx = mm_malloc(sizeof(lir_ctx_t));
    if (!ctx) return NULL;
    ctx->h = ctx->t = NULL;
    ctx->cid = 0;
    ctx->lid = 0;
    return ctx;
}

int LIR_destroy_ctx(lir_ctx_t* ctx) {
    if (!ctx) return -1;
    lir_block_t* cur = ctx->h;
    while (cur) {
        lir_block_t* nxt = cur->next;
        mm_free(cur);
        cur = nxt;
    }

    mm_free(ctx);
    return 0;
}

static long _curr_id = 0;
lir_subject_t* LIR_create_subject(
    int t, registers_t r, int v_id, long offset, const char* strval, long intval, int size, int s_id
) {
    lir_subject_t* subj = mm_malloc(sizeof(lir_subject_t));
    if (!subj) return NULL;

    str_memset(subj, 0, sizeof(lir_subject_t));

    subj->t    = t;
    subj->size = (char)size;
    subj->id   = _curr_id++;

    switch (t) {
        case LIR_REGISTER: subj->storage.reg.reg = LIR_format_register(r, size); break;
        case LIR_GLVARIABLE:
        case LIR_STVARIABLE: 
            subj->storage.var.offset = offset;
            subj->storage.var.v_id   = v_id;
        break;
        case LIR_MEMORY:   subj->storage.var.offset = offset; break;
        case LIR_CONSTVAL: subj->storage.cnst.value = intval; break;
        case LIR_LABEL:    subj->storage.lb.lb_id   = v_id;   break;
        case LIR_FNAME:
        case LIR_RAWASM:
        case LIR_STRING: subj->storage.str.sid = v_id; break;
        case LIR_NUMBER: 
            if (strval) str_strncpy(subj->storage.num.value, strval, LIR_VAL_MSIZE);
        break;
        
        default: break;
    }

    return subj;
}

lir_block_t* LIR_create_block(lir_operation_t op, lir_subject_t* fa, lir_subject_t* sa, lir_subject_t* ta) {
    lir_block_t* blk = mm_malloc(sizeof(lir_block_t));
    if (!blk) return NULL;
    blk->op   = op;
    blk->farg = fa;
    blk->sarg = sa;
    blk->targ = ta;
    blk->next = NULL;
    blk->prev = NULL;
    return blk;
}

int LIR_append_block(lir_block_t* block, lir_ctx_t* ctx) {
    if (!ctx || !block) return -1;
    if (!ctx->h) ctx->h = ctx->t = block;
    else {
        block->prev  = ctx->t;
        ctx->t->next = block;
        ctx->t       = block;
    }
    
    return 0;
}

int LIR_remove_block(lir_block_t* block, lir_ctx_t* ctx) {
    block->prev->next = block->next;
    block->prev = NULL;
    block->next = NULL;
    mm_free(block);
    return 1;
}

int LIR_unload_blocks(lir_block_t* block) {
    if (!block) return -1;
    while (block) {
        lir_block_t* nxt = block->next;
        if (block->farg) mm_free(block->farg);
        if (block->sarg) mm_free(block->sarg);
        if (block->targ) mm_free(block->targ);
        mm_free(block);
        block = nxt;
    }

    return 0;
}
