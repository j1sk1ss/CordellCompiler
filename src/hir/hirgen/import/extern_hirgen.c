#include <hir/hirgens/hirgens.h>

int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx) {
    switch (node->c->t->t_type) {
        case FUNC_PROT_TOKEN: return HIR_BLOCK1(ctx, HIR_FEXT, HIR_SUBJ_CONST(node->c->c->sinfo.v_id)); 
        default: {
            if (!TKN_isdecl(node->c->t)) break;
            return HIR_BLOCK1(ctx, HIR_OEXT, HIR_SUBJ_CONST(node->c->c->sinfo.v_id));
        }
    }

    return 0;
}
