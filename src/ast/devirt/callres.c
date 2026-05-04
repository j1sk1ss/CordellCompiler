#include <ast/devirt.h>

static int _find_and_register_resolved_call(ast_node_t* node, sym_table_t* smt) {
    if (!node) return 0;
    _find_and_register_resolved_call(node->siblings.n, smt);
    _find_and_register_resolved_call(node->c, smt);
    if (!node->t) return 0;
    if (
        node->t->t_type == CALLING_TOKEN      &&
        node->c->t->t_type == FUNC_NAME_TOKEN &&
        node->c->c
    ) node->c->sinfo.v_id = FNTB_create_resolved_copy(node->c->sinfo.v_id, node->c->c->t->t_type, &smt->f);
    return 1;
}

int AST_resolve_calls(ast_ctx_t* sctx, sym_table_t* smt) {
    return _find_and_register_resolved_call(sctx->r, smt);
}
