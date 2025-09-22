#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int _lir_generator(hir_block_t* h, hir_operation_t end, lir_ctx_t* ctx, sym_table_t* smt) {
    return 1;
}

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;

    long offset = 0;
    stack_map_t stackmap = { .offset = 0 };
    scope_stack_t scopes = { .top = -1 };
    scope_stack_t heap   = { .top = -1 };
    sstack_t params = { .top = -1 };

    while (h) {
        switch (h->op) {
            case HIR_STRT: {
                break;
            }

            /* Load args from entry program stack, v_id - farg.storage.var.id */
            case HIR_STARGLD: {
                break;
            }

            /* make / end scope, scope_id - farg.cnst */
            case HIR_MKSCOPE: scope_push(&scopes, h->farg->storage.cnst.value, offset); break;
            case HIR_ENDSCOPE: {
                scope_elem_t se;
                scope_pop_top(&scopes, &se);
                offset = se.offset;
                break;
            }

            /* Generate function body and function declaration, func_id - farg.str.s_id */
            case HIR_FDCL: {
                func_info_t fi;
                if (FNTB_get_info_id(h->farg->storage.str.s_id, &fi, &smt->f)) {
                    if (fi.global) LIR_BLOCK1(ctx, LIR_MKGLB, LIR_SUBJ_STRING(h->farg->storage.str.s_id));
                    lir_subject_t* fend = LIR_SUBJ_LABEL();
                    LIR_BLOCK1(ctx, LIR_JMP, fend);
                    LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
                    /* function body generation... */
                    LIR_BLOCK1(ctx, LIR_MKLB, fend);
                }

                break;
            }

            /* Function return command, ret_val - farg */
            case HIR_FRET: {
                // mov rax, ...
                LIR_BLOCK0(ctx, LIR_FRET);
                break;
            }

            /* Allocate stack memory for declaration, v_id - farg.var.id, val (opt) - sarg? */
            case HIR_VARDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
                int vrsize = LIR_get_hirtype_size(h->farg->t);
                int vroff  = stack_map_alloc(vrsize, &stackmap);
                VRTB_update_offset(h->farg->storage.var.v_id, vroff, &smt->v);
                if (h->sarg) {
                    /* mov rax, ... */
                    LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_OFF(vroff, vrsize), LIR_SUBJ_REG(RAX, 8));
                }

                break;
            }

            /* Allocate array in stack / in .data, v_id - farg.var.id, size - sarg? */
            case HIR_ARRDECL: {
                break;
            }

            case HIR_STRDECL: {
                break;
            }

            /* Ger dref value by link in sarg, move it to farg */
            case HIR_GDREF: {
                variable_info_t src;
                VRTB_get_info_id(h->sarg->storage.var.v_id, &src, &smt->v);
                LIR_BLOCK2(
                    ctx, LIR_GDREF,
                    LIR_SUBJ_REG(RAX, LIR_get_hirtype_size(h->farg->t)),
                    LIR_SUBJ_OFF(src.offset, LIR_get_hirtype_size(h->farg->t))
                );

                variable_info_t dst;
                VRTB_get_info_id(h->farg->storage.var.v_id, &dst, &smt->v);
                LIR_BLOCK2(
                    ctx, LIR_iMOV,
                    LIR_SUBJ_OFF(dst.offset, LIR_get_hirtype_size(h->sarg->t)),
                    LIR_SUBJ_REG(RAX, LIR_get_hirtype_size(h->sarg->t))
                );

                break;
            }

            /* Store value from sarg to location with address from farg */
            case HIR_LDREF: {
                /* mov rax, ... */

                variable_info_t dst;
                VRTB_get_info_id(h->farg->storage.var.v_id, &dst, &smt->v);
                LIR_BLOCK2(
                    ctx, LIR_LDREF,
                    LIR_SUBJ_OFF(dst.offset, LIR_get_hirtype_size(h->sarg->t)),
                    LIR_SUBJ_REG(RAX, LIR_get_hirtype_size(h->sarg->t))
                );

                break;
            }

            /* Push argument, arg - farg */
            case HIR_FARGST:
            case HIR_PRMST: stack_push(&params, h->farg); break;

            default: break;
        }
        
        h = h->next;
    }

    return 1;
}
