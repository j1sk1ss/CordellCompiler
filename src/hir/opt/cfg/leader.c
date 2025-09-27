#include <hir/opt/cfg.h>

static hir_block_t* _find_label(hir_block_t* h, hir_block_t* e, long lbid) {
    while (h) {
        if (h->op == HIR_MKLB && h->farg->id == lbid) return h;
        if (h == e) break;
        h = h->next;
    }

    return NULL;
}

static int _add_leader(hir_block_t* h, leader_ctx_t* ctx) {
    if (HIR_CFG_leaders_contains(h, ctx)) return 0;
    leader_t* nl = (leader_t*)mm_malloc(sizeof(leader_t));
    if (!nl) return 0;
    str_memset(nl, 0, sizeof(leader_t));
    nl->h = h;
    return list_add(&ctx->lst, nl);
}

int HIR_CFG_leaders_contains(hir_block_t* h, leader_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->lst, &it);
    leader_t* l;
    while ((l = (leader_t*)list_iter_next(&it))) {
        if (l->h == h) return 1;
    }
    
    return 0;
}

int HIR_CFG_unload_leaders(leader_ctx_t* ctx) {
    return list_free_force(&ctx->lst);
}

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        hir_block_t* curr = fb->entry;
        if (!curr) return 0;

        _add_leader(curr, &fb->leaders);
        while (curr) {
            if (curr->op == HIR_MKLB) _add_leader(curr, &fb->leaders);
            // else if (curr->op == HIR_FRET || curr->op == HIR_EXITOP) {
            //     _add_leader(curr, &fb->leaders);
            //     if (curr->next) _add_leader(curr->next, &fb->leaders);
            // }
            else if (HIR_isjmp(curr->op)) {
                hir_block_t* label = NULL;
                if (curr->farg && curr->farg->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->farg->id);
                if (curr->sarg && curr->sarg->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->sarg->id);
                if (curr->targ && curr->targ->t == HIR_LABEL) label = _find_label(fb->entry, fb->exit, curr->targ->id);
                if (label)      _add_leader(label, &fb->leaders);
                if (curr->next) _add_leader(curr->next, &fb->leaders);
            }

            if (curr == fb->exit) break;
            curr = curr->next;
        }
    }

    return 1;
}
