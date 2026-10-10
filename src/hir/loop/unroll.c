#include <hir/loop.h>

#define UNROLL_MAX_BLOCKS       8
#define UNROLL_MAX_INSTRUCTIONS 64
#define IS_UNSUPPORTED(op) \
    (op == HIR_VARDECL || op == HIR_VARDECL || op == HIR_STRDECL || op == HIR_REF || op == HIR_STASM || op == HIR_DEFER_START)

typedef struct {
    loop_node_t* loop;
    list_t       blocks;       /* Original CFG blocks in function order.             */
    map_t        definitions;  /* Original temporary ID -> defining HIR instruction. */
    map_t        block_labels; /* Original CFG block -> label of its copy.           */
    map_t        labels;       /* Original label ID -> label of its copy.            */
    map_t        temporaries;  /* Original temporary ID -> fresh variable ID.        */
} unroll_plan_t;

static int _is_flow_break(hir_operation_t op) {
    return HIR_is_jmp(op) || HIR_is_term(op);
}

static int _can_copy_fallthrough(loop_node_t* loop, cfg_block_t* bb) {
    if (_is_flow_break(bb->hmap.exit->op)) return 1;
    if (!bb->l) return 0;
    return set_has(&loop->blocks, bb->l) || (bb->l->hmap.entry && bb->l->hmap.entry->op == HIR_MKLB);
}

static int _uses_local_temporary(hir_subject_t* s, map_t* definitions) {
    if (!s) return 0;
    if (HIR_is_tmptype(s->t)) return map_get(definitions, s->storage.var.v_id, NULL);
    if (s->t == HIR_ARGLIST) {
        foreach (hir_subject_t* arg, &s->storage.list.h) {
            if (_uses_local_temporary(arg, definitions)) return 1;
        }
    }

    return 0;
}

static int _temporary_use_is_local(hir_subject_t* s, unroll_plan_t* plan, cfg_block_t* use, set_t* available) {
    if (!s) return 1;
    if (s->t == HIR_ARGLIST) {
        foreach (hir_subject_t* arg, &s->storage.list.h) {
            if (!_temporary_use_is_local(arg, plan, use, available)) return 0;
        }
    }

    hir_block_t* definition = NULL;
    if (
        !HIR_is_tmptype(s->t) ||
        !map_get(&plan->definitions, s->storage.var.v_id, (void**)&definition)
    ) return 1;
    
    foreach (cfg_block_t* bb, &plan->blocks) {
        iterate_hir_instructions (bb) {
            if (hh != definition) continue;
            if (bb == use) return set_has(available, (void*)s->storage.var.v_id);
            return set_has(&use->dom, bb);
        }
    }

    return 0;
}

