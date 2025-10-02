#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_declaration(
    lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params,
    scope_stack_t* scopes, scope_stack_t* heap, stack_map_t* stk, long* offset
) {
    switch (h->op) {
        case HIR_VRDEALL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            // variable_info_t vi;
            // if (VRTB_get_info_id(h->farg->storage.cnst.value, &vi, &smt->v)) {
            //     stack_map_free(vi.offset, vi.size, stk);
            // }

            break;
        }
        
        case HIR_VARDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            LIR_allocate_var(h->farg, stk, smt, offset);
            break;
        }

        case HIR_STRDECL:
        case HIR_ARRDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            array_info_t ai;
            int arroff = LIR_allocate_arr(ctx, h->farg, h->sarg, heap, scopes, stk, smt, &ai, offset);
            int elsize = LIR_get_asttype_size(ai.el_type);
            if (h->sarg->t != HIR_NUMBER) break;
            for (int i = 0; i < h->sarg->storage.cnst.value; i++) {
                stack_elem_t se;
                stack_top_addr(params, &se);
                if (!se.data.addrdata) break;

                LIR_BLOCK2(
                    ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff - elsize * i, elsize), 
                    LIR_format_variable(ctx, se.data.addrdata, smt)
                );

                stack_pop(params);
            }
            break;
        }
    }
}