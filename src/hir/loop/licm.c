#include <hir/loop.h>

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

    /* Create the preheader block,
       set the preheader type. */
    cfg_block_t* preheader = HIR_CFG_create_cfg_block(NULL);
    if (!preheader) return NULL;
    preheader->id = cctx->cid++;
    preheader->type = CFG_LOOP_PREHEADER;

    /* Refactor the navigation. We need to set the 'l' to the current header.
       Also, we need to refactor all links in predcessors as well */
    preheader->l = header;
    set_foreach (cfg_block_t* p, &header->pred) {
        if (set_has(loop, p)) continue; /* Skip all blocks from the current loop */
                                        /* We mustn't update blocks from the     */
                                        /* loop.                                 */
        if (p->l && p->l == header)     p->l   = preheader;
        if (p->jmp && p->jmp == header) p->jmp = preheader;
        set_add(&preheader->pred, p);
        set_remove(&header->pred, p);
    }
    
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
    - `invariant_defs` - 
    - `inductive` - Inductive variables set. 
                    Note: Inductive variable is a variable like `a += 1`, etc.
    
Returns 1 if succeeds, otherwise will return 0.
*/
static int _get_invariant_defs(set_t* loop_hir, set_t* invariant_defs, set_t* inductive) {
    int changed = 1;
    do {
        changed = 0;
        set_foreach (hir_block_t* hh, loop_hir) {
            if (
                set_has(invariant_defs, hh) || /* Already in the invariant set        */
                HIR_sideeffect_op(hh->op)      /* If this operation has a side effect */
            ) continue;

            int invariant = 1;
            hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
            for (int i = HIR_writeop(hh->op); i < 3; i++) {
                hir_subject_t* s = args[i];
                if (
                    !s ||                      /* - If this is the 'NULL' value                                */
                    !s->home ||                /* - If this is the non-home value such as numbers, consts, etc */
                    !HIR_is_vartype(s->t)      /* - If this isn't a variable                                   */
                ) continue;

                /* Is this is an inductive variable */
                if (set_has(inductive, (void*)args[i]->storage.var.v_id)) {
                    invariant = 0;
                    break;
                }

                if (
                    set_has(loop_hir, s->home) &&       /* If the variable's home within the loop            */
                    !set_has(invariant_defs, s->home)   /* If the variable's home isn't an invariant command */
                ) {
                    invariant = 0;
                    break;
                }
            }

            if (invariant) {
                set_add(invariant_defs, hh);
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

/*
Find variable IDs that use the provided vid variable as the source of their value.
Params:
    - `loop_hir` - Loop HIR commands.
    - `s` - Output set.
    - `vid` - Target variable ID.
    - `smt` - Symtable.

Returns 1 if it founds something.
*/
static int _find_usage(set_t* loop_hir, set_t* s, long vid, sym_table_t* smt) {
    int res = 0;
    set_foreach (hir_block_t* hh, loop_hir) {
        hir_subject_t* args[2] = { hh->sarg, hh->targ };
        for (int i = 0; i < 2; i++) {
            hir_subject_t* ss = args[i];
            if (!ss || !HIR_is_vartype(ss->t)) continue;

            /* Get the base vID over the SSA form */
            variable_info_t vi;
            if (!VRTB_get_info_id(ss->storage.var.v_id, &vi, &smt->v)) continue;
            long src_id = vi.p_id < 0 ? vi.v_id : vi.p_id;

            if (src_id == vid) {
                set_add(s, (void*)hh->farg->storage.var.v_id);
                res = 1;
            }
        }
    }

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
    int changed = 1;
    while (changed) {
        changed = 0;
        set_foreach (hir_block_t* hh, loop_hir) {
            if (
                !HIR_writeop(hh->op) ||      /* - If this isn't a write operation.          */
                !hh->farg            ||      /* - If there is no first argument in command. */
                !HIR_is_vartype(hh->farg->t) /* - If the first argument isn't a variable.   */
            ) continue;

            /* Basic vID over the SSA form */
            long vid = hh->farg->storage.var.v_id;
            variable_info_t vi;
            if (VRTB_get_info_id(vid, &vi, &smt->v)) {
                vid = vi.p_id < 0 ? vi.v_id : vi.p_id;
            }

            if (
                _find_usage(loop_hir, s, vid, smt) && /* - If this variable used somewhere. */
                set_add(s, (void*)vid)                /* - Check if we add the variable ID. */
            ) changed = 1;
        }
    }

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
static int _licm_process(cfg_ctx_t* cctx, loop_node_t* node, sym_table_t* smt, int licm) {
    cfg_block_t* preheader = _insert_preheader(cctx, node->header, &node->blocks);
    if (!preheader) return 0;
    if (!licm) return 1;

    set_t loop_hir, inductive, invariant_defs;
    if (
        !set_init(&loop_hir,       SET_NO_CMP) ||
        !set_init(&inductive,      SET_NO_CMP) ||
        !set_init(&invariant_defs, SET_NO_CMP)
    ) {
        set_free(&invariant_defs);
        set_free(&inductive);
        set_free(&loop_hir);
        return 0;
    }

    _get_loop_hir_blocks(&node->blocks, &loop_hir);
    _get_inductive_variables(&loop_hir, &inductive, smt);
    _get_invariant_defs(&loop_hir, &invariant_defs, &inductive);

    list_t linear;
    list_init(&linear);
    foreach (cfg_block_t* cb, &node->header->pfunc->blocks) {
        if (!set_has(&node->blocks, cb)) continue;
        hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
        while (hh) {
            if (set_has(&invariant_defs, hh)) list_push_back(&linear, hh);
            hh = HIR_get_next(hh, cb->hmap.exit, 1);
        }
    }

    int changed = 0;
    foreach (hir_block_t* inv, &linear) {
        cfg_block_t* src_cfg = _get_hir_block_cfg(&node->blocks, inv);
        if (!src_cfg) continue;

        HIR_CFG_remove_hir_block(src_cfg, inv);
        HIR_CFG_append_hir_block_back(preheader, inv);
        HIR_unlink_block(inv);
        HIR_insert_block_before(inv, preheader->l->hmap.entry);

        changed = 1;
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
int _licm_loop_node_process(cfg_ctx_t* cctx, loop_node_t* node, sym_table_t* smt, int licm) {
    int changed = 0;
    foreach (loop_node_t* ch, &node->children) {
        changed |= _licm_loop_node_process(cctx, ch, smt, licm);
    }

    changed |= _licm_process(cctx, node, smt, licm);
    return changed;
}

int HIR_LTREE_licm(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        int changed = 0;
        do {
            changed = 0;
            ltree_ctx_t lctx;
            list_init(&lctx.loops);

            HIR_LTREE_build_loop_tree(fb, &lctx);
            if (!list_size(&lctx.loops)) {
                HIR_LTREE_unload_ctx(&lctx);
                continue;
            }

            foreach (loop_node_t* root, &lctx.loops) {
                changed |= _licm_loop_node_process(cctx, root, smt, 1);
            }

            HIR_LTREE_unload_ctx(&lctx);
        } while (changed);
    }

    return 1;
}

int HIR_LTREE_canonicalization(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        ltree_ctx_t lctx;
        list_init(&lctx.loops);

        HIR_LTREE_build_loop_tree(fb, &lctx);
        if (!list_size(&lctx.loops)) {
            HIR_LTREE_unload_ctx(&lctx);
            continue;
        }
        
        foreach (loop_node_t* root, &lctx.loops) {
            _licm_loop_node_process(cctx, root, NULL, 0);
        }

        HIR_LTREE_unload_ctx(&lctx);
    }

    return 1;
}
