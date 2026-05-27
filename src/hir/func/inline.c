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
    int index = 0, size = list_size(args);
    hir_subject_t** args_flatten = (hir_subject_t**)list_flatten(args);
    if (!args_flatten) return 0;

    foreach (cfg_block_t* bb, &f->blocks) {
        iterate_hir_instructions (bb) {
            if (index >= size) break;
            if (hh->op == HIR_FARGLD) {
                hir_block_t* copy = HIR_copy_block(hh, 1);
                HIR_unload_subject(copy->sarg);
                copy->op   = HIR_STORE;
                copy->sarg = HIR_copy_subject(args_flatten[index++]);
                HIR_insert_block_before(copy, pos);
            }
        }
    }

    mm_free(args_flatten);
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
            iterate_ref_hir_args (hir_subject_t** curr, h, 0) {
                if (
                    (*curr)->t == HIR_LABEL && 
                    (*curr)->id == old->id
                ) *curr = new;
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
    hir_subject_t* exit_label = HIR_SUBJ_LABEL();
    while (hh && hh->op != HIR_FEND) {
        hir_block_t  *nblock = NULL, *rlb = NULL;
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
                    rlb = HIR_create_block(HIR_JMP, exit_label, NULL, NULL);
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
            if (!nentry) {
                nentry = nblock;
            }
        }
        else {
_skip_instruction: {}
            HIR_unload_blocks(nblock);
        }

        if (rlb) {
            HIR_insert_block_before(rlb, pos);
            rlb = NULL;
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

    HIR_insert_block_before(HIR_create_block(HIR_MKLB, exit_label, NULL, NULL), pos);
    return 1;
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
        int  loop_count;     /* Loops count                                             */
        int  loop_nested;    /* Max depth for a loop nested size                        */
        int  bb_size;        /* The source function size in base blocks                 */
        int  hir_size;       /* The source function size in hir blocks                  */
        int  funccals;       /* Count of funcalls in the function                       */
        int  syscalls;       /* Count of syscalls in the function                       */
        char has_inline_mod; /* Whether the function has or not an inline modifier      */
        char has_sideeff;    /* Whether the function has a syscall, memmory, etc.       */
        int  ifs_count;      /* How many ifs in the functions                           */
        int  returns_count;  /* How many returns in the function                        */
        int  params_count;   /* How mny parameters in the function's signature          */
        char is_void_ret;    /* Whether the function returns void                       */
    } src_info;
    struct { /* Information about the dest location */
        int  loop_nested;    /* If it in a loop, is it a nested loop? And how deep?     */
        int  loop_size_bb;   /* If it in a loop, how big is it (in bb)?                 */
        int  loop_size_hir;  /* If it in a loop, how big is it (in hir)?                */
        int  func_count;     /* How many functions in the past and in the further code? */
        int  near_break;     /* Distance to nearest 'break' statement or -1             */
        char is_dom;         /* Is this function will go to one of the branches?        */
        char is_start;       /* Is this is a start function?                            */
        int  params_count;   /* How mny parameters in the function's signature          */
    } dst_info;
    struct {
        char in_same_file;   /* Whether functions in the same file                      */
        char in_same_dir;    /* Whether functions in the same directory                 */
    } general_info;
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

// TODO: docs
static file_position_t* _find_first_pos(cfg_func_t* f) {
    foreach (cfg_block_t* bb, &f->blocks) {
        iterate_hir_instructions (bb) {
            if (hh->op == HIR_SETPOS) return &hh->farg->storage.pos;
        }
    }

    return NULL;
}

// TODO: docs
static inline int _count_params(cfg_func_t* f, sym_table_t* smt) {
    func_info_t fi;
    if (!FNTB_get_info_id(f->f_id, &fi, &smt->f)) return 0;
    int count = 0;
    for (ast_node_t* arg = fi.args->c; arg; arg = arg->siblings.n) count++;
    return count;
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
    cfg_func_t* f, cfg_block_t* pos, hir_block_t* hpos, 
    list_t* src_floops, list_t* dst_floops, 
    inline_candidate_info_t* info, sym_table_t* smt
) {
    loop_node_t* loop = _find_loop(dst_floops, pos);
    if (loop) {
        info->dst_info.loop_size_bb = set_size(&loop->blocks);
        info->dst_info.loop_nested  = HIR_LTREE_nested_count(loop);
        info->dst_info.near_break   = _find_nearest_break(hpos, pos);
    }

    file_position_t *src_pos = _find_first_pos(f), *dst_pos = _find_first_pos(pos->pfunc);
    if (
        src_pos && dst_pos &&
        src_pos->file && dst_pos->file
    ) {
        info->general_info.in_same_file = is_same_file(src_pos->file, dst_pos->file);
        info->general_info.in_same_dir  = is_same_dir(src_pos->file, dst_pos->file);
    }

    info->dst_info.params_count = _count_params(pos->pfunc, smt);
    info->src_info.params_count = _count_params(f, smt);
    
    info->src_info.loop_nested = 0;
    foreach (loop_node_t* l, src_floops) {
        info->src_info.loop_nested = MAX(info->src_info.loop_nested, HIR_LTREE_nested_count(l));
    }
    
    info->src_info.loop_count = list_size(src_floops);
    info->src_info.bb_size = list_size(&f->blocks);
    foreach (cfg_block_t* bb, &f->blocks) {
        info->src_info.hir_size += HIR_CFG_count_blocks_in_bb(bb, 0);
        iterate_hir_instructions (bb) {
            if (
                hh->op == HIR_FRET || 
                hh->op == HIR_EXITOP
            ) info->src_info.returns_count++;
            if (hh->op == HIR_IFOP2) info->src_info.ifs_count++;
            if (HIR_is_funccall(hh->op)) info->src_info.funccals++;
            if (
                hh->op == HIR_SYSC || 
                hh->op == HIR_STORE_SYSC
            ) info->src_info.syscalls++; 
            if (hh->op == HIR_LDREF) info->src_info.has_sideeff = 1;
        }
        if (info->src_info.syscalls) info->src_info.has_sideeff = 1;
    }
    
    func_info_t fi;
    if (FNTB_get_info_id(pos->pfunc->f_id, &fi, &smt->f)) {
        info->dst_info.is_start = fi.flags.entry;
    }
    
    if (FNTB_get_info_id(f->f_id, &fi, &smt->f)) {
        if (
            fi.rtype && 
            fi.rtype->t->t_type == I0_TYPE_TOKEN && !fi.rtype->t->flags.ptr
        ) info->src_info.is_void_ret = 1;
        if (fi.flags.inln == SOFT_YES_INLINE) info->src_info.has_inline_mod = 1;
    }

    return 1;
}

/*
Heuristic function for evaluating an inline candidate.
Note 1: If this function returns 1 - the provided function
        can be inlined.
Note 2: This function collects the following information about
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

    list_t *src_floops, *dst_floops;
    if (!map_get(&lctx->lmap, f->f_id, (void**)&src_floops)) return 0;
    if (!map_get(&lctx->lmap, pos->pfunc->f_id, (void**)&dst_floops)) return 0;

    _collect_information(f, pos, hpos, src_floops, dst_floops, &iinfo, smt);
    return checker((int*)&iinfo, (int)sizeof(inline_candidate_info_t));
}


static int _inline_heuristic_desider(int* data, int size) {
    if (!data || size != sizeof(inline_candidate_info_t)) return 0;
    inline_candidate_info_t* parsed = (inline_candidate_info_t*)data;
    int score = 0;
    if (parsed->src_info.bb_size <= 2)       score += 4;
    else if (parsed->src_info.bb_size <= 5)  score += 3;
    else if (parsed->src_info.bb_size <= 10) score += 2;
    else if (parsed->src_info.bb_size > 15)  score -= 3;
    score -= parsed->src_info.loop_count * (parsed->src_info.loop_nested + 1) * 2; /* If we have a loop in the source function  */
    score += parsed->dst_info.loop_nested * parsed->dst_info.loop_nested;          /* If we're in a loop at the destination pos */
    return score >= 3;
}

