#include <hir/cfg.h>

int HIR_FUNC_perform_tre(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (cb->l || cb->jmp) continue;

            func_info_t fi;
            if (!FNTB_get_info_id(fb->fid, &fi, &smt->f)) continue;

            set_foreach (cfg_block_t* p, &cb->pred) {
                hir_block_t* exit = p->hmap.exit;
                if (!HIR_funccall(exit->op)) continue;
                
                if (exit->sarg->storage.str.s_id != fb->fid) continue;
                hir_subject_t* lb = HIR_SUBJ_LABEL();
                hir_block_t* hlb = HIR_create_block(HIR_MKLB, lb, NULL, NULL);

                hir_block_t* hh = fb->entry;
                while (hh && hh->op != HIR_MKSCOPE) hh = hh->next;
                hh = hh->next;
                while (hh && hh->op != HIR_MKSCOPE) hh = hh->next;
                HIR_insert_block_after(hlb, hh);

                hir_block_t* jmp = HIR_create_block(HIR_JMP, lb, NULL, NULL);
                HIR_insert_block_before(jmp, exit);
                
                ast_node_t* ast_arg = fi.args->child;
                foreach (hir_subject_t* arg, &exit->targ->storage.list.h) {
                    if (!ast_arg) break;
                    hir_block_t* argload = HIR_create_block(HIR_STORE, HIR_SUBJ_ASTVAR(ast_arg->child), arg, NULL);
                    HIR_insert_block_before(argload, jmp);
                    ast_arg = ast_arg->sibling;
                }

                exit->unused = 1;
            }
        }
    }

    return 1;
}
