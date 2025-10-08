#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_allocate_arr(
    lir_ctx_t* ctx, hir_subject_t* v, hir_subject_t* size, scope_stack_t* heap, 
    scope_stack_t* scopes, sym_table_t* smt, array_info_t* ai, long* offset
) {
    long arroff  = -1;
    long arrsize = DEFAULT_TYPE_SIZE;
    int elsize   = LIR_get_asttype_size(ai->el_type);

    if (!ai->heap) {
        arrsize = elsize * ai->size;
        arroff  = stack_map_alloc(arrsize, &ctx->stk);
        print_debug("x86_64_allocate_arr, size=%i, off=%i", arrsize, arroff);
    }
    else {
        arroff = stack_map_alloc(DEFAULT_TYPE_SIZE, &ctx->stk);
        print_debug("x86_64_allocate_arr, heap allocation for [rbp - %i], scope=%i", arroff, scope_id_top(scopes));
        scope_push(heap, scope_id_top(scopes), arroff);
        x86_64_store_var_reg(LIR_iMOV, ctx, size, RAX, smt);
        LIR_BLOCK1(ctx, LIR_PUSH, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(12));
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RDI, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(0));
        LIR_BLOCK0(ctx, LIR_SYSC);
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
        x86_64_reg_op(ctx, RDI, DEFAULT_TYPE_SIZE, RAX, DEFAULT_TYPE_SIZE, LIR_iMOV);
        LIR_BLOCK1(ctx, LIR_POP, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
        x86_64_reg_op(ctx, RAX, DEFAULT_TYPE_SIZE, RBX, DEFAULT_TYPE_SIZE, LIR_iADD);
        x86_64_reg_op(ctx, RDI, DEFAULT_TYPE_SIZE, RAX, DEFAULT_TYPE_SIZE, LIR_iMOV);
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(12));
        LIR_BLOCK0(ctx, LIR_SYSC);
    }

    *offset = MAX(*offset, arroff);
    VRTB_update_memory(v->storage.var.v_id, arroff, arrsize, -1, &smt->v);
    return arroff;
}

int x86_64_deallocate_heap(lir_ctx_t* ctx, int s_id, scope_stack_t* heap) {
    if (scope_id_top(&ctx->heap) == s_id) {
        scope_elem_t hinfo;
        scope_pop_top(&ctx->heap, &hinfo);
        print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", s_id, hinfo.offset);
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RDI, DEFAULT_TYPE_SIZE), LIR_SUBJ_OFF(hinfo.offset, 8));
        LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(12));
        LIR_BLOCK0(ctx, LIR_SYSC);
    }

    return 1;
}
