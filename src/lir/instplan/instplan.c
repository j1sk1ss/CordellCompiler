#include <lir/instplan/instplan.h>

#ifdef DEBUG
static void _dump_instructions_dag_dot(instructions_dag_t* dag, long bid) {
    printf("==== INSTRUCTIONS DAG DUMP ====\n");
    printf("digraph InstructionsDAG%ld {\n", bid);
    printf("  rankdir=TB;\n");
    printf("  node [shape=box, style=filled, fillcolor=lightgray];\n\n");

    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* node;
    while (map_iter_next(&it, (void**)&node)) {
        printf("  node_%p [label=\"%i\"];\n", node, node->b->op);
    }

    map_iter_init(&dag->alive_edges, &it);
    while (map_iter_next(&it, (void**)&node)) {
        set_iter_t sit;
        set_iter_init(&node->vert, &sit);
        instructions_dag_node_t* target;
        while (set_iter_next(&sit, (void**)&target)) {
            printf("  node_%p -> node_%p;\n", target, node);
        }
    }

    printf("}\n");
    printf("===============================\n");
}
#endif

static instructions_dag_node_t* _create_dag_node(lir_block_t* lh) {
    instructions_dag_node_t* nd = (instructions_dag_node_t*)mm_malloc(sizeof(instructions_dag_node_t));
    if (!nd) return NULL;
    set_init(&nd->vert);
    set_init(&nd->users);
    nd->b = lh;
    return nd;
}

static instructions_dag_node_t* _set_node(lir_block_t* lh, instructions_dag_t* dag) {
    instructions_dag_node_t* nd = _create_dag_node(lh);
    if (!nd) return 0;
    map_put(&dag->alive_edges, (long)lh, nd);
    return nd;
}

static instructions_dag_node_t* _find_or_create_node(lir_block_t* lh, instructions_dag_t* dag) {
    if (!lh) return NULL;
    instructions_dag_node_t* nd;
    if (map_get(&dag->alive_edges, (long)lh, (void**)&nd)) {
        return nd;
    }

    return _set_node(lh, dag);
}

static lir_block_t* _find_first_cmp(lir_block_t* lh, lir_block_t* exit) {
    while (lh) {
        if (lh->op == LIR_CMP) return lh;
        if (lh == exit) break;
        lh = lh->prev;
    }

    return NULL;
}

static lir_block_t* _find_src(lir_block_t* lh, lir_block_t* exit, lir_subject_t* trg) {
    while (lh) {
        if (lh->farg && LIR_subj_equals(lh->farg, trg)) return lh;
        if (lh == exit) break;
        lh = lh->prev;
    }

    return NULL;
} 

