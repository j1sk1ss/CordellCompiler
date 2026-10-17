#include <lir/func.h>

static inline lir_block_t* _previous(lir_block_t* lh, cfg_block_t* bb) {
    while (lh) {
        if (
            !lh->unused && lh->op != LIR_ENDSCOPE && 
            lh->op != LIR_FEND && lh->op != LIR_SETPOS
        ) return lh;
        lh = lh == bb->lmap.entry ? NULL : lh->prev;
    }

    return NULL;
}

int LIR_FUNC_perform_tco(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        func_info_t fi;
        if (!FNTB_get_info_id(fb->f_id, &fi, &smt->f) || fi.flags.entry || fi.flags.naked || fi.flags.onlybody) continue;
        
        int frame_exposed = 0;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_lir_instructions (bb) {
                if (
                    lh->op == LIR_REF || lh->op == LIR_REF_ARGS || 
                    lh->op == LIR_RAW || lh->op == LIR_VLADECL  || 
                    lh->op == LIR_ARRDECL
                ) frame_exposed = 1;
            }
        }

        if (frame_exposed) continue;

        foreach (cfg_block_t* bb, &fb->blocks) {
            if (set_size(&bb->succ)) continue;

            lir_block_t* call = _previous(bb->lmap.exit, bb);
            lir_subject_t* result = NULL;
            if (call && call->op == LIR_FRET) {
                result = call->farg;
                call = _previous(call == bb->lmap.entry ? NULL : call->prev, bb);
            }

            if (result) {
                if (
                    !call || call->op != LIR_LOADFRET || 
                    !LIR_subj_equals(result, call->farg)
                ) continue;
                call = _previous(call == bb->lmap.entry ? NULL : call->prev, bb);
            }
            
            if (
                !call || 
                (call->op != LIR_FCLL && call->op != LIR_ECLL) || 
                call->farg->t != LIR_FNAME
            ) continue;
            
            func_info_t callee;
            if (
                !FNTB_get_info_id(call->farg->storage.str.sid, &callee, &smt->f) || 
                callee.flags.vargs || callee.flags.naked || callee.flags.onlybody
            ) continue;
            
            /* DO NOT create a tail-call if there are stack-based arguments */
            if (list_size(&call->targ->storage.list.h) > (CONF_get_system_type() == I386 ? 0 : 6)) continue;
            call->op = LIR_JMP;

            for (
                lir_block_t* lh = call == bb->lmap.exit ? NULL : call->next; 
                lh; 
                lh = lh == bb->lmap.exit ? NULL : lh->next
            ) lh->unused = 1;
        }
    }

    return 1;
}
