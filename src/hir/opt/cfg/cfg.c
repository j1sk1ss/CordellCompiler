/*
cfg.c - Create CFG
*/

#include <hir/opt/cfg.h>

cfg_block_t* CFG_create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->entry = e;
    block->exit  = e;
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

int CFG_insert_cfg_block(cfg_func_t* f, cfg_block_t* b, cfg_block_t* next) {
    if (!f || !b || !next) return 0;

    list_add(&f->blocks, b);

    b->l   = next;
    b->jmp = NULL;

    set_iter_t pit;
    set_iter_init(&next->pred, &pit);
    cfg_block_t* p;
    while (set_iter_next(&pit, (void**)&p)) {
        if (p->l == next)   p->l   = b;
        if (p->jmp == next) p->jmp = b;
        set_add(&b->pred, p);
    }

    set_free(&next->pred);
    set_init(&next->pred);
    set_add(&next->pred, b);
    return 1;
}

static int _add_cfg_block(hir_block_t* entry, hir_block_t* exit, cfg_func_t* f, cfg_ctx_t* ctx) {
    cfg_block_t* b = CFG_create_cfg_block(entry);
    if (!b) return 0;
    b->id    = ctx->cid++;
    b->exit  = exit;
    b->pfunc = f;
    return list_add(&f->blocks, b);
}
// #define DRAGONBOOK_CFG_LEADER
int CFG_create_cfg_blocks(cfg_func_t* f, cfg_ctx_t* ctx) {
    int term = 0;
    hir_block_t* hh = f->entry;
    while (hh) {
        hir_block_t* entry = hh;
#ifdef DRAGONBOOK_CFG_LEADER
        if (term) {
            while (hh && hh != f->exit && !set_has(&f->leaders, hh)) hh = hh->next;
            if (hh) {
                entry = hh;
                if (hh != f->exit) hh = hh->next;
            }

            term = 0;
        }

        while (hh->next && hh != f->exit && !set_has(&f->leaders, hh->next)) {
            if (set_has(&f->terminators, hh)) {
                term = 1;
                break;
            }

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
            switch (cb->exit->op) {
                case HIR_FRET: 
                case HIR_FEND:
                case HIR_STEND: 
                case HIR_EXITOP: break;

                case HIR_JMP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->exit->farg->id);
                    cb->jmp = lb;
                    break;
                }

                case HIR_IFOP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->exit->sarg->id);
                    cb->l   = (cfg_block_t*)list_iter_current(&bit);
                    cb->jmp = lb;
                    break;
                }

                case HIR_IFLGOP:
                case HIR_IFLGEOP:
                case HIR_IFLWOP:
                case HIR_IFLWEOP:
                case HIR_IFCPOP:
                case HIR_IFNCPOP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->exit->targ->id);
                    cb->l   = (cfg_block_t*)list_iter_current(&bit);
                    cb->jmp = lb;
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
        set_free(&fb->terminators);
        list_free_force(&fb->blocks);
        mm_free(fb);
    }

    list_free_force(&ctx->funcs);
    return 1;
}
