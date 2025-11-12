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

lir_block_t* LIR_planner_get_next_func_abi(lir_block_t* entry, lir_block_t* exit, int offset);
lir_block_t* LIR_planner_get_next_sysc_abi(lir_block_t* entry, lir_block_t* exit, int offset);
lir_block_t* LIR_planner_get_func_res(lir_block_t* fn, lir_block_t* exit);
int LIR_plan_instructions(cfg_ctx_t* cctx, target_info_t* trginfo);

#endif