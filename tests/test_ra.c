#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <prep/token.h>
#include <prep/markup.h>

#include <ast/astgen.h>

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/dfg.h>
#include <hir/ra.h>

#include "ast_helper.h"
#include "hir_helper.h"
#include "ral_helper.h"
#include "symtb_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        return 1;
    }

    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    sym_table_t smt;
    SMT_init(&smt);
    ast_ctx_t sctx = { .r = NULL };

    AST_parse_tokens(&tokens, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&irctx, &cfgctx);
    
    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    LIR_DFG_collect_defs(&cfgctx);
    LIR_DFG_collect_uses(&cfgctx);
    LIR_DFG_compute_inout(&cfgctx);
    HIR_CFG_make_allias(&cfgctx, &smt);
    LIR_DFG_create_deall(&cfgctx, &smt);

    cfg_print(&cfgctx);

    igraph_t ig;
    LIR_RA_build_igraph(&cfgctx, &ig, &smt);

    map_t clrs;
    LIR_RA_color_igraph(&ig, &clrs);
    igraph_dump_dot(&ig);
    map_free(&clrs);

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    print_symtab(&smt);

    LIR_RA_unload_igraph(&ig);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(irctx.h);
    list_free_force(&tokens);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    close(fd);
    return 0;
}
