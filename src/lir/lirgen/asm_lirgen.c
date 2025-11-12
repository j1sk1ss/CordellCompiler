#include <lir/lirgens/lirgens.h>

int x86_64_generate_asmblock(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params) {
    switch (h->op) {
        case HIR_RAW: {
            str_info_t si;
            if (!STTB_get_info_id(h->farg->storage.str.s_id, &si, &smt->s)) break;

            int argnum = -1;
            hir_subject_t* arg = NULL;

            const char* p = str_strchr(si.value, '%');
            if (p && str_isdigit((unsigned char)*(++p))) argnum = str_atoi(p);
            if (argnum >= 0) arg = params->data[params->top - argnum].data;
            LIR_BLOCK2(ctx, LIR_RAW, LIR_SUBJ_RAWASM(h->farg->storage.str.s_id), x86_64_format_variable(arg));
            break;
        }

        case HIR_STASM: {
            list_iter_t it;
            list_iter_tinit(&h->targ->storage.list.h, &it);
            hir_subject_t* s;
            while ((s = list_iter_prev(&it))) stack_push(params, s);
            break;
        }

        case HIR_ENDASM: {
            for (int i = 0; i < list_size(&h->targ->storage.list.h); i++) stack_pop(params);
            break;
        }

        default: break;
    }

    return 1;
}