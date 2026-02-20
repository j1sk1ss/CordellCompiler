#include <hir/hirgens/hirgens.h>

int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, HIR_STRT, HIR_SUBJ_FUNCNAME(node));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(node->sinfo.s_id));

    int argnum = 0;
    ast_node_t* t;
    for (t = node->c; t && t->t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
        if (t->t->t_type == VAR_ARGUMENTS_TOKEN) continue; /* Skip variadic argument */
        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->c));
        HIR_BLOCK2(ctx, HIR_STARGLD, HIR_SUBJ_ASTVAR(t->c), HIR_SUBJ_CONST(argnum++));
    }

    SET_AND_DUMP_POPARG(HIR_STARGLD, argnum, { HIR_generate_block(t, ctx, smt); });
    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(node->sinfo.s_id));
    HIR_BLOCK0(ctx, HIR_STEND);
    return 1;
}
