#include <lir/copyprop.h>

/*
Add variables referenced by a subject to the USE set.
Params:
    - `use` - USE set to update.
    - `arg` - LIR subject to inspect.

Returns 1 if subject was handled, otherwise 0.
*/
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
            iterate_lir_instructions (bb) {
                if (!lh->unused) {
                    iterate_lir_args (lir_subject_t* arg, lh, LIR_is_writeop(lh->op)) {
                        _mark_used_var(&use, arg);
                    }

                    switch (lh->op) {
                        case LIR_TF64: case LIR_TF32: 
                        case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8: 
                        case LIR_TU64: case LIR_TU32: case LIR_TU16: case LIR_TU8:
                        case LIR_iMOV: case LIR_aMOV: {                        
                            if (lh->farg && lh->farg->t == LIR_VARIABLE) {
                                set_add(&def, lh->farg->storage.var.v_id);
                                map_put(&def_trg, lh->farg->storage.var.v_id, lh);
                            }

                            break;
                        }
                        default: break;
                    }
                }
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

/*
Replace readable operands with their propagated copy when a mapping exists.
Params:
    - `l` - LIR block to rewrite.
    - `gen` - Map from variable/register keys to copied subjects.
    - `t` - Subject type to replace.

Returns 1 if succeeds.
*/
static int _replace_with_copy(lir_block_t* l, map_t* gen, lir_subject_type_t t) {
    iterate_ref_lir_args (lir_subject_t** curr, l, LIR_is_writeop(l->op)) {
        lir_subject_t* dst;
        if (
            (*curr)->t == t && 
            map_get(
                gen, 
                t == LIR_VARIABLE ? (*curr)->storage.var.v_id : LIR_format_register((*curr)->storage.reg.reg, 1), 
                (void**)&dst
            )
        ) {
            if ((*curr)->home == l) LIR_unload_subject(*curr);
            *curr = LIR_copy_subject(dst);
            (*curr)->home = l;
        }
    }


    return 1;
}

// TODO: docs
static inline int _is_local_variable(lir_subject_t* s, sym_table_t* smt) {
    if (!s || s->t != LIR_VARIABLE) return 0;
    variable_info_t vi;
    return VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) && !vi.vfs.glob;
}

// TODO: docs
static int _is_copy_candidate(lir_block_t* lh, sym_table_t* smt, set_t* addr_taken, long* src_size) {
    if (!lh || !_is_local_variable(lh->farg, smt)) return 0;
    if (set_has(addr_taken, (void*)lh->farg->storage.var.v_id)) return 0;
    long sarg_size = lh->sarg ? lh->sarg->size : 0;
    switch (lh->op) {
        case LIR_TF64: case LIR_TF32:
        case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8:
        case LIR_TU64: case LIR_TU32: case LIR_TU16: case LIR_TU8: {
            if (!lh->sarg || (lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL)) return 0;
            sarg_size = lh->farg->size;
            break;
        }
        case LIR_iMOV: {
            if (!lh->sarg) return 0;
            if (
                lh->sarg->t == LIR_VARIABLE &&
                (
                    !_is_local_variable(lh->sarg, smt) ||
                    set_has(addr_taken, (void*)lh->sarg->storage.var.v_id)
                )
            ) return 0;
            break;
        }
        default: return 0;
    }

    if (src_size) *src_size = sarg_size;
    return 1;
}

// TODO: docs
static int _map_put_subject_copy(map_t* m, long key, lir_subject_t* src, long src_size) {
    lir_subject_t* old = NULL;
    if (map_get(m, key, (void**)&old)) LIR_unload_subject(old);
    lir_subject_t* copy = LIR_copy_subject(src);
    if (!copy) return 0;
    copy->size = src_size;
    return map_put(m, key, copy);
}

// TODO: docs
static int _map_copy_subjects(map_t* dst, map_t* src) {
    map_init(dst, src->cmp);
    for (long i = 0; i < src->capacity; i++) {
        if (!src->entries[i].used) continue;
        lir_subject_t* copy = LIR_copy_subject((lir_subject_t*)src->entries[i].value);
        if (!copy || !map_put(dst, src->entries[i].key, copy)) return 0;
    }

    return 1;
}

