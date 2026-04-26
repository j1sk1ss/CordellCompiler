#include <hir/loop.h>

static int _count_commands(cfg_block_t* bb) {
    int res = 0;
    hir_block_t* hb = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
    while (hb) {
        switch (hb->op) {
            case HIR_IFOP2:
            case HIR_JMP:
            case HIR_MKLB:
            case HIR_MKSCOPE:
            case HIR_ENDSCOPE:
            case HIR_STASM:
            case HIR_ENDASM:
            case HIR_SETPOS:
            case HIR_NOP:
            case HIR_VRDEALL:
            case HIR_PHI:
            case HIR_PHI_PREAMBLE: break;
            default: res++;
        }

        hb = HIR_get_next(hb, bb->hmap.exit, 1);
    }

    return res;
}

static int _mark_loop_dead(loop_node_t* root) {
    foreach (loop_node_t* ch, &root->children) {
        _mark_loop_dead(ch);
    }

    int loop_content = 0;
    set_foreach (cfg_block_t* bb, &root->blocks) {
        loop_content += _count_commands(bb);
    }

    if (!loop_content) {
        set_foreach (cfg_block_t* bb, &root->blocks) {
            hir_block_t* hb = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hb) {
                hb->unused = 1;
                hb = HIR_get_next(hb, bb->hmap.exit, 1);
            }
        }
    }

    return 1;
}

int HIR_LOOP_perform_dle(ltree_ctx_t* lctx) {
    map_foreach (list_t* loops, &lctx->lmap) {
        foreach (loop_node_t* loop, loops) {
            _mark_loop_dead(loop);
        }
    }

    return 1;
}
