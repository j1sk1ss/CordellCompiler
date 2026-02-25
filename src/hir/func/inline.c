#include <hir/func.h>

/*
Replace function argument pushing with the argument assigning.
Params:
    - `f` - Current function.
    - `args` - Function's argument list.
    - `pos` - Current position in the HIR.
              Note: This is the point, where inline must start.

Returns 1 if succeeds.
*/
static int _inline_arguments(cfg_func_t* f, list_t* args, hir_block_t* pos) {
    list_iter_t it;
    list_iter_hinit(args, &it);

    hir_block_t* hh = HIR_FUNC_get_next(NULL, f, NULL, 0);
    while (hh) {
        if (hh->op == HIR_FARGLD) {
            hir_block_t* nblock = HIR_copy_block(hh, 1);
            nblock->op = HIR_STORE;
            HIR_unload_subject(nblock->sarg);
            nblock->sarg = (hir_subject_t*)list_iter_next(&it);
            HIR_insert_block_before(nblock, pos);
        }

        hh = HIR_FUNC_get_next(hh, f, NULL, 1);
    }

    return 1;
}

/*
Find old labels and replace them with a new one.
Params:
    - `h` - HIR entry block.
    - `e` - HIR exit block.
    - `lb` - Old labels.
    - `new` - New labels.

Returns 1 if succeeds.
*/
static int _replace_label_usage(hir_block_t* h, hir_block_t* e, hir_subject_t* old, hir_subject_t* new, cfg_func_t* fb) {
    while (h) {
        if (h->op != HIR_MKLB) {
            hir_subject_t* args[3] = { h->farg, h->sarg, h->targ };
            for (int i = 0; i < 3; i++) {
                if (args[i] && args[i]->t == HIR_LABEL && args[i]->id == old->id) {
                    switch (i) {
                        case 0:  h->farg = new; break;
                        case 1:  h->sarg = new; break;
                        default: h->targ = new; break;
                    }
                }
            }
        }

        h = HIR_FUNC_get_next(h, fb, e, 1);
    }

    return 1;
}

/*
Insert function body into the source HIR position.
Params:
    - `f` - Current function.
    - `res` - The target save point for the function call.
              Note: It can be NULL pointer, if there is
                    no expected output in the source code.
    - `pos` - Current position in the HIR.
              Note: This is the point, where inline must start.

Returns 1 if succeeds.
*/
static int _inline_function(cfg_func_t* f, hir_subject_t* res, hir_block_t* pos) {
    hir_block_t *nentry = NULL, *nexit = pos;
    hir_block_t* hh = HIR_FUNC_get_next(NULL, f, NULL, 0);
    while (hh && hh->op != HIR_MKSCOPE) hh = HIR_FUNC_get_next(hh, f, NULL, 1);
    hh = HIR_FUNC_get_next(hh, f, NULL, 1);
    while (hh && hh->op != HIR_MKSCOPE) hh = HIR_FUNC_get_next(hh, f, NULL, 1);

    /* Basic instruction copy without label track
       - Will copy only instruction and variables
       - Will preserve old labels */
    int scopes = -1;
    while (hh && hh->op != HIR_FEND) {
        hir_block_t* nblock = NULL;
        if (!hh->unused) {
            nblock = HIR_copy_block(hh, 0);
            switch (hh->op) {
                case HIR_MKSCOPE: {
                    if (scopes++ < 0) goto _skip_instruction;
                    break;
                }
                case HIR_ENDSCOPE: {
                    if (--scopes < 0) goto _skip_instruction;
                    break;
                }
                case HIR_FRET: {
                    if (!res) goto _skip_instruction;
                    else {
                        nblock->op   = HIR_STORE;
                        nblock->sarg = hh->farg;
                        HIR_unload_subject(nblock->farg);
                        nblock->farg = res;
                    }
                }
                default: break;
            }
        }

        if (nblock) {
            HIR_insert_block_before(nblock, pos);
            if (!nentry) nentry = nblock;
        }
        else {
_skip_instruction: {}
            HIR_unload_blocks(nblock);
        }

        hh = HIR_FUNC_get_next(hh, f, NULL, 1);
    }

    /* Re-link labels */
    hh = nentry;
    while (hh) {
        if (hh->op == HIR_MKLB) {
            hir_subject_t* nlb = HIR_copy_subject(hh->farg);
            _replace_label_usage(nentry, nexit, hh->farg, nlb, f);
            hh->farg = nlb;
        }

        hh = HIR_FUNC_get_next(hh, f, nexit, 1);
    }

    return 1;
}

