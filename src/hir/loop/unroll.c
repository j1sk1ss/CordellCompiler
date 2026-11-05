#include <hir/loop.h>

#define UNROLL_MAX_INSTRUCTIONS 256

typedef struct {
    loop_node_t* loop;
    cfg_block_t* exit;
    hir_block_t* comparison;
    big_int_t    count;
    int          posttested;
} unroll_info_t;

static void _rename_subject(hir_subject_t* s, map_t* versions) {
    if (!s) return;
    void* id = NULL;
    if (HIR_is_vartype(s->t) && map_get(versions, s->storage.var.v_id, &id)) s->storage.var.v_id = (symbol_id_t)id;
    else if (s->t == HIR_ARGLIST) {
        foreach (hir_subject_t* arg, &s->storage.list.h) _rename_subject(arg, versions);
    }
    else if (s->t == HIR_PHISET) {
        set_foreach (int_tuple_t* incoming, &s->storage.set.h) {
            if (map_get(versions, incoming->y, &id)) incoming->y = (symbol_id_t)id;
        }
    }
    
    s->hash = 0;
}

static int _uses_variable(hir_subject_t* s, symbol_id_t id) {
    if (!s) return 0;
    if (HIR_is_vartype(s->t)) return s->storage.var.v_id == id;
    if (s->t == HIR_ARGLIST) {
        foreach (hir_subject_t* arg, &s->storage.list.h) {
            if (_uses_variable(arg, id)) return 1;
        }
    }
    else if (s->t == HIR_PHISET) {
        set_foreach (int_tuple_t* incoming, &s->storage.set.h) {
            if (incoming->y == id) return 1;
        }
    }

    return 0;
}

static hir_block_t* _get_dead_comparison(loop_node_t* loop, loop_iteration_condition_t* condition) {
    hir_block_t* comparison = condition->comparison;
    if (!comparison || !comparison->farg || !HIR_is_vartype(comparison->farg->t)) return NULL;
    if (comparison->farg->ptr || HIR_is_global(comparison->farg->t)) return NULL;
    foreach (cfg_block_t* bb, &loop->header->pfunc->blocks) {
        iterate_hir_instructions (bb) {
            if (hh->unused || hh == comparison || hh == condition->branch) continue;
            iterate_hir_args (hir_subject_t* arg, hh, 0) {
                if (_uses_variable(arg, comparison->farg->storage.var.v_id)) return NULL;
            }
        }
    }

    return comparison;
}

static int _is_valid_loop(loop_node_t* loop, loop_iteration_info_t* iterations) {
    if (
        list_size(&loop->children) || iterations->status != LOOP_ITERATION_FINITE ||
        list_size(&iterations->conditions) != 1 || !loop->preheader || !loop->latch->hmap.exit->next ||
        loop->header->hmap.entry->op != HIR_MKLB
    ) return 0;

    loop_iteration_condition_t* condition = list_get_head(&iterations->conditions);
    cfg_block_t* exit                     = set_has(&loop->blocks, condition->block->l) ? condition->block->jmp : condition->block->l;
    if (
        !exit || !exit->hmap.entry ||
        exit->hmap.entry->op != HIR_MKLB
    ) return 0;

    int instructions = 0;
    hir_block_t* previous = NULL;
    cfg_block_t* expected = loop->header;
    foreach (cfg_block_t* bb, &loop->blocks_sorted) {
        if (
            bb != expected || !bb->hmap.entry || !bb->hmap.exit ||
            (previous && previous->next != bb->hmap.entry)
        ) return 0;

        previous = bb->hmap.exit;
        set_foreach (cfg_block_t* pred, &bb->pred) {
            if (bb != loop->header && !set_has(&loop->blocks, pred)) return 0;
        }

        if (bb == condition->block) {
            if (bb != loop->header && bb != loop->latch) return 0;
            expected = set_has(&loop->blocks, bb->l) ? bb->l : bb->jmp;
        }
        else {
            if (set_size(&bb->succ) != 1) return 0;
            expected = bb->l ? bb->l : bb->jmp;
        }

        iterate_hir_instructions (bb) {
            if (hh->unused) continue;
            if (++instructions > UNROLL_MAX_INSTRUCTIONS) return 0;
            switch (hh->op) {
                case HIR_VARDECL: case HIR_STRDECL:     case HIR_REF:
                case HIR_STASM:   case HIR_DEFER_START: return 0;
                default: break;
            }
        }
    }

    return (
        expected == loop->header && list_get_tail(&loop->blocks_sorted) == loop->latch &&
        iterations->count <= UNROLL_MAX_INSTRUCTIONS / instructions - !condition->posttested
    );
}

static int _copy_iteration(unroll_info_t* info, int header_only, hir_block_t* anchor, map_t* versions, sym_table_t* smt) {
    loop_node_t* loop = info->loop;
    foreach (cfg_block_t* bb, &loop->blocks_sorted) {
        if (header_only && bb != loop->header) break;
        int_tuple_t incoming[UNROLL_MAX_INSTRUCTIONS];
        
        int count = 0;
        iterate_hir_instructions (bb) {
            if (
                hh->unused || hh == info->comparison || hh->op == HIR_MKLB ||
                hh->op == HIR_PHI || HIR_is_jmp(hh->op)
            ) continue;
            
            if (hh->op == HIR_PHI_PREAMBLE && map_get(versions, hh->farg->storage.var.v_id, NULL)) {
                void* id = (void*)hh->sarg->storage.var.v_id;
                map_get(versions, hh->sarg->storage.var.v_id, &id);
                incoming[count++] = (int_tuple_t){ .x = hh->farg->storage.var.v_id, .y = (symbol_id_t)id };
                continue;
            }
            
            hir_block_t* copy = HIR_copy_block(hh, 1);
            if (!copy) return 0;
            
            int write = HIR_is_writeop(copy->op);
            iterate_hir_args (hir_subject_t* arg, copy, write || copy->op == HIR_PHI_PREAMBLE) {
                _rename_subject(arg, versions);
            }
            
            if (write && copy->farg && HIR_is_vartype(copy->farg->t) && !HIR_is_global(copy->farg->t)) {
                variable_info_t vi;
                if (!VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) goto _failure;
                symbol_id_t id = VRTB_add_copy(&vi, &smt->v);
                if (id == NO_SYMBOL_ID || !map_put(versions, vi.v_id, (void*)id)) goto _failure;
                copy->farg->storage.var.v_id = id;
            }

            HIR_insert_block_before(copy, anchor);
            continue;
_failure: {}
            HIR_unload_block(copy);
            return 0;
        }

        for (int i = 0; i < count; i++) {
            if (!map_put(versions, incoming[i].x, (void*)incoming[i].y)) return 0;
        }
    }

    return 1;
}

