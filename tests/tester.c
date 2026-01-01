#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef PREP_TESTING
    #include <prep/token.h>
    #include <prep/markup.h>
#endif

#ifdef AST_TESTING
    #include <ast/ast.h>
    #include <ast/astgen.h>
    #include <ast/astgen/astgen.h>
    #include "misc/ast_helper.h"
#ifdef AST_OPT_TESTING
    #include <ast/opt/condunroll.h>
    #include <ast/opt/deadscope.h>
#endif
#endif

#ifdef SEM_TESTING
    #include <sem/semantic.h>
#endif

#ifdef HIR_TESTING
    #include <hir/hirgen.h>
    #include <hir/hirgens/hirgens.h>
    #include <hir/cfg.h>
#ifdef HIR_SSA_TESTING
    #include <hir/ssa.h>
#ifdef HIR_DAG_TESTING
    #include <hir/dag.h>
#ifdef CONSTFOLD_TESTING
    #include <hir/constfold.h>
#endif
    #include "misc/dag_helper.h"
#endif
    #include <hir/func.h>
    #include <hir/loop.h>
#endif
    #include "misc/hir_helper.h"
#endif

#ifdef LIR_TESTING
    #include <lir/lirgen.h>
    #include <lir/lirgens/lirgens.h>
#ifdef CONSTFOLD_TESTING
    #include <lir/constfold.h>
#endif
#ifdef LIR_INSTSEL_TESTING
    #include <lir/selector/instsel.h>
    #include <lir/selector/memsel.h>
    #include <lir/selector/savereg.h>
    #include <lir/selector/x84_64_gnu_nasm.h>
#endif
#ifdef LIR_INSTPLAN_TESTING
    #include <lir/instplan/targinfo.h>
    #include <lir/instplan/instplan.h>
#endif
#ifdef LIR_REGALLOC_TESTING
    #include <lir/dfg.h>
    #include <lir/regalloc/regalloc.h>
    #include <lir/regalloc/x84_64_gnu_nasm.h>
    #include "misc/ral_helper.h"
#endif
#ifdef LIR_PEEPHOLE_TESTING
    #include <lir/peephole/peephole.h>
    #include <lir/peephole/x84_64_gnu_nasm.h>
#endif
    #include "misc/lir_helper.h"
#endif

#ifdef CODEGEN_TESTING
    #include <asm/asmgen.h>
    #include <asm/x86_64_asmgen.h>
#endif

#include "misc/symtb_helper.h"

int main(__attribute__ ((unused)) int argc, char* argv[]) {
    mm_init();

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        return 1;
    }

#ifdef SRC_PRINT
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);
#endif

#ifdef PREP_TESTING
    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) { // Analyzation
        fprintf(stderr, "ERROR! tkn == NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens); // Analyzation
    MRKP_variables(&tokens); // Analyzation

#ifdef TOKEN_PRINT
    printf("\nTokens:\n");
    foreach (token_t* h, &tokens) {
        printf(
            "%sline=%i, type=%i, data=[%s], %s%s%s%s\n",
            h->flags.glob ? "glob " : "", 
            h->lnum, 
            h->t_type, 
            h->body->body,
            h->flags.ptr  ? "ptr "  : "", 
            h->flags.ro   ? "ro "   : "",
            h->flags.dref ? "dref " : "",
            h->flags.ref  ? "ref "  : ""
        );
    }
#endif
#endif

    sym_table_t smt;
    SMT_init(&smt);

#ifdef AST_TESTING
    ast_ctx_t sctx = { .r = NULL, .fentry = "_main" };
    stack_init(&sctx.scopes.stack);
    AST_parse_tokens(&tokens, &sctx, &smt); // Analyzation

#ifdef AST_OPT_TESTING
    OPT_condunroll(&sctx); // Transform
    OPT_deadscope(&sctx);  // Transform
#endif

#ifdef AST_PRINT
    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);
#endif
#endif

#ifdef SEM_TESTING
    SEM_perform_ast_check(&sctx, &smt);
#endif

#ifdef HIR_TESTING
    hir_ctx_t hirctx = { NULL };
    HIR_generate(&sctx, &hirctx, &smt);     // Analyzation

    cfg_ctx_t cfgctx = { .cid = 0 };
    HIR_CFG_build(&hirctx, &cfgctx, &smt);  // Analyzation
    printf("CFGv1:\n"); cfg_print(&cfgctx);

    HIR_FUNC_perform_tre(&cfgctx, &smt);    // Transform
    HIR_CFG_unload(&cfgctx);                // Analyzation
    HIR_CFG_build(&hirctx, &cfgctx, &smt);  // Analyzation

    HIR_LOOP_mark_loops(&cfgctx);           // Analyzation
    HIR_FUNC_perform_inline(&cfgctx);       // Transform
    HIR_CFG_unload(&cfgctx);                // Analyzation
    HIR_CFG_build(&hirctx, &cfgctx, &smt);  // Analyzation

    call_graph_t callctx;
    HIR_CG_build(&cfgctx, &callctx, &smt);  // Analyzation
    HIR_CG_perform_dfe(&callctx, &smt);     // Transformation
    HIR_CG_apply_dfe(&cfgctx, &callctx);    // Analyzation
    call_graph_print_dot(&callctx);
    
#ifdef HIR_PRINT
    printf("\n\n========== HIRv1 ==========\n");
    hir_block_t* hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }
