#include <hir/loop.h>

static cfg_block_t* _insert_preheader(cfg_ctx_t* cctx, cfg_block_t* header, set_t* loop) {
    set_iter_t it;
    set_iter_init(&header->pred, &it);
    cfg_block_t* p;
    while (set_iter_next(&it, (void**)&p)) {
        if (p->type == CFG_LOOP_PREHEADER) return p;
    }

    cfg_block_t* preheader = HIR_CFG_create_cfg_block(NULL);
    if (!preheader) return NULL;
    preheader->type = CFG_LOOP_PREHEADER;

    preheader->id = cctx->cid++;
    list_insert(&header->pfunc->blocks, preheader, header);
    preheader->l = header;

    set_iter_init(&header->pred, &it);
    while (set_iter_next(&it, (void**)&p)) {
        if (set_has(loop, p)) continue;
        if (p->l && p->l == header)     p->l   = preheader;
        if (p->jmp && p->jmp == header) p->jmp = preheader;
        set_add(&preheader->pred, p);
        set_remove(&header->pred, p);
    }
    
    set_add(&header->pred, preheader);
    return preheader;
}

static int _get_loop_hir_blocks(set_t* loop, set_t* b) {
    set_iter_t it;
    set_iter_init(loop, &it);
    cfg_block_t* bb;
    while (set_iter_next(&it, (void**)&bb)) {
        hir_block_t* hh = bb->hmap.entry;
        while (hh) {
            set_add(b, hh);
            if (hh == bb->hmap.exit) break;
            hh = hh->next;
        }
    }

    return 1;
}

static int _get_invariant_defs(set_t* loop_hir, set_t* invariant_defs, set_t* inductive) {
    int changed = 1;
    while (changed) {
        changed = 0;

        set_iter_t it;
        set_iter_init(loop_hir, &it);
        hir_block_t* hh;
        while (set_iter_next(&it, (void**)&hh)) {
            if (set_has(invariant_defs, hh)) continue;
            if (HIR_sideeffect_op(hh->op)) continue;

            int invariant = 1;
            hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
            for (int i = HIR_writeop(hh->op); i < 3; i++) {
                hir_subject_t* s = args[i];
                if (!s || !s->home || !HIR_is_vartype(s->t)) continue;
                if (set_has(inductive, (void*)args[i]->storage.var.v_id)) {
                    invariant = 0;
                    break;
                }

                if (set_has(loop_hir, s->home)) {
                    if (!set_has(invariant_defs, s->home)) {
                        invariant = 0;
                        break;
                    }
                }
            }

            if (invariant) {
                set_add(invariant_defs, hh);
                changed = 1;
            }
        }
    }

    return 1;
}

static cfg_block_t* _get_hir_block_cfg(set_t* s, hir_block_t* trg) {
    set_iter_t it;
    set_iter_init(s, &it);
    cfg_block_t* bb;
    while (set_iter_next(&it, (void**)&bb)) {
        hir_block_t* hh = bb->hmap.entry;
        while (hh) {
            if (hh == trg) return bb;
            if (hh == bb->hmap.exit) break;
            hh = hh->next;
        }
    }

    return NULL;
}

