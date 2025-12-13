/* dfg.c - Compute all stuff for liveness analysis.
IN  - Live variables from previous blocks
OUT - Live variables after this block
DEF - All new variables that defined first time
USE - All variables that has been readed by someone

IN  = union(USE, (DEF - OUT))
OUT = union(IN successors)
*/

#include <lir/dfg.h>

int LIR_DFG_collect_defs(cfg_ctx_t* cctx) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        foreach(cfg_block_t* cb, &fb->blocks) {
            lir_block_t* hl = cb->lmap.entry;
            while (hl) {
                if (!hl->unused && LIR_writeop(hl->op)) {
                    if (hl->farg->t == LIR_VARIABLE) set_add(&cb->def, (void*)hl->farg->storage.var.v_id);
                }

                if (hl == cb->lmap.exit) break;
                hl = hl->next;
            }
        }
    }

    return 1;
}

int LIR_DFG_collect_uses(cfg_ctx_t* cctx) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        foreach(cfg_block_t* cb, &fb->blocks) {
            lir_block_t* lh = cb->lmap.entry;
            while (lh) {
                lir_subject_t* args[3] = { lh->farg, lh->sarg, lh->targ };
                for (int i = LIR_writeop(lh->op); i < 3; i++) {
                    if (!args[i]) continue;
                    switch (args[i]->t) {
                        case LIR_VARIABLE: set_add(&cb->use, (void*)args[i]->storage.var.v_id); break;
                        case LIR_ARGLIST: {
                            foreach(lir_subject_t* arg, &args[i]->storage.list.h) {
                                set_add(&cb->use, (void*)arg->storage.var.v_id);
                            }

                            break;
                        }

                        default: break;
                    }
                }

                lh = LIR_get_next(lh, cb->lmap.exit, 1);
            }
        }
    }

    return 1;
}

static int _compute_out(cfg_block_t* cfg) {
    set_t out;
    set_init(&out, SET_CMP);
    if (cfg->l)   set_union(&out, &out, &cfg->l->curr_in);
    if (cfg->jmp) set_union(&out, &out, &cfg->jmp->curr_in);
    set_free(&cfg->curr_out);
    set_copy(&cfg->curr_out, &out);
    set_free(&out);
    return 1;
}

static int _compute_in(cfg_block_t* cfg) {
    set_t tmp;
    set_copy(&tmp, &cfg->curr_out);
    set_minus_set(&tmp, &cfg->def);
    set_free(&cfg->curr_in);
    set_union(&cfg->curr_in, &cfg->use, &tmp);
    set_free(&tmp);
    return 1;
}

int LIR_DFG_compute_inout(cfg_ctx_t* cctx) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        while (1) {
            list_iter_t bit;
            list_iter_tinit(&fb->blocks, &bit);
            cfg_block_t* cb;
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                _compute_out(cb);
                _compute_in(cb);
            }

            int same = 1;
            list_iter_tinit(&fb->blocks, &bit);
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                if (
                    !set_equal(&cb->curr_in, &cb->prev_in) || 
                    !set_equal(&cb->curr_out, &cb->prev_out)
                ) {
                    same = 0;
                    break;
                }
            }

            if (same) break;
            list_iter_tinit(&fb->blocks, &bit);
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                set_free(&cb->prev_in);
                set_copy(&cb->prev_in, &cb->curr_in);
                set_free(&cb->prev_out);
                set_copy(&cb->prev_out, &cb->curr_out);
            }
        }
    }

    return 1;
}
