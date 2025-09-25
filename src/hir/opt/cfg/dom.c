#include <hir/opt/cfg.h>

int HIR_CFG_compute_dom(cfg_func_t* func) {
    cfg_block_t* entry = func->cfg_head;
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        set_init(&b->dom);
        if (b == entry) set_add_addr(&b->dom, b); 
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
            if (b == entry) continue;

            set_t nd;
            set_init(&nd);

            int first = 1;
            if (b->lpred) {
                if (!first) set_intersect_addr(&nd, &nd, &b->lpred->dom);
                else {
                    set_copy_addr(&nd, &b->lpred->dom);
                    first = 0;
                }
            }

            set_iter_t it;
            cfg_block_t* p;
            set_iter_init(&b->jmppred, &it);
            while ((p = set_iter_next_addr(&it))) {
                if (first) set_intersect_addr(&nd, &nd, &p->dom); 
                else {
                    set_copy_addr(&nd, &p->dom);
                    first = 0;
                }
            }

            set_add_addr(&nd, b);
            if (set_equal_addr(&nd, &b->dom)) set_free(&nd);
            else {
                set_free(&b->dom);
                b->dom = nd;
                changed = 1;
            }
        }
    }

    return 1;
}

int HIR_CFG_compute_idom(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        if (b == func->cfg_head) {
            b->idom = NULL;
            continue;
        }

        cfg_block_t* idom = NULL;
        set_iter_t it;
        set_iter_init(&b->dom, &it);
        cfg_block_t* d;
        while ((d = set_iter_next_addr(&it))) {
            if (d == b) continue;

            int candidate = 1;
            set_iter_t it2;
            set_iter_init(&b->dom, &it2);
            cfg_block_t* d2;
            while ((d2 = set_iter_next_addr(&it2))) {
                if (d2 == b || d2 == d) continue;
                if (set_has_addr(&d->dom, d2)) {
                    candidate = 0;
                    break;
                }
            }

            if (candidate) {
                idom = d;
                break;
            }
        }

        b->idom = idom;
    }

    return 1;
}

static int _build_domtree(cfg_func_t* func) {
    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        b->dom_c = NULL;
        b->dom_s = NULL;
    }

    for (cfg_block_t* b = func->cfg_head; b; b = b->next) {
        if (!b->idom) continue;
        b->dom_s = b->idom->dom_c;
        b->idom->dom_c = b;
    }

    return 1;
}

static int _compute_domf_rec(cfg_block_t* b) {
    cfg_block_t* succs[2] = { b->l, b->jmp };
    for (int i = 0; i < 2; i++) {
        cfg_block_t* s = succs[i];
        if (!s) continue;
        if (s->idom != b) {
            set_add_addr(&b->domf, s);
        }
    }

    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        _compute_domf_rec(c);

        set_iter_t it;
        set_iter_init(&c->domf, &it);
        cfg_block_t* y;
        while ((y = set_iter_next_addr(&it))) {
            if (y->idom != b) {
                set_add_addr(&b->domf, y);
            }
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
                switch (hh->op) {
                    case HIR_STORE:
                    case HIR_iADD:
                    case HIR_iSUB:
                    case HIR_iMUL:
                    case HIR_iDIV:
                    case HIR_VARDECL: {
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

                        break;
                    }
                    default: break;
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
