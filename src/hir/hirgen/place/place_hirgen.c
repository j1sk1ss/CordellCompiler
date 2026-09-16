#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_place(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* buffer = HIR_generate_elem(node->c, ctx, smt);
    type_info_t ti;
    if (TPTB_get_info_id(node->c->siblings.n->sinfo.t_id, &ti, &smt->t)) {
        hir_subject_t* vtable = HIR_load_vtable(&ti, ctx, NULL, smt);
        if (vtable) HIR_BLOCK2(ctx, HIR_LDREF, buffer, vtable);
    }

    return buffer;
}