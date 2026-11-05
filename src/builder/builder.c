/* Basically, this is the main logic of the compiler.
   There you can see how CLI arguments affect the process,
   changes the output and invoke optimizations.

   If you want to alter the file, feel free to do so. But, remember
   to split logic among the files (in the directory). Previous
   versions of the builder were huge and I was forced to split
   it to different files.  */
#include <builder.h>
// idk
#include <gem_data.h>

static int _generate_ast(options_t* opts, ast_ctx_t* sctx, sym_table_t* smt, deftb_t* macros, list_t* ttokens) {
    foreach (char* input_file, &opts->locations.files) {
        int fd = open(input_file, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s isn't found!\n", input_file);
            return 0;
        }

        finder_ctx_t finctx = {
            .bpath = opts->locations.include,
            .spath = opts->locations.stdlib
        };

        pp_ctx_t ppctx;
        PP_init_pp_ctx(&ppctx);

        fd = PP_perform(fd, &finctx, &ppctx, macros);
        if (fd < 0) {
            fprintf(stderr, "Failed to preprocess %s\n", input_file);
            return 0;
        }

        if (opts->flags.preprocess_only) {
            FILE* output = stdout;
            if (opts->locations.output) {
                output = fopen(opts->locations.output, "w");
                if (!output) {
                    fprintf(stderr, "Can't open output file %s: %s\n", opts->locations.output, strerror(errno));
                    close(fd);
                    return 0;
                }
            }

            if (!BUILD_copy_fd_to_stream(fd, output)) {
                fprintf(stderr, "Can't write preprocessed output for %s\n", input_file);
                if (output != stdout) fclose(output);
                close(fd);
                return 0;
            }

            if (output != stdout) fclose(output);
            close(fd);
            continue;
        }

        list_t* tokens = (list_t*)mm_malloc(sizeof(list_t));
        if (!tokens) {
            fprintf(stderr, "Can't allocate token list\n");
            close(fd);
            return 0;
        }

        list_init(tokens);
        if (!list_push_back(ttokens, tokens)) {
            mm_free(tokens);
            fprintf(stderr, "Can't save token list\n");
            close(fd);
            return 0;
        }

        if (!TKN_tokenize(fd, tokens) || !list_size(tokens)) {
            fprintf(stderr, "ERROR! tkn == NULL!\n");
            close(fd);
            return 0;
        }

        MRKP_mnemonics(tokens);
        MRKP_variables(tokens);

        if (!AST_parse_tokens(tokens, sctx, smt)) {
            fprintf(stderr, "AST tree creation error!\n");
            close(fd);
            return 0;
        }

        close(fd);
    }
    return 1;
}

