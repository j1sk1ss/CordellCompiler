#include "../../misc/cfg_fixture.h"
#include "../../misc/testing.h"

typedef struct {
    unsigned long incoming[CFG_TEST_MAX_BLOCKS];
    int calls, duplicate, skip, stop;
} walk_t;

static cfg_dfs_action_t _visit(cfg_block_t* b, long pred, walk_t* walk) {
    unsigned long bit = 1UL << (pred < 0 ? CFG_TEST_MAX_BLOCKS : pred);
    if (walk->incoming[b->id] & bit) walk->duplicate = 1;
    walk->incoming[b->id] |= bit;
    walk->calls++;
    if (b->id == walk->stop) return CFG_DFS_STOP;
    if (b->id == walk->skip) return CFG_DFS_SKIP;
    return CFG_DFS_CONTINUE;
}

static int _test_diamond_and_cycle() {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 6, 0);
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 0, 2, 1);
    cfg_fixture_edge(&f, 1, 3, 0);
    cfg_fixture_edge(&f, 2, 3, 0);
    cfg_fixture_edge(&f, 3, 4, 0);
    cfg_fixture_edge(&f, 4, 0, 1);
    unsigned long expected[] = {(1UL << CFG_TEST_MAX_BLOCKS) | 16, 1, 1, 6, 8, 0};
    for (unsigned long long counter = 1; counter <= 2; counter++) {
        walk_t walk = { .skip = -1, .stop = -1 };
        assert(CFG_DFS_WALK_COUNTER(&f.blocks[0], -1, counter, _visit, &walk), "CFG walk failed!");
        assert(walk.calls == 7 && !walk.duplicate, "CFG walk revisited an edge or missed an edge!");
        for (int i = 0; i < 6; i++) {
            assert(walk.incoming[i] == expected[i], "CFG walk reported wrong predecessors!");
        }
    }
    cfg_fixture_free(&f);
    return 0;
}

static int _test_skip_stop_and_empty() {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 4, 0);
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 0, 2, 1);
    cfg_fixture_edge(&f, 1, 3, 0);
    walk_t skip = { .skip = 1, .stop = -1 };
    assert(CFG_DFS_WALK_COUNTER(&f.blocks[0], -1, 1, _visit, &skip), "Skip aborted the walk!");
    assert(skip.calls == 3 && !skip.incoming[3], "Skip traversed children or lost the other branch!");
    walk_t stop = { .skip = -1, .stop = 0 };
    int allocated = mm_get_allocated();
    assert(!CFG_DFS_WALK_COUNTER(&f.blocks[0], -1, 2, _visit, &stop), "Stop did not abort the walk!");
    assert(stop.calls == 1, "Stop continued the walk!");
    assert(mm_get_allocated() == allocated, "Stopped walk leaked its pending stack!");
    walk_t empty = { .skip = -1, .stop = -1 };
    assert(CFG_DFS_WALK_COUNTER((cfg_block_t*)NULL, -1, 3, _visit, &empty), "Empty walk failed!");
    assert(!empty.calls, "Empty walk invoked its callback!");
    cfg_fixture_free(&f);
    return 0;
}

typedef struct {
    int depth;
} path_t;

typedef struct {
    int seen[5], bad_depth, skip, stop;
} state_walk_t;

static cfg_dfs_action_t _visit_state(cfg_block_t* b, long pred, path_t* state, state_walk_t* walk) {
    const int expected[] = {0,1,1,2,2};
    (void)pred;
    walk->seen[b->id]++;
    if (state->depth != expected[b->id]) walk->bad_depth = 1;
    state->depth++;
    if (b->id == walk->stop) return CFG_DFS_STOP;
    if (b->id == walk->skip) return CFG_DFS_SKIP;
    return CFG_DFS_CONTINUE;
}

static int _test_path_state() {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 5, 0);
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 0, 2, 1);
    cfg_fixture_edge(&f, 1, 3, 0);
    cfg_fixture_edge(&f, 2, 4, 0);
    path_t initial = { .depth = 0 };
    state_walk_t walk = { .skip = -1, .stop = -1 };
    assert(CFG_DFS_WALK_STATE_COUNTER(&f.blocks[0], -1, 1, path_t, initial, _visit_state, &walk), "State walk failed!");
    assert(!walk.bad_depth && initial.depth == 0, "Branch state leaked into a sibling or caller!");
    for (int i = 0; i < 5; i++) {
        assert(walk.seen[i] == 1, "State walk missed or repeated a block!");
    }
    state_walk_t skip = { .skip = 1, .stop = -1 };
    assert(CFG_DFS_WALK_STATE_COUNTER(&f.blocks[0], -1, 2, path_t, initial, _visit_state, &skip), "State skip aborted the walk!");
    assert(!skip.seen[3] && skip.seen[4] == 1 && !skip.bad_depth, "State skip lost branch isolation!");
    state_walk_t stop = { .skip = -1, .stop = 0 };
    int allocated = mm_get_allocated();
    assert(!CFG_DFS_WALK_STATE_COUNTER(&f.blocks[0], -1, 3, path_t, initial, _visit_state, &stop), "State stop did not abort!");
    assert(stop.seen[0] == 1 && !stop.seen[1] && !stop.seen[2], "State stop continued traversal!");
    assert(mm_get_allocated() == allocated, "Stopped state walk leaked memory!");
    cfg_fixture_free(&f);
    return 0;
}

int main() {
    mm_init();
    assert(!_test_diamond_and_cycle(), "CFG edge walk test failed!");
    assert(!_test_skip_stop_and_empty(), "CFG walk control test failed!");
    assert(!_test_path_state(), "CFG path state test failed!");
    assert(mm_get_allocated() == 0, "CFG walks leaked memory!");
    return 0;
}
