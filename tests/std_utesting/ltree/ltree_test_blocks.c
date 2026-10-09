#include <hir/loop.h>
#include "../../misc/testing.h"

typedef struct {
    cfg_ctx_t ctx;
    cfg_func_t func;
    cfg_block_t blocks[9];
    ltree_ctx_t loops;
} fixture_t;

static void _init_fixture(fixture_t* f, int count) {
    list_init(&f->ctx.funcs);
    list_init(&f->func.blocks);
    f->func.used = 1;
    list_push_back(&f->ctx.funcs, &f->func);
    map_init(&f->loops.lmap, MAP_NO_CMP);
    for (int i = 0; i < count; i++) {
        cfg_block_t* b = &f->blocks[i];
        b->id = i;
        b->pfunc = &f->func;
        set_init(&b->pred, SET_NO_CMP);
        set_init(&b->dom, SET_CMP);
        list_push_back(&f->func.blocks, b);
    }
}

static void _edge(cfg_block_t* from, cfg_block_t* to, int jump) {
    if (jump) from->jmp = to;
    else from->l = to;
    set_add(&to->pred, from);
}

static void _free_fixture(fixture_t* f) {
    HIR_LTREE_unload_ctx(&f->loops);
    foreach (cfg_block_t* b, &f->func.blocks) {
        set_free(&b->pred);
        set_free(&b->dom);
    }
    list_free(&f->func.blocks);
    list_free(&f->ctx.funcs);
}

static int _test_exit_blocks() {
    fixture_t f = { 0 };
    _init_fixture(&f, 9);
    cfg_block_t *pre = &f.blocks[0], *header = &f.blocks[1], *split = &f.blocks[2];
    cfg_block_t *left = &f.blocks[3], *right = &f.blocks[4], *latch = &f.blocks[5];
    cfg_block_t *brk = &f.blocks[6], *exit = &f.blocks[7], *tail = &f.blocks[8];
    _edge(pre, header, 0);
    _edge(header, split, 0);
    _edge(header, exit, 1);
    _edge(split, left, 0);
    _edge(split, right, 1);
    _edge(left, latch, 0);
    _edge(right, latch, 0);
    _edge(right, brk, 1);
    _edge(brk, exit, 0);
    _edge(latch, header, 1);
    _edge(exit, tail, 0);

    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(set_has(&exit->dom, header), "Exit must be dominated by the header!");
    assert(set_has(&tail->dom, header), "Tail must be dominated by the header!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &f.loops), "Loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&f.loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(list_size(roots) == 1, "Expected one loop!");
    loop_node_t* loop = list_get_head(roots);
    assert(loop->header == header && loop->latch == latch, "Wrong loop endpoints!");
    assert(set_size(&loop->blocks) == 5, "Wrong natural loop size!");
    assert(set_has(&loop->blocks, split), "Split block missing!");
    assert(set_has(&loop->blocks, left) && set_has(&loop->blocks, right), "Loop branches missing!");
    assert(!set_has(&loop->blocks, pre), "Preheader included in the loop!");
    assert(!set_has(&loop->blocks, brk), "Branch that only exits included in the loop!");
    assert(!set_has(&loop->blocks, exit), "Exit included in the loop!");
    assert(!set_has(&loop->blocks, tail), "Code after the loop included!");
    assert(header->type == CFG_LOOP_HEADER && latch->type == CFG_LOOP_LATCH, "Loop endpoint types overwritten!");
    assert(brk->type == CFG_DEFAULT_BLOCK && exit->type == CFG_DEFAULT_BLOCK && tail->type == CFG_DEFAULT_BLOCK, "Outside blocks marked as loop blocks!");
    _free_fixture(&f);
    return 0;
}

static int _test_self_loop() {
    fixture_t f = { 0 };
    _init_fixture(&f, 3);
    cfg_block_t *pre = &f.blocks[0], *header = &f.blocks[1], *exit = &f.blocks[2];
    _edge(pre, header, 0);
    _edge(header, header, 0);
    _edge(header, exit, 1);
    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &f.loops), "Self-loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&f.loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(list_size(roots) == 1, "Expected one self-loop!");
    loop_node_t* loop = list_get_head(roots);
    assert(loop->header == header && loop->latch == header, "Wrong self-loop endpoints!");
    assert(set_size(&loop->blocks) == 1 && set_has(&loop->blocks, header), "Wrong self-loop membership!");
    assert(header->type == CFG_LOOP_HEADER, "Self-loop header type lost!");
    _free_fixture(&f);
    return 0;
}

static int _test_nested_loops() {
    fixture_t f = { 0 };
    _init_fixture(&f, 7);
    cfg_block_t *pre = &f.blocks[0], *outer_header = &f.blocks[1], *inner_header = &f.blocks[2];
    cfg_block_t *body = &f.blocks[3], *inner_latch = &f.blocks[4];
    cfg_block_t *outer_latch = &f.blocks[5], *exit = &f.blocks[6];
    _edge(pre, outer_header, 0);
    _edge(outer_header, inner_header, 0);
    _edge(inner_header, body, 0);
    _edge(body, inner_latch, 0);
    _edge(inner_latch, inner_header, 1);
    _edge(inner_latch, outer_latch, 0);
    _edge(outer_latch, outer_header, 1);
    _edge(outer_latch, exit, 0);
    assert(HIR_CFG_compute_dom(&f.func), "Dominance analysis failed!");
    assert(HIR_LOOP_mark_loops(&f.ctx, &f.loops), "Nested loop collection failed!");
    list_t* roots = NULL;
    assert(map_get(&f.loops.lmap, f.func.f_id, (void**)&roots), "Loop list missing!");
    assert(list_size(roots) == 1, "Expected one outer loop!");
    loop_node_t* outer = list_get_head(roots);
    assert(outer->header == outer_header && set_size(&outer->blocks) == 5, "Wrong outer loop!");
    assert(list_size(&outer->children) == 1, "Inner loop missing from the tree!");
    loop_node_t* inner = list_get_head(&outer->children);
    assert(inner->p == outer && inner->header == inner_header, "Wrong inner loop parent!");
    assert(set_size(&inner->blocks) == 3, "Wrong inner loop size!");
    assert(!set_has(&inner->blocks, outer_latch), "Inner loop includes its exit!");
    assert(!set_has(&outer->blocks, exit), "Outer loop includes its exit!");
    assert(inner_header->type == CFG_LOOP_HEADER && inner_latch->type == CFG_LOOP_LATCH, "Outer loop overwrote inner endpoint types!");
    _free_fixture(&f);
    return 0;
}

int main() {
    mm_init();
    assert(!_test_exit_blocks(), "Exit block regression failed!");
    assert(!_test_self_loop(), "Self-loop regression failed!");
    assert(!_test_nested_loops(), "Nested loop regression failed!");
    assert(mm_get_allocated() == 0, "Loop collection leaked memory!");
    return 0;
}
