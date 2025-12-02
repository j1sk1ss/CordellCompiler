#include <lir/constfold.h>

static int _apply_constfold_on_subject(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    if (vi.vdi.defined) {
        s->storage.cnst.value = vi.vdi.definition;
        s->t = LIR_CONSTVAL;
        return 1;
    }

    return 0;
}

int LIR_apply_sparse_const_propagation(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        foreach(cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                lir_subject_t* args[] = { lh->farg, lh->sarg, lh->targ };
                for (int i = 0; i < 3; i++) {
                    if (!args[i]) continue;
                    if (args[i]->t == LIR_ARGLIST) {
                        foreach(lir_subject_t* s, &args[i]->storage.list.h) {
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
