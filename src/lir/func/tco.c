#include <lir/func.h>

static inline lir_block_t* _get_previous_non_system(lir_block_t* lh, cfg_block_t* bb) {
    while (lh) {
        if (
            !lh->unused && lh->op != LIR_ENDSCOPE && 
            lh->op != LIR_FEND && lh->op != LIR_SETPOS
        ) return lh;
        lh = lh == bb->lmap.entry ? NULL : lh->prev;
    }

    return NULL;
}

status_code_t LIR_FUNC_perform_tco(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        func_info_t fi;
        if (
            !FNTB_get_info_id(fb->f_id, &fi, &smt->f) || 
            fi.flags.entry || fi.flags.onlybody
        ) continue;

        foreach (cfg_block_t* bb, &fb->blocks) {
            if (set_size(&bb->succ)) continue;

            lir_block_t* call = _get_previous_non_system(bb->lmap.exit, bb);
            lir_subject_t* result = NULL;
            if (call && call->op == LIR_FRET) {
                result = call->farg;
                call = _get_previous_non_system(call == bb->lmap.entry ? NULL : call->prev, bb);
            }

            if (result) {
                if (
                    !call || call->op != LIR_LOADFRET || 
                    !LIR_subj_equals(result, call->farg)
                ) continue;
                call = _get_previous_non_system(call == bb->lmap.entry ? NULL : call->prev, bb);
            }
            
            if (
                !call || 
                (call->op != LIR_FCLL && call->op != LIR_ECLL) || 
                call->farg->t != LIR_FNAME
            ) continue;
            
            int send_stack = 0;
            foreach (lir_subject_t* param, &call->targ->storage.list.h) {
                if (param->t != LIR_VARIABLE) continue;
                set_t slaves;
                ALLIAS_get_slaves(param->storage.var.v_id, &slaves, &smt->m);
                send_stack = set_size(&slaves) > 0;
                set_free(&slaves);
                if (send_stack) break;
            }

            /* ! DO NOT ! create a tail call if parameters depend on the stack of caller
                          'cause we will deallocate the stackframe */
            if (send_stack) continue;

            func_info_t callee;
            if (
                !FNTB_get_info_id(call->farg->storage.str.sid, &callee, &smt->f) || 
                callee.flags.vargs || (callee.flags.naked != fi.flags.naked) || callee.flags.onlybody // we skip those functions 'cause we don't want to work with stack
                                                                                                      // naked and only body also will brake any non-onlybody / non-naked functions
            ) continue;
            
            /* ! DO NOT ! create a tail-call if there are stack-based arguments */
            if (list_size(&call->targ->storage.list.h) > CONF_register_parameters()) continue;
            call->op = LIR_JMP;

            for (
                lir_block_t* lh = call == bb->lmap.exit ? NULL : call->next; 
                lh; 
                lh = lh == bb->lmap.exit ? NULL : lh->next
            ) lh->unused = 1;
        }
    }

    return GENERAL_SUCCESS;
}
