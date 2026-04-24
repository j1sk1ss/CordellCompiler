#include <lir/copyprop.h>

// TODO: docs
static inline int _check_lsubject_home(lir_subject_t* s, lir_block_t* l) {
    return s->home == l;
}

static int _replace_with_copy(lir_block_t* l, map_t* gen) {
    lir_subject_t** args[] = { &l->farg, &l->sarg, &l->targ };
    for (int i = LIR_is_writeop(l->op); i < 3; i++) {
        lir_subject_t** curr = args[i];
        lir_subject_t* dst;
        if (
            *curr && (*curr)->t == LIR_VARIABLE && 
            map_get(gen, (*curr)->storage.var.v_id, (void**)&dst)
        ) {
            if (_check_lsubject_home(*curr, l)) LIR_unload_subject(*curr);
            *curr = LIR_copy_subject(dst);
            (*curr)->home = l;
        }
    }


    return 1;
}

static int _mark_used_var(set_t* use, lir_subject_t* arg) {
    if (!arg) return 0;
    if (arg->t == LIR_VARIABLE) {
        set_add(use, arg->storage.var.v_id);
        return 1;
    }

    if (arg->t == LIR_ARGLIST) {
        foreach (lir_subject_t* it, &arg->storage.list.h) {
            if (it->t == LIR_VARIABLE) {
                set_add(use, it->storage.var.v_id);
            }
        }
    }

    return 1;
}

int LIR_drop_unused_variables(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        set_t use, def;
        map_t def_trg;
        map_init(&def_trg, MAP_NO_CMP);
        set_init(&use, SET_NO_CMP);
        set_init(&def, SET_NO_CMP);
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                if (!lh->unused) {
                    if (LIR_is_writeop(lh->op)) {
                        _mark_used_var(&use, lh->sarg);
                        _mark_used_var(&use, lh->targ);
                    } 
                    else {
                        _mark_used_var(&use, lh->farg);
                        _mark_used_var(&use, lh->sarg);
                        _mark_used_var(&use, lh->targ);
                    }

                    if (lh->op == LIR_iMOV && lh->farg && lh->farg->t == LIR_VARIABLE) {
                        set_add(&def, lh->farg->storage.var.v_id);
                        map_put(&def_trg, lh->farg->storage.var.v_id, lh);
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }

        set_minus_set(&def, &use);
        set_foreach (symbol_id_t unused, &def) {
            lir_block_t* dst;
            if (map_get(&def_trg, unused, (void**)&dst)) dst->unused = 1;
        }

        map_free(&def_trg);
        set_free(&use);
        set_free(&def);
    }

    return 1;
}

int LIR_copy_propagation(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            map_t gen;
            map_init(&gen, MAP_NO_CMP);

            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                switch (lh->op) {
                    case LIR_iMOV: {
                        if (lh->farg->t != LIR_VARIABLE) break;
                        _replace_with_copy(lh, &gen);
                        map_put(&gen, lh->farg->storage.var.v_id, LIR_copy_subject(lh->sarg));
                        break;
                    }
                    default: {
                        if (
                            LIR_is_readop(lh->op) &&
                            lh->op != LIR_aMOV &&
                            lh->op != LIR_REF
                        ) _replace_with_copy(lh, &gen);
                        break;
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }

           map_free_force_op(&gen, (int (*)(void*))LIR_unload_subject);
        }
    }
    
    return 1;
}
