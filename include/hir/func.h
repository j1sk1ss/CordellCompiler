#ifndef FUNC_H_
#define FUNC_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <symtab/functb.h>

typedef struct {
    long  fid;
    set_t edges;
} call_graph_node_t;

typedef struct {
    map_t verts;
} call_graph_t;

int HIR_CG_build(cfg_ctx_t* cctx, call_graph_t* ctx, sym_table_t* smt);
int HIR_CG_unload(call_graph_t* ctx);

#endif