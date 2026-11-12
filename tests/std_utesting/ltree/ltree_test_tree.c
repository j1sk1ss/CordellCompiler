#include <hir/loop.h>
#include "../../misc/cfg_fixture.h"
#include "../../misc/testing.h"

static loop_node_t* _find_header(list_t* loops, cfg_block_t* header) {
    foreach (loop_node_t* loop, loops) {
        if (loop->header == header) return loop;
    }
    return NULL;
}

static int _test_without_natural_loops(int irreducible) {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 4, 0);
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 0, 2, 1);
    cfg_fixture_edge(&f, 1, 2, 0);
    cfg_fixture_edge(&f, 2, 3, 0);
    if (irreducible) cfg_fixture_edge(&f, 2, 1, 1);
    ltree_ctx_t loops;
    map_init(&loops.lmap, MAP_NO_CMP);
    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &loops), "Loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(!list_size(roots), "Graph without a natural loop produced a loop!");
    foreach (cfg_block_t* b, &f.func.blocks) {
        assert(b->type == CFG_DEFAULT_BLOCK, "Graph without a natural loop marked loop blocks!");
    }
    HIR_LTREE_unload_ctx(&loops);
    cfg_fixture_free(&f);
    return 0;
}

static int _test_sibling_loops() {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 6, 0);
    /* Sequential loops {1,2} and {3,4}, sharing no loop blocks. */
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 1, 2, 0);
    cfg_fixture_edge(&f, 1, 3, 1);
    cfg_fixture_edge(&f, 2, 1, 1);
    cfg_fixture_edge(&f, 3, 4, 0);
    cfg_fixture_edge(&f, 3, 5, 1);
    cfg_fixture_edge(&f, 4, 3, 1);
    ltree_ctx_t loops;
    map_init(&loops.lmap, MAP_NO_CMP);
    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &loops), "Sibling loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(list_size(roots) == 2, "Sequential loops were nested or lost!");
    loop_node_t* first = _find_header(roots, &f.blocks[1]);
    loop_node_t* second = _find_header(roots, &f.blocks[3]);
    assert(first && second, "Sibling loop headers missing!");
    assert(!first->p && !second->p && !list_size(&first->children) && !list_size(&second->children), "Sibling loops have parents or children!");
    assert(cfg_fixture_set_matches(&f, &first->blocks, 6), "Wrong first loop blocks!");
    assert(cfg_fixture_set_matches(&f, &second->blocks, 24), "Wrong second loop blocks!");
    HIR_LTREE_unload_ctx(&loops);
    cfg_fixture_free(&f);
    return 0;
}

static int _test_three_levels(int reverse) {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, 8, reverse);
    /* Headers 1,2,3 and latches 6,5,4 form three nested natural loops. */
    cfg_fixture_edge(&f, 0, 1, 0);
    cfg_fixture_edge(&f, 1, 2, 0);
    cfg_fixture_edge(&f, 1, 7, 1);
    cfg_fixture_edge(&f, 2, 3, 0);
    cfg_fixture_edge(&f, 2, 6, 1);
    cfg_fixture_edge(&f, 3, 4, 0);
    cfg_fixture_edge(&f, 3, 5, 1);
    cfg_fixture_edge(&f, 4, 3, 1);
    cfg_fixture_edge(&f, 5, 2, 1);
    cfg_fixture_edge(&f, 6, 1, 1);
    ltree_ctx_t loops;
    map_init(&loops.lmap, MAP_NO_CMP);
    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &loops), "Deep loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(list_size(roots) == 1, "Expected one outer loop!");
    loop_node_t* loop = list_get_head(roots);
    loop_node_t* parent = NULL;
    const unsigned long masks[] = {126,60,24};
    for (int depth = 0; depth < 3; depth++) {
        assert(loop->header == &f.blocks[depth + 1], "Wrong nested header!");
        assert(loop->latch == &f.blocks[6 - depth], "Wrong nested latch!");
        assert(loop->p == parent, "Loop did not choose its closest containing parent!");
        assert(HIR_LTREE_nested_count(loop) == depth + 1, "Wrong loop nesting depth!");
        assert(cfg_fixture_set_matches(&f, &loop->blocks, masks[depth]), "Wrong nested loop membership!");
        assert(loop->header->type == CFG_LOOP_HEADER && loop->latch->type == CFG_LOOP_LATCH, "Nested loop endpoint types lost!");
        assert(list_size(&loop->children) == (depth < 2), "Wrong number of nested children!");
        parent = loop;
        loop = list_get_head(&loop->children);
    }
    assert(HIR_LTREE_nested_count(NULL) == 0, "NULL loop has a nesting depth!");
    HIR_LTREE_unload_ctx(&loops);
    cfg_fixture_free(&f);
    return 0;
}

int main() {
    mm_init();
    assert(!_test_without_natural_loops(0), "Acyclic graph test failed!");
    assert(!_test_without_natural_loops(1), "Irreducible graph test failed!");
    assert(!_test_sibling_loops(), "Sibling loops test failed!");
    assert(!_test_three_levels(0), "Deep nesting test failed!");
    assert(!_test_three_levels(1), "Reordered deep nesting test failed!");
    assert(mm_get_allocated() == 0, "Loop tree tests leaked memory!");
    return 0;
}
