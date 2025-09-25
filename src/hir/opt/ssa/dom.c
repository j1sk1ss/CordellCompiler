#include <hir/opt/ssa.h>

static void HIR_SSA_collect_defs(long v_id, cfg_ctx_t* cctx, set_t* out) {
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
                        if (hh->farg && HIR_is_vartype(hh->farg->t)) {
                            if (hh->farg->storage.var.v_id == v_id) {
                                has_def = 1;
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

            if (bh->jmp) bh = bh->jmp;
            else bh = bh->l;
        }

        fh = fh->next;
    }
}
