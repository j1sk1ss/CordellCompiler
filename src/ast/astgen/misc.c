#include <ast/astgen/astgen.h>

int annotation_reserve(ast_ctx_t* ctx) {
    int prev_off = ctx->an_off;
    ctx->an_off  = ctx->annots.top + 1;
    return prev_off;
}

int annotation_unreserve(ast_ctx_t* ctx, int off) {
    ctx->an_off = off;
    return 1;
}

symbol_id_t type_lookup(token_t* t, ast_ctx_t* ctx, sym_table_t* smt) {
    type_info_t ti;
    for (int s = ctx->scopes.stack.top; s >= 0; s--) {
        if (TPTB_get_info(t->body, ctx->scopes.stack.data[s].d, &ti, &smt->t)) return ti.id;
    }

    return NO_SYMBOL_ID;
}

int var_lookup(ast_node_t* node, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    int found = var_lookup(node->siblings.n, ctx, smt);
    found |= var_lookup(node->c, ctx, smt);
    if (!node->t) return 0;

    if (node->t->t_type == UNKNOWN_STRING_TOKEN) {
        variable_info_t varinfo = { .type = UNKNOWN_NUMERIC_TOKEN };
        for (int s = ctx->scopes.stack.top; s >= 0; s--) {
            if (VRTB_get_info(node->t->body, ctx->scopes.stack.data[s].d, &varinfo, &smt->v)) {
                node->sinfo.v_id    = varinfo.v_id;
                node->sinfo.s_id    = varinfo.s_id;
                node->sinfo.t_id    = varinfo.t_id;
                node->t->flags.vla  = varinfo.vfs.vla;
                node->t->flags.ptr  = varinfo.vfs.ptr;
                node->t->flags.ro   = varinfo.vfs.ro;
                node->t->flags.glob = varinfo.vfs.glob;
                node->t->t_type     = TKN_get_var_from_type(varinfo.type);
                return 1;
            }
        }

        func_info_t funcinfo;
        for (int s = ctx->scopes.stack.top; s >= 0; s--) {
            if (FNTB_get_info(node->t->body, ctx->scopes.stack.data[s].d, &funcinfo, &smt->f)) {
                node->sinfo.v_id   = funcinfo.id;
                node->sinfo.s_id   = funcinfo.s_id;
                node->t->flags.ext = funcinfo.flags.external;
                node->t->t_type    = CALL_ADDR_TOKEN;
                return 1;
            }
        }
    }
    else if (node->t->t_type == STRING_VALUE_TOKEN) {
        str_info_t strinfo;
        if (STTB_get_info(node->t->body, &strinfo, &smt->s)) {
            node->sinfo.v_id = strinfo.id;
            return 1;
        }
        else {
            node->sinfo.v_id = STTB_add_info(node->t->body, STR_INDEPENDENT, &smt->s);
            return 1;
        }
    }

    return found;
}
