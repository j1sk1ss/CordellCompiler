#include <hir/loop.h>
#include <hir/constfold.h>
#include "../../misc/cfg_fixture.h"
#include "../../misc/testing.h"

typedef struct {
    cfg_fixture_t cfg;
    sym_table_t smt;
    loop_node_t loop;
    hir_subject_type_t type;
    hir_block_t *initial, *compare, *update;
} fixture_t;

static void _set_definition(fixture_t* f, long id, long value, int state) {
    variable_info_t* vi = NULL;
    if (!map_get(&f->smt.v.vartb, id, (void**)&vi)) return;
    vi->vdi.definition = value;
    vi->vdi.defined = state;
}

static hir_subject_t* _var(fixture_t* f, long id) {
    return HIR_SUBJ_STKVAR(id, f->type, 0);
}

static hir_subject_t* _number(fixture_t* f, long n) {
    hir_subject_t* s = HIR_create_subject(HIR_is_sign(f->type) ? HIR_I64CONSTVAL : HIR_U64CONSTVAL, 0, NULL, n);
    /* Constants have the counter's type, including its bit width. */
    s->t = f->type == HIR_STKVARI8 ? HIR_I8CONSTVAL : f->type == HIR_STKVARU8 ? HIR_U8CONSTVAL : s->t;
    s->storage.cnst.value = n;
    return s;
}

static hir_block_t* _append(fixture_t* f, int block, hir_operation_t op, hir_subject_t* a, hir_subject_t* b, hir_subject_t* c) {
    hir_block_t* hh = HIR_create_block(op, a, b, c);
    cfg_block_t* bb = &f->cfg.blocks[block];
    if (bb->hmap.exit) HIR_insert_block_after(hh, bb->hmap.exit);
    else bb->hmap.entry = hh;
    bb->hmap.exit = hh;
    return hh;
}

static void _build(fixture_t* f, long initial, long bound, long step, hir_operation_t op, int posttested, int is_unsigned) {
    f->type = is_unsigned ? HIR_STKVARU8 : HIR_STKVARI8;
    SMT_init(&f->smt);
    for (int id = 0; id < 100; id++) {
        VRTB_add_info(NULL, is_unsigned ? U8_TYPE_TOKEN : I8_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &f->smt.v);
    }
    _set_definition(f, 10, initial, DEFINED_VARIABLE);
    cfg_fixture_init(&f->cfg, 8, 0);
    cfg_block_t* b = f->cfg.blocks;
    b[0].type = CFG_LOOP_PREHEADER;
    b[1].type = CFG_LOOP_HEADER;
    b[2].type = CFG_LOOP_LATCH;
    cfg_fixture_edge(&f->cfg, 0, 1, 0);
    cfg_fixture_edge(&f->cfg, 1, 2, 0);
    cfg_fixture_edge(&f->cfg, 2, 1, posttested ? 0 : 1);
    cfg_fixture_edge(&f->cfg, posttested ? 2 : 1, 3, 1);
    f->initial = _append(f, 0, HIR_STORE, _var(f, 10), _number(f, initial), NULL);
    hir_subject_t* incoming = HIR_SUBJ_SET();
    set_add(&incoming->storage.set.h, inttuple_create(0, 10));
    set_add(&incoming->storage.set.h, inttuple_create(2, 12));
    _append(f, 1, HIR_PHI, _var(f, 0), _var(f, 11), incoming);
    if (!posttested) {
        f->compare = _append(f, 1, op, _var(f, 14), _var(f, 11), _number(f, bound));
        _append(f, 1, HIR_IFOP2, _var(f, 14), NULL, NULL);
    }
    else _append(f, 1, HIR_JMP, NULL, NULL, NULL);
    f->update = _append(f, 2, step < 0 ? HIR_iSUB : HIR_iADD, _var(f, 13), _var(f, 11), _number(f, step < 0 ? -step : step));
    _append(f, 2, HIR_STORE, _var(f, 12), _var(f, 13), NULL);
    if (posttested) {
        if (op != HIR_NOP) f->compare = _append(f, 2, op, _var(f, 14), _var(f, 12), _number(f, bound));
        _append(f, 2, HIR_IFOP2, _var(f, op == HIR_NOP ? 12 : 14), NULL, NULL);
    }
    else _append(f, 2, HIR_JMP, NULL, NULL, NULL);
    f->loop.header = &b[1];
    f->loop.preheader = &b[0];
    f->loop.latch = &b[2];
    set_init(&f->loop.blocks, SET_NO_CMP);
    set_init(&f->loop.exits, SET_NO_CMP);
    set_init(&f->loop.ind, SET_NO_CMP);
    list_init(&f->loop.blocks_sorted);
    list_init(&f->loop.children);
    for (int i = 1; i <= 2; i++) {
        set_add(&f->loop.blocks, &b[i]);
        list_push_back(&f->loop.blocks_sorted, &b[i]);
    }
    set_add(&f->loop.exits, &b[3]);
}