int BUILD_start(int argc, char* argv[]) {
    if (argc == 1) {
        BUILD_print_help_message();
        return EXIT_FAILURE;
    }

    mm_init();

    options_t options;
    if (!BUILD_parse_input_args(argv, argc, &options)) {
        fprintf(stderr, "Can't parse input arguments\n");
        return EXIT_FAILURE;
    }

    if (options.flags.show_help) {
        BUILD_print_help_message();
        BUILD_unload_options(&options);
        return EXIT_SUCCESS;
    }
    else if (options.flags.show_something) {
        fwrite(gem_data, 1, gem_data_len, stdout);
        BUILD_unload_options(&options);
        return EXIT_SUCCESS;
    }
    else if (options.flags.show_version) {
        BUILD_print_version(stdout);
        BUILD_unload_options(&options);
        return EXIT_SUCCESS;
    }

    char stdlib_path[PATH_MAX] = { 0 }, runtime_path[PATH_MAX] = { 0 };
    options.locations.stdlib   = BUILD_find_stdlib(argv[0], stdlib_path, sizeof(stdlib_path));
    options.locations.runtime  = BUILD_find_runtime_library(argv[0], runtime_path, sizeof(runtime_path));

    if (options.flags.print_stdlib) {
        if (options.locations.stdlib) {
            puts(options.locations.stdlib);
            BUILD_unload_options(&options);
            return EXIT_SUCCESS;
        }

        fprintf(stderr, "CPL standard library isn't found\n");
        BUILD_unload_options(&options);
        return EXIT_FAILURE;
    }

    if (!list_size(&options.locations.files)) {
        fprintf(stderr, "No input files\n");
        BUILD_unload_options(&options);
        return EXIT_FAILURE;
    }

    CONF_set_config(BUILD_make_config(&options));

    deftb_t macros;
    MCTB_init(&macros);
    PP_predefine(&macros);

    foreach (cli_define_t* define, &options.locations.defines) {
        MCTB_put_define(define->name,  define->value,  &macros);
    }

    list_t token_lists, object_files;
    list_init(&token_lists);
    list_init(&object_files);

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    if (!_generate_ast(&options, &sctx, &smt, &macros, &token_lists)) {
        BUILD_unload_token_lists(&token_lists);
        AST_unload_ctx(&sctx);
        SMT_unload(&smt);
        MCTB_unload(&macros);
        list_free(&object_files);
        BUILD_unload_options(&options);
        return EXIT_FAILURE;
    }

    if (options.flags.preprocess_only) {
        AST_unload_ctx(&sctx);
        SMT_unload(&smt);
        goto _exit;
    }

    if (!AST_finalize_parse(&sctx, &smt)) {
        fprintf(stderr, "AST finalization error!\n");
        return 1;
    }

    if (
        list_size(&options.locations.symtab_types) &&
        !BUILD_emit_requested_symtabs(&smt, &options.locations.symtab_types)
    ) {
        fprintf(stderr, "Symtab dump emission failed\n");
        return 1;
    }

    if (!BUILD_emit_ast(&sctx, &options)) return 1;

    if (options.flags.ast_analysis && (STRICT_ACTION(SEM_perform_ast_check(&sctx, &smt, DEF_CONF) <= 0))) {
        fprintf(stderr, "CSA: AST analysis failed\n");
        return 1;
    }

    hir_ctx_t hirctx = { 0 };
    HIR_generate(&sctx, &hirctx, &smt);
    cfg_ctx_t cfgctx = { .cid = 0 };
    HIR_CFG_build(&hirctx, &cfgctx, &smt);
    call_graph_t callctx;
    HIR_CG_build(&cfgctx, &callctx, &smt);

    // High function preparation
    HIR_FUNC_set_unused_duplicated_functions(&cfgctx);       // clear functions (after generic resolution)
    HIR_FUNC_set_last_return(&cfgctx);                       // last return
    if (!HIR_FUNC_generate_test_function(&hirctx, &cfgctx, &smt)) { // @[test] functions
        fprintf(stderr, "Test entry generation failed\n");
        HIR_CG_unload(&callctx);
        HIR_CFG_unload(&cfgctx);
        HIR_unload_blocks(hirctx.hot.h);
        BUILD_unload_token_lists(&token_lists);
        AST_unload_ctx(&sctx);
        SMT_unload(&smt);
        return 1;
    }

    // OPT: Tail recursion elimination
    if (options.config.tre) {
        HIR_FUNC_perform_tre(&cfgctx, &smt);
    }

    RELOAD_CFG;

    HIR_CFG_create_domdata(&cfgctx);
    HIR_FUNC_set_defer_calls(&cfgctx);
        
    ltree_ctx_t lctx;
    map_init(&lctx.lmap, MAP_NO_CMP);
    HIR_LOOP_mark_loops(&cfgctx, &lctx);

    // OPT: Function inline optimization
    if (options.config.finline) {
        HIR_FUNC_perform_inline(&cfgctx, &lctx, &smt);
        HIR_LTREE_unload_ctx(&lctx);
        RELOAD_CFG;
        HIR_CFG_create_domdata(&cfgctx);
        map_init(&lctx.lmap, MAP_NO_CMP);
        HIR_LOOP_mark_loops(&cfgctx, &lctx);
    }

    HIR_LTREE_canonicalization(&cfgctx, &lctx);

    HIR_CFG_unload_domdata(&cfgctx);
    HIR_CFG_finalize_before_dom(&cfgctx);
    HIR_CFG_create_domdata(&cfgctx);

    ssa_ctx_t ssactx;
    map_init(&ssactx.vers, MAP_NO_CMP);
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);
    map_free_force(&ssactx.vers);
    HIR_compute_homes(&hirctx);

    // OPT: LICM
    if (
        options.config.licm &&
        options.config.constant // licm without constant will break the code
    ) {
        HIR_LTREE_licm(&cfgctx, &lctx, &smt);
    }

    // OPT: Z3 optimization
    if (options.config.z3opt) {
#ifndef CPL_ENABLE_Z3
        fprintf(stderr, "'--z3opt' flag is active, but Z3 isn't installed! Install Z3 to use Z3 optimizations!\n");
#endif
        Z3OPT_deadbranch(&cfgctx, &smt);
    }

    HIR_CFG_make_allias(&cfgctx, &smt);
    dag_ctx_t dagctx = { .curr_id = 0 };
    HIR_DAG_init(&dagctx);

    int needs_hir_analysis = options.flags.hir_analysis || options.build_mode == BUILD_MODE_ANALYSIS;
    if (
        options.config.constant || // constfold uses DAG info
        needs_hir_analysis      || // HIR CSA uses DAG info
        options.config.z3opt       // Z3 removed brunches, now we need to aknowledge it
    ) {
        HIR_DAG_generate(&cfgctx, &dagctx, &smt);
    }

    if (options.config.constant || needs_hir_analysis) {
        HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);
        HIR_DAG_mark_unused_entries(&cfgctx, &dagctx);
        int folded = 0;
        do {
            folded = 0;
            HIR_sparse_const_propagation(&dagctx, &smt);
            folded = HIR_sparse_const_funcall_propagation(&cfgctx, &smt) || folded;
            folded = HIR_sparce_const_fret_propagation(&cfgctx, &smt)    || folded;
        } while (folded);
        if (options.config.licm) HIR_LOOP_perform_dle(&lctx, &smt);
    }

    if (options.config.z3opt) {
        HIR_DAG_mark_unused_entries(&cfgctx, &dagctx);
    }

    if (needs_hir_analysis && (STRICT_ACTION(SEM_perform_hir_check(&cfgctx, &dagctx, &smt, DEF_CONF) <= 0))) {
        fprintf(stderr, "CSA: HIR analysis failed\n");
        return 1;
    }

    // OPT: Loop unroll
    if (options.config.unroll) {
        int unrolled = HIR_LTREE_unroll(&lctx, &smt);
        if (unrolled < 0) {
            fprintf(stderr, "Loop unrolling failed\n");
            HIR_DAG_unload(&dagctx);
            HIR_LTREE_unload_ctx(&lctx);
            HIR_CG_unload(&callctx);
            HIR_CFG_unload(&cfgctx);
            HIR_unload_blocks(hirctx.hot.h);
            BUILD_unload_token_lists(&token_lists);
            AST_unload_ctx(&sctx);
            SMT_unload(&smt);
            return 1;
        }
        else if (unrolled > 0) {
            HIR_compute_homes(&hirctx);
            HIR_LTREE_unload_ctx(&lctx);
            HIR_CFG_unload_domdata(&cfgctx);
            HIR_CFG_create_domdata(&cfgctx);
            map_init(&lctx.lmap, MAP_NO_CMP);
            HIR_LOOP_mark_loops(&cfgctx, &lctx);
        }
    }

    HIR_CFG_squeeze_blocks(&cfgctx);

    if (!BUILD_emit_hir(&hirctx, &cfgctx, &smt, &options)) return 1;

    if (options.build_mode == BUILD_MODE_ANALYSIS) {
        HIR_DAG_unload(&dagctx);
        HIR_LTREE_unload_ctx(&lctx);
        HIR_CG_unload(&callctx);
        HIR_CFG_unload(&cfgctx);
        HIR_unload_blocks(hirctx.hot.h);
        BUILD_unload_token_lists(&token_lists);
        AST_unload_ctx(&sctx);
        SMT_unload(&smt);
        goto _exit;
    }

    lir_ctx_t lirctx = { 0 };
    LIR_generate(&cfgctx, &lirctx, &smt);

    // OPT: Tail Call Optimization
    if (options.config.tco) {
        LIR_FUNC_perform_tco(&cfgctx, &smt);
    }

    // OPT: Copy propagation
    if (options.config.copy_prop) {
        LIR_variable_copy_propagation(&cfgctx, &smt);
        LIR_drop_unused_variables(&cfgctx);
    }

    inst_selector_t  inst_sel;
    register_saver_t reg_save;
    mem_selector_t   mem_sel;
    peephole_t       pph;
    asm_gen_t        asmgen;
    long             (*precolor)(lir_registers_t);
    switch (CONF_get_system_type()) {
        default:
        case MACHO64: {
            inst_sel.select_instructions = x86_64_macho_nasm_instruction_selection;
            reg_save.save_registers      = x86_64_macho_nasm_caller_saving;
            mem_sel.select_memory        = x86_64_macho_nasm_memory_selection;
            mem_sel.validate_memory      = x86_64_macho_nasm_memory_validation;
            asmgen.generator             = x86_64_macho_nasm_generate_asm;
            precolor                     = x86_64_gnu_precolored_reg_to_color;
            pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
            break;
        }
        case LINUX64: {
            inst_sel.select_instructions = x86_64_gnu_nasm_instruction_selection;
            reg_save.save_registers      = x86_64_gnu_nasm_caller_saving;
            mem_sel.select_memory        = x86_64_gnu_nasm_memory_selection;
            mem_sel.validate_memory      = x86_64_gnu_nasm_memory_validation;
            asmgen.generator             = x86_64_gnu_nasm_generate_asm;
            precolor                     = x86_64_gnu_precolored_reg_to_color;
            pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
            break;
        }
        case I386: {
            inst_sel.select_instructions = i386_gnu_nasm_instruction_selection;
            reg_save.save_registers      = i386_gnu_nasm_caller_saving;
            mem_sel.select_memory        = i386_gnu_nasm_memory_selection;
            mem_sel.validate_memory      = i386_gnu_nasm_memory_validation;
            asmgen.generator             = i386_gnu_nasm_generate_asm;
            precolor                     = i386_gnu_precolored_reg_to_color;
            pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
            break;
        }
    }

    LIR_select_instructions(&cfgctx, &smt, &inst_sel);

    LIR_DFG_compute_inout(&cfgctx);
    LIR_DFG_create_deall(&cfgctx, &smt);

    if (!BUILD_emit_lir(&lirctx, &cfgctx, &smt, &options)) return 1;

    map_t colors;
    map_init(&colors, MAP_NO_CMP);
    LIR_RA_init_colors(&colors, &smt, precolor);

    LIR_regalloc(&cfgctx, &smt, &colors);
    LIR_RA_sort_phi_movs(&cfgctx, &colors);
    LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel);

    // OPT: Copy propagation -- part 2
    if (options.config.copy_prop) {
        LIR_register_copy_propagation(&cfgctx);
    }

    LIR_destroy_ssa(&cfgctx);
    LIR_save_registers(&cfgctx, &smt, &reg_save);

    // OPT: Peephole optimization
    if (options.config.peephole) {
        LIR_peephole_optimization(&cfgctx, &pph);
    }
        
    LIR_validate_memory(&cfgctx, &smt, &mem_sel);
    LIR_clear_global_variables(&cfgctx, &smt);

    char *asm_path = BUILD_make_temp_path(), *obj_path = BUILD_make_temp_path();
    if (!asm_path || !obj_path) {
        fprintf(stderr, "Can't create temporary files\n");
        return 1;
    }

    FILE* asm_file = fopen(asm_path, "w");
    if (!asm_file) {
        fprintf(stderr, "Can't open temporary asm file %s: %s\n", asm_path, strerror(errno));
        return 1;
    }

    ASM_generate(&cfgctx, &smt, &asmgen, asm_file);
    if (options.config.emit_asm) {
        const char* asm_output = BUILD_output_path_or_default(options.locations.asm_output, "output.s");
        FILE* asm_emit = fopen(asm_output, "w");
        if (!asm_emit) {
            fprintf(stderr, "Can't open ASM output file %s: %s\n", asm_output, strerror(errno));
            return 1;
        }

        ASM_generate(&cfgctx, &smt, &asmgen, asm_emit);
        fclose(asm_emit);
    }

    fclose(asm_file);

    if (!BUILD_compile_asm_to_object(&options, asm_path, obj_path)) {
        fprintf(stderr, "ASM compilation failed\n");
        return 1;
    }

    unlink(asm_path);
    mm_free(asm_path);
    if (options.build_mode != BUILD_MODE_OBJECT) {
        if (!list_push_back(&object_files, obj_path)) {
            mm_free(obj_path);
            fprintf(stderr, "Can't save object file path\n");
            return 1;
        }
    }
    else {
        const char* object_output = BUILD_output_path_or_default(options.locations.output, "output.o");
        if (!BUILD_move_file_path(obj_path, object_output)) {
            fprintf(stderr, "Can't write object file %s: %s\n", object_output, strerror(errno));
            mm_free(obj_path);
            return 1;
        }

        mm_free(obj_path);
    }

    map_free(&colors);
    LIR_unload_blocks(lirctx.h);
    HIR_DAG_unload(&dagctx);
    HIR_LTREE_unload_ctx(&lctx);
    HIR_CG_unload(&callctx);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(hirctx.hot.h);
    BUILD_unload_token_lists(&token_lists);
    AST_unload_ctx(&sctx);

    SMT_unload(&smt);

_exit: {}

    if (
        options.build_mode == BUILD_MODE_EXECUTABLE &&
        !options.flags.preprocess_only              &&
        list_size(&object_files) > 0
    ) {
        if (!BUILD_link_objects(&options, &object_files)) {
            fprintf(stderr, "Linking failed\n");
            return 1;
        }
    }

    foreach (char* object_file, &object_files) {
        if (object_file) {
            unlink(object_file);
            mm_free(object_file);
        }
    }

    MCTB_unload(&macros);
    list_free(&object_files);
    BUILD_unload_token_lists(&token_lists);
    BUILD_unload_options(&options);
    return EXIT_SUCCESS;
}
