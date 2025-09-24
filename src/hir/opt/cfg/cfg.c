#include <hir/opt/cfg.h>

static cfg_block_t* _create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->entry = e;
    return block;
}

static int _add_cfg_block(hir_block_t* entry, hir_block_t* exit, cfg_func_t* f, cfg_ctx_t* ctx) {
    cfg_block_t* b = _create_cfg_block(entry);
    if (!b) return 0;

    b->id   = ctx->cid++;
    b->exit = exit;
    if (!f->cfg_head) {
        f->cfg_head = b;
        return 1;
    }

    cfg_block_t* h = f->cfg_head;
    while (h->next) h = h->next;
    h->next = b;
    return 1;
}

int _create_cfg_blocks(cfg_func_t* f, cfg_ctx_t* ctx) {
    hir_block_t* hh = f->entry;
    while (hh) {
        hir_block_t* entry = hh;
        while (hh->next && hh != f->exit && !HIR_CFG_leaders_contains(hh->next, &f->leaders)) {
            hh = hh->next;
        }

        _add_cfg_block(entry, hh, f, ctx);
        if (hh == f->exit) break;
        hh = hh->next;
    }

    return 1;
}

int HIR_build_cfg(hir_ctx_t* hctx, cfg_ctx_t* ctx) {
    if (!hctx || !ctx || !hctx->h) return 0;

    /* Generate functions */
    HIR_CFG_split_by_functions(hctx, ctx);
    HIR_CFG_mark_leaders(ctx);

    /* Basic blocks in functions */
    cfg_func_t* fh = ctx->h;
    while (fh) {
        _create_cfg_blocks(fh, ctx);
        fh = fh->next;
    }

    /* JMP linking */

    return 1;
}
