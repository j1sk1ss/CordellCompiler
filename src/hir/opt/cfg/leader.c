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
    leader_t* l = ctx->h;
    while (l) {
        if (l->h == h) return 1;
        l = l->next;
    }

    leader_t* nl = (leader_t*)mm_malloc(sizeof(leader_t));
    if (!nl) return 0;
    str_memset(nl, 0, sizeof(leader_t));

    nl->h = h;
    nl->next = ctx->h;
    ctx->h = nl;
    return 1;
}

int HIR_CFG_leaders_contains(hir_block_t* h, leader_ctx_t* ctx) {
    leader_t* l = ctx->h;
    while (l) {
        if (l->h == h) return 1;
        l = l->next;
    }

    return 0;
}

int HIR_CFG_unload_leaders(leader_ctx_t* ctx) {
    leader_t* h = ctx->h;
    while (h) {
        leader_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx) {
    cfg_func_t* cfunc = ctx->h;
    while (cfunc) {
        hir_block_t* curr = cfunc->entry;
        if (!curr) return 0;

        _add_leader(curr, &cfunc->leaders);
        while (curr) {
            if (curr->op == HIR_MKLB) _add_leader(curr, &cfunc->leaders);
            if (HIR_isjmp(curr->op)) {
                hir_block_t* label = NULL;
                if (curr->farg && curr->farg->t == HIR_LABEL) label = _find_label(cfunc->entry, cfunc->exit, curr->farg->id);
                if (curr->sarg && curr->sarg->t == HIR_LABEL) label = _find_label(cfunc->entry, cfunc->exit, curr->sarg->id);
                if (curr->targ && curr->targ->t == HIR_LABEL) label = _find_label(cfunc->entry, cfunc->exit, curr->targ->id);
                if (label)      _add_leader(label, &cfunc->leaders);
                if (curr->next) _add_leader(curr->next, &cfunc->leaders);
            }

            if (curr == cfunc->exit) break;
            curr = curr->next;
        }

        cfunc = cfunc->next;
    }

    return 1;
}
