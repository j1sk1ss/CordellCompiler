#ifndef RA_H_
#define RA_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/list.h>
#include <symtab/symtab.h>
#include <hir/opt/cfg.h>

typedef struct igraph_node {
    long  v_id;
    int   color;
    set_t v;
} igraph_node_t;

typedef struct {
    list_t nodes;
} igraph_t;

int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt);
int HIR_RA_unload_igraph(igraph_t* g);
int HIR_RA_color_igraph(igraph_t* g, int max_colors);

#endif