#include <hir/func.h>

static int _inline_arguments(cfg_func_t* f, list_t* args, hir_block_t* pos) {
    list_iter_t it;
    list_iter_hinit(args, &it);
    hir_block_t* hh = f->entry;
    while (hh) {
        if (hh->op == HIR_FARGLD) {
            hir_block_t* nblock = HIR_copy_block(hh);
            nblock->op = HIR_STORE;
            HIR_unload_subject(nblock->sarg);
            nblock->sarg = (hir_subject_t*)list_iter_next(&it);
            HIR_insert_block_before(nblock, pos);
        }

        if (hh == f->exit) break;
        hh = hh->next;
    }

    return 1;
}

static int _inline_function(cfg_func_t* f, hir_subject_t* res, hir_block_t* pos) {
    hir_block_t* hh = f->entry;
    while (hh && hh->op != HIR_MKSCOPE) hh = hh->next;
    hh = hh->next;
    while (hh && hh->op != HIR_MKSCOPE) hh = hh->next;

    while (hh && hh->op != HIR_FEND) {
        if (!hh->unused) {
            hir_block_t* nblock = HIR_copy_block(hh);
            switch (hh->op) {
                case HIR_FRET: {
                    if (!res) nblock->unused = 1;
                    else {
                        nblock->op   = HIR_STORE;
                        nblock->sarg = hh->farg;
                        HIR_unload_subject(nblock->farg);
                        nblock->farg = res;
                    }

                    break;
                }
                default: break;
            }

            HIR_insert_block_before(nblock, pos);
        }

        if (hh == f->exit) break;
        hh = hh->next;
    }

    return 1;
}

static cfg_func_t* _get_funcblock(cfg_ctx_t* cctx, long fid) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        if (fb->fid == fid) return fb;
    }

    return NULL;
}

static int _inline_candidate(cfg_func_t* f, cfg_block_t* pos) {
    if (!f) return 0;
    int score = 0;
    if (
        pos->type == CFG_LOOP_BLOCK ||
        pos->type == CFG_LOOP_LATCH
    ) score += 2;

    int block_count = list_size(&f->blocks);
    if (block_count <= 2)       score += 3;
    else if (block_count <= 5)  score += 2;
    else if (block_count <= 10) score += 1;
    else if (block_count > 15)  score -= 3;
    return score >= 3;
}

int HIR_FUNC_perform_inline(cfg_ctx_t* cctx) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        foreach(cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = bb->hmap.entry;
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

                if (hh == bb->hmap.exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}
