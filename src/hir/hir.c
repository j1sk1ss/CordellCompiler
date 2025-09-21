#include <hir/hir.h>

hir_ctx_t* HIR_create_ctx() {
    hir_ctx_t* ctx = mm_malloc(sizeof(hir_ctx_t));
    if (!ctx) return NULL;
    ctx->h = ctx->t = NULL;
    ctx->cid = 0;
    ctx->lid = 0;
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
hir_subject_t* HIR_create_subject(
    int t, registers_t r, int dref, 
    int v_id, const char* strval,
    long intval, int size, int s_id
) {
    hir_subject_t* subj = mm_malloc(sizeof(hir_subject_t));
    if (!subj) return NULL;

    str_memset(subj, 0, sizeof(hir_subject_t));

    subj->t    = t;
    subj->size = (char)size;
    subj->dref = (char)dref;
    subj->id   = _curr_id++;

    switch (t) {
        case REGISTER:
            subj->storage.reg.reg = r;
        break;

        case TMPVARSTR: case TMPVARARR: case TMPVARF64: case TMPVARU64:
        case TMPVARI64: case TMPVARF32: case TMPVARU32: case TMPVARI32:
        case TMPVARU16: case TMPVARI16: case TMPVARU8:  case TMPVARI8:
        case STKVARSTR: case STKVARARR: case STKVARF64: case STKVARU64:
        case STKVARI64: case STKVARF32: case STKVARU32: case STKVARI32:
        case STKVARU16: case STKVARI16: case STKVARU8:  case STKVARI8:
            subj->storage.var.v_id = v_id;
            subj->storage.var.s_id = s_id;
        break;

        case GLBVARSTR: case GLBVARARR: case GLBVARF64: case GLBVARU64:
        case GLBVARI64: case GLBVARF32: case GLBVARU32: case GLBVARI32:
        case GLBVARU16: case GLBVARI16: case GLBVARU8:  case GLBVARI8:
            if (strval) str_strncpy(subj->storage.gvar.name, strval, IR_VAL_MSIZE);
        break;

        case NUMBER:
            if (strval) str_strncpy(subj->storage.num.value, strval, IR_VAL_MSIZE);
        break;

        case CONSTVAL:
            subj->storage.cnst.value = intval;
        break;

        case LABEL:
        case RAWASM:
        case STRING:
            if (strval) str_strncpy(subj->storage.str.value, strval, IR_VAL_MSIZE);
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
    
    if (fa) blk->args = 1;
    if (sa) blk->args = 2;
    if (ta) blk->args = 3;
    return blk;
}

int HIR_insert_block(hir_block_t* block, hir_ctx_t* ctx) {
    if (!ctx || !block) return -1;
    if (!ctx->h) ctx->h = ctx->t = block;
    else {
        block->prev  = ctx->t;
        ctx->t->next = block;
        ctx->t = block;
    }
    
    return 0;
}

int HIR_remove_block(hir_block_t* block, hir_ctx_t* ctx) {
    block->prev->next = block->next;
    block->prev = NULL;
    block->next = NULL;
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
