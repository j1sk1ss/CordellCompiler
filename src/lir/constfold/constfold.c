#include <lir/constfold.h>

static int _apply_constfold_on_subject(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    if (vi.vdi.defined) {
        s->t = LIR_CONSTVAL;
        s->storage.cnst.value = vi.vdi.definition;
        return 1;
    }

    return 0;
}

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
                    if (args[i]->t == LIR_ARGLIST) {
                        list_iter_t it;
                        list_iter_hinit(&args[i]->storage.list.h, &it);
                        lir_subject_t* s;
                        while ((s = (lir_subject_t*)list_iter_next(&it))) {
                            _apply_constfold_on_subject(s, smt);
                        }

                        continue;
                    }

                    _apply_constfold_on_subject(args[i], smt);
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}
