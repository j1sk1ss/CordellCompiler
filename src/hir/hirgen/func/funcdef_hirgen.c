#include <hir/hirgens/hirgens.h>

int HIR_generate_function_block(ast_node_t* node, symbol_id_t f_id, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);

    func_info_t fi;
    if (!FNTB_get_info_id(f_id == NO_SYMBOL_ID ? node->c->sinfo.v_id : f_id, &fi, &smt->f) || fi.flags.generic) return 0;

    hir_subject_t* lguards = NULL;
    if (fi.flags.local) {
        lguards = HIR_SUBJ_LABEL();
        HIR_BLOCK1(ctx, HIR_JMP, lguards);
    }
    
    HIR_BLOCK1(ctx, HIR_FDCL, HIR_SUBJ_FUNCNAME(node->c));
    HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(node->c->siblings.n->sinfo.s_id));

    int argnum = 0, arg_count = 0; // TODO: Separate iterator from a dec and float value
    for (ast_node_t* tmp = node->c->siblings.n->c; tmp && tmp->t && tmp->t->t_type != SCOPE_TOKEN; tmp = tmp->siblings.n) {
        arg_count++;
    }

    ast_node_t* t;
    for (t = node->c->siblings.n->c; t && t->t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
        if (t->t->t_type == VAR_ARGUMENTS_TOKEN) {
            FNTB_update_func(fi.id, FNTB_SET_VARGS, &smt->f);
            fi.flags.vargs = 1;
            continue;
        }

        HIR_BLOCK1(ctx, HIR_VARDECL, HIR_SUBJ_ASTVAR(t->c));
        HIR_BLOCK3(
            ctx, 
            fi.flags.entry ? HIR_STARGLD : HIR_FARGLD, 
            HIR_SUBJ_ASTVAR(t->c), 
            HIR_SUBJ_CONST(argnum),
            HIR_SUBJ_CONST(arg_count - argnum)
        );
        argnum++;
    }

    hir_subject_t* vargs = NULL;
    if (fi.flags.vargs) {
        vargs = HIR_SUBJ_STKVAR(VRTB_add_info(NULL, I0_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v), HIR_STKVARI0, 1);
        HIR_BLOCK1(ctx, HIR_REF_ARGS, vargs);
    }

    SET_AND_DUMP_POPARG(fi.flags.entry ? HIR_STARGLD : HIR_FARGLD, argnum, fi.rtype ? fi.rtype->t : NULL, vargs, { 
        HIR_generate_block(t, ctx, smt); 
    });

    if (list_size(&ctx->cold.blocks)) {
        HIR_BLOCK1(ctx, fi.flags.entry ? HIR_EXITOP : HIR_FRET, HIR_SUBJ_CONST(0));
        HIR_dump_cold(ctx);
    }
    
    HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(node->c->siblings.n->sinfo.s_id));
    HIR_BLOCK0(ctx, HIR_FEND);
    if (lguards) HIR_BLOCK1(ctx, HIR_MKLB, lguards);
    return 1;
}
