#ifndef REGISTER_ALLOCATION_H_
#define REGISTER_ALLOCATION_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>

typedef struct igraph_node {
    symbol_id_t v_id;     /* v_id - Link to smt.v variable                   */
    int         color;    /* gc.c field. Used for graph coloring             */
    set_t       v;        /* Neighbour vertexies                             */
} igraph_node_t;

typedef struct {
    map_t nodes;
} igraph_t;

/* 
Put all avaliable variable into the colored map with the initial value -1.
Note: If a variable already has a register, it will preserve it for further
      color graph solution.
Note 2: By default, all variables !must! have -1 in the register field!
Params:
    - `colors` - Colored map.
    - `smt` - Symtable.

Returns 1 if it succeeds.
*/
int LIR_RA_init_colors(map_t* colors, sym_table_t* smt);

/*
Find a node from the interference graph.
Params:
    - `g` - Graph itself.
    - `v_id` - Variable to find in the graph.

Returns a related to the variable_id node from the graph.
*/
igraph_node_t* LIR_RA_find_ig_node(igraph_t* g, symbol_id_t v_id);

/*
Build the interference graph.
Params:
    - `cctx` - CFG context.
    - `g` - Graph itself.
    - `smt` - Symtable.

Returns 1 if succeeds. Otherwise will return 0.
*/
int LIR_RA_build_igraph(cfg_ctx_t* cctx, igraph_t* g, sym_table_t* smt);

/*
Unload the provided interference graph.
Params:
    - `g` - Interference graph.

Returns 1 if succeeds.
*/
int LIR_RA_unload_igraph(igraph_t* g);

/*
Solve the graph coloring problem and generate the colors map.
Params:
    - `g` - Interference graph.
    - `colors` - Output colors map.

Returns 1 if succeeds.
*/
int LIR_RA_color_igraph(igraph_t* g, map_t* colors);

#endif