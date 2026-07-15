#include <lir/copyprop.h>

int LIR_clear_global_variables(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_lir_instructions (bb) {
                if (lh->op == LIR_OEXT) continue;
                iterate_lir_args (lir_subject_t* s, lh, 0) {
                    variable_info_t vi;
                    if (
                        s->t == LIR_VARIABLE && 
                        VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)
                    ) VRTB_set_used(vi.v_id, &smt->v);
                }
            }
        }
    }

    return 1;
}
