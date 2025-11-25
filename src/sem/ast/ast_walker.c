#include <sem/ast/ast_walker.h>

int ASTWLK_register_visitor(token_type_t trg, int (*perform)(ast_node_t*), ast_walker_t* ctx) {
    ast_visitor_t* v = ASTVIS_create_visitor(trg, perform);
    if (!v) return 0;
    return list_add(&ctx->visitors, v);
}

static int _ast_walk(ast_node_t* nd, ast_walker_t* ctx) {
    if (!nd) return 0;
    _ast_walk(nd->child, ctx);
    _ast_walk(nd->sibling, ctx);
    
    list_iter_t it;
    ast_visitor_t* v;
    list_iter_hinit(&ctx->visitors, &it);
    while ((v = (ast_visitor_t*)list_iter_next(&it))) {
        v->perform(nd);
    }

    return 1;
}

int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx) {
    return _ast_walk(actx->r, ctx);
}

int ASTWLK_unload_ctx(ast_walker_t* ctx) {
    list_free_force(&ctx->visitors);
    return mm_free(ctx);
}
