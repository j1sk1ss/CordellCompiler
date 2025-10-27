#include <hir/ltree.h>

static loop_node_t* _loop_node_create(cfg_block_t* header, cfg_block_t* latch, set_t* loop_blocks) {
    loop_node_t* n = (loop_node_t*)mm_malloc(sizeof(loop_node_t));
    if (!n) return NULL;
    n->header = header;
    n->latch  = latch;
    list_init(&n->children);
    set_init(&n->blocks);
    set_copy(&n->blocks, loop_blocks);
    return n;
}

static int _get_loop_blocks(cfg_block_t* entry, cfg_block_t* exit, set_t* b) {
    if (!set_add(b, entry)) return 0;
    if (entry == exit) return 0;

    set_iter_t it;
    set_iter_init(&entry->pred, &it);
    cfg_block_t* bb;
    while (set_iter_next(&it, (void**)&bb)) {
        _get_loop_blocks(bb, exit, b);
    }

    return 1;
}

static int _collect_loops_for_func(cfg_func_t* fb, list_t* l) {
    list_iter_t bit;
    list_iter_hinit(&fb->blocks, &bit);
    cfg_block_t* cb;
    while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
        if (cb->jmp && set_has(&cb->dom, cb->jmp)) {
            cfg_block_t* header = cb->jmp;
            cfg_block_t* latch  = cb;

            set_t loop_blocks;
            set_init(&loop_blocks);
            
            _get_loop_blocks(latch, header, &loop_blocks);
            set_add(&loop_blocks, header);

            loop_node_t* node = _loop_node_create(header, latch, &loop_blocks);
            list_push_back(l, (void*)node);

            set_free(&loop_blocks);
        }
    }

    return 1;
}

int HIR_LTREE_build_loop_tree(cfg_func_t* fb, ltree_ctx_t* ctx) {
    list_t rl;
    list_init(&rl);

    _collect_loops_for_func(fb, &rl);
    print_debug("_collect_loops_for_func complete, size(rl)=%i", list_size(&rl));

    list_iter_t it_i;
    list_iter_hinit(&rl, &it_i);
    loop_node_t* ni;
    while ((ni = (loop_node_t*)list_iter_next(&it_i))) {
        loop_node_t* best_parent = NULL;
        int best_size = 2147483647;

        list_iter_t it_j;
        list_iter_hinit(&rl, &it_j);
        loop_node_t* nj;
        while ((nj = (loop_node_t*)list_iter_next(&it_j))) {
            if (nj == ni) continue;
            if (set_has(&nj->blocks, &ni->blocks) && set_size(&nj->blocks) > set_size(&ni->blocks)) {
                int sz = set_size(&nj->blocks);
                if (sz < best_size) {
                    best_size = sz;
                    best_parent = nj;
                }
            }
        }

        if (best_parent) list_push_back(&best_parent->children, ni);
        else list_push_back(&ctx->loops, ni);
    }

    list_free(&rl);
    return 1;
}

static int _loop_node_free(loop_node_t* n) {
    if (!n) return 0;
    list_iter_t it;
    list_iter_hinit(&n->children, &it);
    loop_node_t* ch;
    while ((ch = (loop_node_t*)list_iter_next(&it))) {
        _loop_node_free(ch);
    }

    list_free(&n->children);
    set_free(&n->blocks);
    mm_free(n);
    return 1;
}

int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->loops, &it);
    loop_node_t* n;
    while ((n = list_iter_next(&it))) {
        _loop_node_free(n);
    }

    list_free_force(&ctx->loops);
    return 1;
}
