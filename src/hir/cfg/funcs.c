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
static cfg_func_t* _create_funcblock(hir_block_t* entry) {
    cfg_func_t* b = (cfg_func_t*)mm_malloc(sizeof(cfg_func_t));
    if (!b) return NULL;
    str_memset(b, 0, sizeof(cfg_func_t));
    b->hmap.entry = entry;
    b->id         = entry->farg->storage.str.s_id;
    list_init(&b->blocks);
    set_init(&b->locals, SET_NO_CMP);
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
static cfg_func_t* _add_funcblock(hir_block_t* entry, cfg_ctx_t* ctx, sym_table_t* smt) {
    func_info_t fi;
    if (!FNTB_get_info_id(entry->farg->storage.str.s_id, &fi, &smt->f)) return 0;
    cfg_func_t* b = _create_funcblock(entry);
    if (!b) return 0;
    b->id     = ctx->cid++;
    b->fid    = entry->farg->storage.str.s_id;
    b->fentry = fi.flags.entry;
    list_add(&ctx->funcs, b);
    return b;
}

static inline hir_block_t* _check_is_local(hir_block_t* curr, set_t* locals) {
    if (!set_size(locals)) return NULL;
    set_foreach (cfg_func_t* fb, locals) {
        if (curr == fb->hmap.entry) return fb->hmap.exit;
    }

    return NULL;
}

hir_block_t* HIR_FUNC_get_next(hir_block_t* curr, cfg_func_t* fb, hir_block_t* opt_exit, int skip) {
    if (!skip) return fb->hmap.entry;
    if (
        curr == fb->hmap.exit ||
        (opt_exit && curr == opt_exit)
    ) return NULL;
    while (curr) {
        hir_block_t* nstart = _check_is_local(curr, &fb->locals);
        if (nstart) {
            curr = nstart->next;
            continue;
        }

        if (
            curr == fb->hmap.exit          ||
            (opt_exit && curr == opt_exit) ||
            (!curr->unused && skip-- <= 0)
        ) break;
        curr = curr->next;
    }

    return curr;
}

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt) {
    sstack_t entries;
    stack_init(&entries);

    hir_block_t* h = hctx->h;
    while (h) {
        switch (h->op) {
            case HIR_FDCL:
            case HIR_STRT: {
                cfg_func_t* fb = _add_funcblock(h, ctx, smt);
                stack_push(&entries, fb);
                break;
            }
            case HIR_FEND:
            case HIR_STEND: {
                cfg_func_t* fb = NULL, *p;
                if (!stack_pop(&entries, (void**)&fb)) break;
                fb->hmap.exit = h;
                if (stack_top(&entries, (void**)&p)) {
                    set_add(&p->locals, fb);
                }

                goto _handled_instruction;
            }
            default: break;
        }

        if (!stack_top(&entries, NULL)) list_add(&ctx->out, h);
_handled_instruction: {}
        h = h->next;
    }

    stack_free(&entries);
    return 1;
}
