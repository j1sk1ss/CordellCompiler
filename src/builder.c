#include <builder.h>

#define OPTION_INLUCDE               "-I"
#define OPTION_OUTPUT                "--output"
#define OPTION_ENABLE_AST_ANALYSIS   "--ast-analysis"
#define OPTION_ENABLE_IR_ANALYSIS    "--ir-analysis"
#define OPTION_DEBUG                 "--debug"
#define OPTION_NO_DEBUG              "--no-debug"
#define OPTION_NO_OPTIMIZATION       "-O0"
#define OPTION_ROUGHT_OPTIMIZATION   "-O1"
#define OPTION_GOOD_OPTIMIZATION     "-O2"
#define OPTION_MAX_OPTIMIZATION      "-O3"
#define OPTION_ARCH                  "--arch"
#define OPTION_ASM_COMPILER          "--asm-compiler"
#define OPTION_ASM_FORMAT            "--asm-format"
#define OPTION_LINKER                "--linker"
#define OPTION_LINKER_MODE           "--linker-mode"
#define OPTION_LINKER_NO_PIE         "--linker-no-pie"
#define OPTION_LINKER_PIE            "--linker-pie"
#define OPTION_LINKER_M32            "--linker-m32"
#define OPTION_LINKER_NO_M32         "--linker-no-m32"
#define OPTION_ENTRY_NAME            "--entry-name"
#define OPTION_RO_SECTION            "--ro-section"
#define OPTION_GLOB_SECTION          "--glob-section"
#define OPTION_CODE_SECTION          "--code-section"
#define OPTION_FULL_BYTNESS          "--full-bytness"
#define OPTION_HALF_BYTNESS          "--half-bytness"
#define OPTION_QUART_BYTNESS         "--quart-bytness"
#define OPTION_EIGHT_BYTNESS         "--eight-bytness"
#define OPTION_SYS_TYPE              "--sys-type"
#define OPTION_TRE                   "--tre"
#define OPTION_NO_TRE                "--no-tre"
#define OPTION_FINLINE               "--finline"
#define OPTION_NO_FINLINE            "--no-finline"
#define OPTION_LICM                  "--licm"
#define OPTION_NO_LICM               "--no-licm"
#define OPTION_CONSTANT              "--constant"
#define OPTION_NO_CONSTANT           "--no-constant"
#define OPTION_PEEPHOLE              "--peephole"
#define OPTION_NO_PEEPHOLE           "--no-peephole"