/* Check whether this is a loop that we CAN optimize.
Will return 1 if we can, and 0 if not.
*/
static int _is_valid_loop(unroll_plan_t* plan) {
    loop_node_t* loop = plan->loop;
    if (
        loop->p || list_size(&loop->children) || set_size(&loop->blocks) > UNROLL_MAX_BLOCKS || /* too big                             */
        !loop->header->hmap.entry || loop->header->hmap.entry->op != HIR_MKLB ||                /* doesn't have a lable in the header  */
        loop->latch->hmap.exit->op != HIR_JMP && loop->latch->hmap.exit->op != HIR_IFOP2        /* doesn't have a condition            */
    ) return 0;

    int backedges = 0, instructions = 0;
    cfg_block_t* exit_target = NULL;
    set_foreach (cfg_block_t* bb, &loop->blocks_sorted) {
        if (
            !set_has(&bb->dom, loop->header) ||                                                 /* If this is a non-dominated block    */
            !bb->hmap.entry || !bb->hmap.exit || bb->hmap.exit->unused ||                       /* it doesn't have an entry and exit   */
            !_can_copy_fallthrough(loop, bb)                                                    /* or we can't simply copy it          */
        ) return 0;

        if (bb->l == loop->header)   backedges++;
        if (bb->jmp == loop->header) backedges++;
        if (backedges > 1) return 0;

        cfg_block_t* successors[] = { bb->l, bb->jmp };
        for (int i = 0; i < 2; i++) {
            cfg_block_t* succ = successors[i];
            if (!succ || set_has(&loop->blocks, succ)) continue;
            if (exit_target && exit_target != succ)    return 0;                                /* several exit targets                */
            exit_target = succ;
        }

        set_foreach (cfg_block_t* pred, &bb->pred) {
            if (
                bb != loop->header && 
                !set_has(&loop->blocks, pred)                                                   /* several entry points                */
            ) return 0;
        }

        iterate_hir_instructions (bb) {
            if (++instructions > UNROLL_MAX_INSTRUCTIONS || IS_UNSUPPORTED(hh->op)) return 0;
            if (!HIR_is_writeop(hh->op) || !hh->farg || !HIR_is_tmptype(hh->farg->t)) continue;
            symbol_id_t id = hh->farg->storage.var.v_id;
            if (HIR_is_arrtype(hh->farg->t) || map_get(&plan->definitions, id, NULL)) return 0;
            if (!map_put(&plan->definitions, id, hh)) return -1;
        }

        if (!list_push_back(&plan->blocks, bb)) return -1;
    }

    if (
        !backedges                                   || /* there is no backedges in the loop */
        list_get_head(&plan->blocks) != loop->header || 
        list_get_tail(&plan->blocks) != loop->latch  || 
        !loop->latch->hmap.exit->next
    ) return 0;

    foreach (cfg_block_t* bb, &loop->header->pfunc->blocks) {
        if (!set_has(&loop->blocks, bb)) {
            iterate_hir_instructions (bb) {
                iterate_hir_args (hir_subject_t* arg, hh, 0) {
                    if (_uses_local_temporary(arg, &plan->definitions)) return 0;
                }
            }

            continue;
        }

        set_t available;
        if (!set_init(&available, SET_NO_CMP)) return -1;

        int valid = 1;
        iterate_hir_instructions (bb) {
            iterate_hir_args (hir_subject_t* arg, hh, HIR_is_writeop(hh->op)) {
                if (!_temporary_use_is_local(arg, plan, bb, &available)) valid = 0;
            }

            if (HIR_is_writeop(hh->op) && hh->farg && HIR_is_tmptype(hh->farg->t)) {
                if (!set_add(&available, (void*)hh->farg->storage.var.v_id)) {
                    set_free(&available);
                    return -1;
                }
            }
        }

        set_free(&available);
        if (!valid) return 0;
    }

    return 1;
}

static int _allocate_names(unroll_plan_t* plan, sym_table_t* smt) {
    foreach (cfg_block_t* bb, &plan->blocks) {
        hir_subject_t* label = HIR_SUBJ_LABEL();
        if (!label) return 0;
        if (!map_put(&plan->block_labels, (long)bb, label)) {
            HIR_unload_subject(label);
            return 0;
        }

        if (
            bb->hmap.entry->op == HIR_MKLB && 
            !map_put(&plan->labels, bb->hmap.entry->farg->id, label)
        ) return 0;
    }

    map_foreach (hir_block_t* definition, &plan->definitions) {
        symbol_id_t old_id = definition->farg->storage.var.v_id;
        variable_info_t vi;
        if (!VRTB_get_info_id(old_id, &vi, &smt->v)) return 0;
        symbol_id_t new_id = VRTB_add_copy(&vi, &smt->v);
        if (
            new_id == NO_SYMBOL_ID || 
            !map_put(&plan->temporaries, old_id, (void*)new_id)
        ) return 0;
    }

    return 1;
}

static hir_subject_t* _copy_subject(hir_subject_t* s, unroll_plan_t* plan, int latch_terminator) {
    hir_subject_t* copy = HIR_copy_subject_and_label(s);
    if (copy && HIR_is_tmptype(copy->t)) {
        void* new_id = NULL;
        if (map_get(&plan->temporaries, s->storage.var.v_id, &new_id)) {
            copy->storage.var.v_id = (symbol_id_t)new_id;
            copy->hash = 0;
        }
    }

    return copy;
}

static hir_block_t* _copy_instruction(hir_block_t* hh, unroll_plan_t* plan, int latch_terminator) {
    hir_subject_t* copies[3] = { 0 };
    iterate_hir_args (hir_subject_t* arg, hh, 0) {
        copies[i] = _copy_subject(arg, plan, latch_terminator);
        if (arg && !copies[i]) goto _failure;
    }

    hir_block_t* copy = HIR_create_block(hh->op, copies[0], copies[1], copies[2]);
    if (copy) return copy;
_failure: {}
    for (int i = 0; i < 3; i++) HIR_unload_subject(copies[i]);
    return NULL;
}

