#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_declaration(
    lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params,
    scope_stack_t* scopes, scope_stack_t* heap, stack_map_t* stk, long* offset
) {
    switch (h->op) {
        case HIR_VRDEALL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            variable_info_t vi;
            if (VRTB_get_info_id(h->farg->storage.var.v_id, &vi, &smt->v)) {
                stack_map_free(vi.offset, LIR_get_hirtype_size(h->farg->t), stk);
            }

            break;
        }
        
        case HIR_VARDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            alloc_info_t alloc;
            if (LIR_allocate_var(h->farg, stk, smt, &alloc, offset)) {
                if (h->sarg) {
                    LIR_BLOCK2(
                        ctx, LIR_iMOV, LIR_SUBJ_OFF(alloc.offset, alloc.size), 
                        LIR_SUBJ_REG(RAX, alloc.size)
                    );
                }
            }

            break;
        }

        case HIR_ARRDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;

            variable_info_t vi;
            if (VRTB_get_info_id(h->farg->storage.var.v_id, &vi, &smt->v)) {
                array_info_t ai;
                if (ARTB_get_info(vi.name, vi.s_id, &ai, &smt->a)) {
                    int arroff = -1;
                    int elsize = LIR_get_asttype_size(ai.el_type);

                    if (!ai.heap) {
                        int arrsize = elsize * h->sarg->storage.cnst.value;
                        arroff = stack_map_alloc(arrsize, stk);
                        for (int i = 0; i < h->sarg->storage.cnst.value; i++) {
                            stack_elem_t se;
                            stack_top_addr(params, &se);
                            if (!se.data.addrdata) break;

                            LIR_BLOCK2(
                                ctx, LIR_iMOV,  LIR_SUBJ_OFF(arroff - elsize * i, elsize), 
                                LIR_format_variable(se.data.addrdata, smt)
                            );

                            stack_pop(params);
                        }
                    }
                    else {
                        arroff = stack_map_alloc(DEFAULT_TYPE_SIZE, stk);
                        scope_push(heap, scope_id_top(scopes), arroff);
                        print_debug("Heap allocation for [rbp - %i]", arroff);
                    }

                    *offset = MAX(*offset, arroff);
                    VRTB_update_offset(h->farg->storage.var.v_id, arroff, &smt->v);
                }

            }

            break;
        }

        case HIR_STRDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            break;
        }
    }
}