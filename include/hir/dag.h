#ifndef DAG_H_
#define DAG_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <hir/hir.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>

typedef struct dag_node {
    unsigned long    hash;
    long             id;
    hir_operation_t  op;
    hir_subject_t*   src;
    set_t            link;
    set_t            args;
    set_t            users;
} dag_node_t;

typedef struct {
    long  curr_id;
    map_t dag;
    map_t groups;
} dag_ctx_t;

unsigned long HIR_DAG_compute_hash(dag_node_t* nd);
dag_node_t* HIR_DAG_create_node(hir_subject_t* src);
dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create);
int HIR_DAG_unload_node(dag_node_t* nd);
int HIR_DAG_unload(dag_ctx_t* ctx);

#define DAG_GET_NODE(ctx, u) HIR_DAG_get_node(ctx, u, 1)

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx);

#endif