static int _collect_loops(loop_node_t* loop, list_t* out, sym_table_t* smt) {
    foreach (loop_node_t* child, &loop->children) {
        if (!_collect_loops(child, out, smt)) return 0;
    }
    if (list_size(&loop->children)) return 1;

    int success = 1;
    loop_iteration_info_t iterations;
    if (!HIR_LOOP_analyze_iterations(loop, &iterations, smt)) success = 0;
    else if (_is_valid_loop(loop, &iterations)) {
        loop_iteration_condition_t* condition = list_get_head(&iterations.conditions);
        unroll_info_t* info = mm_malloc(sizeof(unroll_info_t));
        if (!info || !list_push_back(out, info)) {
            mm_free(info);
            success = 0;
        }
        else {
            info->loop       = loop;
            info->exit       = set_has(&loop->blocks, condition->block->l) ? condition->block->jmp : condition->block->l;
            info->comparison = _get_dead_comparison(loop, condition);
            info->count      = iterations.count;
            info->posttested = condition->posttested;
        }
    }

    HIR_LOOP_free_iteration_info(&iterations);
    return success;
}

static int _unroll_loop(unroll_info_t* info, sym_table_t* smt) {
    loop_node_t* loop    = info->loop;
    cfg_block_t* header = loop->header;
    hir_block_t* anchor = loop->latch->hmap.exit->next;

    map_t versions;
    map_init(&versions, MAP_NO_CMP);

    iterate_hir_instructions (header) {
        if (hh->op != HIR_PHI) continue;
        set_foreach (int_tuple_t* incoming, &hh->targ->storage.set.h) {
            if (incoming->x == loop->preheader->id && !map_put(&versions, hh->sarg->storage.var.v_id, (void*)incoming->y)) goto _done;
        }
    }

    for (big_int_t i = 0; i < info->count; i++) {
        if (!_copy_iteration(info, 0, anchor, &versions, smt)) goto _done;
    }
    
    if (
        !info->posttested && 
        !_copy_iteration(info, 1, anchor, &versions, smt)
    ) goto _done;

    hir_subject_t* label = HIR_copy_subject_and_label(info->exit->hmap.entry->farg);
    if (!label) goto _done;

    hir_block_t* jump = HIR_create_block(HIR_JMP, label, NULL, NULL);
    if (!jump) {
        HIR_unload_subject(label);
        goto _done;
    }

    HIR_insert_block_before(jump, anchor);

    foreach (cfg_block_t* bb, &header->pfunc->blocks) {
        if (set_has(&loop->blocks, bb)) continue;
        iterate_hir_instructions (bb) {
            if (hh->op == HIR_PHI_PREAMBLE && bb == loop->preheader && map_get(&versions, hh->farg->storage.var.v_id, NULL)) {
                hh->unused = 1;
                continue;
            }

            iterate_hir_args (hir_subject_t* arg, hh, hh->op == HIR_PHI ? 2 : HIR_is_writeop(hh->op) || hh->op == HIR_PHI_PREAMBLE) {
                _rename_subject(arg, &versions);
            }
            
            if (hh->op == HIR_PHI && bb == info->exit && info->posttested) {
                set_foreach (int_tuple_t* incoming, &hh->targ->storage.set.h) {
                    if (incoming->x == loop->latch->id) incoming->x = header->id;
                }
            }
        }
    }

    foreach (cfg_block_t* bb, &loop->blocks_sorted) {
        iterate_hir_instructions (bb) {
            if (hh != header->hmap.entry) hh->unused = 1;
        }

        set_foreach (cfg_block_t* succ, &bb->succ) {
            set_remove(&succ->pred, bb);
        }
    }

    RESET_SET(&header->succ);
    if (!set_add(&header->succ, info->exit) || !set_add(&info->exit->pred, header)) goto _done;
    
    header->l         = NULL;
    header->jmp       = info->exit;
    header->hmap.exit = jump;
    header->type      = CFG_DEFAULT_BLOCK;
    
    foreach (cfg_block_t* bb, &loop->blocks_sorted) {
        if (bb == header) continue;
        list_remove(&header->pfunc->blocks, bb);
        HIR_CFG_unload_block(bb);
    }

_done: {}
    map_free(&versions);
    return 1;
}

int HIR_LTREE_unroll(ltree_ctx_t* lctx, sym_table_t* smt) {
    list_t loops;
    list_init(&loops);
    map_foreach (list_t* roots, &lctx->lmap) {
        foreach (loop_node_t* loop, roots) {
            if (!_collect_loops(loop, &loops, smt)) goto _done;
        }
    }

    foreach (unroll_info_t* info, &loops) {
        if (!_unroll_loop(info, smt)) goto _done;
    }

_done: {}
    list_free_force(&loops);
    return 1;
}
