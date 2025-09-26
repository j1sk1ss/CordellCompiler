#include <hir/opt/dfg.h>

int HIR_DFG_collect_defs(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        while (bh) {
            set_init(&bh->def);
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op == HIR_PHI && HIR_is_vartype(hh->targ->t)) set_add_int(&bh->def, hh->targ->storage.var.v_id);
                else if (HIR_writeop(hh->op) && HIR_is_vartype(hh->farg->t)) set_add_int(&bh->def, hh->farg->storage.var.v_id);
                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->next;
        }

        fh = fh->next;
    }

    return 1;
}

int HIR_DFG_collect_uses(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        while (bh) {
            set_init(&bh->use);
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op != HIR_PHI) {
                    hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
                    for (int i = HIR_writeop(hh->op); i < 3; i++) {
                        if (args[i] && HIR_is_vartype(args[i]->t)) {
                            set_add_int(&bh->use, args[i]->storage.var.v_id);
                        }
                    }
                }

                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->next;
        }

        fh = fh->next;
    }

    return 1;
}
