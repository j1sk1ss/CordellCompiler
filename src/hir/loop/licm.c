#include <hir/loop.h>

// TODO: docs
static symbol_id_t _gather_base_vid(symbol_id_t v_id, sym_table_t* smt) {
    variable_info_t vi;
    if (!VRTB_get_info_id(v_id, &vi, &smt->v)) return v_id;
    return vi.p_id == NO_SYMBOL_ID ? vi.v_id : _gather_base_vid(vi.p_id, smt);
}

/*
Insert a preheader block in the CFG context before the header block.
Params:
    - `cctx` - CFG context.
    - `header` - Current loop header.
    - `loop` - Current loop.

Return the preheader block or the 'NULL' value.
*/
static cfg_block_t* _insert_preheader(cfg_ctx_t* cctx, cfg_block_t* header, set_t* loop) {
    /* Check if the current block has a 'preheader' as an
       ancestor. 
       We don't need to insert an extra preheader block. */
    set_foreach (cfg_block_t* p, &header->pred) {
        if (p->type == CFG_LOOP_PREHEADER) return p;
    }

    /* Collect external predecessors first. Do not modify header->pred
       while iterating over it. */
    list_t external_preds;
    list_init(&external_preds);
    set_foreach (cfg_block_t* p, &header->pred) {
        if (!set_has(loop, p)) list_push_back(&external_preds, p);
    }

    if (!list_size(&external_preds)) {
        list_free(&external_preds);
        return NULL;
    }

    /* Create the preheader block and set the preheader type. */
    hir_block_t* anchor = HIR_create_block(HIR_NOP, NULL, NULL, NULL);
    HIR_insert_block_before(anchor, header->hmap.entry);
    cfg_block_t* preheader = HIR_CFG_create_cfg_block(anchor);
    if (!preheader) {
        list_free(&external_preds);
        return NULL;
    }

    preheader->id    = cctx->cid++;
    preheader->type  = CFG_LOOP_PREHEADER;
    preheader->l     = header;
    preheader->pfunc = header->pfunc;

    /* Redirect all external predecessors from header to preheader. */
    foreach (cfg_block_t* p, &external_preds) {
        if (set_has(loop, p)) continue; /* Skip all blocks from the current loop */
                                        /* We mustn't update blocks from the     */
                                        /* loop.                                 */
        if (p->l && p->l == header)     p->l   = preheader;
        if (p->jmp && p->jmp == header) p->jmp = preheader;
        set_add(&preheader->pred, p);
        set_remove(&header->pred, p);
    }

    list_free(&external_preds);

    list_insert(&header->pfunc->blocks, preheader, header);
    set_add(&header->pred, preheader);
    return preheader;
}

/*
Extract HIR blocks from the provided set of the loop blocks.
Params:
    - `loop` - Loop blocks.
    - `b` - Output set of HIR blocks.

Returns 1 if succeeds, otherwise will return 0.
*/
static int _get_loop_hir_blocks(set_t* loop, set_t* b) {
    set_foreach (cfg_block_t* bb, loop) {
        hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
        while (hh) {
            set_add(b, hh);
            hh = HIR_get_next(hh, bb->hmap.exit, 1);
        }
    }

    return 1;
}

// TODO: docs
static int _hir_can_be_licm_def(hir_block_t* hh) {
    if (
        !hh || !HIR_is_writeop(hh->op) || HIR_is_sideeffect_op(hh->op) ||
        !hh->farg || !HIR_is_vartype(hh->farg->t)
    ) return 0;
    return 1;
}

// TODO: docs
static int _set_add_vid_with_base(set_t* s, symbol_id_t v_id, sym_table_t* smt) {
    int changed = 0;
    changed |= set_add(s, (void*)v_id);
    if (smt) {
        symbol_id_t base_id = _gather_base_vid(v_id, smt);
        changed |= set_add(s, (void*)base_id);
    }

    return changed;
}

// TODO: docs
static int _set_add_subject_vid_with_base(set_t* s, hir_subject_t* subj, sym_table_t* smt) {
    if (!subj || !HIR_is_vartype(subj->t)) return 0;
    return _set_add_vid_with_base(s, subj->storage.var.v_id, smt);
}

