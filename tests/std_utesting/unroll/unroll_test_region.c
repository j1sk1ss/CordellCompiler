#include <hir/loop.h>
#include "../../misc/testing.h"

enum { WHILE_LOOP, COUNTED_LOOP, SELF_LOOP, LIVE_OUT, DECLARATION, LOOP_CARRIED,
       MULTIPLE_BACKEDGES, MULTIPLE_EXITS, NESTED, TOO_MANY_BLOCKS, TOO_MANY_INSTRUCTIONS, NO_CALL_LOOP };

static int _posttested(int kind) {
    return kind == COUNTED_LOOP || kind == SELF_LOOP;
}

typedef struct {
    hir_ctx_t hir;
    cfg_ctx_t cfg;
    ltree_ctx_t loops;
    sym_table_t* smt;
    symbol_id_t variable, bound, condition, update;
    unsigned long header_id, exit_id;
    hir_block_t* header;
    hir_block_t* latch;
} fixture_t;

static hir_subject_t* _label(unsigned long id) {
    hir_subject_t* s = HIR_SUBJ_LABEL();
    s->id = id;
    return s;
}

static hir_subject_t* _var(symbol_id_t id, int temporary) {
    return HIR_SUBJ_STKVAR(id, temporary ? HIR_TMPVARI64 : HIR_STKVARI64, 0);
}

static void _build_loop(fixture_t* f, int kind) {
    f->smt = mm_malloc(sizeof(sym_table_t));
    SMT_init(f->smt);
    string_t* name = create_string("_main");
    symbol_id_t fid = FNTB_add_info(name, NULL, (func_info_flags_t){ .entry = 1, .used = 1 }, 0, NULL, NULL, NULL, &f->smt->f);
    destroy_string(name);
    f->variable = VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &f->smt->v);
    f->bound = VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &f->smt->v);
    f->condition = VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &f->smt->v);
    f->update = VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &f->smt->v);
    hir_subject_t* header_label = HIR_SUBJ_LABEL();
    f->header_id = header_label->id;
    hir_subject_t* body_label = kind == SELF_LOOP ? NULL : HIR_SUBJ_LABEL();
    unsigned long body_id = body_label ? body_label->id : 0;
    hir_subject_t* latch_label = kind == SELF_LOOP ? NULL : HIR_SUBJ_LABEL();
    unsigned long latch_id = latch_label ? latch_label->id : 0;
    hir_subject_t* exit_label = HIR_SUBJ_LABEL();
    f->exit_id = exit_label->id;
    HIR_BLOCK1(&f->hir, HIR_STRT, HIR_SUBJ_FNAMETB(fid));
    HIR_BLOCK1(&f->hir, HIR_VARDECL, _var(f->variable, 0));
    HIR_BLOCK2(&f->hir, HIR_STORE, _var(f->variable, 0), HIR_SUBJ_CONST(_posttested(kind) ? 7 : 0));
    HIR_BLOCK2(&f->hir, HIR_STORE, _var(f->bound, 1), HIR_SUBJ_CONST(7));
    HIR_BLOCK1(&f->hir, HIR_MKLB, header_label);
    f->header = f->hir.hot.t;
    if (kind == COUNTED_LOOP) HIR_BLOCK1(&f->hir, HIR_JMP, _label(body_id));
    else if (kind != SELF_LOOP) {
        HIR_BLOCK3(&f->hir, HIR_iLWR, _var(f->condition, 1), _var(f->variable, 0), _var(f->bound, 1));
        HIR_BLOCK3(&f->hir, HIR_IFOP2, _var(f->condition, 1), _label(body_id), _label(f->exit_id));
    }
    if (body_label) HIR_BLOCK1(&f->hir, HIR_MKLB, body_label);
    if (kind == NESTED) {
        hir_subject_t* inner = HIR_SUBJ_LABEL();
        hir_subject_t* inner_body = HIR_SUBJ_LABEL();
        hir_subject_t* after_inner = HIR_SUBJ_LABEL();
        HIR_BLOCK1(&f->hir, HIR_MKLB, inner);
        HIR_BLOCK3(&f->hir, HIR_IFOP2, HIR_SUBJ_CONST(0), _label(inner_body->id), _label(after_inner->id));
        HIR_BLOCK1(&f->hir, HIR_MKLB, inner_body);
        HIR_BLOCK1(&f->hir, HIR_JMP, _label(inner->id));
        HIR_BLOCK1(&f->hir, HIR_MKLB, after_inner);
    }
    if (kind == TOO_MANY_BLOCKS) {
        for (int i = 0; i < 9; i++) {
            HIR_BLOCK1(&f->hir, HIR_MKLB, HIR_SUBJ_LABEL());
            HIR_BLOCK0(&f->hir, HIR_NOP);
        }
    }
    if (kind == TOO_MANY_INSTRUCTIONS) {
        for (int i = 0; i < 65; i++) HIR_BLOCK0(&f->hir, HIR_NOP);
    }
    if (kind == DECLARATION) HIR_BLOCK1(&f->hir, HIR_VARDECL, _var(f->variable, 0));
    HIR_BLOCK3(&f->hir, _posttested(kind) ? HIR_iSUB : HIR_iADD, _var(f->update, 1),
               _var(kind == LOOP_CARRIED ? f->update : f->variable, kind == LOOP_CARRIED), HIR_SUBJ_CONST(1));
    HIR_BLOCK2(&f->hir, HIR_STORE, _var(f->variable, 0), _var(f->update, 1));
    if (kind != NO_CALL_LOOP) {
        hir_subject_t* args = HIR_SUBJ_LIST();
        list_push_back(&args->storage.list.h, _var(f->update, 1));
        list_push_back(&args->storage.list.h, _var(f->bound, 1));
        HIR_BLOCK3(&f->hir, HIR_FCLL, NULL, HIR_SUBJ_FNAMETB(fid), args);
    }
    hir_subject_t* extra_exit = NULL;
    if (kind == MULTIPLE_BACKEDGES) {
        HIR_BLOCK3(&f->hir, HIR_IFOP2, _var(f->condition, 1), _label(f->header_id), _label(latch_id));
    }
    if (kind == MULTIPLE_EXITS) {
        extra_exit = HIR_SUBJ_LABEL();
        HIR_BLOCK3(&f->hir, HIR_IFOP2, _var(f->condition, 1), _label(latch_id), _label(extra_exit->id));
    }
    /* Normally body -> latch is an implicit fallthrough. */
    if (latch_label) HIR_BLOCK1(&f->hir, HIR_MKLB, latch_label);
    if (_posttested(kind)) {
        HIR_BLOCK3(&f->hir, HIR_IFOP2, _var(f->variable, 0), _label(f->header_id), _label(f->exit_id));
    }
    else {
        /* Exercise the shared-subject ownership used by the HIR generator. */
        HIR_BLOCK1(&f->hir, HIR_JMP, header_label);
    }
    f->latch = f->hir.hot.t;
    HIR_BLOCK1(&f->hir, HIR_MKLB, exit_label);
    HIR_BLOCK1(&f->hir, HIR_EXITOP, _var(kind == LIVE_OUT ? f->update : f->variable, kind == LIVE_OUT));
    if (extra_exit) {
        HIR_BLOCK1(&f->hir, HIR_MKLB, extra_exit);
        HIR_BLOCK1(&f->hir, HIR_EXITOP, _var(f->variable, 0));
    }
    HIR_BLOCK0(&f->hir, HIR_STEND);
    HIR_CFG_build(&f->hir, &f->cfg, f->smt);
    foreach (cfg_func_t* func, &f->cfg.funcs) func->used = 1;
    HIR_CFG_create_domdata(&f->cfg);
    map_init(&f->loops.lmap, MAP_NO_CMP);
    HIR_LOOP_mark_loops(&f->cfg, &f->loops);
}

