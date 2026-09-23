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
    HIR_BLOCK0(ctx, HIR_MKSCOPE);

    ast_node_t* body = HIR_generate_argument_load(node->c, ctx, &fi);
    SET_AND_DUMP_POPARG(NULL, NULL, { HIR_generate_block(body, ctx, smt); });

    if (list_size(&ctx->cold.blocks)) {
        HIR_BLOCK1(ctx, HIR_FRET, HIR_SUBJ_CONST(0));
        HIR_dump_cold(ctx);
    }

    HIR_BLOCK0(ctx, HIR_ENDSCOPE);
    HIR_BLOCK0(ctx, HIR_FEND);
    HIR_BLOCK1(ctx, HIR_MKLB, lguards);

    if (!ret) return NULL;

    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARI0, VRTB_add_info(NULL, TMP_I0_TYPE_TOKEN, NO_SYMBOL_ID, (basic_object_info_t){ .ptr = 1 }, &smt->v));
    res->ptr = 1;
    
    HIR_BLOCK2(ctx, HIR_REF, res, HIR_SUBJ_FUNCNAME(node));
    return res;
}