// TODO: docs
static int _set_add_block_vids_with_base(set_t* s, hir_block_t* hh, sym_table_t* smt) {
    if (!hh) return 0;
    int changed = 0;
    hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
    for (int i = 0; i < 3; i++) {
        changed |= _set_add_subject_vid_with_base(s, args[i], smt);
    }

    return changed;
}

// TODO: docs
static int _hir_uses_any_vid_from_set(hir_block_t* hh, set_t* s, sym_table_t* smt, set_t* loop_hir);

/*
Get HIR blocks that aren't affect on the loop environment.
For instance:
```cpl
    i32 a = 0;
    while a < 10; {
        i32 b = 10 + 10; : <- Invariant definition :
        a += b;
    }
```

Parmas:
    - `loop_hir` - Loop HIR blocks.
    - `invariant_defs` - Output set of invariant blocks which are ready to be moved.
    - `inductive` - Inductive variables set. 
                    Note: Inductive variable is a variable like `a += 1`, etc.
    
Returns 1 if succeeds, otherwise will return 0.
*/
static int _get_invariant_defs(set_t* loop_hir, set_t* invariant_defs, set_t* inductive, sym_table_t* smt) {
    int changed = 1;
    do {
        changed = 0;
        set_foreach (hir_block_t* hh, loop_hir) {
            if (
                set_has(invariant_defs, hh) || /* Already in the invariant set        */
                !_hir_can_be_licm_def(hh)      /* If this operation has a side effect */
            ) continue;

            int invariant = !_hir_uses_any_vid_from_set(hh, inductive, smt, loop_hir);
            hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
            for (int i = HIR_is_writeop(hh->op); i < 3; i++) {
                hir_subject_t* s = args[i];
                if (
                    !s ||                 /* - If this is the 'NULL' value                                */
                    !HIR_is_vartype(s->t) /* - If this isn't a variable                                   */
                ) continue;

                symbol_id_t raw_id = s->storage.var.v_id;
                symbol_id_t src_id = _gather_base_vid(raw_id, smt);
                if (
                    set_has(inductive, (void*)raw_id) ||
                    set_has(inductive, (void*)src_id) ||
                    (
                        s->home &&                           /* - If this is the non-home value such as numbers, consts, etc */
                        set_has(loop_hir, s->home) &&         /* If the variable's home within the loop                       */
                        !set_has(invariant_defs, s->home)     /* If the variable's home isn't an invariant command            */
                    )
                ) {
                    invariant = 0;
                    break;
                }
            }

            if (invariant && set_add(invariant_defs, hh)) {
                changed = 1;
            }
        }
    } while (changed);

    return 1;
}

/*
Search for the CFG block with the 'trg' HIR block.
Params:
    - `s` - Output set.
    - `trg` - HIR block for search.

Returns the CFG block or the 'NULL' value.
*/
static cfg_block_t* _get_hir_block_cfg(set_t* s, hir_block_t* trg) {
    set_foreach (cfg_block_t* bb, s) {
        hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
        while (hh) {
            if (hh == trg) return bb;
            hh = HIR_get_next(hh, bb->hmap.exit, 1);
        }
    }

    return NULL;
}

// TODO: docs
static int _hir_rhs_depends_on_vid(
    hir_block_t* hh,
    symbol_id_t v_id,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
);

// TODO: docs
static int _hir_subject_depends_on_vid(
    hir_subject_t* s,
    symbol_id_t v_id,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
) {
    if (!s || !HIR_is_vartype(s->t)) return 0;

    symbol_id_t raw_id = s->storage.var.v_id;
    symbol_id_t src_id = _gather_base_vid(raw_id, smt);
    if (raw_id == v_id || src_id == v_id) return 1;

    if (!s->home || !set_has(loop_hir, s->home)) return 0;
    if (set_has(visited, s->home)) return 0;

    set_add(visited, s->home);
    return _hir_rhs_depends_on_vid(s->home, v_id, smt, loop_hir, visited);
}

