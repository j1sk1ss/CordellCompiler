/* deall.c - Create deallocation points in HIR
*/

#include <lir/dfg.h>

static int _already_deallocated(long id, cfg_block_t* bb) {
    lir_block_t* lh = bb->lmap.exit;
    while (lh) {
        if (lh->op == LIR_VRDEALL && lh->farg->storage.cnst.value == id) return 1;
        if (lh == bb->lmap.entry) break;
        lh = lh->prev;
    }

    return 0;
}

int LIR_DFG_create_deall(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_t appeared;
            set_union(&appeared, &cb->curr_in, &cb->def);

            set_foreach (long vid, &appeared) {
                if (set_has(&cb->curr_out, (void*)vid)) continue;

                set_t owners;
                int hasown = 0;
                if (ALLIAS_get_owners(vid, &owners, &smt->m)) {
                    set_foreach (long svid, &owners) {
                        if (set_has(&cb->curr_out, (void*)svid)) {
                            hasown = 1;
                            break;
                        }
                    }
                }

                set_free(&owners);
                if (hasown) continue;
                if (!_already_deallocated(vid, cb)) {
                    LIR_insert_block_before(LIR_create_block(LIR_VRDEALL, LIR_SUBJ_CONST(vid), NULL, NULL), cb->lmap.exit);
                }

                map_foreach (allias_t* al, &smt->m.allias) {
                    if (!set_has(&al->owners, (void*)vid)) continue;
                    if (ALLIAS_mark_owner(al->v_id, vid, &smt->m)) {
                        if (!_already_deallocated(al->v_id, cb)) {
                            LIR_insert_block_before(LIR_create_block(LIR_VRDEALL, LIR_SUBJ_CONST(al->v_id), NULL, NULL), cb->lmap.exit);
                        }

                        set_free_force(&al->delown);
                        set_init(&al->delown, SET_NO_CMP);
                    }

                    break;
                }
            }

            set_free(&appeared);
        }
    }

    return 1;
}
