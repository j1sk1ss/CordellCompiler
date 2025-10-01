#ifndef REGISTER_ALLOCATION_H_
#define REGISTER_ALLOCATION_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <symtab/symtab.h>
#include <hir/opt/cfg.h>

typedef struct igraph_node {
    char  used;  /* deall.c field. Used for track current slot user */
    long  v_id;  /* v_id - Link to smt.v variable                   */
    int   color; /* gc.c field. Used for graph coloring             */
    set_t v;     /* Neighbour vertexies                             */
} igraph_node_t;

typedef struct {
    list_t nodes;
} igraph_t;

int HIR_RA_create_deall(cfg_ctx_t* cctx, igraph_t* g, map_t* colors);
igraph_node_t* HIR_RA_find_ig_node(igraph_t* g, long v_id);
int HIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt);
int HIR_RA_unload_igraph(igraph_t* g);
int HIR_RA_color_igraph(igraph_t* g, map_t* colors);

#endif