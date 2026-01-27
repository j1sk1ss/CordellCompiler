/* cfg.c - Create CFG */
#include <hir/cfg.h>

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

cfg_block_t* HIR_CFG_create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->type       = CFG_DEFAULT_BLOCK;
    block->hmap.entry = e;
    block->hmap.exit  = e;
    set_init(&block->visitors, SET_NO_CMP);
    set_init(&block->pred,     SET_NO_CMP);
    set_init(&block->curr_in,  SET_CMP);
    set_init(&block->curr_out, SET_CMP);
    set_init(&block->prev_in,  SET_CMP);
    set_init(&block->prev_out, SET_CMP);
    set_init(&block->def,      SET_NO_CMP);
    set_init(&block->use,      SET_CMP);
    set_init(&block->domf,     SET_NO_CMP);
    set_init(&block->dom,      SET_CMP);
    set_init(&block->phi,      SET_NO_CMP);
    return block;
}

int HIR_CFG_insert_cfg_block_before(cfg_func_t* f, cfg_block_t* b, cfg_block_t* trg) {
    if (!f || !b || !trg) return 0;
    list_add(&f->blocks, b);
    b->l = trg;

    set_foreach (cfg_block_t* p, &trg->pred) {
        if (p->l && p->l == trg)     p->l   = b;
        if (p->jmp && p->jmp == trg) p->jmp = b;
    }

    set_copy(&b->pred, &trg->pred);
    set_free(&trg->pred);

    set_init(&trg->pred, SET_NO_CMP);
    set_add(&trg->pred, b);
    return 1;
}

/*
Add CFG block to the function's blocks list.
Params:
    - `entry` - Block's entry instruction.
    - `exit` - Block's exit instruction.
    - `f` - Basic function.
    - `ctx` - CFG context.

Return 1 if the block was sucesfully appended.
*/
static int _add_cfg_block(hir_block_t* entry, hir_block_t* exit, cfg_func_t* f, cfg_ctx_t* ctx) {
    cfg_block_t* b = HIR_CFG_create_cfg_block(entry);
    if (!b) return 0;
    b->id        = ctx->cid++;
    b->hmap.exit = exit;
    b->pfunc     = f;
    return list_add(&f->blocks, b);
}

#define DRAGONBOOK_CFG_LEADER /* Note: Don't undef this flag given the current compiler's pipeline. */
                              /*       If you want to change the leaders' creation method           */
                              /*       be aware of the possible future issues.                      */
int CFG_create_cfg_blocks(cfg_func_t* f, cfg_ctx_t* ctx) {
    hir_block_t* hh = HIR_get_next(f->hmap.entry, f->hmap.exit, 0);
    while (hh) {
#ifdef DRAGONBOOK_CFG_LEADER
        hir_block_t* entry = hh;
        while ((entry = hh) && hh->next && hh != f->hmap.exit && !set_has(&f->leaders, hh)) hh = hh->next;
        while (hh->next && hh != f->hmap.exit && !set_has(&f->leaders, hh->next)) {
            hh = hh->next;
            if (HIR_isterm(hh->op)) break;
        }

        _add_cfg_block(entry, hh, f, ctx);
#else
        if (!HIR_issyst(hh->op)) _add_cfg_block(hh, hh, f, ctx);
#endif
        hh = HIR_get_next(hh, f->hmap.exit, 1);
    }

    return 1;
}
#undef DRAGONBOOK_CFG_LEADER

int HIR_CFG_build(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt) {
    if (!hctx || !ctx || !hctx->h) return 0;

    list_init(&ctx->funcs);
    list_init(&ctx->out);

    HIR_CFG_split_by_functions(hctx, ctx, smt); /* Split input flatten instructions to          */
                                                /* the list of functions.                       */
    HIR_CFG_mark_leaders(ctx);                  /* Build the leaders' context with the          */
                                                /* given list of functions.                     */

    foreach (cfg_func_t* fb, &ctx->funcs) {
        CFG_create_cfg_blocks(fb, ctx);         /* Create CFG blocks based on the leaders' list */

        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            switch (cb->hmap.exit->op) {
                case HIR_FRET:  case HIR_FEND:
                case HIR_STEND: case HIR_EXITOP: break;
                case HIR_JMP: {
                    cb->jmp = HIR_CFG_function_findlb(fb, cb->hmap.exit->farg->id);
                    break;
                }

                case HIR_IFOP2: {
                    cb->l   = HIR_CFG_function_findlb(fb, cb->hmap.exit->sarg->id);
                    cb->jmp = HIR_CFG_function_findlb(fb, cb->hmap.exit->targ->id);
                    break;
                }

                default: {
                    cb->l = (cfg_block_t*)list_iter_current(&bit); 
                    break;
                }
            }
        }
    }

    foreach (cfg_func_t* fb, &ctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (cb->l)   set_add(&cb->l->pred, cb);
            if (cb->jmp) set_add(&cb->jmp->pred, cb);
        }
    }

    return 1;
}

int HIR_CFG_cleanup_navigation(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_free(&cb->visitors);
            set_init(&cb->visitors, SET_NO_CMP);
            cb->visited = 0;
        }
    }

    return 1;
}

int HIR_CFG_cleanup_blocks_temporaries(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_free(&cb->prev_in);
            set_init(&cb->prev_in, SET_CMP);
            
            set_free(&cb->prev_out);
            set_init(&cb->prev_out, SET_CMP);
        }

        set_free(&fb->leaders);
        set_init(&fb->leaders, SET_NO_CMP);
    }

    return 1;
}

int HIR_CFG_unload(cfg_ctx_t* ctx) {
    foreach (cfg_func_t* fb, &ctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
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
            set_free(&cb->phi);
            mm_free(cb);
        }

        set_free(&fb->leaders);
        list_free_force(&fb->blocks);
        mm_free(fb);
    }

    list_free_force(&ctx->funcs);
    list_free(&ctx->out);
    return 1;
}