static int _find_usage(set_t* loop_hir, set_t* s, long vid, sym_table_t* smt) {
    int res = 0;
    set_iter_t it;
    set_iter_init(loop_hir, &it);
    hir_block_t* hh;
    while (set_iter_next(&it, (void**)&hh)) {
        hir_subject_t* args[2] = { hh->sarg, hh->targ };
        for (int i = 0; i < 2; i++) {
            hir_subject_t* ss = args[i];
            if (!ss || !HIR_is_vartype(ss->t)) continue;
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

static int _get_inductive_variables(set_t* loop_hir, set_t* s, sym_table_t* smt) {
    int changed = 1;
    while (changed) {
        changed = 0;
        set_iter_t it;
        set_iter_init(loop_hir, &it);
        hir_block_t* hh;
        while (set_iter_next(&it, (void**)&hh)) {
            if (!HIR_writeop(hh->op) || !hh->farg || !HIR_is_vartype(hh->farg->t)) continue;
            long vid = hh->farg->storage.var.v_id;
            variable_info_t vi;
            if (VRTB_get_info_id(vid, &vi, &smt->v)) vid = vi.p_id < 0 ? vi.v_id : vi.p_id;
            if (_find_usage(loop_hir, s, vid, smt)) {
                if (set_add(s, (void*)vid)) {
                    changed = 1;
                }
            }
        }
    }

    return 1;
}

static int _licm_process(cfg_ctx_t* cctx, loop_node_t* node, sym_table_t* smt, int licm) {
    int changed = 0;
    cfg_block_t* header = node->header;
    cfg_block_t* latch  = node->latch;

    print_debug("LICM (tree), loop header=%i, latch=%i", header->id, latch->id);
    cfg_block_t* preheader = _insert_preheader(cctx, header, &node->blocks);
    if (!preheader) return 0;
    if (!licm) return 1;

    set_t loop_hir;
    set_init(&loop_hir);
    _get_loop_hir_blocks(&node->blocks, &loop_hir);
    print_debug("_get_loop_hir_blocks complete, size(loop_blocks)=%i, size(loop_hir)=%i", set_size(&node->blocks), set_size(&loop_hir));

    set_t inductive;
    set_init(&inductive);
    _get_inductive_variables(&loop_hir, &inductive, smt);
    print_debug("_get_inductive_variables complete, size(inductive)=%i", set_size(&inductive));

    set_t invariant_defs;
    set_init(&invariant_defs);
    _get_invariant_defs(&loop_hir, &invariant_defs, &inductive);
    print_debug("_get_invariant_defs complete, size(invariant_defs)=%i", set_size(&invariant_defs));

    list_t linear;
    list_init(&linear);

    list_iter_t it;
    list_iter_hinit(&node->header->pfunc->blocks, &it);
    cfg_block_t* cb3;
    while ((cb3 = (cfg_block_t*)list_iter_next(&it))) {
        if (!set_has(&node->blocks, cb3)) continue;
        hir_block_t* hh = cb3->hmap.entry;
        while (hh) {
            if (set_has(&invariant_defs, hh)) list_push_back(&linear, hh);
            if (hh == cb3->hmap.exit) break;
            hh = hh->next;
        }
    }

    print_debug("set2linear complete, size(linear)=%i", list_size(&linear));
    list_iter_hinit(&linear, &it);
    hir_block_t* inv;
    while ((inv = (hir_block_t*)list_iter_next(&it))) {
        cfg_block_t* src_cfg = _get_hir_block_cfg(&node->blocks, inv);
        if (!src_cfg) continue;
        HIR_CFG_remove_hir_block(src_cfg, inv);
        HIR_CFG_append_hir_block_back(preheader, inv);
        HIR_unlink_block(inv);
        HIR_insert_block_before(inv, preheader->l->hmap.entry);
        changed = 1;
    }

    print_debug("preheader move complete for loop header=%i", header->id);
    set_free(&inductive);
    list_free(&linear);
    set_free(&invariant_defs);
    set_free(&loop_hir);
    return changed;
}

int _licm_loop_node_process(cfg_ctx_t* cctx, loop_node_t* node, sym_table_t* smt, int licm) {
    int changed = 0;
    list_iter_t it;
    list_iter_hinit(&node->children, &it);
    loop_node_t* ch;
    while ((ch = (loop_node_t*)list_iter_next(&it))) {
        changed |= _licm_loop_node_process(cctx, node, smt, licm);
    }

    changed |= _licm_process(cctx, node, smt, licm);
    return changed;
}

int HIR_LTREE_licm(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        int changed = 0;
        do {
            changed = 0;
            ltree_ctx_t lctx;
            list_init(&lctx.loops);

            HIR_LTREE_build_loop_tree(fb, &lctx);
            print_debug("HIR_LTREE_build_loop_tree size(loops)=%i", list_size(&lctx.loops));
            if (!list_size(&lctx.loops)) {
                HIR_LTREE_unload_ctx(&lctx);
                continue;
            }

            list_iter_t rit;
            list_iter_hinit(&lctx.loops, &rit);
            loop_node_t* root;
            while ((root = (loop_node_t*)list_iter_next(&rit))) {
                changed |= _licm_loop_node_process(cctx, root, smt, 1);
            }

            HIR_LTREE_unload_ctx(&lctx);
        } while (changed);
    }

    print_debug("HIR_LTREE_licm (with cycle tree) complete");
    return 1;
}

int HIR_LTREE_canonicalization(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        ltree_ctx_t lctx;
        list_init(&lctx.loops);

        HIR_LTREE_build_loop_tree(fb, &lctx);
        print_debug("HIR_LTREE_build_loop_tree size(loops)=%i", list_size(&lctx.loops));
        if (!list_size(&lctx.loops)) {
            HIR_LTREE_unload_ctx(&lctx);
            continue;
        }

        list_iter_t rit;
        list_iter_hinit(&lctx.loops, &rit);
        loop_node_t* root;
        while ((root = (loop_node_t*)list_iter_next(&rit))) {
            _licm_loop_node_process(cctx, root, NULL, 0);
        }

        HIR_LTREE_unload_ctx(&lctx);
    }

    print_debug("HIR_LTREE_canonicalization complete");
    return 1;
}