static int _build_instructions_dag(cfg_block_t* bb, instructions_dag_t* dag) {
    lir_block_t* lh = bb->lmap.entry;
    while (lh) {
        switch (lh->op) {
            case LIR_ARRDECL: {
                instructions_dag_node_t* inst = _set_node(lh, dag);

                list_iter_t el_it;
                list_iter_hinit(&lh->targ->storage.list.h, &el_it);
                lir_subject_t* elem;
                while ((elem = (lir_subject_t*)list_iter_next(&el_it))) {
                    if (elem->t != LIR_VARIABLE) continue;
                    instructions_dag_node_t* src = _find_or_create_node(_find_src(lh, bb->lmap.entry, elem), dag);
                    if (src) {
                        set_add(&inst->vert, src);
                        set_add(&src->users, inst);
                    }
                }

                break;
            }

            case LIR_LOADFARG:
            case LIR_LOADFRET:
            case LIR_STARGLD: {
                _set_node(lh, dag);
                break;
            }

            case LIR_TI64:
            case LIR_TI32:
            case LIR_TI16:
            case LIR_TI8:
            case LIR_TU64:
            case LIR_TU32:
            case LIR_TU16:
            case LIR_TU8:
            case LIR_TF64:
            case LIR_TF32:
            case LIR_REF:
            case LIR_iMOV:
            case LIR_GDREF:
            case LIR_LDREF: {
                instructions_dag_node_t* src  = _find_or_create_node(_find_src(lh, bb->lmap.entry, lh->sarg), dag);
                instructions_dag_node_t* inst = _set_node(lh, dag);
                if (src) {
                    set_add(&inst->vert, src);
                    set_add(&src->users, inst);
                }

                break;
            }
            
            case LIR_JE:
            case LIR_JNE: {
                instructions_dag_node_t* src  = _find_or_create_node(_find_first_cmp(lh, bb->lmap.entry), dag);
                instructions_dag_node_t* inst = _set_node(lh, dag);
                if (src) {
                    set_add(&inst->vert, src);
                    set_add(&src->users, inst);
                }

                break;
            }

            case LIR_VRUSE:
            case LIR_STFARG: {
                instructions_dag_node_t* src  = _find_or_create_node(_find_src(lh, bb->lmap.entry, lh->farg), dag);
                instructions_dag_node_t* inst = _set_node(lh, dag);
                if (src) {
                    set_add(&inst->vert, src);
                    set_add(&src->users, inst);
                }

                break;
            }

            case LIR_SYSC:
            case LIR_ECLL:
            case LIR_FCLL: {
                lir_block_t* (*finder)(lir_block_t *, lir_block_t *, int) = LIR_planner_get_next_func_abi;
                if (lh->op == LIR_SYSC) finder = LIR_planner_get_next_sysc_abi;

                int reg_num = 0;
                lir_block_t* abi_reg;
                instructions_dag_node_t* inst = _set_node(lh, dag);
                while ((abi_reg = (lir_block_t*)finder(lh->prev, bb->lmap.entry, reg_num++))) {
                    instructions_dag_node_t* src = _find_or_create_node(abi_reg, dag);
                    if (src) {
                        set_add(&inst->vert, src);
                        set_add(&src->users, inst);
                    }
                }

                lir_block_t* fres = LIR_planner_get_func_res(lh, bb->lmap.exit);
                if (fres) {
                    instructions_dag_node_t* res = _set_node(fres, dag);
                    set_add(&res->vert, inst);
                    set_add(&inst->users, res);
                    lh = lh->next;
                }

                break;
            }

            case LIR_bOR:
            case LIR_CMP:
            case LIR_iLWR:
            case LIR_iLRE:
            case LIR_iLRG:
            case LIR_iLGE:
            case LIR_iCMP:
            case LIR_iNMP:
            case LIR_iDIV:
            case LIR_iMOD:
            case LIR_bXOR:
            case LIR_bAND:
            case LIR_iMUL:
            case LIR_iSUB:
            case LIR_iADD: {
                lir_block_t* rax = _find_src(lh, bb->lmap.entry, lh->farg);
                lir_block_t* rbx = _find_src(lh, bb->lmap.entry, lh->sarg);
                
                instructions_dag_node_t* rax_nd = _find_or_create_node(rax, dag);
                instructions_dag_node_t* rbx_nd = _find_or_create_node(rbx, dag);
                instructions_dag_node_t* inst   = _set_node(lh, dag);

                if (rax_nd) {
                    set_add(&inst->vert, rax_nd);
                    set_add(&rax_nd->users, inst);
                }

                if (rbx_nd) {
                    set_add(&inst->vert, rbx_nd);
                    set_add(&rbx_nd->users, inst);
                }

                break;
            }

            default: break;
        }
        
        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        nd->remaining_deps = set_size(&nd->vert);
    }

    return 1;
}

static int _unload_dag(instructions_dag_t* dag) {
    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        set_free(&nd->vert);
        set_free(&nd->users);
    }

    return map_free_force(&dag->alive_edges);
}

static int _dag_toposort(instructions_dag_t* dag, list_t* sorted) {
    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        nd->indegree = 0;
    }

    map_iter_init(&dag->alive_edges, &it);
    while (map_iter_next(&it, (void**)&nd)) {
        set_iter_t sit;
        set_iter_init(&nd->vert, &sit);
        instructions_dag_node_t* pred;
        while (set_iter_next(&sit, (void**)&pred)) {
            pred->indegree++;
        }
    }

    list_t queue;
    list_init(&queue);
    map_iter_init(&dag->alive_edges, &it);
    while (map_iter_next(&it, (void**)&nd)) {
        if (!nd->indegree) {
            list_push_back(&queue, nd);
        }
    }

    while (list_size(&queue)) {
        nd = list_pop_front(&queue);
        list_push_back(sorted, nd);

        set_iter_t sit;
        set_iter_init(&nd->vert, &sit);
        instructions_dag_node_t* succ;
        while (set_iter_next(&sit, (void**)&succ)) {
            succ->indegree--;
            if (!succ->indegree) {
                list_push_back(&queue, succ);
            }
        }
    }

    list_free(&queue);
    return 1;
}

