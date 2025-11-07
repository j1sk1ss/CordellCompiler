#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <prep/token.h>
#include <prep/markup.h>

#include <ast/syntax.h>

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/func.h>
#include <hir/loop.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/dag.h>
#include <hir/dfg.h>

#include <lir/lirgen.h>
#include <lir/lirgens/lirgens.h>
#include <lir/instsel/instsel.h>
#include <lir/instsel/x84_64_gnu_nasm.h>
#include <lir/instplan/instplan.h>
#include <lir/instplan/targinfo.h>
#include <lir/instplan/x86_64_gnu_nasm.h>
#include <lir/regalloc/regalloc.h>
#include <lir/regalloc/x84_64_gnu_nasm.h>

#include "ast_helper.h"
#include "hir_helper.h"
#include "dag_helper.h"
#include "lir_helper.h"
#include "ral_helper.h"
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

    hir_ctx_t hirctx = { .h = NULL, .t = NULL };
    HIR_generate(&sctx, &hirctx, &smt);

/*
========================
CFGv1 from HIR...
========================
*/

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&hirctx, &cfgctx);
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
    HIR_CFG_build(&hirctx, &cfgctx);
    HIR_CG_apply_dfe(&cfgctx, &callctx);

    HIR_LOOP_mark_loops(&cfgctx);
    HIR_FUNC_perform_inline(&cfgctx);
    HIR_CFG_unload(&cfgctx);
    HIR_CFG_build(&hirctx, &cfgctx);
    HIR_CG_apply_dfe(&cfgctx, &callctx);
    
/*
========================
SSA form building...
========================
*/

    HIR_CFG_create_domdata(&cfgctx);
    HIR_LTREE_canonicalization(&cfgctx);
    HIR_CFG_unload_domdata(&cfgctx);
    HIR_CFG_create_domdata(&cfgctx);

    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

/*
========================
Loop LICM opt...
========================
*/

    HIR_compute_homes(&hirctx);
    HIR_LTREE_licm(&cfgctx, &smt);

/*
========================
CFG -> DAG
========================
*/

    dag_ctx_t dagctx;
    HIR_DAG_init(&dagctx);
    HIR_DAG_generate(&cfgctx, &dagctx, &smt);
    HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);
    dump_dag_dot(&dagctx, &smt);

/*
========================
Variable deallocation
========================
*/

    HIR_DFG_collect_defs(&cfgctx);
    HIR_DFG_collect_uses(&cfgctx);
    HIR_DFG_compute_inout(&cfgctx);
    HIR_CFG_make_allias(&cfgctx, &smt);
    HIR_DFG_create_deall(&cfgctx, &smt);

/*
========================
HIR debug information...
========================
*/

    printf("\n\n========== HIR ==========\n");
    hir_block_t* hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }

/*
========================
HIR -> LIR...
========================
*/

    HIR_CFG_cleanup_blocks_temporaries(&cfgctx);
    lir_ctx_t lirctx = { .h = NULL, .t = NULL };
    LIR_generate(&cfgctx, &lirctx, &smt);

/*
========================
LIR debug information...
========================
*/

    printf("\n\n========== LIRv1 ==========\n");
    lir_block_t* lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, 1, &smt);
        lh = lh->next;
    }

/*
========================
LIR instruction selection
========================
*/

    inst_selector_h inst_sel = { 
        .select_instructions = x86_64_gnu_nasm_instruction_selection,
        .select_memory       = x86_64_gnu_nasm_memory_selection
    };

    LIR_select_instructions(&cfgctx, &smt, &inst_sel);
    printf("\n\n========== LIR selected instructions ==========\n");
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, 1, &smt);
        lh = lh->next;
    }

/*
========================
LIR instruction planning
========================
*/

    target_info_t trginfo;
    TRGINF_load("/Users/nikolaj/Documents/Repositories/CordellCompiler/src/lir/instplan/Ivy_Bridge.trgcpl", &trginfo);

    inst_planner_t inst_plan = {
        .func_finder     = x86_64_gnu_nasm_planner_get_next_func_abi,
        .func_res_finder = x86_64_gnu_nasm_planner_get_func_res
    };

    LIR_plan_instructions(&cfgctx, &trginfo, &inst_plan);
    TRGINF_unload(&trginfo);

/*
========================
LIR register selection
========================
*/

    map_t colors;
    map_init(&colors);
    LIR_RA_init_colors(&colors, &smt);
    
    regalloc_t regall = { .regallocate = x86_64_regalloc_graph };
    LIR_regalloc(&cfgctx, &smt, &colors, &regall);
    printf("Register colors:\n"); colors_regalloc_dump_dot(&colors);

    LIR_select_memory(&cfgctx, &colors, &smt, &inst_sel);

/*
========================
LIR debug information...
========================
*/

    printf("\n\n========== LIR planned and regalloc ==========\n");
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, 1, &smt);
        lh = lh->next;
    }

/*
========================
Debug information...
========================
*/

    printf("CFGv2:\n"); cfg_print(&cfgctx);
    print_symtab(&smt);

/*
========================
Cleanup...
========================
*/

    LIR_unload_blocks(lirctx.h);
    HIR_DAG_unload(&dagctx);

    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.h);

    list_free_force(&tokens);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    close(fd);
    return 0;
}