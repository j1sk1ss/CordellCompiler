#include <hir/dag.h>

static unsigned long _get_dag_hash(dag_node_t* nd) {
    if (!set_size(&nd->args)) return HIR_hash_subject(nd->src);
    unsigned long h = nd->op * 1315423911UL;

    set_iter_t it;
    dag_node_t* arg;
    set_iter_init(&nd->args, &it);
    while (set_iter_next(&it, (void**)&arg)) {
        h ^= _get_dag_hash(arg) + 0x9e3779b97f4a7c15UL + (h << 6) + (h >> 2);
    }

    return h;
}

unsigned long HIR_DAG_compute_hash(dag_node_t* nd) {
    return _get_dag_hash(nd);
}

dag_node_t* HIR_DAG_create_node(hir_subject_t* src) {
    dag_node_t* nd = (dag_node_t*)mm_malloc(sizeof(dag_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(dag_node_t));

    set_init(&nd->users);
    set_init(&nd->args);
    set_init(&nd->link);

    set_add(&nd->link, (void*)HIR_hash_subject(src));
    nd->op  = -1;
    nd->src = src;

    nd->hash = HIR_DAG_compute_hash(nd);
    return nd;
}

dag_node_t* HIR_DAG_get_node(dag_ctx_t* ctx, hir_subject_t* src, int create) {
    if (!ctx || !src) return NULL;

    dag_node_t* nd;
    if (map_get(&ctx->dag, HIR_hash_subject(src), (void**)&nd)) return nd;

    map_iter_t it;
    map_iter_init(&ctx->dag, &it);
    unsigned long sh = HIR_hash_subject(src);
    while (map_iter_next(&it, (void**)&nd)) {
        if (set_has(&nd->link, (void*)sh)) return nd;
    }

    if (create) {
        nd = HIR_DAG_create_node(src);
        nd->id = ctx->curr_id++;
        map_put(&ctx->dag, HIR_hash_subject(src), nd);
        return nd;
    }

    return NULL;
}

int HIR_DAG_unload_node(dag_node_t* nd) {
    if (!nd) return 0;
    set_free(&nd->link);
    set_free(&nd->args);
    set_free(&nd->users);
    mm_free(nd);
    return 1;
}

int HIR_DAG_unload(dag_ctx_t* ctx) {
    map_iter_t it;
    dag_node_t* nd;
    map_iter_init(&ctx->dag, &it);
    while (map_iter_next(&it, (void**)&nd)) {
        HIR_DAG_unload_node(nd);
    }

    map_free(&ctx->groups);
    return map_free(&ctx->dag);
}