// TODO: docs
static inline int _map_assign_subjects(map_t* dst, map_t* src) {
    map_free_force_op(dst, (int (*)(void*))LIR_unload_subject);
    return _map_copy_subjects(dst, src);
}

// TODO: docs
static int _map_subjects_equal(map_t* a, map_t* b) {
    if (a->size != b->size) return 0;
    for (long i = 0; i < a->capacity; i++) {
        if (!a->entries[i].used) continue;
        lir_subject_t* bv = NULL;
        if (
            !map_get(b, a->entries[i].key, (void**)&bv) ||
            !LIR_subj_equals((lir_subject_t*)a->entries[i].value, bv)
        ) return 0;
    }

    return 1;
}

// TODO: docs
static int _map_intersect_subjects(map_t* dst, map_t* a, map_t* b) {
    map_init(dst, MAP_NO_CMP);
    for (long i = 0; i < a->capacity; i++) {
        if (!a->entries[i].used) continue;
        lir_subject_t* bv = NULL;
        if (
            map_get(b, a->entries[i].key, (void**)&bv) &&
            LIR_subj_equals((lir_subject_t*)a->entries[i].value, bv)
        ) {
            lir_subject_t* copy = LIR_copy_subject((lir_subject_t*)a->entries[i].value);
            if (!copy || !map_put(dst, a->entries[i].key, copy)) return 0;
        }
    }

    return 1;
}

// TODO: docs
static int _map_kill_variable(map_t* m, symbol_id_t v_id) {
    lir_subject_t* old = NULL;
    if (map_get(m, v_id, (void**)&old)) {
        map_remove(m, v_id);
        LIR_unload_subject(old);
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        for (long i = 0; i < m->capacity; i++) {
            if (!m->entries[i].used) continue;
            lir_subject_t* src = (lir_subject_t*)m->entries[i].value;
            if (src->t == LIR_VARIABLE && src->storage.var.v_id == v_id) {
                long key = m->entries[i].key;
                map_remove(m, key);
                LIR_unload_subject(src);
                changed = 1;
                break;
            }
        }
    }

    return 1;
}

// TODO: docs
static int _transfer_instruction(lir_block_t* lh, map_t* state, sym_table_t* smt, set_t* addr_taken) {
    if (!lh || lh->unused) return 1;
    if (LIR_is_writeop(lh->op) && lh->farg && lh->farg->t == LIR_VARIABLE) {
        _map_kill_variable(state, lh->farg->storage.var.v_id);
    }

    long src_size = 0;
    if (_is_copy_candidate(lh, smt, addr_taken, &src_size)) {
        _map_put_subject_copy(state, lh->farg->storage.var.v_id, lh->sarg, src_size);
    }

    return 1;
}

// TODO: docs
static int _transfer_block(cfg_block_t* bb, map_t* in, map_t* out, sym_table_t* smt, set_t* addr_taken) {
    map_t tmp;
    _map_copy_subjects(&tmp, in);
    iterate_lir_instructions (bb) {
        _transfer_instruction(lh, &tmp, smt, addr_taken);
    }

    _map_assign_subjects(out, &tmp);
    map_free_force_op(&tmp, (int (*)(void*))LIR_unload_subject);
    return 1;
}

// TODO: docs
static int _compute_block_copy_sets(cfg_block_t* bb, sym_table_t* smt, set_t* addr_taken) {
    set_free(&bb->copy_gen);
    set_init(&bb->copy_gen, SET_NO_CMP);
    set_free(&bb->copy_kill);
    set_init(&bb->copy_kill, SET_NO_CMP);

    iterate_lir_instructions (bb) {
        if (lh->unused) continue;
        if (LIR_is_writeop(lh->op) && lh->farg && lh->farg->t == LIR_VARIABLE) {
            set_add(&bb->copy_kill, (void*)lh->farg->storage.var.v_id);
            set_remove(&bb->copy_gen, (void*)lh->farg->storage.var.v_id);
        }

        if (
            _is_copy_candidate(lh, smt, addr_taken, NULL)
        ) set_add(&bb->copy_gen, (void*)lh->farg->storage.var.v_id);
    }

    return 1;
}

