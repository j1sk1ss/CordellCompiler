#include <hir/hir.h>

hir_ctx_t* HIR_create_ctx() {
    hir_ctx_t* ctx = mm_malloc(sizeof(hir_ctx_t));
    if (!ctx) return NULL;
    ctx->h = ctx->t = NULL;
    return ctx;
}

int HIR_destroy_ctx(hir_ctx_t* ctx) {
    if (!ctx) return -1;
    hir_block_t* cur = ctx->h;
    while (cur) {
        hir_block_t* nxt = cur->next;
        mm_free(cur);
        cur = nxt;
    }

    mm_free(ctx);
    return 0;
}

static long _curr_id = 0;
hir_subject_t* HIR_create_subject(int t, int v_id, const char* strval, long intval, int s_id) {
    hir_subject_t* subj = mm_malloc(sizeof(hir_subject_t));
    if (!subj) return NULL;
    str_memset(subj, 0, sizeof(hir_subject_t));

    subj->t  = t;
    subj->id = _curr_id++;

    switch (t) {
        case HIR_SET: set_init(&subj->storage.set.h); break;
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARF64: case HIR_TMPVARU64:
        case HIR_TMPVARI64: case HIR_TMPVARF32: case HIR_TMPVARU32: case HIR_TMPVARI32:
        case HIR_TMPVARU16: case HIR_TMPVARI16: case HIR_TMPVARU8:  case HIR_TMPVARI8:
        case HIR_STKVARSTR: case HIR_STKVARARR: case HIR_STKVARF64: case HIR_STKVARU64:
        case HIR_STKVARI64: case HIR_STKVARF32: case HIR_STKVARU32: case HIR_STKVARI32:
        case HIR_STKVARU16: case HIR_STKVARI16: case HIR_STKVARU8:  case HIR_STKVARI8:
        case HIR_GLBVARSTR: case HIR_GLBVARARR: case HIR_GLBVARF64: case HIR_GLBVARU64:
        case HIR_GLBVARI64: case HIR_GLBVARF32: case HIR_GLBVARU32: case HIR_GLBVARI32:
        case HIR_GLBVARU16: case HIR_GLBVARI16: case HIR_GLBVARU8:  case HIR_GLBVARI8:
            subj->storage.var.v_id = v_id;
        break;

        case HIR_NUMBER:
            if (strval) str_strncpy(subj->storage.num.value, strval, HIR_VAL_MSIZE);
        break;

        case HIR_CONSTVAL:
            subj->storage.cnst.value = intval;
        break;

        case HIR_FNAME:
        case HIR_RAWASM:
        case HIR_STRING: 
            subj->storage.str.s_id = v_id;
        break;

        default: break;
    }

    return subj;
}

hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta) {
    hir_block_t* blk = mm_malloc(sizeof(hir_block_t));
    if (!blk) return NULL;
    blk->op   = op;
    blk->farg = fa;
    blk->sarg = sa;
    blk->targ = ta;
    blk->next = NULL;
    blk->prev = NULL;
    return blk;
}

int HIR_insert_block(hir_block_t* block, hir_block_t* pos) {
    if (!block || !pos) return 0;
    block->prev = pos->prev;
    block->next = pos;
    if (pos->prev) pos->prev->next = block;
    pos->prev = block;
    return 1;
}

int HIR_append_block(hir_block_t* block, hir_ctx_t* ctx) {
    if (!ctx || !block) return -1;
    if (!ctx->h) ctx->h = ctx->t = block;
    else {
        block->prev  = ctx->t;
        ctx->t->next = block;
        ctx->t       = block;
    }
    
    return 0;
}

int HIR_remove_block(hir_block_t* block) {
    block->prev->next = block->next;
    mm_free(block);
    return 1;
}

int HIR_unload_blocks(hir_block_t* block) {
    if (!block) return -1;
    while (block) {
        hir_block_t* nxt = block->next;
        mm_free(block);
        block = nxt;
    }

    return 0;
}
