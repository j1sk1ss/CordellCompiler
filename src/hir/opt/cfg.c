#include <hir/opt/cfg.h>

hir_block_t* _find_label(hir_block_t* h, long lbid) {
    while (h) {
        if (h->op == HIR_MKLB && h->farg->id == lbid) return h; 
        h = h->next;
    }

    return NULL;
}

static long _blocks = 0;
static cfg_block_t* _create_cfg_block(hir_block_t* e) {
    cfg_block_t* block = (cfg_block_t*)mm_malloc(sizeof(cfg_block_t));
    if (!block) return NULL;
    str_memset(block, 0, sizeof(cfg_block_t));
    block->entry = e;
    block->id    = _blocks++;
    return block;
}

int _find_leaders(hir_block_t* h) {
    while (h) {
        if (
            h->op == HIR_FDCL ||
            h->op == HIR_MKLB ||
            h->op == HIR_IFOP ||
            h->op == HIR_SWITCHOP
        ) {
            cfg_block_t* b = _create_cfg_block(h);
        }

        h = h->next;
    }
}

int HIR_build_cfg(hir_block_t* h, cfg_ctx_t* ctx) {
    hir_block_t* curr = h;
    while (curr) {
        switch (curr->op) {
            case HIR_MKLB: break;
            case HIR_JMP: {
                hir_block_t* label = _find_label(h, h->farg->id);
                break;
            }
        }

        curr = curr->next;
    }

    return 0;
}
