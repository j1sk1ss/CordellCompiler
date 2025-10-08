#include <hir/dag.h>

dag_node_t* HIR_DAG_create_node(hir_subject_t* src, dag_node_t* farg, dag_node_t* sarg) {
    dag_node_t* nd = (dag_node_t*)mm_malloc(sizeof(dag_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(dag_node_t));
    set_init(&nd->users);
    nd->farg = farg;
    nd->sarg = sarg;
    nd->src  = src;
    return nd;
}

int HIR_DAG_insert_node(dag_ctx_t* ctx, hir_operation_t op, hir_subject_t* src, dag_node_t* nd) {
    if (!src) return 0;
    nd->id  = ctx->curr_id++;
    nd->op  = op;
    nd->src = src;
    return map_put(&ctx->dag, (long)nd->src, nd);
}

dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create) {
    dag_node_t* nd;
    if (map_get(&ctx->dag, (long)src, (void**)&nd)) return nd;
    if (create) {
        nd = HIR_DAG_create_node(src, NULL, NULL);
        map_put(&ctx->dag, (long)src, nd);
        return nd;
    }

    return NULL;
}

int HIR_DAG_unload(dag_ctx_t* ctx) {
    map_iter_t it;
    dag_node_t* nd;
    map_iter_init(&ctx->dag, &it);
    while (map_iter_next(&it, (void**)&nd)) {
        set_free(&nd->users);
        mm_free(nd);
    }

    return map_free(&ctx->dag);
}
