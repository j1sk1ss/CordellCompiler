#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int LIR_allocate_var(hir_subject_t* v, stack_map_t* stk, sym_table_t* smt, alloc_info_t* i, long* off) {
    int vrsize = LIR_get_hirtype_size(v->t);
    int vroff  = stack_map_alloc(vrsize, stk);
    print_debug("_allocate_var, size=%i, off=%i", vrsize, vroff);
    if (VRTB_update_offset(v->storage.var.v_id, vroff, &smt->v)) {
        i->offset = vroff;
        i->size   = vrsize;
        *off      = MAX(*off, vroff);
        return 1;
    }

    return 0;
}

int LIR_allocate_scope_heap(lir_ctx_t* ctx, int s_id, scope_stack_t* heap) {
    
}

int LIR_deallocate_scope_heap(lir_ctx_t* ctx, int s_id, scope_stack_t* heap) {
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