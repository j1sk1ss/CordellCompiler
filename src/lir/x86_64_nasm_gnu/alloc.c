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

int LIR_deallocate_scope_heap(ast_node_t* t, lir_ctx_t* ctx) {
    // if (scope_id_top(&ctx->heap) == t->sinfo.s_id) {
    //     scope_elem_t hinfo;
    //     scope_pop_top(&ctx->heap, &hinfo);
    //     print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", t->sinfo.s_id, hinfo.offset);
    //     IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_OFF(hinfo.offset, 8));
    //     IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(12));
    //     IR_BLOCK0(ctx, SYSC);
    // }

    return 1;
}