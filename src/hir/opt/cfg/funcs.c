/*
funcs.c - Split input HIR instructions by function blocks.
*/

#include <hir/opt/cfg.h>

static cfg_func_t* _create_funcblock(hir_block_t* entry, hir_block_t* end) {
    cfg_func_t* b = (cfg_func_t*)mm_malloc(sizeof(cfg_func_t));
    if (!b) return NULL;
    str_memset(b, 0, sizeof(cfg_func_t));
    b->entry = entry;
    b->exit  = end;
    b->id    = entry->farg->storage.str.s_id;
    list_init(&b->blocks);
    return b;
}

static int _add_funcblock(hir_block_t* entry, hir_block_t* end, cfg_ctx_t* ctx) {
    cfg_func_t* b = _create_funcblock(entry, end);
    if (!b) return 0;
    b->id = ctx->cid++;
    list_add(&ctx->funcs, b);
    return 1;
}

cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid) {
    list_iter_t bit;
    list_iter_hinit(&f->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (cb->entry->op == HIR_MKLB && cb->entry->farg->id == lbid) return cb;
    }

    return NULL;
}

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx) {
    hir_block_t* h = hctx->h;
    hir_block_t* fentry = NULL;
    while (h) {
        if (h->op == HIR_FDCL || h->op == HIR_STRT) fentry = h;
        if ((h->op == HIR_FEND || h->op == HIR_STEND) && fentry) {
            _add_funcblock(fentry, h, ctx);
            fentry = NULL;
        } 

        h = h->next;
    }

    return 1;
}
