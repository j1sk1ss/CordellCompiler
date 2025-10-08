#ifndef DAG_HELPER_H_
#define DAG_HELPER_H_
#include "hir_helper.h"
#include <hir/hir_types.h>
#include <hir/dag.h>
static void dump_dag_dot(dag_ctx_t* ctx, sym_table_t* smt) {
    printf("digraph DAG {\n");
    printf("  rankdir=TB;\n");
    printf("  node [shape=box, style=filled, fillcolor=lightgray];\n\n");

    map_iter_t it;
    map_iter_init(&ctx->dag, &it);

    dag_node_t* node;
    while (map_iter_next(&it, (void**)&node)) {
        char buff[128];
        sprintf_hir_subject(buff, node->src, smt);
        const char* opname = hir_op_to_string(node->op);
        printf("  lb%i [label=\"%s\\n%s\"];\n", node->id, opname, buff);
    }

    printf("\n");
    map_iter_init(&ctx->dag, &it);
    while (map_iter_next(&it, (void**)&node)) {
        if (node->farg) printf("  lb%i -> lb%i [label=\"farg\"];\n", node->id, node->farg->id);
        if (node->sarg) printf("  lb%i -> lb%i [label=\"sarg\"];\n", node->id, node->sarg->id);

        set_iter_t sit;
        dag_node_t* user;
        set_iter_init(&node->users, &sit);
        while (set_iter_next(&sit, (void**)&user)) {
            printf("  lb%i -> lb%i [style=dashed, color=gray, label=\"user\"];\n", node->id, user->id);
        }
    }

    printf("}\n");
}
#endif