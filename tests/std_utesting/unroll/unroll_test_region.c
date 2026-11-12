#include <hir/loop.h>
#include "../../misc/testing.h"

static hir_subject_t* _var(long id) {
    return HIR_SUBJ_STKVAR(id, HIR_STKVARI64, 0);
}

static hir_subject_t* _label(long id) {
    hir_subject_t* label = HIR_SUBJ_LABEL();
    label->id = id;
    return label;
}

static hir_block_t* _append(hir_ctx_t* hir, cfg_block_t* bb, hir_operation_t op, hir_subject_t* a, hir_subject_t* b, hir_subject_t* c) {
    hir_block_t* hh = HIR_create_block(op, a, b, c);
    HIR_append_block(hh, hir);
    if (!bb->hmap.entry) bb->hmap.entry = hh;
    bb->hmap.exit = hh;
    return hh;
}

static void _edge(cfg_block_t* from, cfg_block_t* to, int jump) {
    if (jump) from->jmp = to;
    else      from->l   = to;
    set_add(&from->succ, to);
    set_add(&to->pred, from);
}

/* Execute the emitted straight-line HIR, including the existing SSA edge copies. */
static long _execute(hir_ctx_t* hir) {
    long values[1024] = { 0 };
    for (hir_block_t* hh = hir->hot.h; hh; hh = hh->next) {
        if (hh->unused) continue;
        long a = hh->sarg && HIR_is_vartype(hh->sarg->t) ? values[hh->sarg->storage.var.v_id] : 0;
        switch (hh->op) {
            case HIR_STORE: values[hh->farg->storage.var.v_id] = hh->sarg->storage.cnst.value; break;
            case HIR_PHI_PREAMBLE: values[hh->farg->storage.var.v_id] = a; break;
            case HIR_iADD: values[hh->farg->storage.var.v_id] = a + hh->targ->storage.cnst.value; break;
            case HIR_iLWR: values[hh->farg->storage.var.v_id] = a < hh->targ->storage.cnst.value; break;
            case HIR_iLGE: values[hh->farg->storage.var.v_id] = a >= hh->targ->storage.cnst.value; break;
            case HIR_JMP: {
                long target = hh->farg->id;
                for (hh = hir->hot.h; hh && !(hh->op == HIR_MKLB && hh->farg->id == target); hh = hh->next) {}
                if (!hh) return -1;
                break;
            }
            case HIR_EXITOP: return values[hh->farg->storage.var.v_id];
            default: break;
        }
    }
    return -1;
}

