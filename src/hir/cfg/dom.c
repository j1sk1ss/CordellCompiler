/* dom.c - Compute dominance, strict dominance and dominance frontier */
#include <hir/cfg.h>

int HIR_CFG_compute_dom(cfg_func_t* fb) {
    foreach (cfg_block_t* cb, &fb->blocks) {
        if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) set_add(&cb->dom, cb);
        else {
            foreach (cfg_block_t* ccb, &fb->blocks) {
                set_add(&cb->dom, ccb);
            }
        }
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) {
                continue;
            }

            set_t nd;
            set_init(&nd, SET_CMP);

            int first = 1;
            set_foreach (cfg_block_t* p, &cb->pred) {
                if (first) {
                    set_free(&nd);
                    set_copy(&nd, &p->dom);
                    first = 0;   
                }
                else {
                    set_t tmp;
                    set_init(&tmp, SET_CMP);
                    set_intersect(&tmp, &nd, &p->dom);
                    set_free(&nd);
                    set_copy(&nd, &tmp);
                    set_free(&tmp);
                }
            }

            if (first) {
                set_free(&nd);
                set_init(&nd, SET_CMP);
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
    foreach (cfg_block_t* cb, &fb->blocks) {
        if (cb == (cfg_block_t*)list_get_head(&fb->blocks)) {
            cb->sdom = NULL;
            continue;
        }

        cfg_block_t* sdom = NULL;
        set_foreach (cfg_block_t* fd, &cb->dom) {
            if (fd == cb) continue;
            int dominated_by_other = 0;
            set_foreach (cfg_block_t* sd, &cb->dom) {
                if (sd == cb || sd == fd) continue;
                if (set_has(&sd->dom, fd)) {
                    dominated_by_other = 1;
                    break;
                }
            }

            if (!dominated_by_other) {
                sdom = fd;
                break;
            }
        }

        cb->sdom = sdom;
    }

    return 1;
}

/*
Build a dominatiors tree based on the provided CFG's blocks list.
The main idea here is to connect all dominators with their domination.
Note: Additionally, this function connects dominators with each other 
      by sibling pointers.
Params:
    - `fb` - Current function.

Returns 1 if succeed. Otherwise will return 0.
*/
static int _build_domtree(cfg_func_t* fb) {
    foreach (cfg_block_t* cb, &fb->blocks) {
        if (
            !cb->sdom ||    /* If it doesn't have a strict dominant                */
            cb->sdom == cb  /* or it has a strict domination on itself.            */
        ) continue;
        cb->dom_s = cb->sdom->dom_c; /* Sibling is a current child of a dominator. */
        cb->sdom->dom_c = cb;        /* New child of dominator is us.              */
    }

    return 1;
}

/*
Compute the dominance frontier (recursively).
We add dominator as dominance frontier to the related set if:
    - this is a next block for the current block.
    - its dominator isn't the current block.
Params:
    - `b` - Current BasicBlock.
*/
static void _compute_domf_rec(cfg_block_t* b) {
    if (b->l && b->l->sdom != b)     set_add(&b->domf, b->l);
    if (b->jmp && b->jmp->sdom != b) set_add(&b->domf, b->jmp);
    for (cfg_block_t* c = b->dom_c; c; c = c->dom_s) {
        _compute_domf_rec(c);
        set_foreach (cfg_block_t* w, &c->domf) {
            if (w->sdom != b) set_add(&b->domf, w);
        }
    }
}

int HIR_CFG_compute_domf(cfg_func_t* fb) {
    _build_domtree(fb);
    _compute_domf_rec(list_get_head(&fb->blocks));
    return 1;
}

int HIR_CFG_create_domdata(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        HIR_CFG_compute_dom(fb);
        HIR_CFG_compute_sdom(fb);
        HIR_CFG_compute_domf(fb);
    }

    return 1;
}

int HIR_CFG_unload_domdata(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_free(&cb->dom);
            set_init(&cb->dom, SET_CMP);

            set_free(&cb->domf);
            set_init(&cb->domf, SET_NO_CMP);
            
            cb->sdom = cb->dom_c = cb->dom_s = NULL;
        }
    }

    return 1;
}