#endif

#ifdef HIR_SSA_TESTING
    printf("HIR_CFG_create_domdata...\n");
    HIR_CFG_create_domdata(&cfgctx);        // Analyzation
    printf("HIR_LTREE_canonicalization...\n");
    HIR_LTREE_canonicalization(&cfgctx);    // Transform
    printf("HIR_CFG_unload_domdata...\n");
    HIR_CFG_unload_domdata(&cfgctx);        // Analyzation
    printf("HIR_CFG_create_domdata...\n");
    HIR_CFG_create_domdata(&cfgctx);        // Analyzation

    ssa_ctx_t ssactx;
    map_init(&ssactx.vers, MAP_NO_CMP);
    HIR_SSA_insert_phi(&cfgctx, &smt);      // Transform
    HIR_SSA_rename(&cfgctx, &ssactx, &smt); // Transform

    HIR_compute_homes(&hirctx);             // Analyzation
    HIR_LTREE_licm(&cfgctx, &smt);          // Transform
#ifdef HIR_PRINT
    printf("\n\n========== HIRv2 ==========\n");
    hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }
#endif
#ifdef HIR_DAG_TESTING
    HIR_CFG_make_allias(&cfgctx, &smt);          // Analyzation

    dag_ctx_t dagctx;
    HIR_DAG_init(&dagctx);                       // Analyzation
    HIR_DAG_generate(&cfgctx, &dagctx, &smt);    // Analyzation
    HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);       // Analyzation
    dump_dag_dot(&dagctx, &smt);
#ifdef CONSTFOLD_TESTING
    printf("HIR_sparse_const_propagation...\n");
    HIR_sparse_const_propagation(&dagctx, &smt); // Analyzation
#endif
#ifdef HIR_PRINT
    printf("\n\n========== HIR prepared ==========\n");
    hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }
#endif
#endif
#endif
#endif

#ifdef LIR_TESTING
    HIR_CFG_cleanup_navigation(&cfgctx);
    lir_ctx_t lirctx = { .h = NULL, .t = NULL };
    LIR_generate(&cfgctx, &lirctx, &smt);        // Analyzation
#ifdef LIR_PRINT
    printf("\n\n========== LIRv1 ==========\n");
    lir_block_t* lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif

#ifdef LIR_INSTPLAN_TESTING
    target_info_t trginfo;
    TRGINF_load("/Users/nikolaj/Documents/Repositories/CordellCompiler/src/lir/instplan/Ivy_Bridge.trgcpl", &trginfo);
    LIR_plan_instructions(&cfgctx, &trginfo); // Transform
    TRGINF_unload(&trginfo);
#ifdef LIR_PRINT
    printf("\n\n========== LIR planned instructions ==========\n");
    lir_printer_reset();
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif
#endif

