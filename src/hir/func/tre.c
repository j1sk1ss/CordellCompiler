#include <hir/func.h>

int HIR_FUNC_perform_tre(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (
                cb->l || /* Skip all blocks that have a linked successor */
                cb->jmp  /* which will save only end (return) blocks.    */
            ) continue;
            
            func_info_t fi;
            if (!FNTB_get_info_id(fb->f_id, &fi, &smt->f)) continue;
            hir_block_t* exit = cb->hmap.exit;
            
            while ( /* We need to climb a bit upper in a function to point exactly at the term command */
                exit && exit != cb->hmap.entry && /* Check whether this is an end or not.     */
                (
                    exit->op == HIR_FEND     ||   /* If this is a system fend instruction     */ 
                    exit->op == HIR_ENDSCOPE ||   /* If this is a system endscope instruction */
                    exit->op == HIR_FRET          /* If this is an end return statement       */
                )
            ) exit = exit->prev; 
            
            if (
                !HIR_is_funccall(exit->op) ||            /* If this isn't a function call instruction */
                exit->sarg->storage.str.s_id != fb->f_id /* or this isn't a self-call,                */
            ) continue;                                  /* we skip it                                */
            
            /* Now we need to point to the function's start. First we need
               to skip the function's preambule:
                - function definition
                - function arguments */
            hir_block_t* hh = HIR_FUNC_get_next(NULL, fb, NULL, 0);
            while (hh && hh->op != HIR_MKSCOPE) hh = HIR_FUNC_get_next(hh, fb, NULL, 1);
            hh = HIR_FUNC_get_next(hh, fb, NULL, 1);
            while (hh && hh->op != HIR_MKSCOPE) hh = HIR_FUNC_get_next(hh, fb, NULL, 1);
            if (!hh) continue;

            /* Then we need to create a 'reverse' label.
               The idea here is to make a point where we can return dead-flow:
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
                if (!lb) continue;
                hir_block_t* hlb = HIR_create_block(HIR_MKLB, lb, NULL, NULL);
                HIR_insert_block_after(hlb, hh);
            }
            
            hir_block_t* jmp = HIR_create_block(HIR_JMP, lb, NULL, NULL);
            if (!jmp) continue;
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