static int _compute_critical_path(target_info_t* trginfo, instructions_dag_t* dag) {
    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        op_info_t* opinfo;
        if (map_get(&trginfo->info, nd->b->op, (void**)&opinfo)) {
            nd->critical_path = opinfo->latency;
        }
    }

    list_t sorted;
    list_init(&sorted);
    _dag_toposort(dag, &sorted);
    
    list_iter_t it2;
    list_iter_hinit(&sorted, &it2);
    instructions_dag_node_t* node;
    while ((node = (instructions_dag_node_t*)list_iter_next(&it2))) {
        set_iter_t sit;
        set_iter_init(&node->vert, &sit);
        instructions_dag_node_t* dep;
        while (set_iter_next(&sit, (void**)&dep)) {
            op_info_t* opinfo;
            if (map_get(&trginfo->info, dep->b->op, (void**)&opinfo)) {
                int cand = dep->critical_path + opinfo->latency;
                if (cand > node->critical_path) node->critical_path = cand;
            }
        }
    }

    list_free(&sorted);
    return 1;
}

static instructions_dag_node_t* _select_best_node(list_t* ready_list) {
    if (!list_size(ready_list)) return NULL;

    list_iter_t it;
    list_iter_hinit(ready_list, &it);
    instructions_dag_node_t *nd = NULL, *best = NULL;

    best = (instructions_dag_node_t*)list_iter_next(&it);
    int best_cp = best->critical_path;

    while ((nd = (instructions_dag_node_t*)list_iter_next(&it))) {
        if (nd->critical_path > best_cp) {
            best_cp = nd->critical_path;
            best = nd;
        }
    }

    return best;
}

static int _apply_schedule(cfg_block_t* bb, list_t* scheduled) {
    if (!bb || !list_size(scheduled)) return 0;
    lir_block_t* prev = NULL;

    list_iter_t it;
    list_iter_hinit(scheduled, &it);
    instructions_dag_node_t* nd;
    while ((nd = (instructions_dag_node_t*)list_iter_next(&it))) {
        if (prev) {
            HIR_CFG_remove_lir_block(bb, nd->b);
            LIR_unlink_block(nd->b);
            LIR_insert_block_after(nd->b, prev);
            if (bb->lmap.exit == prev) {
                bb->lmap.exit = nd->b;
            }
        }

        prev = nd->b;
    }

    return 1;
}

static int _schedule_block(cfg_block_t* bb, instructions_dag_t* dag, target_info_t* trginfo) {
    _compute_critical_path(trginfo, dag);

    list_t ready_list;
    list_init(&ready_list);

    map_iter_t mit;
    map_iter_init(&dag->alive_edges, &mit);
    instructions_dag_node_t* nd;
    while (map_iter_next(&mit, (void**)&nd)) {
        if (!set_size(&nd->vert)) {
            list_push_back(&ready_list, nd);
        }
    }

    list_t scheduled;
    list_init(&scheduled);

    while (list_size(&ready_list)) {
        instructions_dag_node_t* best = _select_best_node(&ready_list);
        list_push_back(&scheduled, best);
        list_remove(&ready_list, best);

        set_iter_t sit;
        set_iter_init(&best->users, &sit);
        instructions_dag_node_t* child;
        while (set_iter_next(&sit, (void**)&child)) {
            child->remaining_deps--;
            if (!child->remaining_deps) {
                list_push_back(&ready_list, child);
            }
        }
    }

    _apply_schedule(bb, &scheduled);
    list_free(&scheduled);
    list_free(&ready_list);
    return 1;
}

int LIR_plan_instructions(cfg_ctx_t* cctx, target_info_t* trginfo) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            instructions_dag_t dag;
            map_init(&dag.alive_edges);
            _build_instructions_dag(bb, &dag);
#ifdef DEBUG
            _dump_instructions_dag_dot(&dag, bb->id);
#endif
            _schedule_block(bb, &dag, trginfo);
            _unload_dag(&dag);
        }
    }
    
    return 1;
}
