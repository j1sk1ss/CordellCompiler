#include <hir/func.h>

int HIR_FUNC_perform_devirt(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                func_info_t fi;
                if (HIR_funccall(hh->op) && FNTB_get_info_id(hh->sarg->storage.str.s_id, &fi, &smt->f)) {
                    list_t funcs;
                    list_init(&funcs);
                    if (FNTB_collect_info(fi.name, &funcs, &smt->f) && list_size(&funcs) > 1) {
                        int most_fit = -99;
                        foreach (func_info_t* func, &funcs) {
                            int fargs = 0, fits = 4;
                            fn_iterate_args(func) {
                                fargs++;
                            }
                            
                            /* Check the arguments size */
                            if (list_size(&hh->targ->storage.list.h) != fargs) {
                                continue; /* Fatal */
                            }
                            
                            int arg_index = 0;
                            fn_iterate_args(func) {
                                int hir_arg_index = 0;
                                foreach (hir_subject_t* hir_arg, &hh->targ->storage.list.h) {
                                    if (hir_arg_index++ < arg_index) continue;
                                    if (HIR_convop(hir_arg->t) != HIR_convop(HIR_get_tmptype_tkn(arg->t, 1))) {
                                        fits--; /* Argument doesn't match */
                                    }

                                    break;
                                }

                                arg_index++;
                            }
                                           
                            if (fits > most_fit) {
                                if ( /* Change the destination variable */
                                    hh->farg && 
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
