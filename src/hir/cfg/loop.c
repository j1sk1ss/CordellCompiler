#include <hir/cfg.h>

static int _get_loop_blocks(cfg_block_t* entry, cfg_block_t* exit, set_t* b) {
    if (!set_add(b, entry)) return 0;
    if (entry == exit) return 0;

    set_iter_t it;
    set_iter_init(&entry->pred, &it);
    cfg_block_t* bb;
    while (set_iter_next(&it, (void**)&bb)) {
        _get_loop_blocks(bb, exit, b);
    }

    return 1;
}

static cfg_block_t* _insert_preheader(cfg_ctx_t* cctx, cfg_block_t* header, set_t* loop) {
    cfg_block_t* preheader = HIR_CFG_create_cfg_block(NULL);
    if (!preheader) return NULL;

    preheader->id = cctx->cid++;
    list_add(&header->pfunc->blocks, preheader);
    preheader->l = header;

    set_iter_t it;
    set_iter_init(&header->pred, &it);
    cfg_block_t* p;
    while (set_iter_next(&it, (void**)&p)) {
        if (set_has(loop, p)) continue;
        if (p->l && p->l == header)     p->l   = preheader;
        if (p->jmp && p->jmp == header) p->jmp = preheader;
        set_add(&preheader->pred, p);
        set_remove(&header->pred, p);
    }

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

static int _get_invariant_defs(set_t* loop_hir, set_t* invariant_defs) {
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

int HIR_CFG_loop_licm_canonicalization(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb->jmp && set_has(&cb->dom, cb->jmp)) {
                cfg_block_t* header = cb->jmp;
                cfg_block_t* latch  = cb;

                set_t loop_blocks;
                set_init(&loop_blocks);
                _get_loop_blocks(latch, header, &loop_blocks);
                set_add(&loop_blocks, header);

                cfg_block_t* preheader = _insert_preheader(cctx, header, &loop_blocks);
                if (!preheader) {
                    set_free(&loop_blocks);
                    continue;
                }

                set_t loop_hir;
                set_init(&loop_hir);
                _get_loop_hir_blocks(&loop_blocks, &loop_hir);
                print_debug("_get_loop_hir_blocks complete");

                set_t invariant_defs;
                set_init(&invariant_defs);
                _get_invariant_defs(&loop_hir, &invariant_defs);
                print_debug("_get_invariant_defs complete");

                list_t linear;
                list_init(&linear);
                
                list_iter_t it;
                list_iter_hinit(&fb->blocks, &it);
                cfg_block_t* cb3;
                while ((cb3 = (cfg_block_t*)list_iter_next(&it))) {
                    if (!set_has(&loop_blocks, cb3)) continue;
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
                    cfg_block_t* src_cfg = _get_hir_block_cfg(&loop_blocks, inv);
                    if (!src_cfg) continue;
                    HIR_CFG_remove_hir_block(src_cfg, inv);
                    HIR_CFG_append_hir_block_back(preheader, inv);
                    HIR_unlink_block(inv);
                    HIR_insert_block_before(inv, preheader->l->hmap.entry);
                }

                print_debug("preheader move complete");

                list_free(&linear);
                set_free(&invariant_defs);
                set_free(&loop_hir);
                set_free(&loop_blocks);

                print_debug("cleanup complete");
            }
        }
    }

    print_debug("HIR_CFG_loop_licm_canonicalization complete");
    return 1;
}
