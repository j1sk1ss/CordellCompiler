#include <hir/dump.h>

static void _save_hir_subject(hir_subject_t* s, dump_ctx_t* ctx) {
    write(ctx->fd, s, sizeof(hir_subject_t));
}

static int _save_hir_block(hir_block_t* hb, dump_ctx_t* ctx) {
    write(ctx->fd, &hb->op, sizeof(int));

    int args_count = 0;
    hir_subject_t* args[] = { hb->farg, hb->sarg, hb->targ };
    for (int i = 0; i < 3; i++) {
        if (args[i]) args_count++;
    }

    write(ctx->fd, &args_count, sizeof(int));
    for (int i = 0; i < 3; i++) {
        if (args[i]) _save_hir_subject(args[i], ctx);
    }

    return 1;
}

static int _save_bb(cfg_block_t* bb, dump_ctx_t* ctx) {
    int blocks_count = 0;
    hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
    while (hh) {
        if (!hh->unused) blocks_count++;
        hh = HIR_get_next(hh, bb->hmap.exit, 1);
    }

    write(ctx->fd, &bb->id, sizeof(long));
    write(ctx->fd, &bb->type, sizeof(int));

    long no_link = -1;
    if (bb->jmp) write(ctx->fd, &bb->jmp->id, sizeof(long));
    else write(ctx->fd, &no_link, sizeof(long));
    if (bb->l) write(ctx->fd, &bb->l->id, sizeof(long));
    else write(ctx->fd, &no_link, sizeof(long));

    write(ctx->fd, &blocks_count, sizeof(int));
    while (hh) {
        if (!hh->unused) _save_hir_block(hh, ctx);
        hh = HIR_get_next(hh, bb->hmap.exit, 1);
    }

    return 0;
}

static int _save_fb(cfg_func_t* fb, dump_ctx_t* ctx) {
    write(ctx->fd, &fb->id, sizeof(long));
    int is_entry = fb->fentry;
    write(ctx->fd, &is_entry, sizeof(int));
    int blocks_count = list_size(&fb->blocks);
    write(ctx->fd, &blocks_count, sizeof(int));
    write(ctx->fd, &fb->fid, sizeof(long));
    foreach (cfg_block_t* bb, &fb->blocks) {
        _save_bb(bb, ctx);
    }
    
    return 0;
}

int HIR_save(cfg_ctx_t* cctx, dump_ctx_t* ctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        _save_fb(fb, ctx);
    }

    return 1;
}
