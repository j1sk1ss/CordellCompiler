#ifndef INSTPLAN_H_
#define INSTPLAN_H_

#include <std/set.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/instplan/targinfo.h>

typedef struct {
    set_t        vert;
    lir_block_t* b;
} instructions_dag_node_t;

typedef struct {
    list_t edges;
} instructions_dag_t;

int LIR_build_instructions_dag(cfg_block_t* bb, instructions_dag_t* dag);

typedef struct {
    int (*plan_instructions)(cfg_block_t*, instructions_dag_t*);
} inst_planner_t;

int LIR_plan_instructions(cfg_ctx_t* cctx, inst_planner_t* planner);

#endif