#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <preproc/pp.h>
#include <prep/token.h>
#include <prep/markup.h>

#include <ast/ast.h>
#include <ast/astgen.h>
#include <ast/astgen/astgen.h>
#include "../../../misc/ast_helper.h"

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/func.h>
#include <hir/dag.h>
#include <hir/constfold.h>
#include "../../../misc/hir_helper.h"

#include <lir/lirgen.h>
#include <lir/lirgens/lirgens.h>
#include <lir/constfold.h>
#include <lir/selector/instsel.h>
#include <lir/selector/memsel.h>
#include <lir/selector/savereg.h>
#include <lir/selector/x84_64_gnu_nasm.h>
#include <lir/selector/x84_64_macho_nasm.h>
#include <lir/peephole/peephole.h>
#include <lir/peephole/x84_64_gnu_nasm.h>
#include <lir/dfg.h>
#include <lir/regalloc/ra.h>
#include <lir/regalloc/regalloc.h>
#include <lir/regalloc/x84_64_gnu_nasm.h>
#include "../../../misc/lir_helper.h"

#include <asm/asmgen.h>
#include <asm/x86_64_gnu_nasm_asmgen.h>
#include <asm/x86_64_macho_nasm_asmgen.h>

#define RELOAD_CFG                          \
    HIR_CFG_unload(&cfgctx);                \
    HIR_CFG_build(&hirctx, &cfgctx, &smt);  \
    HIR_CG_unload(&callctx);                \
    HIR_CG_build(&cfgctx, &callctx, &smt);  \
    HIR_CG_perform_dfe(&callctx, &smt);     \
    HIR_CG_apply_dfe(&cfgctx, &callctx);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments! Expected 3, got %i!\n", argc);
        return 1;
    }

    mm_init();

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s isn't found!\n", argv[1]);
        return 1;
    }

    finder_ctx_t finctx = { .bpath = argv[2] };
    fd = PP_perform(fd, &finctx);
    if (fd < 0) {
        fprintf(stderr, "Processed file %s isn't found!\n", argv[1]);
        return 1;
    }

    char pdata[2048] = { 0 };
    pread(fd, pdata, 2048, 0);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens) || !list_size(&tokens)) {
        fprintf(stderr, "ERROR! tkn == NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    if (!AST_parse_tokens(&tokens, &sctx, &smt)) {
        fprintf(stderr, "AST tree creation error!\n");
        return 1;
    }

    hir_ctx_t hirctx = { 0 };
    HIR_generate(&sctx, &hirctx, &smt);

    call_graph_t callctx;
    cfg_ctx_t cfgctx = { .cid = 0 };
    HIR_CFG_build(&hirctx, &cfgctx, &smt);
    HIR_CG_build(&cfgctx, &callctx, &smt);

    HIR_FUNC_set_last_return(&cfgctx);
    HIR_FUNC_perform_tre(&cfgctx, &smt);

    RELOAD_CFG; // Rebuild after Last_ret + TRE

    HIR_CFG_create_domdata(&cfgctx);
    ltree_ctx_t lctx;
    map_init(&lctx.lmap, MAP_NO_CMP);
    HIR_LOOP_mark_loops(&cfgctx, &lctx);
    
    HIR_FUNC_perform_inline(&cfgctx, &lctx, &smt, HIR_FUNC_inline_euristic_desider);

    RELOAD_CFG; // Rebuild after inlined functions

    HIR_CFG_finilize_before_dom(&cfgctx);
    HIR_LTREE_canonicalization(&cfgctx, &lctx);
    HIR_CFG_unload_domdata(&cfgctx);
    HIR_CFG_create_domdata(&cfgctx);

    ssa_ctx_t ssactx;
    map_init(&ssactx.vers, MAP_NO_CMP);
    HIR_SSA_insert_phi(&cfgctx, &smt);      // Transform
    HIR_SSA_rename(&cfgctx, &ssactx, &smt); // Transform
    map_free_force(&ssactx.vers);

    HIR_compute_homes(&hirctx);
    HIR_LTREE_licm(&cfgctx, &lctx, &smt);

    HIR_CFG_make_allias(&cfgctx, &smt);
    dag_ctx_t dagctx = { .curr_id = 0 };
    HIR_DAG_init(&dagctx);                       // Analyzation
    HIR_DAG_generate(&cfgctx, &dagctx, &smt);    // Analyzation
    HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);

    HIR_sparse_const_propagation(&dagctx, &smt);
    HIR_CFG_squeeze_blocks(&cfgctx);
    lir_ctx_t lirctx = { .h = NULL, .t = NULL };
    LIR_generate(&cfgctx, &lirctx, &smt);
    inst_selector_t inst_sel = { .select_instructions = x86_64_macho_nasm_instruction_selection };
    LIR_select_instructions(&cfgctx, &smt, &inst_sel); // Transform

    LIR_DFG_compute_inout(&cfgctx);      // Analyzation
    LIR_DFG_create_deall(&cfgctx, &smt); // Transform

    map_t colors;
    map_init(&colors, MAP_NO_CMP);
    LIR_RA_init_colors(&colors, &smt);
    
    regalloc_t regall = { .regallocate = x86_64_regalloc_graph };
    LIR_regalloc(&cfgctx, &smt, &colors, &regall);      // Analyzation

    mem_selector_t mem_sel = { .select_memory = x86_64_macho_nasm_memory_selection };
    LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel); // Transform

    register_saver_t reg_save = { .save_registers = x86_64_macho_nasm_caller_saving };
    LIR_save_registers(&cfgctx, &smt, &reg_save);

    peephole_t pph = { .perform_peephole = x86_64_gnu_nasm_peephole_optimization };
    LIR_peephole_optimization(&cfgctx, &pph);

    asm_gen_t asmgen = { .generator = x86_64_macho_nasm_generate_asm };
    ASM_generate(&cfgctx, &smt, &asmgen, stdout);

    map_free(&colors);
    LIR_unload_blocks(lirctx.h);
    HIR_LTREE_unload_ctx(&lctx);
    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.hot.h);
    list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
    AST_unload_ctx(&sctx);

    SMT_unload(&smt);
    close(fd);
    return EXIT_SUCCESS;
}
