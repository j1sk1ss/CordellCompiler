#include <ast/astgens/astgens.h>

int var_lookup(ast_node_t* node, ast_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    var_lookup(node->sibling, ctx, smt);
    var_lookup(node->child, ctx, smt);
    if (!node->token) return 0;

    if (TKN_isvariable(node->token)) {
        variable_info_t varinfo = { .type = UNKNOWN_NUMERIC_TOKEN };
        for (int s = ctx->scopes.stack.top; s >= 0; s--) {
            short s_id = (short)ctx->scopes.stack.data[s].d;
            if (VRTB_get_info(node->token->body, s_id, &varinfo, &smt->v)) {
                node->sinfo.v_id        = varinfo.v_id;
                node->sinfo.s_id        = varinfo.s_id;
                node->token->flags.heap = varinfo.heap;
                return 1;
            }
        }
    }

    if (node->token->t_type == STRING_VALUE_TOKEN) {
        str_info_t strinfo;
        if (STTB_get_info(node->token->body, &strinfo, &smt->s)) {
            node->sinfo.v_id = strinfo.id;
            return 1;
        }
        else {
            node->sinfo.v_id = STTB_add_info(node->token->body, STR_INDEPENDENT, &smt->s);
            return 1;
        }
    }

    return 0;
}