static int _print_help_message() {
    fprintf(stdout, "Usage: ccpl [options] <input files>\n\n");

    fprintf(stdout, "General options:\n");
    fprintf(stdout, "  %s <dir>\t\tAdd include directory\n", OPTION_INLUCDE);
    fprintf(stdout, "  %s <file>\tSet output file\n", OPTION_OUTPUT);
    fprintf(stdout, "  %s\t\tEnable AST analysis\n", OPTION_ENABLE_AST_ANALYSIS);
    fprintf(stdout, "  %s\t\tEnable IR analysis\n", OPTION_ENABLE_IR_ANALYSIS);
    fprintf(stdout, "  %s\t\tEnable debug mode\n", OPTION_DEBUG);
    fprintf(stdout, "  %s\t\tDisable debug mode\n", OPTION_NO_DEBUG);

    fprintf(stdout, "\nOptimization options:\n");
    fprintf(stdout, "  %s\t\tDisable optimizations\n", OPTION_NO_OPTIMIZATION);
    fprintf(stdout, "  %s\t\tRough optimization level\n", OPTION_ROUGHT_OPTIMIZATION);
    fprintf(stdout, "  %s\t\tGood optimization level\n", OPTION_GOOD_OPTIMIZATION);
    fprintf(stdout, "  %s\t\tMaximum optimization level\n", OPTION_MAX_OPTIMIZATION);
    fprintf(stdout, "  %s\t\tEnable function inlining\n", OPTION_FINLINE);
    fprintf(stdout, "  %s\t\tDisable function inlining\n", OPTION_NO_FINLINE);
    fprintf(stdout, "  %s\t\tEnable LICM\n", OPTION_LICM);
    fprintf(stdout, "  %s\t\tDisable LICM\n", OPTION_NO_LICM);
    fprintf(stdout, "  %s\t\tEnable constant propagation/folding\n", OPTION_CONSTANT);
    fprintf(stdout, "  %s\t\tDisable constant propagation/folding\n", OPTION_NO_CONSTANT);
    fprintf(stdout, "  %s\t\tEnable peephole optimization\n", OPTION_PEEPHOLE);
    fprintf(stdout, "  %s\t\tDisable peephole optimization\n", OPTION_NO_PEEPHOLE);
    fprintf(stdout, "  %s\t\tEnable TRE\n", OPTION_TRE);
    fprintf(stdout, "  %s\t\tDisable TRE\n", OPTION_NO_TRE);

    fprintf(stdout, "\nTarget options:\n");
    fprintf(stdout, "  %s <arch>\t\tSet target architecture (x86_64, x86_32, i386)\n", OPTION_ARCH);
    fprintf(stdout, "  %s\t\tSet full bytness (1, 2, 4, 8, ...)\n", OPTION_FULL_BYTNESS);
    fprintf(stdout, "  %s\t\tUse half bytness (1, 2, 4, 8, ...)\n", OPTION_HALF_BYTNESS);
    fprintf(stdout, "  %s\t\tUse quart bytness (1, 2, 4, 8, ...)\n", OPTION_QUART_BYTNESS);
    fprintf(stdout, "  %s\t\tUse eight bytness (1, 2, 4, 8, ...)\n", OPTION_EIGHT_BYTNESS);
    fprintf(stdout, "  %s <type>\t\tSet system type (macho64, linux64, windows64)\n", OPTION_SYS_TYPE);

    fprintf(stdout, "\nAssembler options:\n");
    fprintf(stdout, "  %s <compiler>\tSet assembler compiler (nasm, ...)\n", OPTION_ASM_COMPILER);
    fprintf(stdout, "  %s <format>\tSet assembler format (macho64, ...)\n", OPTION_ASM_FORMAT);

    fprintf(stdout, "\nLinker options:\n");
    fprintf(stdout, "  %s <linker>\tSet linker (ld, gcc, clang, ...)\n", OPTION_LINKER);
    fprintf(stdout, "  %s <mode>\tSet linker mode\n", OPTION_LINKER_MODE);
    fprintf(stdout, "  %s\t\tDisable PIE\n", OPTION_LINKER_NO_PIE);
    fprintf(stdout, "  %s\t\tEnable PIE\n", OPTION_LINKER_PIE);
    fprintf(stdout, "  %s\t\tEnable m32 mode\n", OPTION_LINKER_M32);
    fprintf(stdout, "  %s\t\tDisable m32 mode\n", OPTION_LINKER_NO_M32);
    fprintf(stdout, "  %s <name>\tSet entry symbol name\n", OPTION_ENTRY_NAME);

    fprintf(stdout, "\nSection options:\n");
    fprintf(stdout, "  %s <name>\tSet read-only section name\n", OPTION_RO_SECTION);
    fprintf(stdout, "  %s <name>\tSet global section name\n", OPTION_GLOB_SECTION);
    fprintf(stdout, "  %s <name>\tSet code section name\n", OPTION_CODE_SECTION);

    return 0;
}

typedef struct {
    struct {
        const char*  include;
        const char** files;
        int          files_count;
        char*        output;
    } locations;
    struct {
        const char*  asm_compiler;
        const char*  asm_format;
        const char*  linker;
        int          linker_use_c_driver;
        int          linker_no_pie;
        int          linker_m32;
    } tools;
    struct {
        const char*  entry_name;
        const char*  ro_section;
        const char*  glob_section;
        const char*  code_section;
        long         full_bytness;
        long         half_bytness;
        long         quart_bytness;
        long         eight_bytness;
        arch_type_t  sys_type;
        int          tre;
        int          finline;
        int          licm;
        int          constant;
        int          peephole;
        int          debug;
    } config;
    struct {
        int          ast_analysis;
        int          hir_analysis;
    } flags;
} options_t;

static char* _dup_string(const char* s) {
    if (!s) return NULL;

    size_t n = strlen(s) + 1;
    char* out = mm_malloc(n);
    if (!out) return NULL;

    memcpy(out, s, n);
    return out;
}

static char* _make_temp_path(void) {
    char template[] = "/tmp/builder-XXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) return NULL;

    close(fd);
    return _dup_string(template);
}

static int _run_tool(const char* tool, char* const argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 0;
    }

    if (pid == 0) {
        execvp(tool, argv);
        perror(tool);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 0;
    }

    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

