#include <hir/func.h>

int HIR_FUNC_perform_tre(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (
                cb->l || /* Skip all blocks that have a linked successor */
                cb->jmp  /* That means, that we skip all non-end blocks  */
            ) continue;
            
            func_info_t fi;
            if (!FNTB_get_info_id(fb->fid, &fi, &smt->f)) continue;
            hir_block_t* exit = cb->hmap.exit;
            
            while (
                exit && exit != cb->hmap.entry && /* Chieck the basic equation                */
                (
                    exit->op == HIR_FEND     ||   /* If this is a system fend instruction     */ 
                    exit->op == HIR_ENDSCOPE ||   /* If this is a system endscope instruction */
                    exit->op == HIR_FRET          /* If this is an end return statement       */
                )
            ) exit = exit->prev; 
            
            if (
                !HIR_funccall(exit->op) ||              /* If this isn't a function call instruction */
                exit->sarg->storage.str.s_id != fb->fid /* or this isn't a self-call.                */
            ) continue;                                 /* We skip such instructions.                */
            
            /* Skip the function's preambule:
                - function definition
                - function arguments */
            hir_block_t* hh = HIR_get_next(fb->hmap.entry, fb->hmap.exit, 0);
            while (hh && hh->op != HIR_MKSCOPE) hh = HIR_get_next(hh, fb->hmap.exit, 1);
            hh = HIR_get_next(hh, fb->hmap.exit, 1);
            while (hh && hh->op != HIR_MKSCOPE) hh = HIR_get_next(hh, fb->hmap.exit, 1);
            if (!hh) continue;

            /* Create the 'reverse' label.
                The idea here is making the point to return at the function's start:
                ```cpl
                function foo() {
                lbX:
                something();
                goto lbX;
                }
                ``` */
            hir_subject_t* lb = NULL;
            if (hh->next && hh->next->op == HIR_MKLB) lb = hh->next->farg; /* If the label already allocated, use it instead 
                                                                              of a new one. */
            else {
                lb = HIR_SUBJ_LABEL();
                if (!lb) {
                    HIR_unload_subject(lb);
                    continue;
                }

                hir_block_t* hlb = HIR_create_block(HIR_MKLB, lb, NULL, NULL);
                HIR_insert_block_after(hlb, hh);
            }
            
            hir_block_t* jmp = HIR_create_block(HIR_JMP, lb, NULL, NULL);
            if (!jmp) {
                HIR_unload_blocks(jmp);
                continue;
            }

            HIR_insert_block_before(jmp, exit);
            
            /* Reassign function arguments with a new set of values.
                For instance:
                ```cpl
                function foo(i32 a) {
                lbX:
                    : return foo(a + 10); :
                    a = a + 10;
                goto lbX;
                }
                ``` */
            ast_node_t* ast_arg = fi.args->c;
            foreach (hir_subject_t* arg, &exit->targ->storage.list.h) {
                if (!ast_arg) break;
                hir_block_t* argload = HIR_create_block(HIR_STORE, HIR_SUBJ_ASTVAR(ast_arg->c), arg, NULL);
                if (!argload) continue;

                HIR_insert_block_before(argload, jmp);
                ast_arg = ast_arg->siblings.n;
            }

            exit->unused = 1;
        }
    }

    return 1;
}
