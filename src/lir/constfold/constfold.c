#include <lir/constfold.h>

int LIR_apply_sparse_const_propagation(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                lir_subject_t* args[] = { lh->farg, lh->sarg, lh->targ };
                for (int i = 0; i < 3; i++) {
                    if (!args[i]) continue;
                    if (args[i]->t != LIR_VARIABLE) continue;
                    variable_info_t vi;
                    if (!VRTB_get_info_id(args[i]->storage.var.v_id, &vi, &smt->v)) continue;
                    if (vi.vdi.defined) {
                        args[i]->t = LIR_CONSTVAL;
                        args[i]->storage.cnst.value = vi.vdi.definition;
                        continue;
                    }
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}
