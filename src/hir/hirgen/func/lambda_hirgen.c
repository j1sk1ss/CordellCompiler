#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_lambda(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    HIR_SET_CURRENT_POS(ctx, node);
    func_info_t fi;
    if (!FNTB_get_info_id(node->sinfo.v_id, &fi, &smt->f)) {
        return NULL;
    }

    hir_subject_t* lguards = HIR_SUBJ_LABEL();
    HIR_BLOCK1(ctx, HIR_JMP, lguards);
    
    HIR_BLOCK1(ctx, HIR_FDCL, HIR_SUBJ_FUNCNAME(node));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(node->c->sinfo.s_id));

    int argnum = 0;
    ast_node_t* t;
    for (t = node->c->c; t && t->t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->c));
        HIR_BLOCK2(ctx, HIR_FARGLD, HIR_SUBJ_ASTVAR(t->c), HIR_SUBJ_CONST(argnum++));
    }

    SET_AND_DUMP_POPARG(HIR_FARGLD, argnum, { HIR_generate_block(t, ctx, smt); });

    if (list_size(&ctx->cold.blocks)) {
        HIR_BLOCK1(ctx, HIR_FRET, HIR_SUBJ_CONST(0));
        HIR_dump_cold(ctx);
    }

    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(node->c->sinfo.s_id));
    HIR_BLOCK0(ctx, HIR_FEND);
    HIR_BLOCK1(ctx, HIR_MKLB, lguards);

    if (!ret) return NULL;
    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARI0, VRTB_add_info(NULL, TMP_I0_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v));
    res->ptr = 1;
    HIR_BLOCK2(ctx, HIR_REF, res, HIR_SUBJ_FUNCNAME(node));
    return res;
}
