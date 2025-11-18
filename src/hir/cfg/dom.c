/* dom.c - Compute dominance, strict dominance and dominance frontier
*/

#include <hir/cfg.h>

int HIR_CFG_compute_dom(cfg_func_t* fb) {
    list_iter_t bit;
    list_iter_hinit(&fb->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) set_add(&cb->dom, cb);
        else {
            list_iter_t bbit;
            list_iter_hinit(&fb->blocks, &bbit);
            cfg_block_t* ccb;
            while ((ccb = (cfg_block_t*)list_iter_next(&bbit))) {
                set_add(&cb->dom, ccb);
            }
        }
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        list_iter_hinit(&fb->blocks, &bit);
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) continue;

            set_t nd;
            set_init(&nd);
            int first = 1;

            set_iter_t it;
            set_iter_init(&cb->pred, &it);
            cfg_block_t* p;
            while (set_iter_next(&it, (void**)&p)) {
                if (first) {
                    set_copy(&nd, &p->dom);
                    first = 0;   
                }
                else {
                    set_t tmp;
                    set_init(&tmp);
                    set_intersect(&tmp, &nd, &p->dom);
                    set_free(&nd);
                    set_copy(&nd, &tmp);
                    set_free(&tmp);
                }
            }

            if (first) {
                set_free(&nd);
                set_init(&nd);
            }

            set_add(&nd, cb);
            if (!set_equal(&nd, &cb->dom)) {
                set_free(&cb->dom);
                set_copy(&cb->dom, &nd);
                changed = 1;
            }

            set_free(&nd);
        }
    }

    return 1;
}

int HIR_CFG_compute_sdom(cfg_func_t* fb) {
    list_iter_t bit;
    list_iter_hinit(&fb->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) {
            cb->sdom = NULL;
            continue;
        }

        cfg_block_t* sdom = NULL;

        set_iter_t it;
        set_iter_init(&cb->dom, &it);
        cfg_block_t* d;
        while (set_iter_next(&it, (void**)&d)) {
            if (d == cb) continue;
            int dominated_by_other = 0;

            set_iter_t it2;
            set_iter_init(&cb->dom, &it2);
            cfg_block_t* other;
            while (set_iter_next(&it2, (void**)&other)) {
                if (other == cb || other == d) continue;
                if (set_has(&other->dom, d)) {
                    dominated_by_other = 1;
                    break;
                }
            }

            if (!dominated_by_other) {
                sdom = d;
                break;
            }
        }

        cb->sdom = sdom;
    }

    return 1;
}

static int _build_domtree(cfg_func_t* fb) {
    list_iter_t bit;
    list_iter_hinit(&fb->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (!cb->sdom || cb->sdom == cb) continue;
        cb->dom_s = cb->sdom->dom_c;
        cb->sdom->dom_c = cb;
    }

    return 1;
}

static void _compute_domf_rec(cfg_block_t* b) {
    if (b->l && b->l->sdom != b)     set_add(&b->domf, b->l);
    if (b->jmp && b->jmp->sdom != b) set_add(&b->domf, b->jmp);

    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        _compute_domf_rec(c);

        set_iter_t it;
        set_iter_init(&c->domf, &it);
        cfg_block_t* w;
        while (set_iter_next(&it, (void**)&w)) {
            if (w->sdom != b) set_add(&b->domf, w);
        }
    }
}

int HIR_CFG_compute_domf(cfg_func_t* fb) {
    _build_domtree(fb);
    _compute_domf_rec(list_get_head(&fb->blocks));
    return 1;
}

int HIR_CFG_collect_defs_by_id(long v_id, cfg_ctx_t* cctx, set_t* out) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            int has_def = 0;
            hir_block_t* hh = cb->hmap.entry;
            while (hh) {
                if (HIR_writeop(hh->op)) {
                    if (hh->farg && HIR_is_vartype(hh->farg->t) && !HIR_is_tmptype(hh->farg->t)) {
                        if (hh->farg->storage.var.v_id == v_id) {
                            has_def = 1;
                            break;
                        }
                    }
                }
                
                if (hh == cb->hmap.exit) break;
                hh = hh->next;
            }

            if (has_def) {
                set_add(out, cb);
            }
        }
    }

    return 1;
}

int HIR_CFG_create_domdata(cfg_ctx_t* cctx) {
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        if (!fb->used) continue;
        HIR_CFG_compute_dom(fb);
        HIR_CFG_compute_sdom(fb);
        HIR_CFG_compute_domf(fb);
    }

    return 1;
}

int HIR_CFG_unload_domdata(cfg_ctx_t* cctx) {
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_free(&cb->dom);
            set_init(&cb->dom);

            set_free(&cb->domf);
            set_init(&cb->domf);
            
            cb->sdom = cb->dom_c = cb->dom_s = NULL;
        }
    }

    return 1;
}