/*
Get function from the CFG context by the provided function ID.
Params:
    - `cctx` - CFG context.
    - `fid` - Function ID.

Returns either a function pointer or the NULL value.
*/
static inline cfg_func_t* _get_funcblock(cfg_ctx_t* cctx, long fid) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (fb->fid == fid) return fb;
    }

    return NULL;
}

/*
Find loop where the input bb is present.
Params:
    - `loops` - Current list of loops.
    - `bb` - The target base block.

Returns a loop node where the base block is present. 
*/
static loop_node_t* _find_loop(list_t* loops, cfg_block_t* bb) {
    foreach (loop_node_t* ln, loops) {
        loop_node_t* found = _find_loop(&ln->children, bb);
        if (found) return found;
    }

    foreach (loop_node_t* ln, loops) {
        if (set_has(&ln->blocks, bb)) return ln;
    }

    return NULL;
}

typedef struct {
    struct { /* Information about a function        */
        int  bb_size;        /* The source function size in base blocks                 */
        int  hir_size;       /* The source function size in hir blocks                  */
        int  funccals;       /* Count of funcalls in the function                       */
        int  syscalls;       /* Count of syscalls in the function                       */
    } src_info;
    struct { /* Information about the dest location */
        int  loop_nested;    /* If it in a loop, is it a nested loop? And how deep?     */
        int  loop_size_bb;   /* If it in a loop, how big is it (in bb)?                 */
        int  loop_size_hir;  /* If it in a loop, how big is it (in hir)?                */
        int  func_count;     /* How many functions in the past and in the further code? */
        int  near_break;     /* Distance to nearest 'break' statement or -1             */
        char is_dom;         /* Is this function will go to one of the branches?        */
        char is_start;       /* Is this is a start function?                            */
    } dst_info;
} inline_candidate_info_t;

/*
How many commands between the 'pos' command and a break command?
Params:
    - `pos` - Current HIR block position.
              Note: Initialy must be the 'NULL' value.
    - `ibb` - Initial base block.

Returns how many commands.
*/
static int _find_nearest_break(hir_block_t* pos, cfg_block_t* ibb) {
    set_t visited;
    set_init(&visited, SET_NO_CMP);

    queue_t blocks;
    queue_init(&blocks);
    queue_push(&blocks, inttuple_create((long)ibb, 0));

    int_tuple_t* tpl;
    while (queue_pop(&blocks, (void**)&tpl)) {
        cfg_block_t* block = (cfg_block_t*)((void*)tpl->x);
        int block_size = tpl->y;
        set_add(&visited, block);
        inttuple_free(tpl);

        hir_block_t* hh = HIR_get_next(pos ? pos : block->hmap.entry, block->hmap.exit, 0);
        pos = NULL;
        while (hh) {
            block_size++;
            if (hh->op == HIR_BREAK) {
                queue_free_force(&blocks);
                set_free(&visited);
                return block_size;
            }

            hh = HIR_get_next(hh, block->hmap.exit, 1);
        }

        if (!set_has(&visited, block->jmp) && block->jmp) queue_push(&blocks, inttuple_create((long)block->jmp, block_size));
        if (!set_has(&visited, block->l) && block->l)     queue_push(&blocks, inttuple_create((long)block->l, block_size));
    }

    queue_free(&blocks);
    set_free(&visited);
    return -1;
}

