#include <optimization.h>

static int _recalc_offs(ast_node_t* r, const char* func, syntax_ctx_t* ctx) {
    if (!r) return 0;
    for (ast_node_t* t = r->child; t; t = t->sibling) {
        if (!t->token) {
            _recalc_offs(t, func, ctx);
            continue;
        }

        t->info.offset = 0;
        switch (t->token->t_type) {
            case FUNC_TOKEN:
                int tmp_off = ctx->vars->offset;
                ctx->vars->offset = 0;
                _recalc_offs(t, (char*)t->child->token->value, ctx);
                ctx->vars->offset = tmp_off;
            break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN:
            case INT_VARIABLE_TOKEN:
            case CHAR_VARIABLE_TOKEN:
            case LONG_VARIABLE_TOKEN:
            case SHORT_VARIABLE_TOKEN:
                if (VRS_intext(t->token)) {
                    variable_info_t info;
                    if (VRM_get_info((char*)t->token->value, func, &info, ctx->vars)) t->info.offset = info.offset;
                    else t->info.offset = VRM_add_info((char*)t->token->value, t->info.size, func, ctx->vars);
                }

                _recalc_offs(t, func, ctx);
            break;
            default: _recalc_offs(t, func, ctx); break;
        }
    }

    return 1;
}

int OPT_offrecalc(syntax_ctx_t* ctx) {
    if (!ctx || !ctx->r) return 0;
    VRM_destroy_ctx(ctx->vars);
    _recalc_offs(ctx->r, NULL, ctx);
    return 1;
}
