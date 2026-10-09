#ifndef CFG_FIXTURE_H_
#define CFG_FIXTURE_H_

#include <hir/cfg.h>

#define CFG_TEST_MAX_BLOCKS 16

typedef struct {
    cfg_ctx_t ctx;
    cfg_func_t func;
    cfg_block_t blocks[CFG_TEST_MAX_BLOCKS];
} cfg_fixture_t;

static inline void cfg_fixture_init(cfg_fixture_t* f, int count, int reverse) {
    list_init(&f->ctx.funcs);
    list_init(&f->func.blocks);
    f->func.used = 1;
    list_push_back(&f->ctx.funcs, &f->func);
    for (int i = 0; i < count; i++) {
        cfg_block_t* b = &f->blocks[i];
        b->id = i;
        b->pfunc = &f->func;
        set_init(&b->pred, SET_NO_CMP);
        set_init(&b->dom, SET_CMP);
        set_init(&b->domf, SET_NO_CMP);
        set_init(&b->visitors, SET_NO_CMP);
    }
    list_push_back(&f->func.blocks, &f->blocks[0]);
    for (int i = 1; i < count; i++) {
        list_push_back(&f->func.blocks, &f->blocks[reverse ? count - i : i]);
    }
}

static inline void cfg_fixture_edge(cfg_fixture_t* f, int from, int to, int jump) {
    if (jump) f->blocks[from].jmp = &f->blocks[to];
    else f->blocks[from].l = &f->blocks[to];
    set_add(&f->blocks[to].pred, &f->blocks[from]);
}

static inline int cfg_fixture_set_matches(cfg_fixture_t* f, set_t* set, unsigned long mask) {
    int count = 0;
    foreach (cfg_block_t* b, &f->func.blocks) {
        int expected = !!(mask & (1UL << b->id));
        if (set_has(set, b) != expected) return 0;
        count += expected;
    }
    return set_size(set) == count;
}

static inline void cfg_fixture_free(cfg_fixture_t* f) {
    foreach (cfg_block_t* b, &f->func.blocks) {
        set_free(&b->pred);
        set_free(&b->dom);
        set_free(&b->domf);
        set_free(&b->visitors);
    }
    list_free(&f->func.blocks);
    list_free(&f->ctx.funcs);
}

#endif
