#include <hir/hir.h>

static inline unsigned long _mix64(unsigned long x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9;
    x ^= x >> 27;
    x *= 0x94d049bb133111eb;
    x ^= x >> 31;
    return x;
}

long HIR_hash_subject(hir_subject_t* s) {
    if (!s) return 0;
    if (s->hash) return s->hash;

    unsigned long h = (unsigned long)s->t;
    switch (s->t) {
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARF64:
        case HIR_TMPVARU64: case HIR_TMPVARI64: case HIR_TMPVARF32:
        case HIR_TMPVARU32: case HIR_TMPVARI32: case HIR_TMPVARU16:
        case HIR_TMPVARI16: case HIR_TMPVARU8:  case HIR_TMPVARI8:
        case HIR_STKVARSTR: case HIR_STKVARARR: case HIR_STKVARF64:
        case HIR_STKVARU64: case HIR_STKVARI64: case HIR_STKVARF32:
        case HIR_STKVARU32: case HIR_STKVARI32: case HIR_STKVARU16:
        case HIR_STKVARI16: case HIR_STKVARU8:  case HIR_STKVARI8:
        case HIR_GLBVARSTR: case HIR_GLBVARARR: case HIR_GLBVARF64:
        case HIR_GLBVARU64: case HIR_GLBVARI64: case HIR_GLBVARF32:
        case HIR_GLBVARU32: case HIR_GLBVARI32: case HIR_GLBVARU16:
        case HIR_GLBVARI16: case HIR_GLBVARU8:  case HIR_GLBVARI8:
            h ^= _mix64(s->storage.var.v_id);
        break;

        case HIR_F64NUMBER:
        case HIR_I64NUMBER:
        case HIR_U64NUMBER:
        case HIR_F32NUMBER:
        case HIR_I32NUMBER:
        case HIR_U32NUMBER:
        case HIR_I16NUMBER:
        case HIR_U16NUMBER:
        case HIR_I8NUMBER:
        case HIR_U8NUMBER:
        case HIR_NUMBER: {
            const char* str = s->storage.num.value;
            while (*str) h = _mix64(h ^ (unsigned char)(*str++));
            break;
        }

        case HIR_F64CONSTVAL:
        case HIR_I64CONSTVAL:
        case HIR_U64CONSTVAL:
        case HIR_F32CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_U32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_U16CONSTVAL:
        case HIR_I8CONSTVAL:
        case HIR_U8CONSTVAL:
        case HIR_CONSTVAL:
            h ^= _mix64(s->storage.cnst.value);
        break;

        case HIR_FNAME:
        case HIR_RAWASM:
        case HIR_STRING:
            h ^= _mix64(s->storage.str.s_id);
        break;

        case HIR_PHISET:
            h ^= _mix64((unsigned long)&s->storage.set.h);
        break;

        default:
            h ^= _mix64(s->id);
        break;
    }

    s->hash = (long)(h ^ (h >> 32));
    return s->hash;
}

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
hir_subject_t* HIR_create_subject(hir_subject_type_t t, int v_id, const char* strval, long intval) {
    hir_subject_t* subj = mm_malloc(sizeof(hir_subject_t));
    if (!subj) return NULL;
    str_memset(subj, 0, sizeof(hir_subject_t));

    subj->t  = t;
    subj->id = _curr_id++;
    subj->users = 1;

    switch (t) {
        case HIR_PHISET:  set_init(&subj->storage.set.h, SET_NO_CMP); break;
        case HIR_ARGLIST: list_init(&subj->storage.list.h);           break;
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

        case HIR_F64NUMBER:
        case HIR_F32NUMBER:
        case HIR_U64NUMBER:
        case HIR_U32NUMBER:
        case HIR_U16NUMBER:
        case HIR_U8NUMBER:
        case HIR_I64NUMBER:
        case HIR_I32NUMBER:
        case HIR_I16NUMBER:
        case HIR_I8NUMBER:
        case HIR_NUMBER:
            if (strval) str_strncpy(subj->storage.num.value, strval, HIR_VAL_MSIZE);
        break;

        case HIR_F64CONSTVAL:
        case HIR_F32CONSTVAL:
        case HIR_U64CONSTVAL:
        case HIR_U32CONSTVAL:
        case HIR_U16CONSTVAL:
        case HIR_U8CONSTVAL:
        case HIR_I64CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_I8CONSTVAL:
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

hir_subject_t* HIR_copy_subject(hir_subject_t* s) {
    if (!s) return NULL;
    hir_subject_t* ns = HIR_create_subject(s->t, s->storage.var.v_id, NULL, s->storage.cnst.value);
    if (!ns) return NULL;

    ns->t = s->t;
    ns->users = s->users;

    switch (ns->t) {
        case HIR_PHISET: {
            set_init(&ns->storage.set.h, SET_NO_CMP);
            set_iter_t it;
            set_iter_init(&s->storage.set.h, &it);
            int_tuple_t* tpl;
            while (set_iter_next(&it, (void**)&tpl)) {
                set_add(&ns->storage.set.h, inttuple_create(tpl->x, tpl->y));
            }

            break;
        }

        case HIR_ARGLIST: {
            list_init(&ns->storage.list.h);
            hir_subject_t* arg;
            foreach(arg, &s->storage.list.h) {
                list_add(&ns->storage.list.h, HIR_copy_subject(arg));
            }

            break;
        }

        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARF64: case HIR_TMPVARU64:
        case HIR_TMPVARI64: case HIR_TMPVARF32: case HIR_TMPVARU32: case HIR_TMPVARI32:
        case HIR_TMPVARU16: case HIR_TMPVARI16: case HIR_TMPVARU8:  case HIR_TMPVARI8:
        case HIR_STKVARSTR: case HIR_STKVARARR: case HIR_STKVARF64: case HIR_STKVARU64:
        case HIR_STKVARI64: case HIR_STKVARF32: case HIR_STKVARU32: case HIR_STKVARI32:
        case HIR_STKVARU16: case HIR_STKVARI16: case HIR_STKVARU8:  case HIR_STKVARI8:
        case HIR_GLBVARSTR: case HIR_GLBVARARR: case HIR_GLBVARF64: case HIR_GLBVARU64:
        case HIR_GLBVARI64: case HIR_GLBVARF32: case HIR_GLBVARU32: case HIR_GLBVARI32:
        case HIR_GLBVARU16: case HIR_GLBVARI16: case HIR_GLBVARU8:  case HIR_GLBVARI8:
            ns->storage.var.v_id = s->storage.var.v_id;
        break;

        case HIR_F64NUMBER:
        case HIR_F32NUMBER:
        case HIR_U64NUMBER:
        case HIR_U32NUMBER:
        case HIR_U16NUMBER:
        case HIR_U8NUMBER:
        case HIR_I64NUMBER:
        case HIR_I32NUMBER:
        case HIR_I16NUMBER:
        case HIR_I8NUMBER:
        case HIR_NUMBER:
            str_strncpy(ns->storage.num.value, s->storage.num.value, HIR_VAL_MSIZE);
        break;

        case HIR_F64CONSTVAL:
        case HIR_F32CONSTVAL:
        case HIR_U64CONSTVAL:
        case HIR_U32CONSTVAL:
        case HIR_U16CONSTVAL:
        case HIR_U8CONSTVAL:
        case HIR_I64CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_I8CONSTVAL:
        case HIR_CONSTVAL:
            ns->storage.cnst.value = s->storage.cnst.value;
        break;

        case HIR_FNAME:
        case HIR_RAWASM:
        case HIR_STRING:
            ns->storage.str.s_id = s->storage.str.s_id;
        break;

        default: break;
    }

    return ns;
}

hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta) {
    hir_block_t* blk = mm_malloc(sizeof(hir_block_t));
    if (!blk) return NULL;
    
    blk->unused = 0;
    blk->op     = op;
    blk->farg   = fa;
    blk->sarg   = sa;
    blk->targ   = ta;
    blk->next   = NULL;
    blk->prev   = NULL;

    if (fa && !fa->home) fa->home = blk;
    if (sa && !sa->home) sa->home = blk;
    if (ta && !ta->home) ta->home = blk;
    
    return blk;
}

hir_block_t* HIR_copy_block(hir_block_t* b) {
    return HIR_create_block(
        b->op, HIR_copy_subject(b->farg), HIR_copy_subject(b->sarg), HIR_copy_subject(b->targ)
    );
}

int HIR_insert_block_before(hir_block_t* block, hir_block_t* pos) {
    if (!block || !pos) return 0;
    block->prev = pos->prev;
    block->next = pos;
    if (pos->prev) pos->prev->next = block;
    pos->prev = block;
    return 1;
}

int HIR_insert_block_after(hir_block_t* block, hir_block_t* pos) {
    if (!block || !pos) return 0;
    block->prev = pos;
    block->next = pos->next;
    if (pos->next) pos->next->prev = block;
    pos->next = block;
    return 1;
}

int HIR_append_block(hir_block_t* block, hir_ctx_t* ctx) {
    if (!ctx || !block) return 0;
    if (!ctx->h) ctx->h = ctx->t = block;
    else {
        block->prev  = ctx->t;
        ctx->t->next = block;
        ctx->t       = block;
    }
    
    return 1;
}

int HIR_unlink_block(hir_block_t* block) {
    if (!block) return 0;
    if (block->prev) block->prev->next = block->next;
    if (block->next) block->next->prev = block->prev;
    block->prev = block->next = NULL;
    return 1;
}

int HIR_unload_subject(hir_subject_t* s) {
    if (!s) return 0;
    switch (s->t) {
        case HIR_PHISET:  set_free_force(&s->storage.set.h);   break;
        case HIR_ARGLIST: list_free_force(&s->storage.list.h); break;
        default: break;
    }
    
    mm_free(s);
    return 1;
}

int HIR_unload_blocks(hir_block_t* block) {
    if (!block) return -1;
    while (block) {
        hir_block_t* nxt = block->next;
        HIR_unload_subject(block->farg);
        HIR_unload_subject(block->sarg);
        HIR_unload_subject(block->targ);
        mm_free(block);
        block = nxt;
    }

    return 0;
}

int HIR_compute_homes(hir_ctx_t* ctx) {
    map_t homes;
    map_init(&homes, MAP_NO_CMP);

    hir_block_t* hh = ctx->h;
    while (hh) {
        hir_subject_t* args[] = { hh->farg, hh->sarg, hh->targ };
        for (int i = 0; i < 3; i++) {
            if (!args[i]) continue;
            args[i]->home = NULL;
            if (!HIR_is_vartype(args[i]->t)) continue;

            hir_block_t* home;
            if (map_get(&homes, args[i]->storage.var.v_id, (void**)&home)) {
                args[i]->home = home;
                continue;
            }

            map_put(&homes, args[i]->storage.var.v_id, hh);
            args[i]->home = hh;
        }

        hh = hh->next;
    }

    map_free(&homes);
    return 1;
}
