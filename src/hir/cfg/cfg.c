/*
cfg.c - Create CFG
*/

#include <hir/cfg.h>

int HIR_CFG_append_hir_block_front(cfg_block_t* bb, hir_block_t* hh) {
    if (bb->hmap.entry && bb->hmap.exit) bb->hmap.entry = hh;
    if (!bb->hmap.entry) bb->hmap.entry = hh;
    if (!bb->hmap.exit)  bb->hmap.exit  = hh;
    return 1;
}

int HIR_CFG_append_hir_block_back(cfg_block_t* bb, hir_block_t* hh) {
    if (bb->hmap.entry && bb->hmap.exit) bb->hmap.exit = hh;
    if (!bb->hmap.entry) bb->hmap.entry = hh;
    if (!bb->hmap.exit)  bb->hmap.exit  = hh;
    return 1;
}

int HIR_CFG_remove_hir_block(cfg_block_t* bb, hir_block_t* hh) {
    if (!bb || !hh) return 0;
    if (bb->hmap.entry == bb->hmap.exit) {
        bb->hmap.entry = bb->hmap.exit = NULL;
        return 1;
    }
    
    if (bb->hmap.entry == hh) bb->hmap.entry = hh->next;
    if (bb->hmap.exit == hh)  bb->hmap.exit = hh->prev;
    return 1;
}

int HIR_CFG_append_lir_block_front(cfg_block_t* bb, lir_block_t* hh) {
    if (bb->lmap.entry && bb->lmap.exit) bb->lmap.entry = hh;
    if (!bb->lmap.entry) bb->lmap.entry = hh;
    if (!bb->lmap.exit)  bb->lmap.exit  = hh;
    return 1;
}

int HIR_CFG_append_lir_block_back(cfg_block_t* bb, lir_block_t* hh) {
    if (bb->lmap.entry && bb->lmap.exit) bb->lmap.exit = hh;
    if (!bb->lmap.entry) bb->lmap.entry = hh;
    if (!bb->lmap.exit)  bb->lmap.exit  = hh;
    return 1;
}

int HIR_CFG_remove_lir_block(cfg_block_t* bb, lir_block_t* hh) {
    if (!bb || !hh) return 0;
    if (bb->lmap.entry == bb->lmap.exit) {
        bb->lmap.entry = bb->lmap.exit = NULL;
        return 1;
    }
    
    if (bb->lmap.entry == hh) bb->lmap.entry = hh->next;
    if (bb->lmap.exit == hh)  bb->lmap.exit = hh->prev;
    return 1;
}

cfg_block_t* HIR_CFG_create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->type       = CFG_DEFAULT_BLOCK;
    block->hmap.entry = e;
    block->hmap.exit  = e;
    set_init(&block->visitors);
    set_init(&block->pred);
    set_init(&block->curr_in);
    set_init(&block->curr_out);
    set_init(&block->prev_in);
    set_init(&block->prev_out);
    set_init(&block->def);
    set_init(&block->use);
    set_init(&block->domf);
    set_init(&block->dom);
    return block;
}

int HIR_CFG_insert_cfg_block_before(cfg_func_t* f, cfg_block_t* b, cfg_block_t* trg) {
    if (!f || !b || !trg) return 0;
    list_add(&f->blocks, b);
    b->l = trg;

    set_iter_t it;
    set_iter_init(&trg->pred, &it);
    cfg_block_t* p;
    while (set_iter_next(&it, (void**)&p)) {
        if (p->l && p->l == trg)     p->l   = b;
        if (p->jmp && p->jmp == trg) p->jmp = b;
    }

    set_copy(&b->pred, &trg->pred);
    set_free(&trg->pred);

    set_init(&trg->pred);
    set_add(&trg->pred, b);
    return 1;
}

static int _add_cfg_block(hir_block_t* entry, hir_block_t* exit, cfg_func_t* f, cfg_ctx_t* ctx) {
    cfg_block_t* b = HIR_CFG_create_cfg_block(entry);
    if (!b) return 0;
    b->id        = ctx->cid++;
    b->hmap.exit = exit;
    b->pfunc     = f;
    return list_add(&f->blocks, b);
}

#define DRAGONBOOK_CFG_LEADER
int CFG_create_cfg_blocks(cfg_func_t* f, cfg_ctx_t* ctx) {
    int term = 0;
    hir_block_t* hh = f->entry;
    while (hh) {
        hir_block_t* entry = hh;
#ifdef DRAGONBOOK_CFG_LEADER
        while (hh->next && hh != f->exit && !set_has(&f->leaders, hh->next)) {
            hh = hh->next;
        }

        _add_cfg_block(entry, hh, f, ctx);
#else
        if (!HIR_issyst(entry->op)) _add_cfg_block(entry, entry, f, ctx);
#endif
        if (hh == f->exit) break;
        hh = hh->next;
    }

    return 1;
}

int HIR_CFG_build(hir_ctx_t* hctx, cfg_ctx_t* ctx) {
    if (!hctx || !ctx || !hctx->h) return 0;

    HIR_CFG_split_by_functions(hctx, ctx);
    HIR_CFG_mark_leaders(ctx);

    list_iter_t fit;
    list_iter_hinit(&ctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        CFG_create_cfg_blocks(fb, ctx);

        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            switch (cb->hmap.exit->op) {
                case HIR_FRET: 
                case HIR_FEND:
                case HIR_STEND: 
                case HIR_EXITOP: break;

                case HIR_JMP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->hmap.exit->farg->id);
                    cb->jmp = lb;
                    break;
                }

                case HIR_IFOP2: {
                    cb->l   = HIR_CFG_function_findlb(fb, cb->hmap.exit->sarg->id);
                    cb->jmp = HIR_CFG_function_findlb(fb, cb->hmap.exit->targ->id);
                    break;
                }

                default: cb->l = (cfg_block_t*)list_iter_current(&bit); break;
            }
        }
    }

    list_iter_hinit(&ctx->funcs, &fit);
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb->l)   set_add(&cb->l->pred, cb);
            if (cb->jmp) set_add(&cb->jmp->pred, cb);
        }
    }

    return 1;
}

int HIR_CFG_cleanup_blocks_temporaries(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_free(&cb->prev_in);
            set_init(&cb->prev_in);
            
            set_free(&cb->prev_out);
            set_init(&cb->prev_out);

            set_free(&cb->visitors);
            set_init(&cb->visitors);
            cb->visited = 0;
        }

        set_free(&fb->leaders);
        set_init(&fb->leaders);
    }

    return 1;
}

int HIR_CFG_unload(cfg_ctx_t* ctx) {
    list_iter_t fit;
    list_iter_hinit(&ctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_free(&cb->def);
            set_free(&cb->use);
            set_free(&cb->curr_in);
            set_free(&cb->curr_out);
            set_free(&cb->prev_in);
            set_free(&cb->prev_out);
            set_free(&cb->dom);
            set_free(&cb->domf);
            set_free(&cb->pred);
            set_free(&cb->visitors);
            mm_free(cb);
        }

        set_free(&fb->leaders);
        list_free_force(&fb->blocks);
        mm_free(fb);
    }

    list_free_force(&ctx->funcs);
    return 1;
}
