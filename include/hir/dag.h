#ifndef DAG_H_
#define DAG_H_

#include <std/mm.h>
#include <std/mem.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <std/qsort.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>

typedef struct dag_node {
    /* Misc info */
    cfg_block_t*     home;
    unsigned long    hash;
    long             id;
    hir_operation_t  op;

    /* Connection with HIR */
    hir_subject_t*   src;
    set_t            link;

    /* Connection with other nodes */
    set_t            args;
    set_t            users;
} dag_node_t;

typedef struct {
    long  curr_id;
    map_t dag;
    map_t groups;
} dag_ctx_t;

/*
Generate DAG's node hash.
Note: This function wall take care about DAG's node arguments.
Params
    - `nd` - DAG node.

Returns hash of the node.
*/
unsigned long HIR_DAG_compute_hash(dag_node_t* nd);

/*
Get DAG node from the context or create a new one.
Params:
    - `ctx` - DAG context.
    - `src` - HIR subject.
    - `create` - Create a new one DAG node.
                 Note: If it is a 1 - Will create a new node when 
                       it doesn't find anything in the DAG context.

Return a DAG node from the context or a new DAG node.
*/
dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create);

/*
Unload the DAG node.
Params:
    - `nd` - DAG node.

Returns 1 if the DAG node was freed.
*/
int HIR_DAG_unload_node(dag_node_t* nd);

/*
Unload the DAG context.
Params:
    - `ctx` - DAG context.

Returns 1 if the DAG context was freed.
*/
int HIR_DAG_unload(dag_ctx_t* ctx);

#define DAG_GET_NODE(ctx, u)     HIR_DAG_get_node(ctx, u, 1)
#define DAG_ACQUIRE_NODE(ctx, u) HIR_DAG_get_node(ctx, u, 0)

int HIR_DAG_init(dag_ctx_t* dctx);;
int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx, sym_table_t* smt);
int HIR_DAG_CFG_rebuild(cfg_ctx_t* cctx, dag_ctx_t* dctx);

#endif