static void _free_fixture(fixture_t* f) {
    HIR_LTREE_unload_ctx(&f->loops);
    HIR_CFG_unload(&f->cfg);
    HIR_unload_blocks(f->hir.hot.h);
    SMT_unload(f->smt);
}

static int _test_region(int kind) {
    fixture_t f = {0};
    _build_loop(&f, kind);
    if (kind == NO_CALL_LOOP) {
        assert(HIR_LOOP_perform_dle(&f.loops), "DLE failed!");
        for (hir_block_t* hh = f.hir.hot.h; hh; hh = hh->next) {
            if (hh->op == HIR_iLWR || hh->op == HIR_iADD || hh->op == HIR_STORE) {
                assert(!hh->unused, "DLE removed live computations from a loop without calls!");
            }
        }
    }
    assert(HIR_LTREE_unroll(&f.loops, f.smt) == 1, "Eligible loop was not unrolled!");
    assert(f.header->farg->id == f.header_id, "Shared original header label was changed!");
    unsigned long copy_header = _posttested(kind) ? f.latch->sarg->id : f.latch->farg->id;
    assert(copy_header != f.header_id, "Original latch still targets original header!");
    int calls = 0, updates = 0, conditions = 0, copy_backedge = 0;
    set_t labels, definitions;
    set_init(&labels, SET_NO_CMP);
    set_init(&definitions, SET_NO_CMP);
    for (hir_block_t* hh = f.hir.hot.h; hh; hh = hh->next) {
        if (hh->op == HIR_MKLB) {
            assert(set_add(&labels, (void*)hh->farg->id), "Duplicate emitted label ID!");
        }
        if (HIR_is_writeop(hh->op) && hh->farg && HIR_is_tmptype(hh->farg->t)) {
            assert(set_add(&definitions, (void*)hh->farg->storage.var.v_id), "Temporary has multiple definitions after unroll!");
        }
        if (hh->op == HIR_iADD || hh->op == HIR_iSUB) updates++;
        if (hh->op == HIR_iLWR) {
            conditions++;
            assert(hh->targ->storage.var.v_id == f.bound, "External temporary was renamed!");
        }
        if (hh->op == HIR_STORE && hh->farg->t == HIR_STKVARI64) {
            assert(hh->farg->storage.var.v_id == f.variable, "Loop state variable was renamed!");
        }
        if (hh->op == HIR_FCLL) {
            calls++;
            hir_subject_t* update = list_get_head(&hh->targ->storage.list.h);
            hir_subject_t* bound = list_get_tail(&hh->targ->storage.list.h);
            assert(bound->storage.var.v_id == f.bound, "External call argument was renamed!");
            assert(set_has(&definitions, (void*)update->storage.var.v_id), "Call argument did not use its copy's definition!");
        }
        if (hh != f.latch && ((hh->op == HIR_JMP && hh->farg->id == f.header_id) ||
            (_posttested(kind) && hh->op == HIR_IFOP2 && hh->sarg->id == f.header_id))) copy_backedge++;
    }
    assert(calls == (kind == NO_CALL_LOOP ? 0 : 2) && updates == 2, "Body was not copied exactly once!");
    assert(conditions == (_posttested(kind) ? 0 : 2), "Condition was not preserved in both copies!");
    assert(copy_backedge == 1, "Copied backedge is missing!");
    set_free(&labels);
    set_free(&definitions);

    HIR_LTREE_unload_ctx(&f.loops);
    HIR_CFG_unload(&f.cfg);
    assert(HIR_CFG_build(&f.hir, &f.cfg, f.smt), "CFG rebuild failed!");
    foreach (cfg_func_t* func, &f.cfg.funcs) {
        func->used = 1;
        assert(HIR_CFG_function_findlb(func, copy_header), "Copied header missing after rebuild!");
        foreach (cfg_block_t* bb, &func->blocks) {
            if (bb->hmap.exit->op == HIR_JMP) assert(bb->jmp, "Unresolved jump after rebuild!");
            if (bb->hmap.exit->op == HIR_IFOP2) assert(bb->l && bb->jmp, "Unresolved conditional jump after rebuild!");
        }
    }
    HIR_CFG_create_domdata(&f.cfg);
    map_init(&f.loops.lmap, MAP_NO_CMP);
    assert(HIR_LOOP_mark_loops(&f.cfg, &f.loops), "Loop tree rebuild failed!");
    list_t* roots = NULL;
    assert(map_get(&f.loops.lmap, 0, (void**)&roots) && list_size(roots) == 1, "Unroll did not leave one enlarged loop!");
    _free_fixture(&f);
    return 0;
}

