#include <hir/func.h>

int HIR_FUNC_perform_devirt(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                func_info_t fi;
                if (HIR_is_funccall(hh->op) && FNTB_get_info_id(hh->sarg->storage.str.s_id, &fi, &smt->f)) {
                    list_t funcs;
                    list_init(&funcs);
                    if (FNTB_collect_info(fi.name, &funcs, &smt->f) && list_size(&funcs) > 1) {
                        int most_fit = -99;
                        foreach (func_info_t* func, &funcs) {
                            int fargs = 0, fits = 4, vargs = 0;
                            fn_iterate_args (func) {
                                if (arg->t->t_type == VAR_ARGUMENTS_TOKEN) vargs = 1;
                                fargs++;
                            }
                            
                            int arg_count = list_size(&hh->targ->storage.list.h);
                            if (arg_count != fargs && !vargs) {
                                continue;
                            }
                            
                            int arg_index = 0;
                            void** args = list_flatten(&hh->targ->storage.list.h);
                            if (args) {
                                fn_iterate_args (func) {
                                    if (arg_count <= arg_index) break;
                                    hir_subject_t* hir_arg = (hir_subject_t*)args[arg_index++];
                                    if (!hir_arg) continue;
                                    if (HIR_get_convop(hir_arg->t) != HIR_get_convop(HIR_get_tmptype_tkn(arg->t, 1))) {
                                        fits--;
                                    }
                                }

                                mm_free(args);
                            }

                            if (fits > most_fit) { // TODO: Move to hirgen, 'cause we need to figure out the further casts, etc.
                                if (
                                    hh->farg && // TODO: Add default arguments too
                                    func->rtype
                                ) hh->farg->t = HIR_get_tmptype_tkn(func->rtype->t, 1);
                                hh->sarg->storage.str.s_id = func->id;
                                most_fit = fits;
                            }    
                        }
                    }

                    list_free(&funcs);
                }

                hh = HIR_get_next(hh, bb->hmap.exit, 1);
            }
        }
    }

    return 1;
}
