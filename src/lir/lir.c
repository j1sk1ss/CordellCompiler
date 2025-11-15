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
lir_subject_t* LIR_create_subject(int t, int reg, int v_id, long offset, const char* strval, long intval, int size) {
    lir_subject_t* subj = mm_malloc(sizeof(lir_subject_t));
    if (!subj) return NULL;
    str_memset(subj, 0, sizeof(lir_subject_t));

    subj->t    = t;
    subj->size = (char)size;
    subj->id   = _curr_id++;

    switch (t) {
        case LIR_ARGLIST:  list_init(&subj->storage.list.h);                       break;
        case LIR_REGISTER: subj->storage.reg.reg = LIR_format_register(reg, size); break;
        case LIR_VARIABLE:
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

    blk->op     = op;
    blk->farg   = fa;
    blk->sarg   = sa;
    blk->targ   = ta;
    blk->next   = NULL;
    blk->prev   = NULL;
    blk->unused = 0;

    if (fa && !fa->home) fa->home = blk;
    if (sa && !sa->home) sa->home = blk;
    if (ta && !ta->home) ta->home = blk;

    return blk;
}

int LIR_subj_equals(lir_subject_t* a, lir_subject_t* b) {
    if (!a || !b) return 0;
    if (a->t != b->t) return 0;
    switch (a->t) {
        case LIR_MEMORY:     return (a->storage.var.offset == b->storage.var.offset) && (a->size == b->size);
        case LIR_VARIABLE:   return a->storage.var.v_id == b->storage.var.v_id;
        case LIR_CONSTVAL:   return a->storage.cnst.value == b->storage.cnst.value;
        case LIR_GLVARIABLE:
        case LIR_STVARIABLE: return (a->storage.var.offset == b->storage.var.offset) && (a->storage.var.v_id == b->storage.var.v_id);
        case LIR_REGISTER:   return LIR_format_register(a->storage.reg.reg, 1) == LIR_format_register(b->storage.reg.reg, 1);
        case LIR_NUMBER:     return !str_memcmp(a->storage.num.value, b->storage.num.value, LIR_VAL_MSIZE);
        case LIR_LABEL:      return a->storage.lb.lb_id == b->storage.lb.lb_id;
        case LIR_STRING:     return a->storage.str.sid == b->storage.str.sid;
        default:             return 0;
    }
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

int LIR_insert_block_after(lir_block_t* block, lir_block_t* pos) {
    if (!block || !pos) return 0;
    block->prev = pos;
    block->next = pos->next;
    if (pos->next) pos->next->prev = block;
    pos->next = block;
    return 1;
}

int LIR_insert_block_before(lir_block_t* block, lir_block_t* pos) {
    if (!block || !pos) return 0;
    block->prev = pos->prev;
    block->next = pos;
    if (pos->prev) pos->prev->next = block;
    pos->prev = block;
    return 1;
}

int LIR_unlink_block(lir_block_t* block) {
    if (!block) return 0;
    if (block->prev) block->prev->next = block->next;
    if (block->next) block->next->prev = block->prev;
    block->prev = block->next = NULL;
    return 1;
}

int LIR_unload_subject(lir_subject_t* s) {
    if (s->t == LIR_ARGLIST) list_free(&s->storage.list.h);
    return mm_free(s);
}

int LIR_unload_blocks(lir_block_t* block) {
    if (!block) return -1;
    while (block) {
        lir_block_t* nxt = block->next;
        if (block->farg) LIR_unload_subject(block->farg);
        if (block->sarg) LIR_unload_subject(block->sarg);
        if (block->targ) LIR_unload_subject(block->targ);
        mm_free(block);
        block = nxt;
    }

    return 0;
}
