/*
deall.c - Create deallocation points in HIR
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
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_t appeared;
            set_init(&appeared);
            set_union(&appeared, &cb->curr_in, &cb->def);

            set_iter_t init;
            set_iter_init(&appeared, &init);
            long vid;
            while (set_iter_next(&init, (void**)&vid)) {
                if (set_has(&cb->curr_out, (void*)vid)) continue;

                set_t owners;
                int hasown = 0;
                if (ALLIAS_get_owners(vid, &owners, &smt->m)) {
                    set_iter_t ownersit;
                    set_iter_init(&owners, &ownersit);
                    while (set_iter_next(&ownersit, (void**)&vid)) {
                        if (set_has(&cb->curr_out, (void*)vid)) {
                            hasown = 1;
                            break;
                        }
                    }
                }

                set_free_force(&owners);
                if (hasown) continue;
                if (!_already_deallocated(vid, cb)) {
                    LIR_insert_block_before(LIR_create_block(LIR_VRDEALL, LIR_SUBJ_CONST(vid), NULL, NULL), cb->lmap.exit);
                }

                map_iter_t mit;
                map_iter_init(&smt->m.allias, &mit);
                allias_t* al;
                while (map_iter_next(&mit, (void**)&al)) {
                    if (!set_has(&al->owners, (void*)vid)) continue;
                    if (ALLIAS_mark_owner(al->v_id, vid, &smt->m)) {
                        if (!_already_deallocated(al->v_id, cb)) {
                            LIR_insert_block_before(LIR_create_block(LIR_VRDEALL, LIR_SUBJ_CONST(al->v_id), NULL, NULL), cb->lmap.exit);
                        }

                        set_free_force(&al->delown);
                        set_init(&al->delown);
                    }

                    break;
                }
            }

            set_free(&appeared);
        }
    }

    return 1;
}
