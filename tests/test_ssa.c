#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/loop.h>
#include "ast_helper.h"
#include "hir_helper.h"
#include "symtb_helper.h"

int main(int argc, char* argv[]) {
    printf("Running test %s...\n", argv[0]);
    mm_init();

/*
========================
Source code reading...
========================
*/

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        return 1;
    }

    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

/*
========================
Tokenization...
========================
*/

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

/*
========================
Tokens preparation...
========================
*/

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

/*
========================
AST generation...
========================
*/

    sym_table_t smt;
    SMT_init(&smt);
    syntax_ctx_t sctx = { .r = NULL };
    STX_create(&tokens, &sctx, &smt);
    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

/*
========================
AST -> HIR...
========================
*/

    hir_ctx_t irctx = { .h = NULL, .t = NULL };
    HIR_generate(&sctx, &irctx, &smt);

/*
========================
CFGv1 from HIR...
========================
*/

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&irctx, &cfgctx);
    printf("CFGv1:\n"); cfg_print(&cfgctx);
    
/*
========================
Call graph building...
========================
*/

    call_graph_t callctx;
    HIR_CG_build(&cfgctx, &callctx, &smt);
    HIR_CG_perform_dfe(&cfgctx, &callctx, &smt);
    HIR_CG_apply_dfe(&cfgctx, &callctx);
    call_graph_print_dot(&callctx);

/*
========================
CFGv2 from HIR and call graph after TRE and inline optimization...
========================
*/

    HIR_CFG_perform_tre(&cfgctx, &smt);
    HIR_CFG_unload(&cfgctx);
    HIR_CFG_build(&irctx, &cfgctx);
    HIR_CG_apply_dfe(&cfgctx, &callctx);

    HIR_LOOP_mark_loops(&cfgctx);
    HIR_FUNC_perform_inline(&cfgctx);
    HIR_CFG_unload(&cfgctx);
    HIR_CFG_build(&irctx, &cfgctx);
    HIR_CG_apply_dfe(&cfgctx, &callctx);
    
/*
========================
SSA form building...
========================
*/

    HIR_CFG_create_domdata(&cfgctx);
    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

/*
========================
Loop LICM opt...
========================
*/

    HIR_compute_homes(&irctx);
    HIR_LTREE_licm_canonicalization(&cfgctx, &smt);

/*
========================
Debug information...
========================
*/

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    printf("CFGv2:\n"); cfg_print(&cfgctx);
    print_symtab(&smt);

/*
========================
Cleanup...
========================
*/

    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(irctx.h);
    list_free_force(&tokens);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    close(fd);
    return 0;
}