#ifdef LIR_INSTSEL_TESTING
    inst_selector_h inst_sel = { 
        .select_instructions = x86_64_gnu_nasm_instruction_selection,
    };

    LIR_select_instructions(&cfgctx, &smt, &inst_sel); // Transform
#ifdef LIR_PRINT
    printf("\n\n========== LIR selected instructions ==========\n");
    lir_printer_reset();
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif
#endif

    SMT_compress(&smt);

#ifdef CONSTFOLD_TESTING
    printf("LIR_apply_sparse_const_propagation...\n");
    LIR_apply_sparse_const_propagation(&cfgctx, &smt); // Transform
#ifdef LIR_PRINT
    printf("\n\n========== LIR const folded ==========\n");
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif
#endif

#ifdef LIR_REGALLOC_TESTING
    printf("LIR_DFG_collect_defs...\n");
    LIR_DFG_collect_defs(&cfgctx);       // Analyzation
    printf("LIR_DFG_collect_uses...\n");
    LIR_DFG_collect_uses(&cfgctx);       // Analyzation
    printf("LIR_DFG_compute_inout...\n");
    LIR_DFG_compute_inout(&cfgctx);      // Analyzation
    printf("LIR_DFG_create_deall...\n");
    LIR_DFG_create_deall(&cfgctx, &smt); // Transform

    map_t colors;
    map_init(&colors, MAP_NO_CMP);
    printf("LIR_RA_init_colors...\n");
    LIR_RA_init_colors(&colors, &smt);
    
    regalloc_t regall = { .regallocate = x86_64_regalloc_graph };
    printf("LIR_regalloc...\n");
    LIR_regalloc(&cfgctx, &smt, &colors, &regall); // Analyzation
    LIR_apply_regalloc(&smt, &colors);             // Analyzation

    mem_selector_h mem_sel = { 
        .select_memory = x86_64_gnu_nasm_memory_selection
    };

    printf("LIR_select_memory...\n");
    LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel); // Transfor
#ifdef LIR_PRINT
    printf("Register colors:\n"); colors_regalloc_dump_dot(&colors);
    printf("\n\n========== LIR planned and regalloc ==========\n");
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif
#endif

#ifdef LIR_PEEPHOLE_TESTING
    peephole_t pph = { .perform_peephole = x86_64_gnu_nasm_peephole_optimization };
    LIR_peephole_optimization(&cfgctx, &pph);
#ifdef LIR_PRINT
    printf("\n\n========== LIR peephole optimization ==========\n");
    lh = lirctx.h;
    while (lh) {
        print_lir_block(lh, &smt);
        lh = lh->next;
    }
#endif
    register_saver_h reg_save = {
        .save_registers = x86_64_gnu_nasm_caller_saving
    };

    printf("LIR_save_registers...\n");
    HIR_CFG_cleanup_navigation(&cfgctx);
    LIR_save_registers(&cfgctx, &reg_save);
#endif
#endif

#ifdef CODEGEN_TESTING
    printf("\n\n========== Final ASM ==========\n");
    asm_gen_t asmgen = { .generator = x86_64_generate_asm };
    ASM_generate(&lirctx, &smt, &asmgen, stdout);
#endif

#ifdef HIR_TESTING
    printf("CFGv2:\n"); cfg_print(&cfgctx);
#endif
    print_symtab(&smt);

#ifdef LIR_REGALLOC_TESTING
    map_free(&colors);
#endif
#ifdef LIR_TESTING
    LIR_unload_blocks(lirctx.h);
#endif
#ifdef HIR_DAG_TESTING
    HIR_DAG_unload(&dagctx);
#endif

#ifdef HIR_TESTING
    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.h);
#endif

#ifdef PREP_TESTING
    list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
#endif
#ifdef AST_TESTING
    stack_free(&sctx.scopes.stack);
    AST_unload(sctx.r);
#endif

    SMT_unload(&smt);
    close(fd);

    fprintf(stdout, "Allocated: %i\n", mm_get_allocated());
    return EXIT_SUCCESS;
}