static int _inline_model_desider(int* data, int size) {
    if (!data || size != sizeof(inline_candidate_info_t)) return 0;
    inline_candidate_info_t* parsed = (inline_candidate_info_t*)data;

    double x[INLINE_FEATURE_COUNT] = { 0 };
    x[INLINE_FEATURE_SITE_CALLEE_IN_SAME_FILE]             = parsed->general_info.in_same_file;
    x[INLINE_FEATURE_SITE_CALLEE_IN_SAME_DIR]              = parsed->general_info.in_same_dir;
    x[INLINE_FEATURE_CALLEE_SIGNATURE_HAS_INLINE_MODIFIER] = parsed->src_info.has_inline_mod;
    x[INLINE_FEATURE_CALLEE_AST_HAS_SIDE_EFFECTS]          = parsed->src_info.has_sideeff;
    x[INLINE_FEATURE_CALLEE_INFO_IR_COUNT]                 = parsed->src_info.hir_size;
    x[INLINE_FEATURE_CALLEE_INFO_FUNCCALLS]                = parsed->src_info.funccals;
    x[INLINE_FEATURE_CALLEE_AST_BRANCH_COUNT]              = parsed->src_info.ifs_count;
    x[INLINE_FEATURE_CALLEE_BODY_RETURN_COUNT]             = parsed->src_info.returns_count;
    x[INLINE_FEATURE_CALLEE_SIGNATURE_PARAM_COUNT]         = parsed->src_info.params_count;
    x[INLINE_FEATURE_CALLEE_SIGNATURE_RETURNS_VOID]        = parsed->src_info.is_void_ret;
    x[INLINE_FEATURE_CALLER_CALLSITE_ARG_COUNT]            = parsed->dst_info.params_count;
    x[INLINE_FEATURE_CALLER_INSTRUCTION_INFO_IS_DOM]       = parsed->dst_info.is_dom;

    return inline_model_predict(x);
}

int HIR_FUNC_perform_inline(cfg_ctx_t* cctx, ltree_ctx_t* lctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        /* Collect information about the environment
           - Basic information about the loops */
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_hir_instructions (bb) {
                if (HIR_is_funccall(hh->op)) {
                    func_info_t trg_fi;
                    if (!FNTB_get_info_id(hh->sarg->storage.str.s_id, &trg_fi, &smt->f)) continue;
                    cfg_func_t* trg;
                    if (!map_get(&cctx->fmap, hh->sarg->storage.str.s_id, (void**)&trg)) continue;

                    if (
                        trg_fi.flags.inln == ALWAYS_INLINE ||
                        (
                            trg_fi.flags.inln != NEVER_INLINE &&
                            _inline_candidate(
                                trg, bb, hh, lctx, smt, 
                                trg_fi.flags.inln == MODEL_INLINE ? _inline_model_desider : _inline_heuristic_desider
                            ) && fb != trg
                        )
                    ) {
                        hir_subject_t* res = NULL;
                        if (
                            hh->op == HIR_STORE_FCLL || 
                            hh->op == HIR_STORE_ECLL
                        ) res = hh->farg;
                        _inline_arguments(trg, &hh->targ->storage.list.h, hh);
                        _inline_function(trg, res, hh);
                        hh->unused = 1;
                    }
                }
            }
        }
    }

    return 1;
}
