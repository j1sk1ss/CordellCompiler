/*
dfg.c - Compute all stuff for liveness analysis.
IN  - Live variables from previous blocks
OUT - Live variables after this block
DEF - All new variables that defined first time
USE - All variables that has been readed by someone

IN  = union(USE, (DEF - OUT))
OUT = union(IN successors)
*/

#include <hir/dfg.h>

int HIR_DFG_collect_defs(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_init(&cb->def);
            hir_block_t* hh = cb->entry;
            while (hh) {
                if (!hh->unused) {
                    if (hh->op == HIR_PHI && HIR_is_vartype(hh->sarg->t)) set_add(&cb->def, (void*)hh->sarg->storage.var.v_id);
                    else if (HIR_writeop(hh->op) && HIR_is_vartype(hh->farg->t)) set_add(&cb->def, (void*)hh->farg->storage.var.v_id);
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}

int HIR_DFG_collect_uses(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_init(&cb->use);
            hir_block_t* hh = cb->entry;
            while (hh) {
                if (!hh->unused) {
                    hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
                    for (int i = HIR_writeop(hh->op); i < 3; i++) {
                        if (!args[i]) continue;
                        if (HIR_is_vartype(args[i]->t)) set_add(&cb->use, (void*)args[i]->storage.var.v_id);
                        else if (args[i]->t == HIR_SET) { /* HIR_PHI (arguments in set) */
                            set_iter_t it;
                            set_iter_init(&hh->targ->storage.set.h, &it);
                            int_tuple_t* tpl;
                            while (set_iter_next(&it, (void**)&tpl)) {
                                set_add(&cb->use, (void*)tpl->y);
                            }
                        }
                        else if (args[i]->t == HIR_LIST) { /* ASM_CALL, FUNCCALL, SYSCALL (arguments in list) */
                            list_iter_t it;
                            list_iter_hinit(&hh->targ->storage.list.h, &it);
                            hir_subject_t* s;
                            while ((s = list_iter_next(&it))) {
                                if (!HIR_is_vartype(s->t)) continue;
                                set_add(&cb->use, (void*)s->storage.var.v_id);
                            }
                        }
                    }
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}

static int _compute_out(cfg_block_t* cfg) {
    set_t out;
    set_init(&out);
    if (cfg->l)   set_union(&out, &out, &cfg->l->curr_in);
    if (cfg->jmp) set_union(&out, &out, &cfg->jmp->curr_in);
    set_free(&cfg->curr_out);
    set_copy(&cfg->curr_out, &out);
    set_free(&out);
    return 1;
}

static int _compute_in(cfg_block_t* cfg) {
    set_t tmp;
    set_init(&tmp);
    set_copy(&tmp, &cfg->curr_out);
    set_minus_set(&tmp, &cfg->def);
    set_free(&cfg->curr_in);
    set_union(&cfg->curr_in, &cfg->use, &tmp);
    set_free(&tmp);
    return 1;
}

int HIR_DFG_compute_inout(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
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
                if (!set_equal(&cb->curr_in, &cb->prev_in) || !set_equal(&cb->curr_out, &cb->prev_out)) {
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
