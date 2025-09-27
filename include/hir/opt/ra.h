#ifndef RA_H_
#define RA_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <symtab/symtab.h>
#include <hir/opt/cfg.h>

typedef struct igraph_node {
    long                v_id;
    set_t               v;
    struct igraph_node* next;
} igraph_node_t;

typedef struct {
    igraph_node_t* h;
} igraph_t;

int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt);

#endif