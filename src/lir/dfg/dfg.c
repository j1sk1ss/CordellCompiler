/* dfg.c - Compute all stuff for liveness analysis.
IN  - Live variables from previous blocks
OUT - Live variables after this block
DEF - All new variables that defined first time
USE - All variables that has been readed by someone

IN  = union(USE, (OUT - DEF))
OUT = union(IN successors)
*/

#include <lir/dfg.h>

static int _add_vars_from_subject(set_t* s, lir_subject_t* subj) {
    if (!subj) return 0;
    switch (subj->t) {
        case LIR_VARIABLE: set_add(s, (void*)subj->storage.var.v_id); break;
        case LIR_ARGLIST: {
            foreach (lir_subject_t* arg, &subj->storage.list.h) {
                if (arg && arg->t == LIR_VARIABLE) {
                    set_add(s, (void*)arg->storage.var.v_id);
                }
            }
         
            break;
        }
        default: break;
    }

    return 1;
}

static int _lir_inst_usedef(lir_block_t* lh, set_t* use, set_t* def) {
    set_init(use, SET_CMP);
    set_init(def, SET_CMP);
    if (!lh || lh->unused) return 0;
    if (lh->op == LIR_phiMOV) {
        _add_vars_from_subject(def, lh->farg);
        _add_vars_from_subject(use, lh->sarg);
        return 0;
    }

    lir_subject_t* args[3] = { lh->farg, lh->sarg, lh->targ };
    for (int i = LIR_is_writeop(lh->op); i < 3; i++) {
        _add_vars_from_subject(use, args[i]);
    }

    if (LIR_is_writeop(lh->op)) {
        _add_vars_from_subject(def, lh->farg);
    }

    return 1;
}

int LIR_DFG_compute_usedef(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            set_free(&cb->use);
            set_free(&cb->def);
            set_init(&cb->use, SET_CMP);
            set_init(&cb->def, SET_CMP);

            lir_block_t* lh = LIR_get_next(cb->lmap.entry, cb->lmap.exit, 0);
            while (lh) {
                set_t inst_use, inst_def, real_use;

                _lir_inst_usedef(lh, &inst_use, &inst_def);

                set_copy(&real_use, &inst_use);
                set_minus_set(&real_use, &cb->def);

                set_union(&cb->use, &cb->use, &real_use);
                set_union(&cb->def, &cb->def, &inst_def);

                set_free(&real_use);
                set_free(&inst_use);
                set_free(&inst_def);

                lh = LIR_get_next(lh, cb->lmap.exit, 1);
            }
        }
    }

    return 1;
}

/*
Compute the 'OUT' set during the liveness analysis.
Params:
    - `cfg` - CFG context.

Returns 1 if succeeds.
*/
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

/*
Compute the 'IN' set during the liveness analysis.
Params:
    - `cfg` - CFG context.

Returns 1 if succeeds.
*/
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
    LIR_DFG_compute_usedef(cctx);
    foreach (cfg_func_t* fb, &cctx->funcs) {
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

typedef struct {
    lir_block_t p;
    long        dst_reg;
    long        src_reg;
    int         done;
} phi_copy_t;

// TODO: docs
static inline void _save_payload(lir_block_t* p, lir_block_t* lh) {
    p->op     = lh->op;
    p->farg   = lh->farg;
    p->sarg   = lh->sarg;
    p->targ   = lh->targ;
    p->unused = lh->unused;
}

// TODO: docs
static inline void _load_payload(lir_block_t* lh, lir_block_t* p) {
    lh->op     = p->op;
    lh->farg   = p->farg;
    lh->sarg   = p->sarg;
    lh->targ   = p->targ;
    lh->unused = p->unused;
}

// TODO: docs
static int _subj_reg(lir_subject_t* s, map_t* colors, long* reg) {
    if (!s) return 0;
    if (s->t == LIR_VARIABLE) {
        void* color = NULL;
        long v = s->storage.var.v_id;
        if (!map_get(colors, v, &color)) return 0;
        *reg = (long)color;
        return 1;
    }

    if (s->t == LIR_REGISTER) {
        *reg = s->storage.reg.reg;
        return 1;
    }

    return 0;
}

// TODO: docs
static int _dst_used_as_src(phi_copy_t* copies, int n, int i) {
    for (int j = 0; j < n; j++) {
        if (i == j || copies[j].done) continue;
        if (copies[i].dst_reg == copies[j].src_reg) return 1;
    }

    return 0;
}

// TODO: docs
static int _sort_phi_array(lir_block_t** nodes, int n, map_t* colors) {
    phi_copy_t* copies = (phi_copy_t*)mm_malloc(sizeof(phi_copy_t) * n);
    if (!copies) return 0;

    for (int i = 0; i < n; i++) {
        _save_payload(&copies[i].p, nodes[i]);
        copies[i].done = 0;
        if (!_subj_reg(nodes[i]->farg, colors, &copies[i].dst_reg)) {
            copies[i].dst_reg = -1;
        }

        if (!_subj_reg(nodes[i]->sarg, colors, &copies[i].src_reg)) {
            copies[i].src_reg = -1;
        }
    }

    int pos = 0;
    while (pos < n) {
        int found = -1;
        for (int i = 0; i < n; i++) {
            if (copies[i].done) continue;
            if (copies[i].dst_reg == copies[i].src_reg) {
                found = i;
                break;
            }

            if (!_dst_used_as_src(copies, n, i)) {
                found = i;
                break;
            }
        }

        if (found < 0) {
            mm_free(copies);
            return 0;
        }

        _load_payload(nodes[pos], &copies[found].p);
        copies[found].done = 1;
        pos++;
    }

    mm_free(copies);
    return 1;
}

// TODO: docs
static int _sort_phi_group(lir_block_t* first, lir_block_t* last, map_t* colors) {
    if (!first || first == last) return 1;

    int n = 0;
    lir_block_t* lh = first;
    while (lh && lh != last && lh->op == LIR_phiMOV && !lh->unused) {
        lir_block_t* next = LIR_get_next(lh, last, 1);
        if (next == lh) return 0;
        lh = next;
        n++;
    }

    if (n <= 1) return 1;
    lir_block_t** nodes = (lir_block_t**)mm_malloc(sizeof(lir_block_t*) * n);
    if (!nodes) return 0;

    lh = first;
    for (int i = 0; i < n; i++) {
        nodes[i] = lh;
        lh = LIR_get_next(lh, last, 1);
    }

    int ok = _sort_phi_array(nodes, n, colors);
    mm_free(nodes);
    return ok;
}

int LIR_RA_sort_phi_movs(cfg_ctx_t* cctx, map_t* colors) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(cb->lmap.entry, cb->lmap.exit, 0);
            while (lh && lh != cb->lmap.exit) {
                if (lh->op != LIR_phiMOV || lh->unused) {
                    lh = LIR_get_next(lh, cb->lmap.exit, 1);
                    continue;
                }

                lir_block_t* first = lh;
                lir_block_t* after = lh;
                while (
                    after &&
                    after != cb->lmap.exit &&
                    after->op == LIR_phiMOV &&
                    !after->unused
                ) after = LIR_get_next(after, cb->lmap.exit, 1);
                
                if (first != after && !_sort_phi_group(first, after, colors)) {
                    return 0;
                }

                lh = after;
            }
        }
    }

    return 1;
}
