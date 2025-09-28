#include <hir/opt/cfg.h>

static hir_block_t* _find_label(hir_block_t* h, hir_block_t* e, long lbid) {
    while (h) {
        if (h->op == HIR_MKLB && h->farg->id == lbid) return h;
        if (h == e) break;
        h = h->next;
    }

    return NULL;
}

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        hir_block_t* curr = fb->entry;
        if (!curr) return 0;

        set_add_addr(&fb->leaders, curr);
        while (curr) {
            if (curr->op == HIR_MKLB) set_add_addr(&fb->leaders, curr);
            else if (HIR_isterm(curr->op)) set_add_addr(&fb->terminators, curr);
            else if (HIR_isjmp(curr->op)) {
                hir_block_t* label = NULL;
                if (curr->farg && curr->farg->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->farg->id);
                if (curr->sarg && curr->sarg->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->sarg->id);
                if (curr->targ && curr->targ->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->targ->id);
                if (label) set_add_addr(&fb->leaders, label);
                if (curr->next) {
                    set_add_addr(&fb->leaders, curr->next);
                }
            }

            if (curr == fb->exit) break;
            curr = curr->next;
        }
    }

    return 1;
}