static int _test_loop(long count, int posttested, int skip, int inverted, int live) {
    sym_table_t* smt = mm_malloc(sizeof(sym_table_t));
    SMT_init(smt);
    for (int i = 0; i < 16; i++) VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &smt->v);
    variable_info_t* initial = NULL;
    map_get(&smt->v.vartb, 0, (void**)&initial);
    initial->vdi.defined = skip == 1 ? UNDEFINED_VARIABLE : DEFINED_VARIABLE;
    initial->vdi.definition = 0;

    cfg_func_t func = { 0 };
    list_init(&func.blocks);
    cfg_block_t* b[4];
    for (int i = 0; i < 4; i++) {
        b[i] = HIR_CFG_create_cfg_block(NULL);
        b[i]->id = i;
        b[i]->pfunc = &func;
        list_push_back(&func.blocks, b[i]);
    }
    _edge(b[0], b[1], 0);
    _edge(b[1], b[2], posttested ? 0 : inverted);
    _edge(b[2], b[1], posttested ? inverted : 1);
    _edge(b[posttested ? 2 : 1], b[3], !inverted);
    hir_ctx_t hir = { 0 };
    _append(&hir, b[0], HIR_STORE, _var(0), HIR_SUBJ_CONST(0), NULL);
    _append(&hir, b[0], HIR_PHI_PREAMBLE, _var(1), _var(0), NULL);
    _append(&hir, b[1], HIR_MKLB, _label(1), NULL, NULL);
    hir_subject_t* incoming = HIR_SUBJ_SET();
    set_add(&incoming->storage.set.h, inttuple_create(0, 0));
    set_add(&incoming->storage.set.h, inttuple_create(2, 2));
    _append(&hir, b[1], HIR_PHI, _var(7), _var(1), incoming);
    if (!posttested) {
        _append(&hir, b[1], inverted ? HIR_iLGE : HIR_iLWR, _var(3), _var(1), HIR_SUBJ_CONST(count));
        _append(&hir, b[1], HIR_IFOP2, _var(3), _label(inverted ? 3 : 2), _label(inverted ? 2 : 3));
    }
    else _append(&hir, b[1], HIR_JMP, _label(2), NULL, NULL);
    _append(&hir, b[2], HIR_MKLB, _label(2), NULL, NULL);
    _append(&hir, b[2], HIR_iADD, _var(2), _var(1), HIR_SUBJ_CONST(skip == 3 ? 0 : 1));
    _append(&hir, b[2], HIR_PHI_PREAMBLE, _var(1), _var(2), NULL);
    if (posttested) {
        _append(&hir, b[2], inverted ? HIR_iLGE : HIR_iLWR, _var(3), _var(2), HIR_SUBJ_CONST(count));
        _append(&hir, b[2], HIR_IFOP2, _var(3), _label(inverted ? 3 : 1), _label(inverted ? 1 : 3));
    }
    else _append(&hir, b[2], HIR_JMP, _label(1), NULL, NULL);
    _append(&hir, b[3], HIR_MKLB, _label(3), NULL, NULL);
    _append(&hir, b[3], HIR_EXITOP, _var(live ? 3 : 1), NULL, NULL);

    loop_node_t loop = { .header = b[1], .latch = b[2], .preheader = b[0] };
    set_init(&loop.blocks, SET_NO_CMP);
    set_init(&loop.exits, SET_NO_CMP);
    list_init(&loop.blocks_sorted);
    list_init(&loop.children);
    for (int i = 1; i <= 2; i++) {
        set_add(&loop.blocks, b[i]);
        list_push_back(&loop.blocks_sorted, b[i]);
    }
    set_add(&loop.exits, b[3]);
    ltree_ctx_t loops;
    map_init(&loops.lmap, MAP_NO_CMP);
    list_t roots;
    list_init(&roots);
    list_push_back(&roots, &loop);
    map_put(&loops.lmap, 0, &roots);
    /* A previous pass can bypass this loop by updating CFG alone. */
    cfg_block_t* bypass = HIR_CFG_create_cfg_block(NULL);
    bypass->pfunc = &func;
    list_push_back(&func.blocks, bypass);
    _edge(bypass, b[3], 1);
    int allocated = mm_get_allocated();
    int result = HIR_LTREE_unroll(&loops, smt);
    assert(result == !skip, "Unexpected unroll result!");
    if (!skip) {
        assert(_execute(&hir) == (live ? inverted : posttested && !count ? 1 : count), "Unroll changed the result!");
        assert(b[0]->l == b[1] && b[1]->jmp == b[3], "CFG does not follow the unrolled body!");
        assert(set_has(&b[1]->succ, b[3]) && set_has(&b[3]->pred, b[1]), "CFG edges are inconsistent!");
        assert(set_size(&b[1]->pred) == 1 && set_size(&b[3]->pred) == 2, "Old loop edges survived!");
        assert(list_size(&func.blocks) == 4, "Old loop blocks survived!");
        set_t definitions;
        set_init(&definitions, SET_NO_CMP);
        for (hir_block_t* hh = hir.hot.h; hh; hh = hh->next) {
            assert(hh->unused || hh->op != HIR_IFOP2, "Unrolled loop retained a conditional branch!");
            if (!hh->unused && HIR_is_writeop(hh->op) && hh->farg && HIR_is_vartype(hh->farg->t)) {
                assert(!set_has(&definitions, (void*)hh->farg->storage.var.v_id), "Copied variable has multiple definitions!");
                set_add(&definitions, (void*)hh->farg->storage.var.v_id);
            }
            if (!live) assert(hh->unused || (hh->op != HIR_iLWR && hh->op != HIR_iLGE), "Unused comparison survived!");
        }
        set_free(&definitions);
    }
    else assert(mm_get_allocated() == allocated, "Skipped loop leaked memory!");
    assert(bypass->jmp == b[3] && set_has(&bypass->succ, b[3]) && set_has(&b[3]->pred, bypass),
           "Unroll changed an unrelated CFG edge!");

    map_free(&loops.lmap);
    list_free(&roots);
    set_free(&loop.blocks);
    set_free(&loop.exits);
    list_free(&loop.blocks_sorted);
    list_free(&loop.children);
    foreach (cfg_block_t* bb, &func.blocks) HIR_CFG_unload_block(bb);
    list_free(&func.blocks);
    HIR_unload_blocks(hir.hot.h);
    SMT_unload(smt);
    return 0;
}

int main(void) {
    mm_init();
    for (int inverted = 0; inverted <= 1; inverted++) {
        for (int post = 0; post <= 1; post++) {
            for (long count = 0; count <= 7; count++) {
                assert(!_test_loop(count, post, 0, inverted, 0), "Finite loop test failed!");
                assert(!_test_loop(count, post, 0, inverted, 1), "Live comparison test failed!");
                assert(mm_get_allocated() == 0, "Finite loop leaked memory!");
            }
            assert(!_test_loop(7, post, 1, inverted, 0), "Unknown loop test failed!");
            assert(!_test_loop(1000, post, 2, inverted, 0), "Large loop test failed!");
            assert(!_test_loop(7, post, 3, inverted, 0), "Infinite loop test failed!");
            assert(mm_get_allocated() == 0, "Skipped loop leaked memory!");
        }
    }
    return 0;
}
