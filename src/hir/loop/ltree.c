#include <hir/loop.h>

/*
Create a loop node.
Params:
    - `header` - Loop node header.
    - `latch` - Loop latch.
    - `loop_locks` - Related loop blocks.

Returns a pointer to a new loop node.
*/
static loop_node_t* _loop_node_create(cfg_block_t* header, cfg_block_t* latch, set_t* loop_blocks) {
    loop_node_t* n = (loop_node_t*)mm_malloc(sizeof(loop_node_t));
    if (!n) return NULL;
    n->header = header;
    n->latch  = latch;
    list_init(&n->children);
    set_copy(&n->blocks, loop_blocks);
    return n;
}

/*
Get blocks in reverse order with the 'pred' field usage.
Params:
    - `entry` - Current entry block.
    - `exit` - Exit block.
    - `b` - Output set.

Returns 1 if succeeds.
*/
static int _get_loop_blocks(cfg_block_t* entry, cfg_block_t* exit, set_t* b) {
    if (
        !set_add(b, entry) ||
        entry == exit
    ) return 0;

    set_foreach (cfg_block_t* bb, &entry->pred) {
        bb->type = CFG_LOOP_BLOCK;
        _get_loop_blocks(bb, exit, b);
    }

    return 1;
}

/*
Unload the input loop node.
Params:
    - `n` - Node to free.

Returns 1 if succeeds.
*/
static int _loop_node_free(loop_node_t* n) {
    if (!n) return 0;
    foreach (loop_node_t* ch, &n->children) {
        _loop_node_free(ch);
    }

    list_free(&n->children);
    set_free(&n->blocks);
    mm_free(n);
    return 1;
}

/*
Collect all loops from the function CFG collection.
Params:
    - `fb` - Function block.
    - `l` - Output list for loop nodes.

Returns 1 if succeeds.
*/
static int _collect_loops_for_func(cfg_func_t* fb, list_t* l) {
    foreach (cfg_block_t* cb, &fb->blocks) {
        if (cb->jmp && set_has(&cb->dom, cb->jmp)) {
            cfg_block_t* header = cb->jmp;
            cfg_block_t* latch  = cb;

            header->type = CFG_LOOP_HEADER;
            header->type = CFG_LOOP_LATCH;

            set_t loop_blocks;
            set_init(&loop_blocks, SET_NO_CMP);
            set_add(&loop_blocks, header);
            
            if (!_get_loop_blocks(latch, header, &loop_blocks)) {
                print_error("Can't obtain loop blocks from the loop!");
                list_free_force_op(l, (int (*)(void*))_loop_node_free);
                set_free(&loop_blocks);
                return 0;
            }

            loop_node_t* node = _loop_node_create(header, latch, &loop_blocks);
            if (!node) {
                print_error("Can't create the loop node from blocks!");
                list_free_force_op(l, (int (*)(void*))_loop_node_free);
                set_free(&loop_blocks);
                return 0;
            }

            list_push_back(l, (void*)node);
            set_free(&loop_blocks);
        }
    }

    return 1;
}

int HIR_LTREE_build_loop_tree(cfg_func_t* fb, ltree_ctx_t* ctx) {
    list_t rl;
    list_init(&rl);

    if (!_collect_loops_for_func(fb, &rl)) {
        print_error("Can't collect loop nodes from the function!");
        list_free_force_op(&rl, (int (*)(void*))_loop_node_free);
        return 0;
    }

    foreach (loop_node_t* ni, &rl) {
        int best_size = INT_MAX;
        loop_node_t* best_parent = NULL;
        foreach (loop_node_t* nj, &rl) {
            if (set_subset(&nj->blocks, &ni->blocks)) {
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

int HIR_LOOP_mark_loops(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        ltree_ctx_t lctx;
        list_init(&lctx.loops);
        if (!HIR_LTREE_build_loop_tree(fb, &lctx)) {
            print_error("Can't build the loop tree here!");
            list_free_force_op(&lctx.loops, (int (*)(void*))_loop_node_free);
            return 0;
        }

        list_free_force_op(&lctx.loops, (int (*)(void *))_loop_node_free);
    }

    return 1;
}

int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx) {
    foreach (loop_node_t* n, &ctx->loops) {
        _loop_node_free(n);
    }

    list_free_force(&ctx->loops);
    return 1;
}