static int _compile_asm_to_object(const options_t* options, const char* asm_path, const char* obj_path) {
    char* const cmd[] = {
        (char*)options->tools.asm_compiler,
        "-f",
        (char*)options->tools.asm_format,
        (char*)asm_path,
        "-o",
        (char*)obj_path,
        NULL,
    };

    return _run_tool(options->tools.asm_compiler, cmd);
}

static int _link_objects(const options_t* options, char* const objects[], int objects_count) {
    int extra = (options->tools.linker_use_c_driver ? 1 : 0) + (options->tools.linker_no_pie ? 1 : 0) + (options->tools.linker_m32 ? 1 : 0);
    int cmd_size = objects_count + 5 + extra;
    char** cmd = mm_malloc((size_t)cmd_size * sizeof(*cmd));
    if (!cmd) return 0;

    int j = 0;
    cmd[j++] = (char*)options->tools.linker;
    if (options->tools.linker_use_c_driver) {
        if (options->tools.linker_no_pie) cmd[j++] = "-no-pie";
        if (options->tools.linker_m32) cmd[j++] = "-m32";
    }
    cmd[j++] = "-o";
    cmd[j++] = options->locations.output ? options->locations.output : "a.out";
    for (int i = 0; i < objects_count; i++) {
        cmd[j++] = objects[i];
    }
    cmd[j] = NULL;

    int ok = _run_tool(options->tools.linker, cmd);
    mm_free(cmd);
    return ok;
}

static int _parse_long_arg(const char* s, long* out) {
    if (!s || !out) return 0;

    char* end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno || !end || *end != '\0') return 0;

    *out = v;
    return 1;
}

static int _parse_sys_type(const char* s, arch_type_t* out) {
    if (!s || !out) return 0;

    if (
        !strcmp(s, "unknown") || 
        !strcmp(s, "UNKNOWN")
    ) {
        *out = UNKNOWN;
        return 1;
    }
    else if (
        !strcmp(s, "macho64") || 
        !strcmp(s, "MACHO64") || 
        !strcmp(s, "macho64") || 
        !strcmp(s, "MACHO64")
    ) {
        *out = MACHO64;
        return 1;
    }
    else if (
        !strcmp(s, "linux64") || 
        !strcmp(s, "LINUX64")
    ) {
        *out = LINUX64;
        return 1;
    }
    else if (
        !strcmp(s, "windows64") || 
        !strcmp(s, "WINDOWS64")
    ) {
        *out = WINDOWS64;
        return 1;
    }

    return 0;
}

static void _set_optimization_profile(options_t* out, int level) {
    if (!out) return;

    out->config.tre = 0;
    out->config.finline = 0;
    out->config.licm = 0;
    out->config.constant = 0;
    out->config.peephole = 0;

    if (level >= 2) {
        out->config.licm = 1;
        out->config.constant = 1;
        out->config.peephole = 1;
    }
    if (level >= 3) {
        out->config.tre = 1;
    }
}

static void _set_arch_profile(options_t* out, const char* arch) {
    if (!out || !arch) return;
    if (
        !strcmp(arch, "x86") || 
        !strcmp(arch, "i386") || 
        !strcmp(arch, "ia32")
    ) {
        out->config.full_bytness  = 4;
        out->config.half_bytness  = 2;
        out->config.quart_bytness = 1;
        out->config.eight_bytness = 1;
        out->tools.asm_format     = "elf32";
        out->tools.linker_m32     = 1;
        return;
    }

    if (
        !strcmp(arch, "x86_64") || 
        !strcmp(arch, "amd64")
    ) {
        out->config.full_bytness  = 8;
        out->config.half_bytness  = 4;
        out->config.quart_bytness = 2;
        out->config.eight_bytness = 1;
        out->tools.asm_format     = "elf64";
        out->tools.linker_m32     = 0;
    }
}

static config_t _make_config(const options_t* options) {
    config_t conf = {
        .system = {
            .entry_name = options->config.entry_name,
            .ro_section = options->config.ro_section,
            .glob_section = options->config.glob_section,
            .code_section = options->config.code_section,
            .bytness = {
                .bytness = options->config.full_bytness,
                .h_bytness = options->config.half_bytness,
                .q_bytness = options->config.quart_bytness,
                .e_bytness = options->config.eight_bytness,
            },
            .sys_type = options->config.sys_type,
        },
        .optimization_flags = {
            .tre = options->config.tre ? 1 : 0,
            .finline = options->config.finline ? 1 : 0,
            .licm = options->config.licm ? 1 : 0,
            .constant = options->config.constant ? 1 : 0,
            .peephole = options->config.peephole ? 1 : 0,
        },
        .compilation_flags = {
            .debug = options->config.debug ? 1 : 0,
        },
    };

    return conf;
}

