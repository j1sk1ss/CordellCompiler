#include <hir/hirgens/hirgens.h>

int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* args = HIR_SUBJ_LIST();
    if (!args) return 0;

    for (ast_node_t* h = node->c->c; h && h->t; h = h->siblings.n) {
        hir_subject_t* el = HIR_generate_elem(h, ctx, smt);
        if (!el) {
            HIR_unload_subject(args);
            HIRGEN_ERROR(node, "Asm: Argument HIR generation error!");
            return 0;
        }
        
        HIR_BLOCK1(ctx, HIR_VRUSE, el);
        list_add(&args->storage.list.h, HIR_copy_subject(el));
    }

    HIR_BLOCK3(ctx, HIR_STASM, NULL, NULL, args);
    for (ast_node_t* l = node->c->siblings.n->c; l; l = l->siblings.n) {
        HIR_BLOCK1(ctx, HIR_RAW, HIR_SUBJ_RAWASM(l));
    }

    HIR_BLOCK3(ctx, HIR_ENDASM, NULL, NULL, HIR_SUBJ_CONST(list_size(&args->storage.list.h)));
    return 1;
}