// TODO: docs
static int _collect_address_taken_variables(cfg_func_t* fb, set_t* addr_taken) {
    set_init(addr_taken, SET_NO_CMP);
    foreach (cfg_block_t* bb, &fb->blocks) {
        iterate_lir_instructions (bb) {
            if (
                !lh->unused && 
                lh->op == LIR_aMOV && lh->farg && lh->farg->t == LIR_VARIABLE
            ) set_add(addr_taken, (void*)lh->farg->storage.var.v_id);
        }
    }

    return 1;
}

// TODO: docs
static int _build_block_in(map_t* dst, cfg_block_t* bb, map_t* out_by_block) {
    map_init(dst, MAP_NO_CMP);
    int first = 1;
    set_foreach (cfg_block_t* pred, &bb->pred) {
        map_t* pred_out = NULL;
        if (!map_get(out_by_block, pred->id, (void**)&pred_out)) continue;
        if (first) {
            map_free(dst);
            _map_copy_subjects(dst, pred_out);
            first = 0;
            continue;
        }

        map_t tmp;
        _map_intersect_subjects(&tmp, dst, pred_out);
        map_free_force_op(dst, (int (*)(void*))LIR_unload_subject);
        *dst = tmp;
    }

    return 1;
}

// TODO: docs
static int _rewrite_block(cfg_block_t* bb, map_t* in, sym_table_t* smt, set_t* addr_taken) {
    map_t state;
    _map_copy_subjects(&state, in);
    iterate_lir_instructions (bb) {
        if (!lh->unused) {
            if (
                LIR_is_readop(lh->op) &&
                lh->op != LIR_aMOV &&
                lh->op != LIR_REF
            ) _replace_with_copy(lh, &state, LIR_VARIABLE);
            _transfer_instruction(lh, &state, smt, addr_taken);
        }
    }

    map_free_force_op(&state, (int (*)(void*))LIR_unload_subject);
    return 1;
}

int LIR_variable_copy_propagation(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        set_t addr_taken;
        _collect_address_taken_variables(fb, &addr_taken);

        map_t in_by_block, out_by_block;
        map_init(&in_by_block, MAP_NO_CMP);
        map_init(&out_by_block, MAP_NO_CMP);
        foreach (cfg_block_t* bb, &fb->blocks) {
            _compute_block_copy_sets(bb, smt, &addr_taken);
            map_t* in  = (map_t*)mm_malloc(sizeof(map_t));
            map_t* out = (map_t*)mm_malloc(sizeof(map_t));
            map_init(in, MAP_NO_CMP);
            map_init(out, MAP_NO_CMP);
            map_put(&in_by_block, bb->id, in);
            map_put(&out_by_block, bb->id, out);
        }

        int changed = 1;
        while (changed) {
            changed = 0;
            foreach (cfg_block_t* bb, &fb->blocks) {
                map_t new_in;
                _build_block_in(&new_in, bb, &out_by_block);

                map_t *old_in = NULL, *old_out = NULL;
                map_get(&in_by_block, bb->id, (void**)&old_in);
                map_get(&out_by_block, bb->id, (void**)&old_out);
                if (!_map_subjects_equal(old_in, &new_in)) {
                    _map_assign_subjects(old_in, &new_in);
                    changed = 1;
                }

                map_t new_out;
                map_init(&new_out, MAP_NO_CMP);
                _transfer_block(bb, old_in, &new_out, smt, &addr_taken);
                if (!_map_subjects_equal(old_out, &new_out)) {
                    _map_assign_subjects(old_out, &new_out);
                    changed = 1;
                }

                map_free_force_op(&new_in, (int (*)(void*))LIR_unload_subject);
                map_free_force_op(&new_out, (int (*)(void*))LIR_unload_subject);
            }
        }

        foreach (cfg_block_t* bb, &fb->blocks) {
            map_t* in = NULL;
            if (map_get(&in_by_block, bb->id, (void**)&in)) {
                _rewrite_block(bb, in, smt, &addr_taken);
            }
        }

        map_foreach (map_t* in, &in_by_block) {
            map_free_force_op(in, (int (*)(void*))LIR_unload_subject);
            mm_free(in);
        }

        map_foreach (map_t* out, &out_by_block) {
            map_free_force_op(out, (int (*)(void*))LIR_unload_subject);
            mm_free(out);
        }
        
        map_free(&in_by_block);
        map_free(&out_by_block);
        set_free(&addr_taken);
    }
    
    return 1;
}
