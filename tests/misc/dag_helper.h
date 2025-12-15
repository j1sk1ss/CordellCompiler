#ifndef DAG_HELPER_H_
#define DAG_HELPER_H_
#include "hir_helper.h"
#include <hir/hir_types.h>
#include <hir/dag.h>
static void dump_dag_dot(dag_ctx_t* ctx, sym_table_t* smt) {
    printf("==== DAG DUMP ====\n");
    printf("digraph DAG {\n");
    printf("  rankdir=TB;\n");
    printf("  node [shape=box, style=filled, fillcolor=lightgray];\n\n");

    map_foreach (dag_node_t* node, &ctx->dag) {
        char buff[128] = { 0 };
        char* bptr = (char*)buff;
        bptr += sprintf(bptr, "dupl: %i, base: ", set_size(&node->link));

        sprintf_hir_subject(bptr, node->src, smt);
        const char* opname = hir_op_to_string(node->op);
        printf("  lb%li [label=\"%s \\n %s \\n %lu\"];\n", node->id, opname, buff, node->hash);
    }

    printf("\n");
    map_foreach (dag_node_t* node, &ctx->dag) {
        set_iter_t sit;
        set_iter_init(&node->args, &sit);
        dag_node_t* arg;
        while (set_iter_next(&sit, (void**)&arg)) {
            printf("  lb%li -> lb%li [label=\"farg\"];\n", node->id, arg->id);
        }

        dag_node_t* user;
        set_iter_init(&node->users, &sit);
        while (set_iter_next(&sit, (void**)&user)) {
            printf("  lb%li -> lb%li [style=dashed, color=gray, label=\"user\"];\n", node->id, user->id);
        }
    }

    printf("}\n");
    printf("==================\n");
}
#endif