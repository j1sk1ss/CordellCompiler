#include <csa/ast/ast_visitor.h>

ast_visitor_t* ASTVIS_create_visitor(unsigned int trg, int (*perform)(AST_VISITOR_ARGS)) {
    ast_visitor_t* v = (ast_visitor_t*)mm_malloc(sizeof(ast_visitor_t));
    if (!v) return NULL;
    v->trg     = trg;
    v->perform = perform;
    return v;
}
// TODO: Check if in a code = in if is present:
/*
```cpl
if a = 100; { <- Maybe this is an error!
}
```
*/
int ASTVIS_unload_visitor(ast_visitor_t* v) {
    if (!v) return 0;
    return mm_free(v);
}
