#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_declaration(
    lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params, scope_stack_t* scopes, scope_stack_t* heap, long* offset
) {
    switch (h->op) {
        case HIR_VRDEALL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            variable_info_t vi;
            if (VRTB_get_info_id(h->farg->storage.cnst.value, &vi, &smt->v) && vi.vmi.reg < 0) {
                stack_map_free(vi.vmi.offset, vi.vmi.size, &ctx->stk);
            }

            break;
        }

        case HIR_STRDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            str_info_t si;
            array_info_t ai;
            if (
                STTB_get_info_id(h->sarg->storage.str.s_id, &si, &smt->s) &&
                ARTB_get_info(h->farg->storage.var.v_id, &ai, &smt->a)
            ) {
                int arroff = x86_64_allocate_arr(ctx, h->farg, NULL, NULL, NULL, smt, &ai, offset);
                int elsize = LIR_get_asttype_size(ai.el_type);

                int pos = 0;
                char* string = si.value;
                while (*string) {
                    LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff - elsize * pos++, elsize), LIR_SUBJ_CONST(*(string++)));
                }

                LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff - elsize * pos, elsize), LIR_SUBJ_CONST(0));
            }

            break;
        }

        case HIR_ARRDECL: {
            if (LIR_is_global_hirtype(h->farg->t)) break;
            array_info_t ai;
            if (ARTB_get_info(h->farg->storage.var.v_id, &ai, &smt->a)) {
                int arroff = x86_64_allocate_arr(ctx, h->farg, h->sarg, heap, scopes, smt, &ai, offset);
                int elsize = LIR_get_asttype_size(ai.el_type);
                if (h->sarg->t != HIR_NUMBER) break;
                for (int i = 0; i < h->sarg->storage.cnst.value; i++) {
                    stack_elem_t se;
                    stack_top_addr(params, &se);
                    if (!se.data.addrdata) break;

                    LIR_BLOCK2(
                        ctx, LIR_iMOV, LIR_SUBJ_OFF(arroff - elsize * i, elsize), x86_64_format_variable(ctx, se.data.addrdata, smt)
                    );

                    stack_pop(params);
                }
            }

            break;
        }
    }
}
