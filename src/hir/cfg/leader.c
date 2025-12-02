/*
leader.c - Mark all leaders (according to DragonBook rules).
Rule 1 - First instruction in function
Rule 2 - Target of JMP instruction
Rule 3 - Next instruction after JMP instruction
*/

#include <hir/cfg.h>

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx) {
    foreach(cfg_func_t* fb, &ctx->funcs) {
        hir_block_t* h = fb->entry;
        set_init(&fb->leaders, SET_NO_CMP);
        set_add(&fb->leaders, h);
        
        while (h) {
            if (h->op == HIR_MKLB)                set_add(&fb->leaders, h);
            else if (HIR_isjmp(h->op) && h->next) set_add(&fb->leaders, h->next);
            if (h == fb->exit) break;
            h = h->next;
        }
    }

    return 1;
}
