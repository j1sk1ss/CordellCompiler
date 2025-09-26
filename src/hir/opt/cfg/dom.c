#include <hir/opt/cfg.h>

int HIR_CFG_compute_dom(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        set_init(&b->dom);
        if (b == func->cfg_head) set_add_addr(&b->dom, b);
        else {
            for (cfg_block_t* t = func->cfg_head; t; t = t->next) {
                set_add_addr(&b->dom, t);
            }
        }
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
            if (b == func->cfg_head) continue;

            set_t nd;
            set_init(&nd);

            int first = 1;
            set_iter_t it;
            set_iter_init(&b->pred, &it);
            cfg_block_t* p = NULL;
            while ((p = set_iter_next_addr(&it))) {
                if (!first) set_intersect_addr(&nd, &nd, &p->dom); 
                else {
                    set_copy_addr(&nd, &p->dom);
                    first = 0;
                }
            }

            set_add_addr(&nd, b);
            if (set_equal_addr(&nd, &b->dom)) set_free(&nd);
            else {
                set_free(&b->dom);
                set_copy_addr(&b->dom, &nd);
                changed = 1;
            }
        }
    }

    return 1;
}

int HIR_CFG_compute_sdom(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        if (b == func->cfg_head) {
            b->sdom = NULL;
            continue;
        }

        set_iter_t it;
        set_iter_init(&b->dom, &it);

        cfg_block_t* sdom = NULL;
        cfg_block_t* dom  = NULL;
        while ((dom = set_iter_next_addr(&it))) {
            if (dom == b) continue;

            int candidate = 1;
            set_iter_t it2;
            set_iter_init(&b->dom, &it2);
            cfg_block_t* secdom = NULL;
            while ((secdom = set_iter_next_addr(&it2))) {
                if (secdom == b || secdom == dom) continue;
                if (set_has_addr(&dom->dom, secdom)) {
                    candidate = 0;
                    break;
                }
            }

            if (candidate) {
                sdom = dom;
                break;
            }
        }

        b->sdom = sdom;
    }

    return 1;
}

static int _build_domtree(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        b->dom_c = NULL;
        b->dom_s = NULL;
    }

    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        if (!b->sdom || b->sdom == b) continue;
        b->dom_s = b->sdom->dom_c;
        b->sdom->dom_c = b;
    }

    return 1;
}

static int _strictly_dominated_by(cfg_block_t* block, cfg_block_t* dominator) {
    cfg_block_t* current = block;
    while (current && current != dominator) current = current->sdom;
    return current != dominator;
}

static int _compute_domf_rec(cfg_block_t* b) {
    if (b->l && _strictly_dominated_by(b->l, b))     set_add_addr(&b->domf, b->l);
    if (b->jmp && _strictly_dominated_by(b->jmp, b)) set_add_addr(&b->domf, b->jmp);
    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        _compute_domf_rec(c);
        set_iter_t it;
        set_iter_init(&c->domf, &it);
        cfg_block_t* y = NULL;
        while ((y = set_iter_next_addr(&it))) {
            if (_strictly_dominated_by(y, b)) set_add_addr(&b->domf, y);
        }
    }

    return 1;
}

int HIR_CFG_compute_domf(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        set_init(&b->domf);
    }

    _build_domtree(func);
    _compute_domf_rec(func->cfg_head);
    return 1;
}

int HIR_CFG_collect_defs(long v_id, cfg_ctx_t* cctx, set_t* out) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        while (bh) {
            int has_def = 0;
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (HIR_writeop(hh->op)) {
                    if (
                        hh->farg && 
                        HIR_is_vartype(hh->farg->t) && 
                        !HIR_is_globtype(hh->farg->t) &&
                        !HIR_is_tmptype(hh->farg->t)    
                    ) {
                        if (hh->farg->storage.var.v_id == v_id) {
                            has_def = 1;
                            break;
                        }
                    }
                }
                
                if (hh == bh->exit) break;
                hh = hh->next;
            }

            if (has_def) {
                set_add_addr(out, bh);
            }

            bh = bh->next;
        }

        fh = fh->next;
    }

    return 1;
}
