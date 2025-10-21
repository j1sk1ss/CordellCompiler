#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <prep/token.h>
#include <prep/token_types.h>
#include <prep/markup.h>

#include <ast/syntax.h>

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/dfg.h>
#include <hir/ra.h>
#include <hir/dag.h>

#include <lir/lirgen.h>
#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

#include <asm/asmgen.h>
#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

#include "ast_helper.h"
#include "hir_helper.h"
#include "dag_helper.h"
#include "lir_helper.h"
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
    syntax_ctx_t sctx = { .r = NULL };

    STX_create(&tokens, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t hirctx = { .h = NULL, .t = NULL };
    HIR_generate(&sctx, &hirctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&hirctx, &cfgctx);
    
    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    dag_ctx_t dagctx;
    HIR_DAG_init(&dagctx);
    HIR_DAG_generate(&cfgctx, &dagctx, &smt);
    HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);
    
    dump_dag_dot(&dagctx, &smt);

    HIR_DFG_collect_defs(&cfgctx);
    HIR_DFG_collect_uses(&cfgctx);
    HIR_DFG_compute_inout(&cfgctx);
    HIR_CFG_make_allias(&cfgctx, &smt);
    HIR_DFG_create_deall(&cfgctx, &smt);

    cfg_print(&cfgctx);

    igraph_t ig;
    HIR_RA_build_igraph(&cfgctx, &ig, &smt);

    map_t clrs;
    HIR_RA_color_igraph(&ig, &clrs);
    igraph_dump_dot(&ig);

    printf("\n\n========== SSA HIR ==========\n");
    hir_block_t* hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }

    lir_ctx_t lirctx = { .h = NULL, .t = NULL, .vars = &clrs };
    lir_gen_t lirgen = {
        .generate = x86_64_generate_lir,
        .mvclean  = x86_64_clean_mov
    };

    LIR_generate(&hirctx, &lirgen, &lirctx, &smt);

    printf("\n\n========== x86_64 LIR ==========\n");
    lir_block_t* lh = lirctx.h;
    while (lh) {
        print_lir_block(lh);
        lh = lh->next;
    }

    printf("\n\n========== x86_64 ASM ==========\n");
    asm_gen_t asmgen = { .generator = x86_64_generate_asm };
    ASM_generate(&lirctx, &smt, &asmgen, stdout);

    print_symtab(&smt);

    HIR_DAG_unload(&dagctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.h);
    LIR_unload_blocks(lirctx.h);
    list_free_force(&tokens);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    close(fd);
    return 0;
}
