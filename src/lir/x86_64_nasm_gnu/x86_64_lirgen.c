#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

typedef struct {
    long offset;
    int  size;
} alloc_info_t;

static int _allocate_var(hir_subject_t* v, stack_map_t* stk, sym_table_t* smt, alloc_info_t* i) {
    int vrsize = LIR_get_hirtype_size(v->t);
    int vroff  = stack_map_alloc(vrsize, stk);
    print_debug("_allocate_var, size=%i, off=%i", vrsize, vroff);
    if (VRTB_update_offset(v->storage.var.v_id, vroff, &smt->v)) {
        i->offset = vroff;
        i->size   = vrsize;
        return 1;
    }

    return 0;
}

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;

    stack_map_t stackmap = { .offset = 0 };
    scope_stack_t scopes = { .top = -1 };
    scope_stack_t heap   = { .top = -1 };
    sstack_t params      = { .top = -1 };

    long offset = 0;
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
            case HIR_MKSCOPE: {
                scope_push(&scopes, h->farg->storage.cnst.value, offset);
                break;
            }

            case HIR_FEND:
            case HIR_ENDSCOPE: {
                /* heap deallocation for current scope */
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
                    LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
                    scope_push(&scopes, h->farg->storage.cnst.value, offset);
                }

                break;
            }

            /* Function return command, ret_val - farg */
            case HIR_FRET: {
                // mov rax, ...
                LIR_BLOCK0(ctx, LIR_FRET);
                break;
            }

            /* Function load arg, v_id - farg.var.id, argnum - sarg.cnst.val */
            case HIR_FARGLD: {
                alloc_info_t alloc;
                if (_allocate_var(h->farg, &stackmap, smt, &alloc)) {
                    static const int abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
                    LIR_BLOCK2(
                        ctx, LIR_iMOV, LIR_SUBJ_OFF(alloc.offset, alloc.size), 
                        LIR_SUBJ_REG(abi_regs[h->sarg->storage.cnst.value], alloc.size)
                    );
                }
            }

            /* Allocate stack memory for declaration, v_id - farg.var.id, val (opt) - sarg? */
            case HIR_VARDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
                alloc_info_t alloc;
                if (_allocate_var(h->farg, &stackmap, smt, &alloc)) {
                    if (h->sarg) {
                        LIR_BLOCK2(
                            ctx, LIR_iMOV, LIR_SUBJ_OFF(alloc.offset, alloc.size), 
                            LIR_SUBJ_REG(RAX, alloc.size)
                        );
                    }
                }

                break;
            }

            /* Allocate array in stack / in .data, v_id - farg.var.id, size - sarg? */
            case HIR_ARRDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;

                variable_info_t vi;
                if (VRTB_get_info_id(h->farg->storage.var.v_id, &vi, &smt->v)) {
                    array_info_t ai;
                    if (ARTB_get_info(vi.name, vi.s_id, &ai, &smt->a)) {
                        int elsize = LIR_get_asttype_size(ai.el_type);
                        if (!ai.heap) {
                            int arrsize = elsize * h->sarg->storage.cnst.value;
                            int arroff  = stack_map_alloc(arrsize, &stackmap);
                            VRTB_update_offset(h->farg->storage.var.v_id, arroff, &smt->v);
                            print_debug("HIR_ARRDECL allocation, size=%i, off=%i", arrsize, arroff);
                        }
                        else { /* Heap allocation */ }
                    }

                }

                break;
            }

            case HIR_STRDECL: {
                if (LIR_is_global_hirtype(h->farg->t)) break;
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

            case HIR_MKLB: HIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;
            case HIR_JMP:  HIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;

            default: break;
        }
        
        h = h->next;
    }

    return 1;
}
