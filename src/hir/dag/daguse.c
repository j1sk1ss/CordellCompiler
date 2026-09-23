#include <hir/dag.h>

static int _subject_refs_node(hir_subject_t* s, dag_node_t* nd) {
    if (!s || !nd) return 0;
    if (s->t == HIR_ARGLIST) {
        foreach (hir_subject_t* arg, &s->storage.list.h) {
            if (_subject_refs_node(arg, nd)) return 1;
        }

        return 0;
    }

    return set_has(&nd->link, (void*)HIR_hash_subject(s));
}

static inline int _hir_arg_is_read(hir_operation_t op, int index) {
    switch (op) {
        case HIR_PHI:      case HIR_VARDECL:    case HIR_STRDECL:
        case HIR_FDCL:     case HIR_FEND:       case HIR_FEXT:
        case HIR_OEXT:     case HIR_STRT:       case HIR_STEND:
        case HIR_MKLB:     case HIR_JMP:        case HIR_BREAK:
        case HIR_SETPOS:   case HIR_BREAKPOINT: case HIR_MKSCOPE:
        case HIR_ENDSCOPE: case HIR_NOP: return 0;
        case HIR_PHI_PREAMBLE: return index == 1;
        case HIR_IFOP2:        return index == 0;
        default:               return !(HIR_is_writeop(op) && index == 0);
    }
}

static int _node_has_live_use(cfg_ctx_t* cctx, dag_node_t* nd) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_hir_instructions (bb) {
                if (hh->unused) continue;
                iterate_hir_args (hir_subject_t* s, hh, 0) {
                    if (!_hir_arg_is_read(hh->op, i)) continue;
                    if (_subject_refs_node(s, nd)) return 1;
                }
            }
        }
    }

    return 0;
}

static hir_block_t* _node_hir_home(dag_node_t* nd) {
    if (!nd || !nd->src || !HIR_is_vartype(nd->src->t)) return NULL;
    hir_block_t* home = nd->src->home;
    if (!home) return NULL;
    hir_subject_t* dst = home->op == HIR_PHI ? 
                                        home->sarg : 
                                        home->farg;
    if (
        (!dst || !_subject_refs_node(dst, nd)) ||
        (home->op != HIR_PHI && !HIR_is_writeop(home->op))
    ) return NULL;
    return home;
}

static int _node_has_required_effect(dag_node_t* nd) {
    hir_block_t* home = _node_hir_home(nd);
    if (!home || home->unused) return 0;
    switch (home->op) {
        case HIR_STORE_UFCLL:
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL:
        case HIR_STORE_SYSC: return 1;
        case HIR_STORE:      return HIR_is_global(home->farg->t);
        default:             return 0;
    }
}

static int _mark_unused_node(cfg_ctx_t* cctx, dag_node_t* nd, set_t* visited) {
    if (!nd || set_has(visited, nd)) return 1;
    set_add(visited, nd);
    if (_node_has_live_use(cctx, nd)) return 1;
    hir_block_t* home = _node_hir_home(nd);
    if (!home || _node_has_required_effect(nd)) return 1;
    home->unused = 1;
    set_foreach (dag_node_t* arg, &nd->args) {
        if (!_mark_unused_node(cctx, arg, visited)) return 0;
    }

    return 1;
}

int HIR_DAG_mark_unused_entries(cfg_ctx_t* cctx, dag_ctx_t* dctx) {
    if (!cctx || !dctx) return 0;
    set_foreach (hir_subject_t* src, &dctx->sources) {
        dag_node_t* nd = DAG_ACQUIRE_NODE(dctx, src);
        set_t visited;
        set_init(&visited, SET_NO_CMP);
        if (!_mark_unused_node(cctx, nd, &visited)) {
            set_free(&visited);
            return 0;
        }

        set_free(&visited);
    }

    return 1;
}