static int _finite(fixture_t* f) {
    loop_iteration_info_t info;
    int success = HIR_LOOP_analyze_iterations(&f->loop, &info, &f->smt);
    int result = success && info.status == LOOP_ITERATION_FINITE;
    HIR_LOOP_free_iteration_info(&info);
    return result;
}

static void _free(fixture_t* f) {
    for (int i = 0; i < 8; i++) HIR_unload_blocks(f->cfg.blocks[i].hmap.entry);
    list_free(&f->loop.children);
    list_free(&f->loop.blocks_sorted);
    set_free(&f->loop.blocks);
    set_free(&f->loop.exits);
    set_free(&f->loop.ind);
    cfg_fixture_free(&f->cfg);
    SMT_unload(&f->smt);
}

static int _test_arithmetic(void) {
    struct { long initial, bound, step; hir_operation_t op; int post, uns; long count; } cases[] = {
        {0, 10, 1, HIR_iLWR, 0, 0, 10}, {0, 10, 3, HIR_iLWR, 0, 0, 4},
        {0, 126, 3, HIR_iLRE, 0, 0, -2}, {0, 127, 1, HIR_iLWR, 0, 0, 127},
        {0, 127, 1, HIR_iLRE, 0, 0, -2}, {127, 127, 1, HIR_iLWR, 0, 0, 0},
        {0, 10, -1, HIR_iLWR, 0, 0, -2}, {10, 0, -1, HIR_iLRG, 0, 0, 10},
        {-120, -128, -1, HIR_iLGE, 0, 0, -2}, {10, 0, 0, HIR_iLRG, 0, 0, -1},
        {0, 10, 2, HIR_iNMP, 0, 0, 5}, {0, 10, 3, HIR_iNMP, 0, 0, -2},
        {10, 10, 1, HIR_iCMP, 0, 0, 1}, {127, 127, 1, HIR_iCMP, 0, 0, -2},
        {100, 0, -1, HIR_NOP, 1, 0, 100}, {0, 0, -1, HIR_NOP, 1, 0, -2},
        {100, 0, -3, HIR_NOP, 1, 0, -2}, {99, 0, -3, HIR_NOP, 1, 0, 33},
        {0, 10, 1, HIR_iLWR, 1, 0, 10}, {127, 127, 1, HIR_iLWR, 1, 0, -2},
        {0, 255, 1, HIR_iLRE, 0, 1, -2}, {0, 255, 1, HIR_iLWR, 0, 1, 255},
        {0, 0, -1, HIR_NOP, 1, 1, -2}, {255, 0, -1, HIR_NOP, 1, 1, 255},
        {10, 10, 0, HIR_iLWR, 0, 0, 0}, {10, 10, 0, HIR_iLWR, 1, 0, 1},
    };
    for (unsigned i = 0; i < sizeof(cases) / sizeof(*cases); i++) {
        fixture_t f = { 0 };
        _build(&f, cases[i].initial, cases[i].bound, cases[i].step, cases[i].op, cases[i].post, cases[i].uns);
        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Iteration analysis failed!");
        if (info.count != cases[i].count) fprintf(stderr, "Iteration count case %u failed\n", i);
        assert(info.count == cases[i].count, "Wrong body iteration count!");
        loop_iteration_status_t status = cases[i].count >= 0 ? LOOP_ITERATION_FINITE :
            cases[i].count == -1 ? LOOP_ITERATION_INFINITE : LOOP_ITERATION_UNKNOWN;
        assert(info.status == status, "Count and status disagree!");
        assert(list_size(&info.iterators) == 1 && list_size(&info.conditions) == 1, "Missing recurrence metadata!");
        loop_iterator_info_t* iterator = list_get_head(&info.iterators);
        loop_iteration_condition_t* condition = list_get_head(&info.conditions);
        assert(iterator->initial_known && iterator->initial_value == cases[i].initial, "Wrong initial value!");
        assert(iterator->step_known && iterator->step_value == cases[i].step, "Wrong normalized step!");
        assert(iterator->initial == f.initial->farg && iterator->update == f.update, "Wrong source instructions!");
        assert(iterator->initial_id == 10 && iterator->next_id == 12, "Wrong SSA recurrence IDs!");
        assert(condition->iterator == iterator && condition->bound_known, "Condition is not linked to its iterator!");
        assert(condition->bound_value == cases[i].bound && condition->posttested == cases[i].post, "Wrong condition metadata!");
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_unknown_and_branches(void) {
    for (int kind = 0; kind < 7; kind++) {
        fixture_t f = { 0 };
        _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
        int expected = 0;
        switch (kind) {
            case 0:
                f.initial->op = HIR_STARGLD;
                _set_definition(&f, 10, 0, UNDEFINED_VARIABLE);
                break;
            case 1:
                HIR_unload_subject(f.compare->targ);
                f.compare->targ = _var(&f, 99); /* Unknown bound. */
                break;
            case 2:
                cfg_fixture_edge(&f.cfg, 2, 3, 0); /* Another exit. */
                f.cfg.blocks[2].hmap.exit->op = HIR_IFOP2;
                f.cfg.blocks[2].hmap.exit->farg = _var(&f, 99);
                break;
            case 3: {
                hir_subject_t* swap = f.compare->sarg;
                f.compare->sarg = f.compare->targ;
                f.compare->targ = swap;
                f.compare->op = HIR_iLRG; /* 10 > i */
                expected = 1;
                break;
            }
            case 4:
                f.compare->op = HIR_iLGE; /* Continue on the false branch. */
                f.cfg.blocks[1].l = &f.cfg.blocks[3];
                f.cfg.blocks[1].jmp = &f.cfg.blocks[2];
                expected = 1;
                break;
            case 5: f.compare->targ->t = HIR_U64CONSTVAL; break;
            case 6: f.update->farg->t = HIR_STKVARF32; break;
        }
        assert(_finite(&f) == expected, "Unknown recurrence or CFG branch handled incorrectly!");
        if (expected) {
            loop_iteration_info_t info;
            assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Normalized condition analysis failed!");
            loop_iteration_condition_t* condition = list_get_head(&info.conditions);
            assert(info.count == 10 && condition->op == HIR_iLWR, "Reversed condition was not normalized!");
            assert(condition->bound_value == 10 && condition->block == &f.cfg.blocks[1], "Wrong normalized bound!");
            HIR_LOOP_free_iteration_info(&info);
        }
        _free(&f);
    }
    return 0;
}

static int _test_structured_body(void) {
    for (int kind = 0; kind < 4; kind++) {
        fixture_t f = { 0 };
        loop_node_t child = { 0 };
        _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
        set_remove(&f.cfg.blocks[1].succ, &f.cfg.blocks[2]);
        set_remove(&f.cfg.blocks[2].pred, &f.cfg.blocks[1]);
        cfg_fixture_edge(&f.cfg, 1, 4, 0);
        set_add(&f.loop.blocks, &f.cfg.blocks[4]);
        list_push_back(&f.loop.blocks_sorted, &f.cfg.blocks[4]);
        _append(&f, 4, HIR_IFOP2, _var(&f, 99), NULL, NULL);
        cfg_fixture_edge(&f.cfg, 4, 5, 0);

        if (kind == 0) { /* Both branches of an if reach the counter update. */
            cfg_fixture_edge(&f.cfg, 4, 6, 1);
            cfg_fixture_edge(&f.cfg, 5, 2, 1);
            cfg_fixture_edge(&f.cfg, 6, 2, 1);
            for (int block = 5; block <= 6; block++) {
                set_add(&f.loop.blocks, &f.cfg.blocks[block]);
                list_push_back(&f.loop.blocks_sorted, &f.cfg.blocks[block]);
                _append(&f, block, HIR_JMP, NULL, NULL, NULL);
            }
        }
        else if (kind == 3) { /* A nested loop may never reach the outer update. */
            cfg_fixture_edge(&f.cfg, 4, 2, 1);
            cfg_fixture_edge(&f.cfg, 5, 4, 1);
            _append(&f, 5, HIR_JMP, NULL, NULL, NULL);
            set_add(&f.loop.blocks, &f.cfg.blocks[5]);
            list_push_back(&f.loop.blocks_sorted, &f.cfg.blocks[5]);
            child.header = &f.cfg.blocks[4];
            child.latch  = &f.cfg.blocks[5];
            list_push_back(&f.loop.children, &child);
        }
        else { /* A break or return can exit before the next counter update. */
            cfg_fixture_edge(&f.cfg, 4, 2, 1);
            set_add(&f.loop.exits, &f.cfg.blocks[5]);
            if (kind == 1) {
                _append(&f, 5, HIR_BREAK, NULL, NULL, NULL);
                _append(&f, 5, HIR_JMP, NULL, NULL, NULL);
                cfg_fixture_edge(&f.cfg, 5, 3, 1);
            }
            else _append(&f, 5, HIR_FRET, NULL, NULL, NULL);
        }

        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Structured-loop analysis failed!");
        assert(info.count == (kind == 0 ? 10 : LOOP_ITERATION_COUNT_UNKNOWN),
               "Structured body produced a wrong iteration count!");
        assert(list_size(&info.iterators) == 1, "Structured body lost iterator metadata!");
        assert(list_size(&info.conditions) == (kind == 1 || kind == 2 ? 2 : 1),
               "Loop condition collection included a body branch or missed an exit!");
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_full_width(void) {
    struct { long initial, bound, step; int uns, inclusive, finite; } cases[] = {
        {0, -1, 1, 1, 0, 1}, {0, -1, 1, 1, 1, 0},
        {LONG_MIN, LONG_MAX, 1, 0, 0, 1}, {LONG_MIN, LONG_MAX, 1, 0, 1, 0},
        {LONG_MIN, LONG_MAX, LONG_MAX, 0, 0, 0},
        {-1, 0, -1, 1, 0, 1},
    };
    for (unsigned i = 0; i < sizeof(cases) / sizeof(*cases); i++) {
        fixture_t f = { 0 };
        _build(&f, cases[i].initial, cases[i].bound, cases[i].step,
               cases[i].step < 0 ? HIR_iLRG : cases[i].inclusive ? HIR_iLRE : HIR_iLWR, 0, cases[i].uns);
        for (int b = 0; b < 5; b++) {
            cfg_block_t* bb = &f.cfg.blocks[b];
            iterate_hir_instructions (bb) {
                iterate_hir_args (hir_subject_t* arg, hh, 0) {
                    switch (arg->t) {
                        case HIR_STKVARI8: arg->t = HIR_STKVARI64; break;
                        case HIR_STKVARU8: arg->t = HIR_STKVARU64; break;
                        case HIR_I8CONSTVAL: arg->t = HIR_I64CONSTVAL; break;
                        case HIR_U8CONSTVAL: arg->t = HIR_U64CONSTVAL; break;
                        default: break;
                    }
                }
            }
        }
        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Full-width analysis failed!");
        assert((info.status == LOOP_ITERATION_FINITE) == cases[i].finite, "Full-width arithmetic overflowed the analysis!");
        if (cases[i].finite) {
            assert(info.count == ((big_int_t)1 << 64) - 1, "Full-width trip count was truncated!");
        }
        else assert(info.count == LOOP_ITERATION_COUNT_UNKNOWN, "Overflow was reported as nontermination!");
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_partial_metadata(void) {
    for (int kind = 0; kind < 3; kind++) {
        fixture_t f = { 0 };
        _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
        if (kind == 0) {
            f.initial->op = HIR_STARGLD;
            _set_definition(&f, 10, 0, UNDEFINED_VARIABLE);
        }
        if (kind == 1) {
            HIR_unload_subject(f.compare->targ);
            f.compare->targ = _var(&f, 99);
        }
        if (kind == 2) {
            HIR_unload_subject(f.update->targ);
            f.update->targ = _var(&f, 99);
        }

        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Partial analysis failed!");
        assert(info.status == LOOP_ITERATION_UNKNOWN && info.count == LOOP_ITERATION_COUNT_UNKNOWN,
               "Unknown count was confused with nontermination!");
        assert(list_size(&info.iterators) == 1 && list_size(&info.conditions) == 1, "Partial metadata was discarded!");
        loop_iterator_info_t* iterator = list_get_head(&info.iterators);
        loop_iteration_condition_t* condition = list_get_head(&info.conditions);
        assert(iterator->initial_known == (kind != 0), "Wrong initial-value availability!");
        assert(iterator->step_known == (kind != 2), "Wrong step availability!");
        assert(condition->bound_known == (kind != 1), "Wrong bound availability!");
        assert(condition->iterator == iterator && condition->comparison == f.compare, "Missing condition references!");
        assert(!set_size(&info.iteration_dependency_ids), "Unknown loop exposed iteration dependency IDs!");
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_multiple_iterators(void) {
    fixture_t f = { 0 };
    _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
    _append(&f, 0, HIR_STORE, _var(&f, 20), _number(&f, 7), NULL);
    _set_definition(&f, 20, 7, DEFINED_VARIABLE);
    hir_subject_t* incoming = HIR_SUBJ_SET();
    set_add(&incoming->storage.set.h, inttuple_create(0, 20));
    set_add(&incoming->storage.set.h, inttuple_create(2, 22));
    hir_block_t* phi = HIR_create_block(HIR_PHI, _var(&f, 1), _var(&f, 21), incoming);
    HIR_insert_block_before(phi, f.compare);
    hir_block_t* update = HIR_create_block(HIR_iSUB, _var(&f, 23), _var(&f, 21), _number(&f, 2));
    HIR_insert_block_before(update, f.cfg.blocks[2].hmap.exit);
    hir_block_t* store = HIR_create_block(HIR_STORE, _var(&f, 22), _var(&f, 23), NULL);
    HIR_insert_block_before(store, f.cfg.blocks[2].hmap.exit);

    loop_iteration_info_t info;
    assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Multiple-iterator analysis failed!");
    assert(info.count == 10 && list_size(&info.iterators) == 2, "Missing independent induction variable!");
    int found = 0;
    foreach (loop_iterator_info_t* iterator, &info.iterators) {
        if (iterator->iterator->storage.var.v_id != 21) continue;
        assert(iterator->initial_value == 7 && iterator->step_value == -2, "Wrong secondary recurrence!");
        found = 1;
    }
    assert(found, "Secondary iterator missing!");
    assert(!set_has(&info.iteration_dependency_ids, (void*)21), "Independent iterator contaminated iteration dependency IDs!");
    HIR_LOOP_free_iteration_info(&info);
    _free(&f);
    return 0;
}

static int _test_constant_conditions(void) {
    for (int kind = 0; kind < 5; kind++) {
        fixture_t f = { 0 };
        int posttested = kind == 2 || kind == 3;
        _build(&f, 0, 10, 1, HIR_iLWR, posttested, 0);
        for (int i = 1; i <= 2; i++) {
            HIR_unload_blocks(f.cfg.blocks[i].hmap.entry);
            f.cfg.blocks[i].hmap.entry = f.cfg.blocks[i].hmap.exit = NULL;
        }
        int unconditional = kind == 4;
        _append(&f, 1, posttested || unconditional ? HIR_JMP : HIR_IFOP2,
                posttested || unconditional ? NULL : _number(&f, kind == 1), NULL, NULL);
        _append(&f, 2, posttested ? HIR_IFOP2 : HIR_JMP,
                posttested ? _number(&f, kind == 3) : NULL, NULL, NULL);
        if (unconditional) {
            set_remove(&f.cfg.blocks[1].succ, &f.cfg.blocks[3]);
            set_remove(&f.cfg.blocks[3].pred, &f.cfg.blocks[1]);
            set_remove(&f.loop.exits, &f.cfg.blocks[3]);
        }

        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Constant-condition analysis failed!");
        int infinite = kind == 1 || kind == 3 || unconditional;
        assert(info.status == (infinite ? LOOP_ITERATION_INFINITE : LOOP_ITERATION_FINITE), "Wrong constant-loop status!");
        assert(info.count == (infinite ? -1 : posttested), "Wrong constant-loop body count!");
        assert(!list_size(&info.iterators), "Constant loop invented an iterator!");
        assert(list_size(&info.conditions) == !unconditional, "Wrong constant-loop conditions!");
        HIR_LOOP_free_iteration_info(&info);
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_invalid_input(void) {
    loop_iteration_info_t info;
    assert(!HIR_LOOP_analyze_iterations(NULL, &info, NULL), "Invalid loop accepted!");
    assert(info.status == LOOP_ITERATION_UNKNOWN && info.count == LOOP_ITERATION_COUNT_UNKNOWN,
           "Failed analysis returned a usable count!");
    HIR_LOOP_free_iteration_info(&info);
    HIR_LOOP_free_iteration_info(NULL);
    assert(!HIR_LOOP_analyze_iterations(NULL, NULL, NULL), "Invalid output accepted!");
    return 0;
}

static int _test_symtable_values(void) {
    long expected[] = { 10, -2, -2, 10, 4, -2, -2, -1 };
    for (int kind = 0; kind < 8; kind++) {
        fixture_t f = { 0 };
        _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
        switch (kind) {
            case 0:
                /* For example, a function argument proven constant at every call site. */
                f.initial->op = HIR_FARGLD;
                break;
            case 1: _set_definition(&f, 10, 0, UNDEFINED_VARIABLE); break;
            case 2: _set_definition(&f, 10, 0, OVERDEFINED_VARIABLE); break;
            case 3: case 5:
                HIR_unload_subject(f.compare->targ);
                f.compare->targ = _var(&f, 99);
                _set_definition(&f, 99, 10, kind == 3 ? DEFINED_VARIABLE : OVERDEFINED_VARIABLE);
                break;
            case 4: case 6: case 7:
                HIR_unload_subject(f.update->targ);
                f.update->targ = _var(&f, 99);
                _set_definition(&f, 99, kind == 7 ? 0 : 3,
                                kind == 6 ? UNDEFINED_VARIABLE : DEFINED_VARIABLE);
                break;
        }

        loop_iteration_info_t info;
        assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Symtable-based analysis failed!");
        assert(info.count == expected[kind], "Constant value did not follow the symtable lattice state!");
        HIR_LOOP_free_iteration_info(&info);
        _free(&f);
    }
    return 0;
}

static int _test_constant_propagation(void) {
    fixture_t f = { 0 };
    _build(&f, 0, 10, 1, HIR_iLWR, 0, 0);
    _set_definition(&f, 10, 0, UNDEFINED_VARIABLE);
    _append(&f, 0, HIR_iADD, _var(&f, 15), _number(&f, 6), _number(&f, 4));
    _append(&f, 0, HIR_iADD, _var(&f, 16), _number(&f, 1), _number(&f, 2));
    HIR_unload_subject(f.compare->targ);
    f.compare->targ = _var(&f, 15);
    HIR_unload_subject(f.update->targ);
    f.update->targ = _var(&f, 16);

    loop_iteration_info_t info;
    assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Analysis before propagation failed!");
    assert(info.count == LOOP_ITERATION_COUNT_UNKNOWN, "Analysis evaluated constants before propagation!");
    HIR_LOOP_free_iteration_info(&info);

    dag_ctx_t dag = { 0 };
    HIR_DAG_init(&dag);
    HIR_DAG_generate(&f.cfg.ctx, &dag, &f.smt);
    HIR_sparse_const_propagation(&dag, &f.smt);
    assert(HIR_LOOP_analyze_iterations(&f.loop, &info, &f.smt), "Analysis after propagation failed!");
    assert(info.count == 4, "Analysis did not use arithmetic constants computed by propagation!");
    loop_iterator_info_t* iterator = list_get_head(&info.iterators);
    loop_iteration_condition_t* condition = list_get_head(&info.conditions);
    assert(iterator->initial_value == 0 && iterator->step_value == 3 && condition->bound_value == 10,
           "Propagated recurrence values were read incorrectly!");
    HIR_LOOP_free_iteration_info(&info);
    HIR_DAG_unload(&dag);
    _free(&f);
    return 0;
}

int main(void) {
    mm_init();
    int allocated = mm_get_allocated();
    assert(!_test_arithmetic(), "Arithmetic termination cases failed!");
    assert(!_test_unknown_and_branches(), "Unsupported loop cases failed!");
    assert(!_test_structured_body(), "Structured loop body cases failed!");
    assert(!_test_full_width(), "64-bit termination cases failed!");
    assert(!_test_partial_metadata(), "Partial iteration metadata cases failed!");
    assert(!_test_multiple_iterators(), "Multiple-iterator metadata cases failed!");
    assert(!_test_constant_conditions(), "Constant-condition cases failed!");
    assert(!_test_invalid_input(), "Invalid-input cases failed!");
    assert(!_test_symtable_values(), "Symtable value cases failed!");
    assert(!_test_constant_propagation(), "Constant-propagation integration failed!");
    assert(mm_get_allocated() == allocated, "Termination analysis leaked memory!");
    return 0;
}
