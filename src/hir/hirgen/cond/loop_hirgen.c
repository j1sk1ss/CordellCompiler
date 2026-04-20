#include <hir/hirgens/hirgens.h>

/*
Generates a counted (or not) loop.
Params:
    - `node` - Entry 'loop' node.
    - `ctx` - HIR context.
    - `count` - Count of loops. Can be negative which will create a regular loop.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
static int _generate_counted_loop_block(ast_node_t* node, hir_ctx_t* ctx, long count, sym_table_t* smt) {
    hir_subject_t* counter = NULL;
    if (count >= 0) {
        counter = HIR_SUBJ_STKVAR(VRTB_add_info(NULL, I64_TYPE_TOKEN, NO_SYMBOL_ID, NULL, &smt->v), HIR_STKVARI64, 0);
        HIR_BLOCK1(ctx, HIR_VARDECL, counter);
        HIR_BLOCK2(ctx, HIR_STORE, counter, HIR_SUBJ_CONST(count));
    }

    ast_node_t* lbranch = node->c;    
    if (lbranch) {
        hir_subject_t* entry_lb = HIR_SUBJ_LABEL();
        hir_subject_t* body_lb  = HIR_SUBJ_LABEL();
        hir_subject_t* end_lb   = HIR_SUBJ_LABEL();

        HIR_BLOCK1(ctx, HIR_MKLB, entry_lb);
        HIR_BLOCK1(ctx, HIR_JMP, body_lb);

        HIR_BLOCK1(ctx, HIR_MKLB, body_lb);
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        
        void* backup = ctx->carry.ptr;
        ctx->carry.ptr = end_lb;
        HIR_generate_block(lbranch->c, ctx, smt);
        ctx->carry.ptr = backup;

        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        if (!counter) HIR_BLOCK1(ctx, HIR_JMP, entry_lb);
        else {
            hir_subject_t* res = HIR_SUBJ_TMPVAR(
                counter->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(counter->t), NO_SYMBOL_ID, NULL, &smt->v)
            );
            HIR_BLOCK3(ctx, HIR_iSUB, res, HIR_copy_subject(counter), HIR_SUBJ_CONST(1));
            HIR_BLOCK2(ctx, HIR_STORE, HIR_copy_subject(counter), res);
            HIR_BLOCK3(ctx, HIR_IFOP2, HIR_copy_subject(counter), entry_lb, end_lb);
        }

        HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    }

    return 1;
}

int HIR_generate_loop_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    HAS_ANNOTATION(COUNTER_ANNOTATION, node, { 
        return _generate_counted_loop_block(node, ctx, annot->data.counter, smt); 
    });

    return _generate_counted_loop_block(node, ctx, -1, smt);
}
