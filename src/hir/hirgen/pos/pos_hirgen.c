#include <hir/hirgens/hirgens.h>

static inline int _compare_pos_to_pos(file_position_t* a, file_position_t* b) {
    if (!a || !b) return 0;
    return a->line == b->line && a->file && b->file && a->file->equals(a->file, b->file);
}

int HIR_generate_position(file_position_t* pos, hir_ctx_t* ctx) {
    if (_compare_pos_to_pos(&ctx->pos, pos)) return 1;
    HIR_BLOCK1(ctx, HIR_SETPOS, HIR_SUBJ_LOCATION(pos));
    str_memcpy(&ctx->pos, pos, sizeof(file_position_t));
    return 1;
}
