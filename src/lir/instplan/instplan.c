#include <lir/instplan/instplan.h>

#ifdef DEBUG
static void _dump_instructions_dag_dot(instructions_dag_t* dag, long bid) {
    printf("==== INSTRUCTIONS DAG DUMP ====\n");
    printf("digraph InstructionsDAG%i {\n", bid);
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
            case LIR_REF:
            case LIR_GDREF:
            case LIR_LDREF:
            case LIR_iMOV: {
                instructions_dag_node_t* src  = _find_or_create_node(_find_src(lh->prev, bb->lmap.entry, lh->sarg), dag);
                instructions_dag_node_t* inst = _set_node(lh, dag);
                if (src) set_add(&inst->vert, src);
                break;
            }
            
            case LIR_JE:
            case LIR_JNE: {
                instructions_dag_node_t* src = _find_or_create_node(_find_first_cmp(lh, bb->lmap.entry), dag);
                instructions_dag_node_t* inst = _set_node(lh, dag);
                if (src) set_add(&inst->vert, src);
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
                lir_block_t* rax = _find_src(lh->prev, bb->lmap.entry, lh->farg);
                lir_block_t* rbx = _find_src(lh->prev, bb->lmap.entry, lh->sarg);
                
                instructions_dag_node_t* rax_nd = _find_or_create_node(rax, dag);
                instructions_dag_node_t* rbx_nd = _find_or_create_node(rbx, dag);
                instructions_dag_node_t* inst   = _set_node(lh, dag);

                if (rax_nd) set_add(&inst->vert, rax_nd);
                if (rbx_nd) set_add(&inst->vert, rbx_nd);
                break;
            }
        }
        
        if (lh == bb->lmap.exit) break;
        lh = lh->next;
    }

    return 1;
}

static int _unload_dag(instructions_dag_t* dag) {
    map_iter_t it;
    map_iter_init(&dag->alive_edges, &it);
    instructions_dag_node_t* nd;
    while (map_iter_next(&it, (void**)&nd)) {
        set_free(&nd->vert);
    }

    return map_free_force(&dag->alive_edges);
}

int LIR_plan_instructions(cfg_ctx_t* cctx, target_info_t* trginfo, inst_planner_t* planner) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = list_iter_next(&bit))) {
            instructions_dag_t dag;
            map_init(&dag.alive_edges);
            _build_instructions_dag(bb, &dag);
#ifdef DEBUG
            _dump_instructions_dag_dot(&dag, bb->id);
#endif
            _unload_dag(&dag);
        }
    }
    
    return 1;
}