// TODO: docs
static int _hir_rhs_depends_on_vid(
    hir_block_t* hh,
    symbol_id_t v_id,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
) {
    if (!hh) return 0;
    hir_subject_t* args[2] = { hh->sarg, hh->targ };
    for (int i = 0; i < 2; i++) {
        if (_hir_subject_depends_on_vid(args[i], v_id, smt, loop_hir, visited)) return 1;
    }

    return 0;
}

// TODO: docs
static int _hir_uses_vid(hir_block_t* hh, symbol_id_t v_id, sym_table_t* smt, set_t* loop_hir) {
    set_t visited;
    if (!set_init(&visited, SET_NO_CMP)) return 0;

    int res = _hir_rhs_depends_on_vid(hh, v_id, smt, loop_hir, &visited);

    set_free(&visited);
    return res;
}

// TODO: docs
static int _hir_rhs_depends_on_any_vid_from_set(
    hir_block_t* hh,
    set_t* s,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
);

// TODO: docs
static int _hir_subject_depends_on_any_vid_from_set(
    hir_subject_t* subj,
    set_t* s,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
) {
    if (!subj || !HIR_is_vartype(subj->t)) return 0;

    symbol_id_t raw_id = subj->storage.var.v_id;
    symbol_id_t src_id = _gather_base_vid(raw_id, smt);
    if (set_has(s, (void*)raw_id) || set_has(s, (void*)src_id)) return 1;

    if (!subj->home || !set_has(loop_hir, subj->home)) return 0;
    if (set_has(visited, subj->home)) return 0;

    set_add(visited, subj->home);
    return _hir_rhs_depends_on_any_vid_from_set(subj->home, s, smt, loop_hir, visited);
}

// TODO: docs
static int _hir_rhs_depends_on_any_vid_from_set(
    hir_block_t* hh,
    set_t* s,
    sym_table_t* smt,
    set_t* loop_hir,
    set_t* visited
) {
    if (!hh) return 0;
    hir_subject_t* args[2] = { hh->sarg, hh->targ };
    for (int i = 0; i < 2; i++) {
        if (_hir_subject_depends_on_any_vid_from_set(args[i], s, smt, loop_hir, visited)) return 1;
    }

    return 0;
}

// TODO: docs
static int _hir_uses_any_vid_from_set(hir_block_t* hh, set_t* s, sym_table_t* smt, set_t* loop_hir) {
    set_t visited;
    if (!set_init(&visited, SET_NO_CMP)) return 0;

    int res = _hir_rhs_depends_on_any_vid_from_set(hh, s, smt, loop_hir, &visited);

    set_free(&visited);
    return res;
}

