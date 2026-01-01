#include <sem/ast/ast_visitor.h>

ast_visitor_t* ASTVIS_create_visitor(unsigned int trg, int (*perform)(ast_node_t*)) {
    ast_visitor_t* v = (ast_visitor_t*)mm_malloc(sizeof(ast_visitor_t));
    if (!v) return NULL;
    v->trg     = trg;
    v->perform = perform;
    return v;
}

int ASTVIS_unload_visitor(ast_visitor_t* v) {
    if (!v) return 0;
    return mm_free(v);
}
