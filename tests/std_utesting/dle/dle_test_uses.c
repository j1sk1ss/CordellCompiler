#include <hir/loop.h>
#include "../../misc/cfg_fixture.h"
#include "../../misc/testing.h"

enum { NO_USE, EXIT_USE, BRANCH_USE, CALL_USE, PHI_USE, DUPLICATE_PHI_DEF, DUPLICATE_WRITE, UNREACHABLE_USE, SECOND_EXIT_USE, COUNTER_USE, UPDATED_USE, BODY_CALL, CASE_COUNT };

static hir_subject_t* _var(long id) {
    return HIR_SUBJ_STKVAR(id, HIR_STKVARI64, 0);
}

static int _test_uses(int kind) {
    sym_table_t smt;
    SMT_init(&smt);
    for (int id = 0; id < 47; id++) {
        VRTB_add_info(NULL, I64_TYPE_TOKEN, 0, EMPTY_BASIC_FLAGS, &smt.v);
    }
    VRTB_update_definition(44, 0, NO_SYMBOL_ID, &smt.v, 1);
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 7, 0);
    cfg_block_t* header = &f.blocks[1];
    f.blocks[0].type = CFG_LOOP_PREHEADER;
    header->type = CFG_LOOP_HEADER;
    f.blocks[2].type = CFG_LOOP_BLOCK;
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 1, 2, 0);
    cfg_fixture_edge(&f, 1, 3, 1);
    cfg_fixture_edge(&f, 2, 1, 1);
    cfg_fixture_edge(&f, 3, 4, 0);
    cfg_fixture_edge(&f, 3, 5, 1);
    cfg_fixture_edge(&f, 5, 4, 0);
    cfg_fixture_edge(&f, 4, 3, 1); /* A later loop must not hang the walk. */
    cfg_fixture_edge(&f, 4, 0, 0); /* An outer backedge may reach this loop again. */

    for (int i = 0; i < 7; i++) {
        hir_block_t* hh = HIR_create_block(HIR_NOP, NULL, NULL, NULL);
        f.blocks[i].hmap.entry = f.blocks[i].hmap.exit = hh;
    }
    header->hmap.exit->op = HIR_IFOP2;
    header->hmap.exit->farg = _var(41);
    hir_block_t* cmp = HIR_create_block(HIR_iLWR, _var(41), _var(42), HIR_SUBJ_CONST(10));
    HIR_insert_block_before(cmp, header->hmap.entry);
    hir_block_t* phi = HIR_create_block(HIR_PHI, _var(40), _var(42), HIR_SUBJ_SET());
    set_add(&phi->targ->storage.set.h, inttuple_create(0, 44));
    set_add(&phi->targ->storage.set.h, inttuple_create(2, 43));
    HIR_insert_block_before(phi, cmp);
    header->hmap.entry = phi;
    f.blocks[0].hmap.exit->op = HIR_STORE;
    f.blocks[0].hmap.exit->farg = _var(44);
    f.blocks[0].hmap.exit->sarg = HIR_SUBJ_CONST(0);
    hir_block_t* update = HIR_create_block(HIR_iADD, _var(45), _var(42), HIR_SUBJ_CONST(1));
    HIR_insert_block_before(update, f.blocks[2].hmap.entry);
    hir_block_t* store = HIR_create_block(HIR_STORE, _var(43), _var(45), NULL);
    HIR_insert_block_before(store, f.blocks[2].hmap.exit);
    f.blocks[2].hmap.entry = update;
    f.blocks[2].hmap.exit->op = HIR_JMP;
    if (kind == BODY_CALL) {
        HIR_insert_block_before(HIR_create_block(HIR_FCLL, NULL, NULL, NULL), f.blocks[2].hmap.exit);
    }

    int target = kind == EXIT_USE ? 3 : kind == UNREACHABLE_USE ? 6 : 5;
    hir_block_t* use = f.blocks[target].hmap.exit;
    switch (kind) {
        case EXIT_USE: case BRANCH_USE: case UNREACHABLE_USE: case SECOND_EXIT_USE:
            use->op = HIR_FRET;
            use->farg = _var(41);
            break;
        case COUNTER_USE: case UPDATED_USE:
            use->op = HIR_FRET;
            use->farg = _var(kind == COUNTER_USE ? 42 : 43);
            break;
        case CALL_USE:
            use->op = HIR_FCLL;
            use->targ = HIR_SUBJ_LIST();
            list_push_back(&use->targ->storage.list.h, _var(41));
            break;
        case PHI_USE: case DUPLICATE_PHI_DEF:
            use->op = HIR_PHI;
            use->farg = _var(40);
            use->sarg = _var(kind == DUPLICATE_PHI_DEF ? 41 : 46);
            use->targ = HIR_SUBJ_SET();
            set_add(&use->targ->storage.set.h, inttuple_create(3, kind == PHI_USE ? 41 : 43));
            break;
        case DUPLICATE_WRITE:
            use->op = HIR_STORE;
            use->farg = _var(41);
            use->sarg = HIR_SUBJ_CONST(0);
            break;
    }

    loop_node_t loop = { .header = header, .preheader = &f.blocks[0], .latch = &f.blocks[2] };
    set_init(&loop.blocks, SET_NO_CMP);
    set_init(&loop.exits, SET_NO_CMP);
    set_init(&loop.ind, SET_NO_CMP);
    list_init(&loop.blocks_sorted);
    list_init(&loop.children);
    set_add(&loop.blocks, header);
    set_add(&loop.blocks, &f.blocks[2]);
    list_push_back(&loop.blocks_sorted, header);
    list_push_back(&loop.blocks_sorted, &f.blocks[2]);
    set_add(&loop.ind, (void*)41);
    set_add(&loop.exits, &f.blocks[3]);
    if (kind == SECOND_EXIT_USE) {
        /* The second exit is reachable only through its own exit edge. */
        set_remove(&f.blocks[3].succ, &f.blocks[5]);
        set_remove(&f.blocks[5].pred, &f.blocks[3]);
        cfg_fixture_edge(&f, 1, 5, 1);
        set_add(&loop.exits, &f.blocks[5]);
    }

    list_t loops;
    list_init(&loops);
    list_push_back(&loops, &loop);
    ltree_ctx_t lctx;
    map_init(&lctx.lmap, MAP_NO_CMP);
    map_put(&lctx.lmap, 0, &loops);
    assert(HIR_LOOP_perform_dle(&lctx, &smt), "DLE failed!");
    int live = kind != NO_USE && kind != UNREACHABLE_USE;
    if (set_has(&f.blocks[0].succ, header) != live) fprintf(stderr, "DLE case %d failed\n", kind);
    assert(set_has(&f.blocks[0].succ, header) == live, "Wrong DLE decision for a condition used after the loop!");

    map_free(&lctx.lmap);
    list_free(&loops);
    list_free(&loop.children);
    list_free(&loop.blocks_sorted);
    set_free(&loop.blocks);
    set_free(&loop.exits);
    set_free(&loop.ind);
    for (int i = 0; i < 7; i++) HIR_unload_blocks(f.blocks[i].hmap.entry);
    cfg_fixture_free(&f);
    SMT_unload(&smt);
    return 0;
}

int main(void) {
    mm_init();
    int allocated = mm_get_allocated();
    for (int kind = 0; kind < CASE_COUNT; kind++) {
        assert(!_test_uses(kind), "Use-after-loop regression failed!");
    }
    assert(mm_get_allocated() == allocated, "DLE leaked memory!");
    return 0;
}
