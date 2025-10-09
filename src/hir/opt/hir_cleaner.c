#include <hir/opt/hir_cleaner.h>

static int _check_usage(cfg_func_t* f, long v_id, hir_block_t* decl) {
    hir_block_t* hh = f->entry;
    while (hh) {
        if (hh != decl && !hh->unused) {
            hir_subject_t* nodes[3] = { hh->farg, hh->sarg, hh->targ };
            for (int i = HIR_writeop(hh->op); i < 3; i++) {
                if (!nodes[i]) continue;
                if (nodes[i]->storage.var.v_id == v_id) return 1;
            }
        }

        if (hh == f->exit) break;
        hh = hh->next;
    }

    return 0;
} 

int HIR_CLN_remove_unused_variables(cfg_ctx_t* cctx) {
    int removed = 0;
    do {
        removed = 0;
        list_iter_t fit;
        list_iter_hinit(&cctx->funcs, &fit);
        cfg_func_t* fb;
        while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
            hir_block_t* hh = fb->entry;
            while (hh) {
                if (HIR_writeop(hh->op) && !hh->unused) {
                    if (!_check_usage(fb, hh->farg->storage.var.v_id, hh)) {
                        hh->unused = 1;
                        removed = 1;
                    }
                }

                if (hh == fb->exit) break;
                hh = hh->next;
            }
        }
    } while (removed);
    return 1;
}
