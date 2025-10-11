#include <hir/hirgens/hirgens.h>

int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    int args = 0;
    ast_node_t* h = node->child;
    for (; h->token; h = h->sibling) {
        HIR_BLOCK1(ctx, HIR_PRMST, HIR_generate_load(h, ctx, smt));
        args++;
    }

    HIR_BLOCK1(ctx, HIR_STASM, HIR_SUBJ_CONST(args));
    for (ast_node_t* l = h->child; l; l = l->sibling) {
        HIR_BLOCK1(ctx, HIR_RAW, HIR_SUBJ_RAWASM(l));
    }

    HIR_BLOCK1(ctx, HIR_ENDASM, HIR_SUBJ_CONST(args));
    return 1;
}
