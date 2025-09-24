#include <hir/opt/cfg.h>

static cfg_func_t* _create_funcblock(hir_block_t* entry, hir_block_t* end) {
    cfg_func_t* b = (cfg_func_t*)mm_malloc(sizeof(cfg_func_t));
    if (!b) return NULL;
    str_memset(b, 0, sizeof(cfg_func_t));
    b->entry = entry;
    b->exit  = end;
    b->id    = entry->farg->storage.str.s_id;
    return b;
}

static int _add_funcblock(hir_block_t* entry, hir_block_t* end, cfg_ctx_t* ctx) {
    cfg_func_t* b = _create_funcblock(entry, end);
    if (!b) return 0;

    b->id = ctx->cid++;
    if (!ctx->h) {
        ctx->h = b;
        return 1;
    }

    cfg_func_t* h = ctx->h;
    while (h->next) h = h->next;
    h->next = b;
    return 1;
}

cfg_func_t* HIR_CFG_find_function(long fid, cfg_ctx_t* ctx) {
    cfg_func_t* h = ctx->h;
    while (h) {
        if (fid == h->id) return h;
        h = h->next;
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
