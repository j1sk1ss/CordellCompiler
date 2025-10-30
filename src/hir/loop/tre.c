#include <hir/loop.h>

int HIR_TRE_perform(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            if (cb->l || cb->jmp) continue;

            func_info_t fi;
            if (!FNTB_get_info_id(fb->fid, &fi, &smt->f)) continue;

            set_iter_t it;
            cfg_block_t* p;
            set_iter_init(&cb->pred, &it);
            while (set_iter_next(&it, (void**)&p)) {
                hir_block_t* exit = p->hmap.exit;
                if (
                    exit->op != HIR_FCLL && exit->op != HIR_STORE_FCLL &&
                    exit->op != HIR_ECLL && exit->op != HIR_STORE_ECLL
                ) continue;
                
                if (exit->sarg->storage.str.s_id != fb->fid) continue;
                hir_subject_t* lb = HIR_SUBJ_LABEL();
                hir_block_t* hlb = HIR_create_block(HIR_MKLB, lb, NULL, NULL);
                HIR_insert_block_after(hlb, fb->entry);

                hir_block_t* jmp = HIR_create_block(HIR_JMP, lb, NULL, NULL);
                HIR_insert_block_before(jmp, exit);
                
                list_iter_t args;
                hir_subject_t* arg;
                ast_node_t* ast_arg = fi.args->child;
                list_iter_hinit(&exit->targ->storage.list.h, &args);
                while ((arg = list_iter_next(&args)) && ast_arg) {
                    hir_block_t* argload = HIR_create_block(HIR_STORE, HIR_SUBJ_ASTVAR(ast_arg->child), arg, NULL);
                    HIR_insert_block_before(argload, jmp);
                    ast_arg = ast_arg->sibling;
                }

                p->hmap.exit = jmp;
                p->l         = NULL;
                p->jmp       = list_get_head(&fb->blocks);

                HIR_unlink_block(exit);
                HIR_unload_blocks(exit);
            }
        }
    }

    return 1;
}
