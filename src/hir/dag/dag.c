#include <hir/dag.h>

dag_node_t* HIR_DAG_create_node(hir_subject_t* src) {
    dag_node_t* nd = (dag_node_t*)mm_malloc(sizeof(dag_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(dag_node_t));
    set_init(&nd->users);
    set_init(&nd->args);
    nd->op  = -1;
    nd->src = src;
    return nd;
}

dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create) {
    dag_node_t* nd;
    if (map_get(&ctx->dag, HIR_hash_subject(src), (void**)&nd)) return nd;
    if (create && src) {
        nd = HIR_DAG_create_node(src);
        nd->id = ctx->curr_id++;
        map_put(&ctx->dag, HIR_hash_subject(src), nd);
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
