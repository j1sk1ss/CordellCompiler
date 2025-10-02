#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int LIR_allocate_var(hir_subject_t* v, stack_map_t* stk, sym_table_t* smt, long* offset) {
    int vrsize = LIR_get_hirtype_size(v->t);
    int vroff  = stack_map_alloc(vrsize, stk);
    print_debug("LIR_allocate_var, size=%i, off=%i", vrsize, vroff);

    if (VRTB_update_memory(v->storage.var.v_id, vroff, HIR_get_type_size(v->t), &smt->v)) {
        *offset = MAX(*offset, vroff);
        return 1;
    }

    return 0;
}

int LIR_allocate_arr(
    lir_ctx_t* ctx, hir_subject_t* v, hir_subject_t* size, scope_stack_t* heap, 
    scope_stack_t* scopes, sstack_t* params, stack_map_t* stk, sym_table_t* smt, long* offset
) {
    variable_info_t vi;
    if (VRTB_get_info_id(v->storage.var.v_id, &vi, &smt->v)) {
        array_info_t ai;
        if (ARTB_get_info(vi.name, vi.s_id, &ai, &smt->a)) {
            long arroff  = -1;
            long arrsize = DEFAULT_TYPE_SIZE;
            int elsize   = LIR_get_asttype_size(ai.el_type);

            if (!ai.heap) {
                arrsize = elsize * size->storage.cnst.value;
                arroff = stack_map_alloc(arrsize, stk);
                for (int i = 0; i < size->storage.cnst.value; i++) {
                    stack_elem_t se;
                    stack_top_addr(params, &se);
                    if (!se.data.addrdata) break;

                    LIR_BLOCK2(
                        ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff - elsize * i, elsize), 
                        LIR_format_variable(ctx, se.data.addrdata, smt)
                    );

                    stack_pop(params);
                }
            }
            else {
                arroff = stack_map_alloc(DEFAULT_TYPE_SIZE, stk);
                scope_push(heap, scope_id_top(scopes), arroff);
                print_debug("Heap allocation for [rbp - %i]", arroff);
                LIR_BLOCK1(ctx, LIR_PUSH, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(12));
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RDI, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(0));
                LIR_BLOCK0(ctx, LIR_SYSC);
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_format_variable(ctx, size, smt), LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RDI, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK1(ctx, LIR_POP, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK2(ctx, LIR_iADD, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RDI, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(12));
                LIR_BLOCK0(ctx, LIR_SYSC);
            }

            *offset = MAX(*offset, arroff);
            VRTB_update_memory(v->storage.var.v_id, arroff, arrsize, &smt->v);
        }
    }
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
