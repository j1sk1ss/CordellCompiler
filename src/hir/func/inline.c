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

    hir_block_t* hh = HIR_get_next(f->hmap.entry, f->hmap.exit, 0);
    while (hh) {
        if (hh->op == HIR_FARGLD) {
            hir_block_t* nblock = HIR_copy_block(hh, 1);
            nblock->op = HIR_STORE;
            HIR_unload_subject(nblock->sarg);
            nblock->sarg = (hir_subject_t*)list_iter_next(&it);
            HIR_insert_block_before(nblock, pos);
        }

        hh = HIR_get_next(hh, f->hmap.exit, 1);
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
static int _replace_label_usage(hir_block_t* h, hir_block_t* e, hir_subject_t* old, hir_subject_t* new) {
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

        h = HIR_get_next(h, e, 1);
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
    hir_block_t* hh = HIR_get_next(f->hmap.entry, f->hmap.exit, 0);
    while (hh && hh->op != HIR_MKSCOPE) hh = HIR_get_next(hh, f->hmap.exit, 1);
    hh = HIR_get_next(hh, f->hmap.exit, 1);
    while (hh && hh->op != HIR_MKSCOPE) hh = HIR_get_next(hh, f->hmap.exit, 1);

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

        hh = HIR_get_next(hh, f->hmap.exit, 1);
    }

    /* Re-link labels */
    hh = nentry;
    while (hh) {
        if (hh->op == HIR_MKLB) {
            hir_subject_t* nlb = HIR_copy_subject(hh->farg);
            _replace_label_usage(nentry, nexit, hh->farg, nlb);
            hh->farg = nlb;
        }

        hh = HIR_get_next(hh, nexit, 1);
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
Euristic function for evaluating an inline candidate.
Note: If this function returns 1 - the provided function
      can be inlined.
Params:
    - `f` - CFG inline candidate function.
    - `pos` - Source HIR position.

Returns 1 if the provided function can be inlined.
*/
static int _inline_candidate(cfg_func_t* f, cfg_block_t* pos) {
    if (!f) return 0;
    int score = 0;
    if (
        pos->type == CFG_LOOP_BLOCK ||
        pos->type == CFG_LOOP_LATCH
    ) score += 2;

    int block_count = list_size(&f->blocks);
    if (block_count <= 2)       score += 4;
    else if (block_count <= 5)  score += 3;
    else if (block_count <= 10) score += 2;
    else if (block_count > 15)  score -= 3;
    return score >= 3;
}

int HIR_FUNC_perform_inline(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                if (HIR_funccall(hh->op)) {
                    cfg_func_t* trg = _get_funcblock(cctx, hh->sarg->storage.str.s_id);
                    if (_inline_candidate(trg, bb) && fb != trg) {
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
    }

    return 1;
}
