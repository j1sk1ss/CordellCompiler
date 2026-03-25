#ifndef HIR_VISITOR_H_
#define HIR_VISITOR_H_

#include <std/mm.h>
#include <std/str.h>
#include <sem/hir/hir_data.h>
#include <hir/hir_types.h>
#include <hir/hir.h>
#include <hir/hirgen.h>

typedef struct {
    unsigned long trg;
    int           (*perform)(HIR_VISITOR_ARGS);
} hir_visitor_t;

hir_visitor_t* HIRVIS_create_visitor(unsigned long trg, int (*perform)(HIR_VISITOR_ARGS));
int HIRVIS_unload_visitor(hir_visitor_t* v);

#endif