static void _set_default_options(options_t* out) {
    memset(out, 0, sizeof(*out));

    out->tools.asm_compiler        = "nasm";
    out->tools.asm_format          = "macho64";
    out->tools.linker              = "clang";
    out->tools.linker_use_c_driver = 1;
    out->tools.linker_no_pie       = 0;
    out->tools.linker_m32          = 0;

    out->config.entry_name         = "_main";
    out->config.ro_section         = "__TEXT,__const";
    out->config.glob_section       = "__DATA,__data";
    out->config.code_section       = "__TEXT,__text";
    out->config.full_bytness       = 8;
    out->config.half_bytness       = 4;
    out->config.quart_bytness      = 2;
    out->config.eight_bytness      = 1;
    out->config.sys_type           = MACHO64;
    out->config.debug              = 0;

    _set_optimization_profile(out, 0);
}

static int _parse_input_args(char* argv[], int argc, options_t* out) {
    if (!argv || argc <= 0 || !out) {
        return 0;
    }

    _set_default_options(out);
    out->locations.files = mm_malloc((size_t)argc * sizeof(*out->locations.files));
    if (!out->locations.files) return 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], OPTION_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.output = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_INLUCDE)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.include = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_ARCH)) {
            if (i + 1 >= argc) goto _fail;
            _set_arch_profile(out, argv[++i]);
        }
        else if (!strcmp(argv[i], OPTION_ASM_COMPILER)) {
            if (i + 1 >= argc) goto _fail;
            out->tools.asm_compiler = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_ASM_FORMAT)) {
            if (i + 1 >= argc) goto _fail;
            out->tools.asm_format = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_LINKER)) {
            if (i + 1 >= argc) goto _fail;
            out->tools.linker = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_LINKER_MODE)) {
            if (i + 1 >= argc) goto _fail;
            const char* mode = argv[++i];
            if (!strcmp(mode, "c") || !strcmp(mode, "driver")) out->tools.linker_use_c_driver = 1;
            else if (!strcmp(mode, "raw") || !strcmp(mode, "ld")) out->tools.linker_use_c_driver = 0;
            else goto _fail;
        }
        else if (!strcmp(argv[i], OPTION_LINKER_NO_PIE)) out->tools.linker_no_pie = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_PIE)) out->tools.linker_no_pie = 0;
        else if (!strcmp(argv[i], OPTION_LINKER_M32)) out->tools.linker_m32 = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_NO_M32)) out->tools.linker_m32 = 0;
        else if (!strcmp(argv[i], OPTION_ENTRY_NAME)) {
            if (i + 1 >= argc) goto _fail;
            out->config.entry_name = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_RO_SECTION)) {
            if (i + 1 >= argc) goto _fail;
            out->config.ro_section = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_GLOB_SECTION)) {
            if (i + 1 >= argc) goto _fail;
            out->config.glob_section = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_CODE_SECTION)) {
            if (i + 1 >= argc) goto _fail;
            out->config.code_section = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_FULL_BYTNESS)) {
            if (i + 1 >= argc || !_parse_long_arg(argv[i + 1], &out->config.full_bytness)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_HALF_BYTNESS)) {
            if (i + 1 >= argc || !_parse_long_arg(argv[i + 1], &out->config.half_bytness)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_QUART_BYTNESS)) {
            if (i + 1 >= argc || !_parse_long_arg(argv[i + 1], &out->config.quart_bytness)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_EIGHT_BYTNESS)) {
            if (i + 1 >= argc || !_parse_long_arg(argv[i + 1], &out->config.eight_bytness)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_SYS_TYPE)) {
            if (i + 1 >= argc || !_parse_sys_type(argv[i + 1], &out->config.sys_type)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_TRE)) out->config.tre = 1;
        else if (!strcmp(argv[i], OPTION_NO_TRE)) out->config.tre = 0;
        else if (!strcmp(argv[i], OPTION_FINLINE)) out->config.finline = 1;
        else if (!strcmp(argv[i], OPTION_NO_FINLINE)) out->config.finline = 0;
        else if (!strcmp(argv[i], OPTION_LICM)) out->config.licm = 1;
        else if (!strcmp(argv[i], OPTION_NO_LICM)) out->config.licm = 0;
        else if (!strcmp(argv[i], OPTION_CONSTANT)) out->config.constant = 1;
        else if (!strcmp(argv[i], OPTION_NO_CONSTANT)) out->config.constant = 0;
        else if (!strcmp(argv[i], OPTION_PEEPHOLE)) out->config.peephole = 1;
        else if (!strcmp(argv[i], OPTION_NO_PEEPHOLE)) out->config.peephole = 0;
        else if (!strcmp(argv[i], OPTION_ENABLE_AST_ANALYSIS)) out->flags.ast_analysis = 1;
        else if (!strcmp(argv[i], OPTION_ENABLE_IR_ANALYSIS)) out->flags.hir_analysis = 1;
        else if (!strcmp(argv[i], OPTION_DEBUG)) out->config.debug = 1;
        else if (!strcmp(argv[i], OPTION_NO_DEBUG)) out->config.debug = 0;
        else if (!strcmp(argv[i], OPTION_NO_OPTIMIZATION)) _set_optimization_profile(out, 0);
        else if (!strcmp(argv[i], OPTION_ROUGHT_OPTIMIZATION)) _set_optimization_profile(out, 1);
        else if (!strcmp(argv[i], OPTION_GOOD_OPTIMIZATION)) _set_optimization_profile(out, 2);
        else if (!strcmp(argv[i], OPTION_MAX_OPTIMIZATION)) _set_optimization_profile(out, 3);
        else if (argv[i][0] == '-') {
            goto _fail;
        }
        else {
            out->locations.files[out->locations.files_count++] = argv[i];
        }
    }

    return 1;

