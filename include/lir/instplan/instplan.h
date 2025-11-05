#ifndef INSTPLAN_H_
#define INSTPLAN_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/instplan/targinfo.h>

typedef struct {
    int          indegree;
    long         critical_path;
    int          remaining_deps;
    set_t        vert;
    set_t        users;
    lir_block_t* b;
} instructions_dag_node_t;

typedef struct {
    map_t alive_edges;
} instructions_dag_t;

typedef struct {
    int (*plan_instructions)(cfg_block_t*, target_info_t*, instructions_dag_t*);
} inst_planner_t;

int LIR_plan_instructions(cfg_ctx_t* cctx, target_info_t* trginfo, inst_planner_t* planner);

#endif