static int _clone_loop_region(unroll_plan_t* plan, hir_ctx_t* fragment) {
    foreach (cfg_block_t* bb, &plan->blocks) {
        hir_subject_t* label = NULL;
        map_get(&plan->block_labels, (long)bb, (void**)&label);

        HIR_BLOCK1(fragment, HIR_MKLB, HIR_copy_subject_and_label(label));
        iterate_hir_instructions (bb) {
            if (hh == bb->hmap.entry && hh->op == HIR_MKLB) continue;
            int latch_terminator = bb == plan->loop->latch && hh == bb->hmap.exit;
            if (!HIR_append_block(_copy_instruction(hh, plan, latch_terminator), fragment)) return 0;
        }

        if (!_is_flow_break(bb->hmap.exit->op)) {
            hir_subject_t* target = NULL;
            if (!map_get(&plan->block_labels, (long)bb->l, (void**)&target)) {
                target = bb->l->hmap.entry->farg;
            }

            HIR_BLOCK1(fragment, HIR_JMP, HIR_copy_subject_and_label(target));
        }
    }
    return 1;
}

static hir_subject_t** _backedge_operand(loop_node_t* loop) {
    hir_block_t* term = loop->latch->hmap.exit;
    if (term->op == HIR_JMP) return &term->farg;
    return loop->latch->l == loop->header ? &term->sarg : &term->targ;
}

static int _label_is_shared(loop_node_t* loop, hir_subject_t** operand) {
    foreach (cfg_block_t* bb, &loop->header->pfunc->blocks) {
        iterate_hir_instructions (bb) {
            iterate_ref_hir_args (hir_subject_t** arg, hh, 0) {
                if (arg != operand && *arg == *operand) return 1;
            }
        }
    }

    return 0;
}

static int _unroll_loop(loop_node_t* loop, sym_table_t* smt) {
    unroll_plan_t plan = { .loop = loop };
    hir_ctx_t fragment = { 0 };
    hir_subject_t* back_target = NULL;
    list_init(&plan.blocks);
    int result = -1;

    if (
        !map_init(&plan.definitions, MAP_NO_CMP)  || 
        !map_init(&plan.block_labels, MAP_NO_CMP) ||
        !map_init(&plan.labels, MAP_NO_CMP)       || 
        !map_init(&plan.temporaries, MAP_NO_CMP)
    ) goto _cleanup;

    result = _is_valid_loop(&plan);
    if (result != 1) goto _cleanup;

    result = -1;
    if (
        !_allocate_names(&plan, smt) || 
        !_clone_loop_region(&plan, &fragment)
    ) goto _cleanup;

    hir_subject_t* copy_header = NULL;
    map_get(&plan.block_labels, (long)loop->header, (void**)&copy_header);
    back_target = _copy_label(copy_header);
    if (!back_target) goto _cleanup;

    hir_subject_t** operand = _backedge_operand(loop);
    if (!_label_is_shared(loop, operand)) HIR_unload_subject(*operand);

    *operand             = back_target;
    back_target->home    = loop->latch->hmap.exit;
    back_target          = NULL;
    hir_block_t* anchor  = loop->latch->hmap.exit->next;
    hir_block_t* prev    = anchor->prev;
    prev->next           = fragment.hot.h;
    fragment.hot.h->prev = prev;
    fragment.hot.t->next = anchor;
    anchor->prev         = fragment.hot.t;
    fragment.hot.h       = fragment.hot.t = NULL;
    result = 1;

_cleanup: {}
    HIR_unload_subject(back_target);
    HIR_unload_blocks(fragment.hot.h);
    list_free(&plan.blocks);
    map_free(&plan.definitions);
    map_free(&plan.labels);
    map_free(&plan.temporaries);
    
    if (plan.block_labels.entries) {
        map_free_force_op(&plan.block_labels, (int (*)(void*))HIR_unload_subject);
    }

    return result;
}

int HIR_LTREE_unroll(ltree_ctx_t* lctx, sym_table_t* smt) {
    int changed = 0;
    map_foreach (list_t* loops, &lctx->lmap) {
        foreach (loop_node_t* loop, loops) {
            int result = _unroll_loop(loop, smt);
            if (result < 0) return -1;
            changed += result;
        }
    }

    return changed;
}
