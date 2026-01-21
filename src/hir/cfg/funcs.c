/* funcs.c - Split input HIR instructions by function blocks. */
#include <hir/cfg.h>

cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid) {
    foreach (cfg_block_t* cb, &f->blocks) {
        if (
            cb->hmap.entry->op == HIR_MKLB && 
            cb->hmap.entry->farg->id == lbid
        ) return cb;
    }

    return NULL;
}

/*
Create a function block.
Params:
    - `entry` - Function's entry HIR block.
    - `end` - Functions's exit HIR block.

Return NULL or pointer to the function block.
*/
static cfg_func_t* _create_funcblock(hir_block_t* entry, hir_block_t* end) {
    cfg_func_t* b = (cfg_func_t*)mm_malloc(sizeof(cfg_func_t));
    if (!b) return NULL;
    str_memset(b, 0, sizeof(cfg_func_t));
    b->hmap.entry = entry;
    b->hmap.exit  = end;
    b->id    = entry->farg->storage.str.s_id;
    list_init(&b->blocks);
    return b;
}

/*
Create and append a function block to the functions list.
Note: Will check if function is presented in the symtable.
Params:
    - `entry` - Function's entry HIR block.
    - `end` - Functions's exit HIR block.
    - `ctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeed. Otherwise will return 0.
*/
static int _add_funcblock(hir_block_t* entry, hir_block_t* end, cfg_ctx_t* ctx, sym_table_t* smt) {
    func_info_t fi;
    if (!FNTB_get_info_id(entry->farg->storage.str.s_id, &fi, &smt->f)) return 0;
    cfg_func_t* b = _create_funcblock(entry, end);
    if (!b) return 0;
    b->id     = ctx->cid++;
    b->fid    = entry->farg->storage.str.s_id;
    b->fentry = fi.flags.entry;
    return list_add(&ctx->funcs, b);
}

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;
    hir_block_t* fentry = NULL;
    while (h) {
        switch (h->op) {
            case HIR_FDCL:
            case HIR_STRT: fentry = h; break;
            case HIR_FEND:
            case HIR_STEND: {
                if (!fentry) break;
                _add_funcblock(fentry, h, ctx, smt);
                fentry = NULL;
                break;
            }
            default: break;
        }

        h = h->next;
    }

    return 1;
}