/*
Collect essential information for inline candidate decisiion.
Params:
    - `f` - Target function to inline.
    - `pos` - BB position for inline.
    - `hpos` - HIR position for inline.
    - `lctx` - Loops context.
    - `info` - Output information.

Returns 1 if succeeds.
*/
static int _collect_information(
    cfg_func_t* f, cfg_block_t* pos, hir_block_t* hpos, ltree_ctx_t* lctx, inline_candidate_info_t* info, sym_table_t* smt
) {
    loop_node_t* loop = _find_loop(&lctx->loops, pos);
    if (loop) {
        info->dst_info.loop_size_bb = set_size(&loop->blocks);
        info->dst_info.loop_nested  = HIR_LTREE_nested_count(loop);
        info->dst_info.near_break   = _find_nearest_break(hpos, pos);
    }
    
    info->src_info.bb_size = list_size(&f->blocks);
    foreach (cfg_block_t* bb, &f->blocks) {
        info->src_info.hir_size += HIR_CFG_count_blocks_in_bb(bb);
        hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
        while (hh) {
            if (HIR_funccall(hh->op)) info->src_info.funccals++;
            if (
                hh->op == HIR_SYSC || 
                hh->op == HIR_STORE_SYSC
            ) info->src_info.syscalls++; 
            hh = HIR_get_next(hh, bb->hmap.exit, 1);
        }
    }

    func_info_t fi;
    if (FNTB_get_info_id(pos->pfunc->fid, &fi, &smt->f)) {
        info->dst_info.is_start = fi.flags.entry;
    }

    return 1;
}

/*
Euristic function for evaluating an inline candidate.
Note 1: If this function returns 1 - the provided function
        can be inlined.
Note 2: This function collects the next information about
        a function:
        - The function size:
            - Base blocks count.
            - HIR blocks count.
        - The function loop tree:
            - Loop tree size.
            - Max nexted loop.
            - The destination loop size.
        - The source position position:
            - Nested loop count.
            - If this a start function.
        - If the target position in a loop, is there a break statement
          near to the position?
        - Is the function is dominated by entry?
Params:
    - `f` - CFG inline candidate function.
    - `pos` - Source BB position.
    - `hpos` - Source HIR position.
    - `lctx` - Source block loop environment.

Returns 1 if the provided function can be inlined.
*/
static int _inline_candidate(
    cfg_func_t* f, cfg_block_t* pos, hir_block_t* hpos, ltree_ctx_t* lctx, sym_table_t* smt, int (*checker)(int*, int)
) {
    if (!f || !pos || !lctx) return 0;
    inline_candidate_info_t iinfo = { 0 };
    _collect_information(f, pos, hpos, lctx, &iinfo, smt);
    return checker((int*)&iinfo, (int)sizeof(inline_candidate_info_t));
}

int HIR_FUNC_perform_inline(cfg_ctx_t* cctx, sym_table_t* smt, int (*checker)(int*, int)) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        
        /* Collect information about the environment
           - Basic information about the loops */
        ltree_ctx_t lctx;
        list_init(&lctx.loops);
        HIR_LTREE_build_loop_tree(fb, &lctx);

        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                if (HIR_funccall(hh->op)) {
                    cfg_func_t* trg = _get_funcblock(cctx, hh->sarg->storage.str.s_id);
                    if (_inline_candidate(trg, bb, hh, &lctx, smt, checker) && fb != trg) {
                        _inline_arguments(trg, &hh->targ->storage.list.h, hh);
                        
                        hir_subject_t* res = NULL;
                        if (
                            hh->op == HIR_STORE_FCLL || 
                            hh->op == HIR_STORE_ECLL
                        ) res = hh->farg;
                        
                        _inline_function(trg, res, hh);
                        hh->unused = 1;
                    }
                }

                hh = HIR_get_next(hh, bb->hmap.exit, 1);
            }
        }

        HIR_LTREE_unload_ctx(&lctx);
    }

    return 1;
}

int HIR_FUNC_inline_euristic_desider(int* data, int size) {
    if (!data || size != sizeof(inline_candidate_info_t)) return 0;
    inline_candidate_info_t* parsed = (inline_candidate_info_t*)data;
    int score = 0;
    if (parsed->src_info.bb_size <= 2)       score += 4;
    else if (parsed->src_info.bb_size <= 5)  score += 3;
    else if (parsed->src_info.bb_size <= 10) score += 2;
    else if (parsed->src_info.bb_size > 15)  score -= 3;
    score += parsed->dst_info.loop_nested * parsed->dst_info.loop_nested;
    return score >= 3;
}
