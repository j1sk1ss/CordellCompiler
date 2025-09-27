#include <hir/opt/cfg.h>

static cfg_block_t* _create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->entry = e;

    set_init(&block->visitors);
    set_init(&block->pred);
    set_init(&block->curr_in);
    set_init(&block->curr_out);
    set_init(&block->prev_in);
    set_init(&block->prev_out);
    set_init(&block->def);
    set_init(&block->use);
    
    return block;
}

static int _add_cfg_block(hir_block_t* entry, hir_block_t* exit, cfg_func_t* f, cfg_ctx_t* ctx) {
    cfg_block_t* b = _create_cfg_block(entry);
    if (!b) return 0;
    b->id   = ctx->cid++;
    b->exit = exit;
    
    cfg_block_t* p = list_get_tail(&f->blocks);
    if (p) p->l = b;

    return list_add(&f->blocks, b);
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

    HIR_CFG_split_by_functions(hctx, ctx);
    HIR_CFG_mark_leaders(ctx);

    list_iter_t fit;
    list_iter_hinit(&ctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        _create_cfg_blocks(fb, ctx);

        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            switch (cb->exit->op) {
                case HIR_FRET: 
                case HIR_FEND:
                case HIR_STEND: cb->l = NULL; break;
                case HIR_JMP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->exit->farg->id);
                    cb->jmp = lb;
                    cb->l = NULL;
                    break;
                }

                case HIR_IFOP: {
                    cfg_block_t* lb = HIR_CFG_function_findlb(fb, cb->exit->sarg->id);
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
                    cb->jmp = lb;
                    break;
                }
            }
        }
    }

    list_iter_hinit(&ctx->funcs, &fit);
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb->l)   set_add_addr(&cb->l->pred, cb);
            if (cb->jmp) set_add_addr(&cb->jmp->pred, cb);
        }
    }

    return 1;
}
