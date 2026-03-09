#include <hir/hirgens/hirgens.h>

static int _generate_counted_loop_block(ast_node_t* node, hir_ctx_t* ctx, long value, sym_table_t* smt) {
    string_t* counter_name = create_string("__counter");
    hir_subject_t* counter = HIR_SUBJ_STKVAR(
        VRTB_add_info(counter_name, I64_TYPE_TOKEN, node->sinfo.s_id, NULL, &smt->v), 
        HIR_STKVARI64, 
        0
    );
    destroy_string(counter_name);
    
    HIR_BLOCK1(ctx, HIR_VARDECL, counter);
    HIR_BLOCK2(ctx, HIR_STORE, counter, HIR_SUBJ_CONST(value));

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

        hir_subject_t* updated_counter = HIR_SUBJ_STKVAR(
            VRTB_add_info(NULL, TMP_I64_TYPE_TOKEN, node->sinfo.s_id, NULL, &smt->v), 
            HIR_TMPVARI64, 
            0
        );

        HIR_BLOCK3(ctx, HIR_iSUB, updated_counter, counter, HIR_SUBJ_CONST(1));
        HIR_BLOCK2(ctx, HIR_STORE, HIR_SUBJ_STKVAR(counter->storage.var.v_id, HIR_STKVARI64, 0), updated_counter);
        HIR_BLOCK3(ctx, HIR_IFOP2, updated_counter, entry_lb, end_lb);

        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    }

    return 1;
}

static int _generate_uncounted_loop_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
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
        HIR_BLOCK1(ctx, HIR_JMP, entry_lb);
        HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    }

    return 1;
}

int HIR_generate_loop_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HAS_ANNOTATION(COUNTER_ANNOTATION, node, { 
        return _generate_counted_loop_block(node, ctx, annot->data.counter, smt); 
    });

    return _generate_uncounted_loop_block(node, ctx, smt);
}
