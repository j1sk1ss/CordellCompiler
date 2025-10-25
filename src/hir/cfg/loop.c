#include <hir/cfg.h>

static int _get_loop_blocks(cfg_block_t* entry, cfg_block_t* exit, set_t* b) {
    if (!set_add(b, entry)) return 0;
    if (entry == exit) return 0;

    set_iter_t it;
    set_iter_init(&entry->pred, &it);
    cfg_block_t* bb;
    while (set_iter_next(&it, (void**)&bb)) {
        _get_loop_blocks(bb, exit, b);
    }

    return 1;
}

static int _insert_preheader(cfg_ctx_t* cctx, cfg_block_t* header, set_t* loop) {
    cfg_block_t* preheader = CFG_create_cfg_block(NULL);
    preheader->id = cctx->cid++;

    list_add(&header->pfunc->blocks, preheader);
    preheader->l = header;

    set_iter_t it;
    set_iter_init(&header->pred, &it);
    cfg_block_t* p;
    while (set_iter_next(&it, (void**)&p)) {
        if (set_has(loop, p)) continue;
        if (p->l && p->l == header)     p->l   = preheader;
        if (p->jmp && p->jmp == header) p->jmp = preheader;
        set_add(&preheader->pred, p);
        set_remove(&header->pred, p);
    }

    return 1;
}

int HIR_CFG_loop_canonicalization(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb->jmp && set_has(&cb->dom, cb->jmp)) {
                cfg_block_t* header = cb->jmp;
                cfg_block_t* latch  = cb;

                set_t loop_blocks;
                set_init(&loop_blocks);
                _get_loop_blocks(latch, header, &loop_blocks);
                set_add(&loop_blocks, header);

                _insert_preheader(cctx, header, &loop_blocks);
                set_free(&loop_blocks);
            }
        }
    }

    return 1;
}