static int _test_skip(int kind) {
    fixture_t f = {0};
    _build_loop(&f, kind);
    int allocated = mm_get_allocated();
    int variables = f.smt->v.curr_id;
    hir_block_t* previous = f.header->prev;
    assert(HIR_LTREE_unroll(&f.loops, f.smt) == 0, "Unsupported loop was unrolled!");
    assert(f.header->prev == previous && f.latch->farg->id == f.header_id, "Skipped loop was modified!");
    assert(f.smt->v.curr_id == variables && mm_get_allocated() == allocated, "Skipped loop allocated variables or leaked memory!");
    _free_fixture(&f);
    return 0;
}

int main() {
    mm_init();
    assert(!_test_region(WHILE_LOOP), "While unroll test failed!");
    assert(mm_get_allocated() == 0, "While test leaked memory!");
    assert(!_test_region(COUNTED_LOOP), "Counted loop unroll test failed!");
    assert(mm_get_allocated() == 0, "Counted loop test leaked memory!");
    assert(!_test_region(SELF_LOOP), "Single-block loop unroll test failed!");
    assert(mm_get_allocated() == 0, "Single-block loop test leaked memory!");
    assert(!_test_region(NO_CALL_LOOP), "Loop without calls test failed!");
    assert(mm_get_allocated() == 0, "Loop without calls test leaked memory!");
    for (int kind = LIVE_OUT; kind <= TOO_MANY_INSTRUCTIONS; kind++) {
        assert(!_test_skip(kind), "Unsupported loop test failed!");
        assert(mm_get_allocated() == 0, "Skipped loop test leaked memory!");
    }
    return 0;
}
