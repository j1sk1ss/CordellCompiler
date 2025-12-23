#include <hir/dag.h>

unsigned long HIR_DAG_compute_hash(dag_node_t* nd) {
    if (!set_size(&nd->args)) return HIR_hash_subject(nd->src);
    unsigned long h = nd->op * 1315423911UL;
    set_foreach (dag_node_t* arg, &nd->args) {
        h ^= _get_dag_hash(arg) + 0x9e3779b97f4a7c15UL + (h << 6) + (h >> 2);
    }

    return h;
}

/*
Create a new DAG node from the source HIR subject.
Params:
    - `src` - HIR subject.

Returns a new DAG node or NULL.
*/
static dag_node_t* _create_node(hir_subject_t* src) {
    dag_node_t* nd = (dag_node_t*)mm_malloc(sizeof(dag_node_t));
    if (!nd) return NULL;
    str_memset(nd, 0, sizeof(dag_node_t));

    set_init(&nd->users, SET_NO_CMP);
    set_init(&nd->args,  SET_NO_CMP);
    set_init(&nd->link,  SET_NO_CMP);

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

    unsigned long sh = HIR_hash_subject(src);
    map_foreach (dag_node_t* nnd, &ctx->dag) {
        if (set_has(&nnd->link, (void*)sh)) return nnd;
    }

    if (create) {
        nd = _create_node(src);
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
    map_free(&ctx->groups);
    return map_free_force_op(&ctx->dag, HIR_DAG_unload_node);
}
