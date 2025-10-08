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
    long             id;
    hir_operation_t  op;
    struct dag_node* farg;
    struct dag_node* sarg;
    hir_subject_t*   src;
    set_t            users;
} dag_node_t;

typedef struct {
    long  curr_id;
    map_t dag;
} dag_ctx_t;

dag_node_t* HIR_DAG_create_node(hir_subject_t* src, dag_node_t* farg, dag_node_t* sarg);
int HIR_DAG_insert_node(dag_ctx_t* ctx, hir_operation_t op, hir_subject_t* src, dag_node_t* nd);
dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create);
int HIR_DAG_unload(dag_ctx_t* ctx);

#define DAG_SUBJ_ARG0() HIR_DAG_create_node(NULL, NULL, NULL)
#define DAG_SUBJ_ARG1(a) HIR_DAG_create_node(NULL, a, NULL)
#define DAG_SUBJ_ARG2(a, b) HIR_DAG_create_node(NULL, a, b)
#define DAG_GET_NODE(ctx, u) HIR_DAG_get_node(ctx, u, 1)
#define DAG_NODE(ctx, op, src, nd) HIR_DAG_insert_node(ctx, op, src, nd)

int HIR_DAG_generate(cfg_ctx_t* cctx, dag_ctx_t* dctx);

#endif