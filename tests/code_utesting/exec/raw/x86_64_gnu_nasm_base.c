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
#include <ast/devirt.h>

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/cfg.h>
#include <hir/ssa.h>
#include <hir/func.h>
#include "../../../misc/cfg_helper.h"

#include <lir/lirgen.h>
#include <lir/lirgens/lirgens.h>
#include <lir/selector/instsel.h>
#include <lir/selector/memsel.h>
#include <lir/selector/savereg.h>
#include <lir/selector/x86_64_gnu_nasm.h>
#include <lir/dfg.h>
#include <lir/regalloc/ra.h>
#include <lir/regalloc/regalloc.h>
#include <lir/regalloc/x86_64_gnu_precolor.h>
#include <lir/copyprop.h>
#include <lir/dump.h>

#include <asm/asmgen.h>
#include <asm/x86_64_gnu_nasm_asmgen.h>

#define RELOAD_CFG                          \
    HIR_CFG_unload(&cfgctx);                \
    HIR_CFG_build(&hirctx, &cfgctx, &smt);  \
    HIR_CG_unload(&callctx);                \
    HIR_CG_build(&cfgctx, &callctx, &smt);  \
    HIR_CG_perform_dfe(&callctx, &smt);     \
    HIR_CG_apply_dfe(&cfgctx, &smt);

static int unload_token_list(list_t* tokens) {
    if (!tokens) return 0;
    list_free_force_op(tokens, (int (*)(void *))TKN_unload_token);
    mm_free(tokens);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough arguments! Expected at least 3, got %i!\n", argc);
        return 1;
    }

    mm_init();

    config_t cfg = {
        .system.entry_name   = "_main",
        .system.ro_section   = ".rodata",
        .system.glob_section = ".data",
        .system.code_section = ".text",
        .system.bytness = {
            .bytness   = 8,
            .h_bytness = 4,
            .q_bytness = 2,
            .e_bytness = 1
        },
        .system.sys_type = LINUX64,
    };
    CONF_set_config(cfg);

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    list_t token_lists;
    list_init(&token_lists);

    const char* include_root = argv[argc - 1];
    for (int i = 1; i < argc - 1; i++) {
        const char* input_file = argv[i];
        int fd = open(input_file, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s isn't found!\n", input_file);
            return 1;
        }

        finder_ctx_t finctx = { .bpath = include_root };
        
        deftb_t macros;
        MCTB_init(&macros);
        PP_predefine(&macros);

        pp_ctx_t ppctx;
        PP_init_pp_ctx(&ppctx);
        
        fd = PP_perform(fd, &finctx, &ppctx, &macros);
        MCTB_unload(&macros);
        if (fd < 0) {
            fprintf(stderr, "Processed file %s isn't found!\n", input_file);
            return 1;
        }

        list_t* tokens = (list_t*)mm_malloc(sizeof(list_t));
        if (!tokens) {
            fprintf(stderr, "Can't allocate token list\n");
            return 1;
        }

        list_init(tokens);
        if (!list_push_back(&token_lists, tokens)) {
            mm_free(tokens);
            fprintf(stderr, "Can't save token list\n");
            return 1;
        }

        if (!TKN_tokenize(fd, tokens) || !list_size(tokens)) {
            fprintf(stderr, "ERROR! tkn == NULL!\n");
            return 1;
        }

        MRKP_mnemonics(tokens);
        MRKP_variables(tokens);

        if (!AST_parse_tokens(tokens, &sctx, &smt)) {
            fprintf(stderr, "AST tree creation error!\n");
            return 1;
        }

        close(fd);
    }

    AST_finalize_parse(&sctx, &smt);

    hir_ctx_t hirctx = { 0 };
    HIR_generate(&sctx, &hirctx, &smt);

    call_graph_t callctx;
    cfg_ctx_t cfgctx = { .cid = 0 };
    HIR_CFG_build(&hirctx, &cfgctx, &smt);
    HIR_CG_build(&cfgctx, &callctx, &smt);

    HIR_FUNC_set_last_return(&cfgctx);
    HIR_FUNC_set_defer_calls(&cfgctx);

    RELOAD_CFG;

    HIR_CFG_create_domdata(&cfgctx);
    ltree_ctx_t lctx;
    map_init(&lctx.lmap, MAP_NO_CMP);
    HIR_LOOP_mark_loops(&cfgctx, &lctx);

    HIR_CFG_finalize_before_dom(&cfgctx);
    HIR_LTREE_canonicalization(&cfgctx, &lctx);
    HIR_CFG_unload_domdata(&cfgctx);
    HIR_CFG_create_domdata(&cfgctx);

    ssa_ctx_t ssactx;
    map_init(&ssactx.vers, MAP_NO_CMP);
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);
    map_free_force(&ssactx.vers);

    HIR_compute_homes(&hirctx);
    HIR_CFG_make_allias(&cfgctx, &smt);

    lir_ctx_t lirctx = { .h = NULL, .t = NULL };
    LIR_generate(&cfgctx, &lirctx, &smt);
    inst_selector_t inst_sel = { .select_instructions = x86_64_gnu_nasm_instruction_selection };
    LIR_select_instructions(&cfgctx, &smt, &inst_sel);

    LIR_DFG_compute_inout(&cfgctx);
    LIR_DFG_create_deall(&cfgctx, &smt);

    map_t colors;
    map_init(&colors, MAP_NO_CMP);
    LIR_RA_init_colors(&colors, &smt, x86_64_gnu_precolored_reg_to_color);
    LIR_regalloc(&cfgctx, &smt, &colors);

    mem_selector_t mem_sel = { 
        .select_memory   = x86_64_gnu_nasm_memory_selection, 
        .validate_memory = x86_64_gnu_nasm_memory_validation 
    };
    LIR_RA_sort_phi_movs(&cfgctx, &colors);
    LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel);

    LIR_destroy_ssa(&cfgctx);

    register_saver_t reg_save = { .save_registers = x86_64_gnu_nasm_caller_saving };
    LIR_save_registers(&cfgctx, &callctx, &smt, &reg_save);

    LIR_validate_memory(&cfgctx, &smt, &mem_sel);
    LIR_clear_global_variables(&cfgctx, &smt);

    asm_gen_t asmgen = { .generator = x86_64_gnu_nasm_generate_asm };
    ASM_generate(&cfgctx, &smt, &asmgen, stdout);

    map_free(&colors);
    LIR_unload_blocks(lirctx.h);
    HIR_LTREE_unload_ctx(&lctx);
    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.hot.h);
    list_free_force_op(&token_lists, (int (*)(void *))unload_token_list);
    AST_unload_ctx(&sctx);

    SMT_unload(&smt);
    return EXIT_SUCCESS;
}
