#include <hir/func.h>

/* Collect all HIR blocks from a CFG function into a queue in traversal order.
Params:
    - `fb` - CFG function to read.
    - `out` - Queue that receives HIR blocks.

Returns 1 if succeeds. */
static int _put_blocks_to_queue(cfg_func_t* fb, queue_t* out) {
    foreach (cfg_block_t* bb, &fb->blocks) {
        iterate_hir_instructions (bb) {
            queue_push(out, hh);
        }
    }

    return 1;
}

/* Compare two CFG functions by their HIR block sequences.
Note: System HIR blocks from the second function are ignored during comparison.
Params:
    - `a` - First CFG function.
    - `b` - Second CFG function.

Returns 1 if functions are equal, otherwise 0. */
static int _function_cmp(cfg_func_t* a, cfg_func_t* b) {
    queue_t aa;
    queue_init(&aa);
    _put_blocks_to_queue(a, &aa);

    int res = 1;
    foreach (cfg_block_t* bb, &b->blocks) {
        hir_block_t* a_hh;
        iterate_hir_instructions (bb) {
            if (!queue_pop(&aa, (void**)&a_hh)) break;
            if (!HIR_is_syst(hh->op)) {
                if (!a_hh || !HIR_block_shallow_equals(hh, a_hh)) {
                    res = 0;
                    goto _forced_exit_of_cmp;
                }
            }
        }
    }

    hir_block_t* extra = NULL;
    if (queue_pop(&aa, (void**)&extra)) {
        res = 0;
    }

_forced_exit_of_cmp: {}
    queue_free(&aa);
    return res;
}

/* Resolve final function replacement through a replacement map. Find one from many of
duplicated functions - the root.
Params:
    - `repl` - Map where keys are old function IDs and values are replacement IDs.
    - `id` - Function ID to resolve.

Returns the final replacement ID or the original ID if there is no replacement. */
static symbol_id_t _resolve(map_t* repl, symbol_id_t id) {
    symbol_id_t next;
    while (map_get(repl, id, (void**)&next)) {
        if (next == id) break;
        id = next;
    }

    return id;
}

/* Find an already used function that can replace the provided function.
Params:
    - `ctx` - CFG context with functions.
    - `a` - Function that needs a replacement candidate.
    - `replacements` - Current replacement map, used to resolve replacement chains.

Returns replacement function if found, otherwise NULL. */
static cfg_func_t* _find_replacement(cfg_ctx_t* ctx, cfg_func_t* a, map_t* replacements) {
    foreach (cfg_func_t* b, &ctx->funcs) {
        if (a == b || !b->used) continue;
        if (_function_cmp(a, b)) {
            symbol_id_t b_id = _resolve(replacements, b->f_id);
            cfg_func_t* final_b = NULL;
            foreach (cfg_func_t* f, &ctx->funcs) {
                if (f->f_id == b_id) {
                    final_b = f;
                    break;
                }
            }

            return final_b;
        }
    }

    return NULL;
}

int HIR_FUNC_set_unused_duplicated_functions(cfg_ctx_t* ctx) {
    map_t replacements;
    map_init(&replacements, MAP_NO_CMP);

    foreach (cfg_func_t* fb, &ctx->funcs) {
        if (!fb->used) continue;
        cfg_func_t* repl = _find_replacement(ctx, fb, &replacements);
        if (!repl) continue;
        map_put(&replacements, fb->f_id, (void*)_resolve(&replacements, repl->f_id));
        fb->used = 0;
    }

    foreach (cfg_func_t* fb, &ctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_hir_instructions (bb) {
                iterate_hir_args (hir_subject_t* arg, hh, 0) {
                    if (arg->t != HIR_FNAME) continue;
                    symbol_id_t old_id = arg->storage.str.s_id;
                    symbol_id_t new_id = _resolve(&replacements, old_id);
                    if (old_id != new_id) {
                        arg->storage.str.s_id = new_id;
                    }
                }
            }
        }
    }

    map_free(&replacements);
    return 1;
}
