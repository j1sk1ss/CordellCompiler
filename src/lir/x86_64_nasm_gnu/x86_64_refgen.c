#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_ref(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_REF: {
            x86_64_store_var_reg(LIR_REF, ctx, h->sarg, RAX, smt);
            x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            break;
        }

        case HIR_GDREF: {
            x86_64_store_var_reg(LIR_GDREF, ctx, h->sarg, RAX, smt);
            x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            break;
        }

        case HIR_LDREF: {
            x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            x86_64_store_var_reg(LIR_LDREF, ctx, h->sarg, RAX, smt);
            break;
        }

        case HIR_GINDEX: {
            x86_64_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
            LIR_BLOCK2(ctx, LIR_iMUL, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(HIR_get_type_size(h->farg->t)));

            array_info_t ai;
            if (!ARTB_get_info(h->sarg->storage.var.v_id, &ai, &smt->a)) x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
            else {
                if (!ai.heap) x86_64_store_var_reg(LIR_REF, ctx, h->sarg, RAX, smt);
                else x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
            }

            x86_64_reg_op(ctx, RAX, DEFAULT_TYPE_SIZE, RBX, DEFAULT_TYPE_SIZE, LIR_iADD);
            x86_64_reg_op(ctx, RAX, DEFAULT_TYPE_SIZE, RAX, DEFAULT_TYPE_SIZE, LIR_GDREF);
            x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            break;
        }

        case HIR_LINDEX: {
            x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RBX, smt);
            LIR_BLOCK2(ctx, LIR_iMUL, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE), LIR_SUBJ_CONST(HIR_get_type_size(h->targ->t)));

            array_info_t ai;
            if (!ARTB_get_info(h->farg->storage.var.v_id, &ai, &smt->a)) x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            else {
                if (!ai.heap) x86_64_store_var_reg(LIR_REF, ctx, h->farg, RAX, smt);
                else x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            }

            x86_64_reg_op(ctx, RAX, DEFAULT_TYPE_SIZE, RBX, DEFAULT_TYPE_SIZE, LIR_iADD);
            x86_64_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
            x86_64_reg_op(ctx, RAX, DEFAULT_TYPE_SIZE, RBX, DEFAULT_TYPE_SIZE, LIR_LDREF);
            break;
        }
    }

    return 1;
}
