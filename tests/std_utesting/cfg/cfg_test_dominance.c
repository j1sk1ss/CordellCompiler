#include "../../misc/cfg_fixture.h"
#include "../../misc/testing.h"

typedef struct {
    int from, to, jump;
} edge_t;

typedef struct {
    const char* name;
    int count, edge_count;
    edge_t edges[12];
    unsigned long dom[CFG_TEST_MAX_BLOCKS];
    int idom[CFG_TEST_MAX_BLOCKS];
    unsigned long frontier[CFG_TEST_MAX_BLOCKS];
} case_t;

static const case_t cases[] = {
    { "single block", 1, 0, {{0}}, {1}, {-1}, {0} },
    { "chain", 4, 3, {{0,1,0}, {1,2,0}, {2,3,0}},
      {1,3,7,15}, {-1,0,1,2}, {0,0,0,0} },
    { "diamond and tail", 5, 5, {{0,1,0}, {0,2,1}, {1,3,0}, {2,3,0}, {3,4,0}},
      {1,3,5,9,25}, {-1,0,0,0,3}, {0,8,8,0,0} },
    { "bypass", 4, 4, {{0,1,0}, {0,2,1}, {1,2,0}, {2,3,0}},
      {1,3,5,13}, {-1,0,0,2}, {0,4,0,0} },
    { "loop and exit", 5, 5, {{0,1,0}, {1,2,0}, {1,4,1}, {2,3,0}, {3,1,1}},
      {1,3,7,15,19}, {-1,0,1,2,1}, {0,2,2,2,0} },
    { "self loop", 3, 3, {{0,1,0}, {1,1,0}, {1,2,1}},
      {1,3,7}, {-1,0,1}, {0,2,0} },
    { "entry self loop", 2, 2, {{0,0,0}, {0,1,1}},
      {1,3}, {-1,0}, {1,0} },
    { "nested loops", 7, 8, {{0,1,0}, {1,2,0}, {1,6,1}, {2,3,0}, {2,5,1}, {3,4,0}, {4,2,1}, {5,1,1}},
      {1,3,7,15,31,39,67}, {-1,0,1,2,3,2,1}, {0,2,6,4,4,2,0} },
    { "irreducible cycle", 4, 5, {{0,1,0}, {0,2,1}, {1,2,0}, {2,1,1}, {2,3,0}},
      {1,3,5,13}, {-1,0,0,2}, {0,4,2,0} },
    { "isolated unreachable block", 4, 2, {{0,1,0}, {1,2,0}},
      {1,3,7,8}, {-1,0,1,-1}, {0,0,0,0} },
};

static int _check_analysis(cfg_fixture_t* f, const case_t* c) {
    unsigned long children = 0;
    for (int i = 0; i < c->count; i++) {
        cfg_block_t* b = &f->blocks[i];
        if (!cfg_fixture_set_matches(f, &b->dom, c->dom[i])) {
            fprintf(stderr, "%s: wrong dominators for B%d\n", c->name, i);
            return 1;
        }
        cfg_block_t* expected = c->idom[i] < 0 ? NULL : &f->blocks[c->idom[i]];
        if (b->sdom != expected) {
            fprintf(stderr, "%s: wrong immediate dominator for B%d\n", c->name, i);
            return 1;
        }
        if (!cfg_fixture_set_matches(f, &b->domf, c->frontier[i])) {
            fprintf(stderr, "%s: wrong dominance frontier for B%d\n", c->name, i);
            return 1;
        }
        for (cfg_block_t* child = b->dom_c; child; child = child->dom_s) {
            unsigned long bit = 1UL << child->id;
            assert(!(children & bit), "Duplicate child or cycle in the dominator tree!");
            assert(child->sdom == b, "Dominator tree child has the wrong parent!");
            children |= bit;
        }
    }
    unsigned long expected_children = 0;
    for (int i = 0; i < c->count; i++) {
        if (c->idom[i] >= 0) expected_children |= 1UL << i;
    }
    assert(children == expected_children, "Dominator tree is missing children!");
    return 0;
}

static int _run_case(const case_t* c, int reverse) {
    cfg_fixture_t f = { 0 };
    cfg_fixture_init(&f, c->count, reverse);
    for (int i = 0; i < c->edge_count; i++) {
        cfg_fixture_edge(&f, c->edges[i].from, c->edges[i].to, c->edges[i].jump);
    }
    assert(HIR_CFG_create_domdata(&f.ctx), "Dominance analysis failed!");
    assert(!_check_analysis(&f, c), "Dominance result mismatch!");

    assert(HIR_CFG_unload_domdata(&f.ctx), "Dominance reset failed!");
    for (int i = 0; i < c->count; i++) {
        cfg_block_t* b = &f.blocks[i];
        assert(!set_size(&b->dom) && !set_size(&b->domf), "Reset left stale dominance sets!");
        assert(!b->sdom && !b->dom_c && !b->dom_s, "Reset left stale tree links!");
    }
    assert(HIR_CFG_create_domdata(&f.ctx), "Dominance rebuild failed!");
    assert(!_check_analysis(&f, c), "Dominance rebuild result mismatch!");
    cfg_fixture_free(&f);
    return 0;
}

int main() {
    mm_init();
    for (unsigned long i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        for (int reverse = 0; reverse <= 1; reverse++) {
            assert(!_run_case(&cases[i], reverse), "Dominance case failed!");
            assert(mm_get_allocated() == 0, "Dominance analysis leaked memory!");
        }
    }
    return 0;
}
