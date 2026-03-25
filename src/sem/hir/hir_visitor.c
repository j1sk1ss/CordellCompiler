#include <sem/hir/hir_visitor.h>

hir_visitor_t* HIRVIS_create_visitor(unsigned long trg, int (*perform)(HIR_VISITOR_ARGS)) {
    hir_visitor_t* v = (hir_visitor_t*)mm_malloc(sizeof(hir_visitor_t));
    if (!v) return NULL;
    v->trg     = trg;
    v->perform = perform;
    return v;
}

int HIRVIS_unload_visitor(hir_visitor_t* v) {
    if (!v) return 0;
    return mm_free(v);
}