_fail: {}
    mm_free((void*)out->locations.files);
    out->locations.files = NULL;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        _print_help_message();
        return EXIT_FAILURE;
    }

    mm_init();

    options_t options;
    if (!_parse_input_args(argv, argc, &options)) {
        fprintf(stderr, "Can't parse input arguments\n");
        return EXIT_FAILURE;
    }

    config_t conf = _make_config(&options);
    if (!CONF_set_config(&conf)) {
        fprintf(stderr, "Can't initialize compilation config\n");
        return EXIT_FAILURE;
    }

    char** object_files = mm_malloc((size_t)options.locations.files_count * sizeof(*object_files));
    if (!object_files && options.locations.files_count > 0) {
        fprintf(stderr, "Can't allocate object files array\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < options.locations.files_count; i++) {
        int fd = open(options.locations.files[i], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s isn't found!\n", options.locations.files[i]);
            return 1;
        }

        finder_ctx_t finctx = { .bpath = options.locations.include };
        fd = PP_perform(fd, &finctx);
        if (fd < 0) {
            fprintf(stderr, "Processed file %s isn't found!\n", options.locations.files[i]);
            return 1;
        }

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

        if (options.flags.ast_analysis) {
            SEM_perform_ast_check(&sctx, &smt);
        }

        hir_ctx_t hirctx = { 0 };
        HIR_generate(&sctx, &hirctx, &smt);

        cfg_ctx_t cfgctx = { .cid = 0 };
        HIR_CFG_build(&hirctx, &cfgctx, &smt);

        if (options.config.tre) {
            HIR_FUNC_perform_tre(&cfgctx, &smt);
            HIR_CFG_unload(&cfgctx);
            HIR_CFG_build(&hirctx, &cfgctx, &smt);
        }

        HIR_LOOP_mark_loops(&cfgctx, NULL); // TODO

        if (options.config.finline) { // TODO
            HIR_FUNC_perform_inline(&cfgctx, NULL, &smt, HIR_FUNC_inline_euristic_desider);
            HIR_CFG_unload(&cfgctx);
            HIR_CFG_build(&hirctx, &cfgctx, &smt);
        }

        HIR_FUNC_set_last_return(&cfgctx);

        call_graph_t callctx;
        HIR_CG_build(&cfgctx, &callctx, &smt);
        HIR_CG_perform_dfe(&callctx, &smt);
        HIR_CG_apply_dfe(&cfgctx, &callctx);

        HIR_CFG_create_domdata(&cfgctx);
        HIR_LTREE_canonicalization(&cfgctx, NULL);
        HIR_CFG_unload_domdata(&cfgctx);
        HIR_CFG_create_domdata(&cfgctx);

        ssa_ctx_t ssactx;
        map_init(&ssactx.vers, MAP_NO_CMP);
        HIR_SSA_insert_phi(&cfgctx, &smt);
        HIR_SSA_rename(&cfgctx, &ssactx, &smt);
        map_free_force(&ssactx.vers);

        HIR_compute_homes(&hirctx);
        if (options.config.licm) {
            HIR_LTREE_licm(&cfgctx, NULL, &smt);
        }

        HIR_CFG_make_allias(&cfgctx, &smt);
        dag_ctx_t dagctx = { .curr_id = 0 };
        HIR_DAG_init(&dagctx);
        if (options.config.constant) {
            HIR_DAG_generate(&cfgctx, &dagctx, &smt);
            HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);
            HIR_sparse_const_propagation(&dagctx, &smt);
        }

        HIR_CFG_squeeze_blocks(&cfgctx);

        if (options.flags.hir_analysis) {
            SEM_perform_hir_check(&cfgctx, &dagctx, &smt);
        }

        lir_ctx_t lirctx = { .h = NULL, .t = NULL };
        LIR_generate(&cfgctx, &lirctx, &smt);
        inst_selector_t inst_sel;
        switch (CONF_get_system_type()) {
            case MACHO64: inst_sel.select_instructions = x86_64_macho_nasm_instruction_selection; break;
            case LINUX64: inst_sel.select_instructions = x86_64_gnu_nasm_instruction_selection; break;
            default: break;
        }

        LIR_select_instructions(&cfgctx, &smt, &inst_sel);

        LIR_DFG_compute_inout(&cfgctx);
        LIR_DFG_create_deall(&cfgctx, &smt);

        map_t colors;
        map_init(&colors, MAP_NO_CMP);
        LIR_RA_init_colors(&colors, &smt);

        regalloc_t regall = { .regallocate = x86_64_regalloc_graph };
        LIR_regalloc(&cfgctx, &smt, &colors, &regall);

        mem_selector_t mem_sel = { .select_memory = x86_64_gnu_nasm_memory_selection };
        LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel);

        register_saver_t reg_save = { .save_registers = x86_64_gnu_nasm_caller_saving };
        LIR_save_registers(&cfgctx, &smt, &reg_save);

        if (options.config.peephole) {
            peephole_t pph = { .perform_peephole = x86_64_gnu_nasm_peephole_optimization };
            LIR_peephole_optimization(&cfgctx, &pph);
        }

        char* asm_path = _make_temp_path();
        char* obj_path = _make_temp_path();
        if (!asm_path || !obj_path) {
            fprintf(stderr, "Can't create temporary files for %s\n", options.locations.files[i]);
            return 1;
        }

        FILE* asm_file = fopen(asm_path, "w");
        if (!asm_file) {
            fprintf(stderr, "Can't open temporary asm file %s: %s\n", asm_path, strerror(errno));
            return 1;
        }

        asm_gen_t asmgen;
        switch (CONF_get_system_type()) {
            case MACHO64: asmgen.generator = x86_64_macho_nasm_generate_asm; break;
            case LINUX64: asmgen.generator = x86_64_gnu_nasm_generate_asm; break;
            default: break;
        }

        ASM_generate(&cfgctx, &smt, &asmgen, asm_file);
        fclose(asm_file);

        if (!_compile_asm_to_object(&options, asm_path, obj_path)) {
            fprintf(stderr, "ASM compilation failed for %s\n", options.locations.files[i]);
            return 1;
        }

        unlink(asm_path);
        mm_free(asm_path);
        object_files[i] = obj_path;

        map_free(&colors);
        LIR_unload_blocks(lirctx.h);
        HIR_CG_unload(&callctx);
        HIR_CFG_unload(&cfgctx);
        HIR_unload_blocks(hirctx.hot.h);
        list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
        AST_unload_ctx(&sctx);

        SMT_unload(&smt);
        close(fd);
    }

    if (options.locations.files_count > 0) {
        if (!_link_objects(&options, object_files, options.locations.files_count)) {
            fprintf(stderr, "Linking failed\n");
            return 1;
        }
    }

    for (int i = 0; i < options.locations.files_count; i++) {
        if (object_files[i]) {
            unlink(object_files[i]);
            mm_free(object_files[i]);
        }
    }

    mm_free(object_files);
    mm_free((void*)options.locations.files);

    return EXIT_SUCCESS;
}