/*
Find inductive variables. Such variables uses their values in the next iteration.
For instance, we consider the next variable (`a`) as the inductive variable.
```cpl
    i32 a;
    while a; {
        a += 1;
    }
```

Params:
    - `loop_hir` - Loop HIR commands.
    - `s` - Output set.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
static int _get_inductive_variables(set_t* loop_hir, set_t* s, sym_table_t* smt) {
    int changed = 0;

    set_foreach (hir_block_t* hh, loop_hir) {
        if (_hir_can_be_licm_def(hh)) continue;
        changed |= _set_add_block_vids_with_base(s, hh, smt);
    }

    set_foreach (hir_block_t* hh, loop_hir) {
        if (!_hir_can_be_licm_def(hh)) continue;
        symbol_id_t raw_id = hh->farg->storage.var.v_id;
        symbol_id_t dst_id = _gather_base_vid(raw_id, smt);
        if (
            _hir_uses_vid(hh, raw_id, smt, loop_hir) ||
            _hir_uses_vid(hh, dst_id, smt, loop_hir)
        ) changed |= _set_add_vid_with_base(s, raw_id, smt);
    }

    while (changed) {
        changed = 0;
        set_foreach (hir_block_t* hh, loop_hir) {
            if (!_hir_can_be_licm_def(hh)) continue;
            symbol_id_t raw_id = hh->farg->storage.var.v_id;
            symbol_id_t dst_id = _gather_base_vid(raw_id, smt);
            if (set_has(s, (void*)raw_id) || set_has(s, (void*)dst_id)) continue;
            if (_hir_uses_any_vid_from_set(hh, s, smt, loop_hir)) {
                changed |= _set_add_vid_with_base(s, raw_id, smt);
            }
        }
    }

    return 1;
}

// TODO: docs
static int _move_to_preheader(cfg_block_t* preheader, cfg_block_t* src_cfg, hir_block_t* inv) {
    if (!preheader || !src_cfg || !inv) return 0;
    HIR_CFG_remove_hir_block(src_cfg, inv);
    HIR_unlink_block(inv);
    HIR_insert_block_after(inv, preheader->hmap.exit);
    preheader->hmap.exit = inv;
    return 1;
}

/*
This function moves invariant definitions to the preheader block.
Params:
    - `cctx` - CFG context.
    - `node` - Considering loop.
    - `smt` - Symtable.
    - `licm` - Perfrorm LICM operation?

Returns 1 if succeeds, otherwise will return 0. 
*/
static int _licm_process(cfg_ctx_t* cctx, loop_node_t* loop, sym_table_t* smt, int licm) {
    cfg_block_t* preheader = _insert_preheader(cctx, loop->header, &loop->blocks);
    if (!preheader) return 0;
    if (!licm) return 1;

    set_t loop_hir, inductive, invariant_defs;
    if (
        !set_init(&invariant_defs, SET_NO_CMP) ||
        !set_init(&inductive,      SET_NO_CMP) ||
        !set_init(&loop_hir,       SET_NO_CMP)
    ) {
        set_free(&invariant_defs);
        set_free(&inductive);
        set_free(&loop_hir);
        return 0;
    }

    _get_loop_hir_blocks(&loop->blocks, &loop_hir);
    _get_inductive_variables(&loop_hir, &inductive, smt);
    _get_invariant_defs(&loop_hir, &invariant_defs, &inductive, smt);

    /* From an unordered set of hir blocks, we need to create an ordered
       list. Order is based on the home function. */
    list_t linear;
    list_init(&linear);
    foreach (cfg_block_t* cb, &loop->header->pfunc->blocks) {
        if (!set_has(&loop->blocks, cb)) continue;
        hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
        while (hh) {
            if (set_has(&invariant_defs, hh)) list_push_back(&linear, hh);
            hh = HIR_get_next(hh, cb->hmap.exit, 1);
        }
    }

    int changed = 0;
    foreach (hir_block_t* inv, &linear) {
        cfg_block_t* src_cfg = _get_hir_block_cfg(&loop->blocks, inv);
        if (!src_cfg) continue;
        changed |= _move_to_preheader(preheader, src_cfg, inv);
    }

    list_free(&linear);
    set_free(&invariant_defs);
    set_free(&inductive);
    set_free(&loop_hir);
    return changed;
}

/*
Invoke the LICM optimization on the loop recursively.
Params:
    - `cctx` - CFG context.
    - `node` - Current loop.
    - `smt` - Symtable.
    - `licm` - Perform LICM transformation.

Returns 1 if has changed, otherwise will return 0.
*/
int _licm_loop_node_process(cfg_ctx_t* cctx, loop_node_t* loop, sym_table_t* smt, int licm) {
    int changed = 0;
    foreach (loop_node_t* ch, &loop->children) {
        changed |= _licm_loop_node_process(cctx, ch, smt, licm);
    }

    changed |= _licm_process(cctx, loop, smt, licm);
    return changed;
}

int HIR_LTREE_licm(cfg_ctx_t* cctx, ltree_ctx_t* lctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        
        list_t* floops;
        if (!map_get(&lctx->lmap, fb->f_id, (void**)&floops)) continue;
        if (!list_size(floops)) continue;

        int changed = 0;
        do {
            changed = 0;
            foreach (loop_node_t* root, floops) {
                changed |= _licm_loop_node_process(cctx, root, smt, 1);
            }
        } while (changed);
    }

    return 1;
}

int HIR_LTREE_canonicalization(cfg_ctx_t* cctx, ltree_ctx_t* lctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        list_t* floops;
        if (!map_get(&lctx->lmap, fb->f_id, (void**)&floops)) continue;
        if (!list_size(floops)) continue;        
        foreach (loop_node_t* root, floops) {
            _licm_loop_node_process(cctx, root, NULL, 0);
        }
    }

    return 1;
}
