#include <hir/hirgens/hirgens.h>

int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    func_info_t fi;
    if (!FNTB_get_info_id(node->c->sinfo.v_id, &fi, &smt->f)) {
        return 0;
    }
    
    HIR_BLOCK1(ctx, HIR_FDCL, HIR_SUBJ_FUNCNAME(node->c));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(node->c->siblings.n->sinfo.s_id));

    int argnum = 0;
    ast_node_t* t;
    for (t = node->c->siblings.n->c; t && t->t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
        if (t->t->t_type == VAR_ARGUMENTS_TOKEN) continue; /* Skip variadic argument */
        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->c));
        HIR_BLOCK2(
            ctx, 
            fi.flags.entry ? HIR_STARGLD : HIR_FARGLD,     /* If this is an entry point, use the stload command */
            HIR_SUBJ_ASTVAR(t->c), 
            HIR_SUBJ_CONST(argnum++)
        );
    }

    int pargnum = ctx->carry.val1;
    ctx->carry.val1 = argnum;
    HIR_generate_block(t, ctx, smt);
    ctx->carry.val1 = pargnum;

    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(node->c->siblings.n->sinfo.s_id));
    HIR_BLOCK0(ctx, HIR_FEND);
    return 1;
}
