#include <hir/hirgens/hirgens.h>

int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* args = HIR_SUBJ_LIST();
    ast_node_t* h = node->child;
    for (; h->token; h = h->sibling) {
        hir_subject_t* el = HIR_generate_elem(h, ctx, smt);
        HIR_BLOCK1(ctx, HIR_VRUSE, el);
        list_add(&args->storage.list.h, el);
    }

    HIR_BLOCK3(ctx, HIR_STASM, NULL, NULL, args);
    for (ast_node_t* l = h->child; l; l = l->sibling) {
        HIR_BLOCK1(ctx, HIR_RAW, HIR_SUBJ_RAWASM(l));
    }

    HIR_BLOCK3(
        ctx, HIR_ENDASM, NULL, NULL, 
        HIR_SUBJ_CONST(list_size(&args->storage.list.h))
    );

    return 1;
}
