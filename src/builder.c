#include <builder.h>

static inline void _print_gem(FILE* stream) {
    fwrite(gem_data, 1, gem_data_len, stream);
}

static inline void _print_version(FILE* stream) {
    fprintf(stream, "cplc %s\n", CCPL_VERSION);
}

static inline void _print_help_row(FILE* stream, const cli_help_option_t* row) {
    char label[64] = { 0 };
    if (row->argument && row->argument[0]) snprintf(label, sizeof(label), "%s %s", row->option, row->argument);
    else                                   snprintf(label, sizeof(label), "%s", row->option);
    fprintf(stream, "  %-28s %s\n", label, row->description);
}

static inline void _print_help_section(FILE* stream, const char* title, const cli_help_option_t* rows, size_t count) {
    fprintf(stream, "\n%s:\n", title);
    for (size_t i = 0; i < count; i++) {
        _print_help_row(stream, &rows[i]);
    }
}

static int _print_help_message() {
    static const cli_help_option_t general_options[] = {
        { OPTION_HELP_SHORT ", " OPTION_HELP, NULL, "Show this help message" },
        { OPTION_VERSION_SHORT ", " OPTION_VERSION, NULL, "Show compiler version" },
        { OPTION_SOMETHING_SHORT ", " OPTION_SOMETHING, NULL, "Show something" },
        { OPTION_PREPROCESS_ONLY, NULL, "Run preprocessor only" },
        { OPTION_WITHOUT_COMPILATION, NULL, "Build AST and HIR, then stop without compilation" },
        { OPTION_INLUCDE, "<dir>", "Add include directory" },
        { OPTION_DEFINE, "<name=value>", "Define preprocessor variable" },
        { OPTION_PRINT_STDLIB, NULL, "Print the standard library directory" },
        { OPTION_OUTPUT, "<file>", "Set output file" },
        { OPTION_ENABLE_AST_ANALYSIS, NULL, "Enable AST analysis" },
        { OPTION_ENABLE_IR_ANALYSIS, NULL, "Enable IR analysis" },
        { OPTION_DEBUG, NULL, "Enable debug mode" },
        { OPTION_NO_DEBUG, NULL, "Disable debug mode" },
    };
    static const cli_help_option_t optimization_options[] = {
        { OPTION_NO_OPTIMIZATION, NULL, "Disable optimizations" },
        { OPTION_ROUGHT_OPTIMIZATION, NULL, "Rough optimization level" },
        { OPTION_GOOD_OPTIMIZATION, NULL, "Good optimization level" },
        { OPTION_MAX_OPTIMIZATION, NULL, "Maximum optimization level" },
        { OPTION_FINLINE, NULL, "Enable function inlining" },
        { OPTION_NO_FINLINE, NULL, "Disable function inlining" },
        { OPTION_LICM, NULL, "Enable LICM" },
        { OPTION_NO_LICM, NULL, "Disable LICM" },
        { OPTION_CONSTANT, NULL, "Enable constant propagation/folding" },
        { OPTION_NO_CONSTANT, NULL, "Disable constant propagation/folding" },
        { OPTION_COPYPROP, NULL, "Enable LIR copy propagation" },
        { OPTION_NO_COPYPROP, NULL, "Disable LIR copy propagation" },
        { OPTION_PEEPHOLE, NULL, "Enable peephole optimization" },
        { OPTION_NO_PEEPHOLE, NULL, "Disable peephole optimization" },
        { OPTION_TRE, NULL, "Enable TRE" },
        { OPTION_NO_TRE, NULL, "Disable TRE" },
    };
    static const cli_help_option_t target_options[] = {
        { OPTION_ARCH, "<arch>", "Set target architecture (x86_64, x86, i386)" },
        { OPTION_FULL_BYTNESS, "<size>", "Set full bytness" },
        { OPTION_HALF_BYTNESS, "<size>", "Set half bytness" },
        { OPTION_QUART_BYTNESS, "<size>", "Set quart bytness" },
        { OPTION_EIGHT_BYTNESS, "<size>", "Set eight bytness" },
        { OPTION_SYS_TYPE, "<type>", "Set system type (macho64, linux64, i386, windows64)" },
    };
    static const cli_help_option_t assembler_options[] = {
        { OPTION_ASM_COMPILER, "<compiler>", "Set assembler compiler (nasm, ...)" },
        { OPTION_ASM_FORMAT, "<format>", "Set assembler format (macho64, elf64, elf32, ...)" },
    };
    static const cli_help_option_t linker_options[] = {
        { OPTION_LINKER, "<linker>", "Set linker (ld, gcc, clang, ...)" },
        { OPTION_LINKER_MODE, "<mode>", "Set linker mode (c, driver, raw, ld)" },
        { OPTION_COMPILE_ONLY_SHORT ", " OPTION_COMPILE_ONLY ", " OPTION_WITHOUT_LINKER, NULL, "Build an object file and skip linking" },
        { OPTION_NO_COMPILE, NULL, "Stop after assembly generation" },
        { OPTION_NO_OBJECT_BUILD, NULL, "Stop after assembly generation without building an object file" },
        { OPTION_LINKER_NO_PIE, NULL, "Disable PIE" },
        { OPTION_LINKER_PIE, NULL, "Enable PIE" },
        { OPTION_LINKER_M32, NULL, "Enable m32 mode" },
        { OPTION_LINKER_NO_M32, NULL, "Disable m32 mode" },
        { OPTION_ENTRY_NAME, "<name>", "Set entry symbol name" },
    };
    static const cli_help_option_t section_options[] = {
        { OPTION_RO_SECTION, "<name>", "Set read-only section name" },
        { OPTION_GLOB_SECTION, "<name>", "Set global section name" },
        { OPTION_CODE_SECTION, "<name>", "Set code section name" },
    };
    static const cli_help_option_t emit_options[] = {
        { OPTION_EMIT_AST, NULL, "Emit AST dump" },
        { OPTION_AST_OUTPUT, "<file>", "Set AST dump output path" },
        { OPTION_EMIT_IR, NULL, "Emit HIR dump" },
        { OPTION_IR_OUTPUT, "<file>", "Set HIR dump output path" },
        { OPTION_EMIT_LIR, NULL, "Emit LIR dump" },
        { OPTION_LIR_OUTPUT, "<file>", "Set LIR dump output path" },
        { OPTION_EMIT_ASM, NULL, "Emit produced assembly code" },
        { OPTION_ASM_OUTPUT, "<file>", "Set assembly output path" },
    };

    _print_version(stdout);
    fprintf(stdout, "Usage: cplc [options] <input files>\n");
    _print_help_section(stdout, "General options",      general_options, sizeof(general_options) / sizeof(general_options[0]));
    _print_help_section(stdout, "Optimization options", optimization_options, sizeof(optimization_options) / sizeof(optimization_options[0]));
    _print_help_section(stdout, "Target options",       target_options, sizeof(target_options) / sizeof(target_options[0]));
    _print_help_section(stdout, "Assembler options",    assembler_options, sizeof(assembler_options) / sizeof(assembler_options[0]));
    _print_help_section(stdout, "Linker options",       linker_options, sizeof(linker_options) / sizeof(linker_options[0]));
    _print_help_section(stdout, "Section options",      section_options, sizeof(section_options) / sizeof(section_options[0]));
    _print_help_section(stdout, "Emit options",         emit_options, sizeof(emit_options) / sizeof(emit_options[0]));
    return 0;
}

static inline int _readable_directory(const char* path) {
    return path && path[0] && !access(path, R_OK | X_OK);
}

static inline int _readable_file(const char* path) {
    return path && path[0] && !access(path, R_OK);
}

static int _path_from_executable(const char* argv0, const char* suffix, char* out, size_t out_size) {
    if (!argv0 || !suffix || !out || !out_size) return 0;
    char executable[PATH_MAX] = { 0 };
#ifdef __linux__
    ssize_t nread = readlink("/proc/self/exe", executable, sizeof(executable) - 1);
    if (nread > 0) executable[nread] = 0;
    else
#endif
    {
        if (!realpath(argv0, executable)) return 0;
    }

    char* slash = strrchr(executable, '/');
    if (!slash) return 0;
    *slash = 0;

    char candidate[PATH_MAX] = { 0 };
    int written = snprintf(candidate, sizeof(candidate), "%s/%s", executable, suffix);
    if (written <= 0 || (size_t)written >= sizeof(candidate)) return 0;

    char resolved[PATH_MAX] = { 0 };
    if (!realpath(candidate, resolved) || !_readable_directory(resolved)) return 0;
    if (strlen(resolved) + 1 > out_size) return 0;
    strcpy(out, resolved);
    return 1;
}

static int _file_from_executable(const char* argv0, const char* suffix, char* out, size_t out_size) {
    if (!argv0 || !suffix || !out || !out_size) return 0;
    char executable[PATH_MAX] = { 0 };
#ifdef __linux__
    ssize_t nread = readlink("/proc/self/exe", executable, sizeof(executable) - 1);
    if (nread > 0) executable[nread] = 0;
    else
#endif
    {
        if (!realpath(argv0, executable)) return 0;
    }

    char* slash = strrchr(executable, '/');
    if (!slash) return 0;
    *slash = 0;

    char candidate[PATH_MAX] = { 0 };
    int written = snprintf(candidate, sizeof(candidate), "%s/%s", executable, suffix);
    if (written <= 0 || (size_t)written >= sizeof(candidate)) return 0;

    char resolved[PATH_MAX] = { 0 };
    if (!realpath(candidate, resolved) || !_readable_file(resolved)) return 0;
    if (strlen(resolved) + 1 > out_size) return 0;
    strcpy(out, resolved);
    return 1;
}

static inline const char* _find_stdlib(const char* argv0, char* out, size_t out_size) {
    const char* env = getenv("CPL_INCLUDE_PATH");
    if (_readable_directory(env))                                            return env;
    if (_path_from_executable(argv0, "../share/cpl/include", out, out_size)) return out;
    if (_path_from_executable(argv0, "../../cpllib", out, out_size))         return out;
    if (_readable_directory(CPL_DEFAULT_INCLUDE_DIR))                        return CPL_DEFAULT_INCLUDE_DIR;
    if (realpath("cpllib", out) && _readable_directory(out))                 return out;
    return NULL;
}

static inline const char* _find_runtime_library(const char* argv0, char* out, size_t out_size) {
    const char* env = getenv("CPL_RUNTIME_LIB");
    if (_readable_file(env))                                                return env;
    if (_file_from_executable(argv0, "../lib/cpl/libcpl.a", out, out_size)) return out;
    if (_file_from_executable(argv0, "cpllib/libcpl.a", out, out_size))     return out;
    if (_readable_file(CPL_DEFAULT_RUNTIME_LIB))                            return CPL_DEFAULT_RUNTIME_LIB;
    return NULL;
}

static char* _make_temp_path(void) {
    char template[] = "/tmp/builder-XXXXXX";
    int fd = mkstemp(template);
    if (fd < 0) return NULL;
    close(fd);
    return strdup(template);
}

static int _run_tool(const char* tool, char* const argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 0;
    }

    if (!pid) {
        execvp(tool, argv);
        perror(tool);
        _exit(127);
    }

    int status = 0;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 0;
    }

    return WIFEXITED(status) && !WEXITSTATUS(status);
}

static int _copy_fd_to_stream(int fd, FILE* stream) {
    char buffer[4096] = { 0 };
    ssize_t nread = 0;
    while ((nread = read(fd, buffer, sizeof(buffer))) > 0) {
        size_t written = fwrite(buffer, 1, (size_t)nread, stream);
        if (written != (size_t)nread) return 0;
    }

    return !nread && !ferror(stream);
}

static int _copy_file_path(const char* src, const char* dst) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) return 0;

    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        close(src_fd);
        return 0;
    }

    int ok = 1;
    char buffer[4096] = { 0 };
    ssize_t nread = 0;
    while ((nread = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t offset = 0;
        while (offset < nread) {
            ssize_t written = write(dst_fd, buffer + offset, (size_t)(nread - offset));
            if (written < 0) {
                ok = 0;
                break;
            }
            offset += written;
        }
        if (!ok) break;
    }

    if (nread < 0) ok = 0;
    if (close(dst_fd) < 0) ok = 0;
    close(src_fd);
    return ok;
}

static int _move_file_path(const char* src, const char* dst) {
    if (rename(src, dst) == 0) return 1;
    if (errno != EXDEV) return 0;
    if (!_copy_file_path(src, dst)) return 0;
    return unlink(src) == 0;
}

static inline int _compile_asm_to_object(const options_t* options, const char* asm_path, const char* obj_path) {
    char* const cmd[] = {
        (char*)options->tools.asm_compiler, "-f",
        (char*)options->tools.asm_format, (char*)asm_path, "-o",
        (char*)obj_path, NULL
    };

    return _run_tool(options->tools.asm_compiler, cmd);
}

static int _link_objects(const options_t* options, char* const objects[], int objects_count) {
    int extra         = (options->tools.linker_use_c_driver ? 1 : 0) + (options->tools.linker_no_pie ? 1 : 0) + (options->tools.linker_m32 ? 1 : 0);
    int runtime_count = options->locations.runtime ? 1 : 0;
    int cmd_size      = objects_count + runtime_count + 5 + extra;
    char** cmd   = (char**)mm_malloc((size_t)cmd_size * sizeof(*cmd));
    if (!cmd) return 0;

    int j = 0;
    cmd[j++] = (char*)options->tools.linker;
    if (options->tools.linker_use_c_driver) {
        if (options->tools.linker_no_pie) cmd[j++] = "-no-pie";
        if (options->tools.linker_m32)    cmd[j++] = "-m32";
    }
    
    cmd[j++] = "-o";
    cmd[j++] = options->locations.output ? options->locations.output : "a.out";
    for (int i = 0; i < objects_count; i++) {
        cmd[j++] = objects[i];
    }
    if (options->locations.runtime) {
        cmd[j++] = (char*)options->locations.runtime;
    }

    cmd[j] = NULL;
    int ok = _run_tool(options->tools.linker, cmd);
    mm_free(cmd);
    return ok;
}

static inline const char* _output_path_or_default(const char* path, const char* fallback) {
    return path ? path : fallback;
}

static int _parse_long_arg(const char* s, long* out) {
    if (!s || !out) return 0;
    char* end = NULL;
    errno = 0;
    long v = strtol(s, &end, 10);
    if (errno || !end || *end) return 0;
    *out = v;
    return 1;
}

static int _parse_sys_type(const char* s, options_t* out) {
    if (!s || !out) return 0;
    if (!strcmp(s, "unknown")) {
        out->config.sys_type = UNKNOWN;
        return 1;
    }
    else if (!strcmp(s, "macho64")) {
        out->config.sys_type     = MACHO64;
        out->config.ro_section   = "__TEXT,__const";
        out->config.glob_section = "__DATA,__data";
        out->config.code_section = "__TEXT,__text";
        return 1;
    }
    else if (!strcmp(s, "linux64")) {
        out->config.sys_type     = LINUX64;
        out->config.ro_section   = ".rodata";
        out->config.glob_section = ".data";
        out->config.code_section = ".text";
        return 1;
    }
    else if (!strcmp(s, "i386")) {
        out->config.sys_type     = I386;
        out->config.ro_section   = ".rodata";
        out->config.glob_section = ".data";
        out->config.code_section = ".text";
        return 1;
    }
    else if (!strcmp(s, "windows64")) {
        out->config.sys_type     = WINDOWS64;
        return 1;
    }

    return 0;
}

static void _set_optimization_profile(options_t* out, int level) {
    if (!out) return;
    out->config.tre           = 0;
    out->config.finline       = 0;
    out->config.licm          = 0;
    out->config.constant      = 0;
    out->config.peephole      = 0;
    out->config.copy_prop     = 0;

    if (level >= 2) {
        out->config.licm      = 1;
        out->config.constant  = 1;
        out->config.peephole  = 1;
    }

    if (level >= 3) {
        out->config.copy_prop = 1;
        out->config.tre       = 1;
        out->config.finline   = 1;
    }
}

static void _set_arch_profile(options_t* out, const char* arch) {
    if (!out || !arch) return;
    if (
        !strcmp(arch, "x86")  || 
        !strcmp(arch, "i386") || 
        !strcmp(arch, "ia32")
    ) {
        out->config.full_bytness  = 4;
        out->config.half_bytness  = 4;
        out->config.quart_bytness = 2;
        out->config.eight_bytness = 1;
        out->config.sys_type      = I386;
        out->tools.asm_format     = "elf32";
        out->tools.linker_m32     = 1;
    }
    else if (
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
            .entry_name     = options->config.entry_name,
            .ro_section     = options->config.ro_section,
            .glob_section   = options->config.glob_section,
            .code_section   = options->config.code_section,
            .bytness        = {
                .bytness    = options->config.full_bytness,
                .h_bytness  = options->config.half_bytness,
                .q_bytness  = options->config.quart_bytness,
                .e_bytness  = options->config.eight_bytness,
            },
            .sys_type       = options->config.sys_type,
        },
        .optimization_flags = {
            .tre            = options->config.tre      ? 1 : 0,
            .finline        = options->config.finline  ? 1 : 0,
            .licm           = options->config.licm     ? 1 : 0,
            .constant       = options->config.constant ? 1 : 0,
            .peephole       = options->config.peephole ? 1 : 0,
        },
        .compilation_flags  = {
            .debug          = options->config.debug ? 1 : 0,
        },
    };

    return conf;
}

typedef struct {
    char* name;
    char* value;
} cli_define_t;

static char* _copy_slice(const char* begin, size_t len) {
    if (!begin) return NULL;
    char* out = (char*)mm_malloc(len + 1);
    if (!out) return NULL;
    memcpy(out, begin, len);
    out[len] = 0;
    return out;
}

static int _valid_define_name(const char* name, size_t name_len) {
    if (!name || !name_len) return 0;
    for (size_t i = 0; i < name_len; i++) {
        char c = name[i];
        int ok =
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_' ||
            (i > 0 && c >= '0' && c <= '9');
        if (!ok) return 0;
    }

    return 1;
}

static int _unload_cli_define(void* data) {
    cli_define_t* define = (cli_define_t*)data;
    if (!define) return 1;
    mm_free(define->name);
    mm_free(define->value);
    mm_free(define);
    return 1;
}

static cli_define_t* _make_define_arg(const char* arg) {
    if (!arg || !arg[0]) return NULL;

    const char* eq  = strchr(arg, '=');
    size_t name_len = eq ? (size_t)(eq - arg) : strlen(arg);
    if (!_valid_define_name(arg, name_len)) return NULL;

    const char* value = eq ? eq + 1 : "1";
    cli_define_t* define = (cli_define_t*)mm_malloc(sizeof(*define));
    if (!define) return NULL;

    define->name  = _copy_slice(arg, name_len);
    define->value = _copy_slice(value, strlen(value));
    if (!define->name || !define->value) {
        _unload_cli_define(define);
        return NULL;
    }

    return define;
}

static int _add_define_arg(options_t* out, const char* arg) {
    if (!out) return 0;
    cli_define_t* define = _make_define_arg(arg);
    if (!define) return 0;
    if (!list_push_back(&out->locations.defines, define)) {
        _unload_cli_define(define);
        return 0;
    }

    return 1;
}

static inline void _apply_cli_defines(pp_ctx_t* ppctx, list_t* defines) {
    if (!ppctx || !defines) return;
    cli_define_t* define = NULL;
    foreach (define, defines) {
        MCTB_put_define(define->name, define->value, &ppctx->defines);
    }
}

static void _set_default_options(options_t* out) {
    memset(out, 0, sizeof(*out));
    list_init(&out->locations.defines);
    out->tools.asm_compiler        = "nasm";
#if defined(__linux__)
    out->tools.asm_format          = "elf64";
    out->tools.linker              = "gcc";
    out->tools.linker_no_pie       = 1;
    out->config.entry_name         = "main";
    out->config.ro_section         = ".rodata";
    out->config.glob_section       = ".data";
    out->config.code_section       = ".text";
    out->config.sys_type           = LINUX64;
#else
    out->tools.asm_format          = "macho64";
    out->tools.linker              = "clang";
    out->tools.linker_no_pie       = 0;
    out->config.entry_name         = "_main";
    out->config.ro_section         = "__TEXT,__const";
    out->config.glob_section       = "__DATA,__data";
    out->config.code_section       = "__TEXT,__text";
    out->config.sys_type           = MACHO64;
#endif
    out->tools.linker_use_c_driver = 1;
    out->tools.linker_m32          = 0;
    out->config.full_bytness       = 8;
    out->config.half_bytness       = 4;
    out->config.quart_bytness      = 2;
    out->config.eight_bytness      = 1;
    out->config.debug              = 0;
    _set_optimization_profile(out, 0);
}

static int _parse_input_args(char* argv[], int argc, options_t* out) {
    if (!argv || argc <= 0 || !out) return 0;
    _set_default_options(out);
    out->locations.files = mm_malloc((size_t)argc * sizeof(*out->locations.files));
    if (!out->locations.files) return 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], OPTION_HELP_SHORT) || !strcmp(argv[i], OPTION_HELP))                out->flags.show_help           = 1;
        else if (!strcmp(argv[i], OPTION_VERSION_SHORT) || !strcmp(argv[i], OPTION_VERSION))     out->flags.show_version        = 1;
        else if (!strcmp(argv[i], OPTION_SOMETHING_SHORT) || !strcmp(argv[i], OPTION_SOMETHING)) out->flags.show_something      = 1;
        else if (!strcmp(argv[i], OPTION_PREPROCESS_ONLY))                                       out->flags.preprocess_only     = 1;
        else if (!strcmp(argv[i], OPTION_WITHOUT_COMPILATION))                                   out->flags.without_compilation = 1;
        else if (!strcmp(argv[i], OPTION_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.output = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_INLUCDE)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.include = argv[++i];
        }
        else if (!strcmp(argv[i], OPTION_DEFINE)) {
            if (i + 1 >= argc || !_add_define_arg(out, argv[i + 1])) goto _fail;
            i++;
        }
        else if (!strncmp(argv[i], OPTION_DEFINE, strlen(OPTION_DEFINE))) {
            if (!_add_define_arg(out, argv[i] + strlen(OPTION_DEFINE))) goto _fail;
        }
        else if (!strcmp(argv[i], OPTION_PRINT_STDLIB)) out->flags.print_stdlib = 1;
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
            if (!strcmp(mode, "c") || !strcmp(mode, "driver"))    out->tools.linker_use_c_driver    = 1;
            else if (!strcmp(mode, "raw") || !strcmp(mode, "ld")) out->tools.linker_use_c_driver    = 0;
            else goto _fail;
        }
        else if (
            !strcmp(argv[i], OPTION_COMPILE_ONLY_SHORT) ||
            !strcmp(argv[i], OPTION_COMPILE_ONLY)       ||
            !strcmp(argv[i], OPTION_WITHOUT_LINKER)
        ) out->flags.compile_only = 1;
        else if (!strcmp(argv[i], OPTION_NO_COMPILE))           out->flags.no_compile      = 1;
        else if (!strcmp(argv[i], OPTION_NO_OBJECT_BUILD))      out->flags.no_object_build = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_NO_PIE))        out->tools.linker_no_pie   = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_PIE))           out->tools.linker_no_pie   = 0;
        else if (!strcmp(argv[i], OPTION_LINKER_M32))           out->tools.linker_m32      = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_NO_M32))        out->tools.linker_m32      = 0;
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
            if (i + 1 >= argc || !_parse_sys_type(argv[i + 1], out)) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_TRE))                  out->config.tre         = 1;
        else if (!strcmp(argv[i], OPTION_NO_TRE))               out->config.tre         = 0;
        else if (!strcmp(argv[i], OPTION_FINLINE))              out->config.finline     = 1;
        else if (!strcmp(argv[i], OPTION_NO_FINLINE))           out->config.finline     = 0;
        else if (!strcmp(argv[i], OPTION_LICM))                 out->config.licm        = 1;
        else if (!strcmp(argv[i], OPTION_NO_LICM))              out->config.licm        = 0;
        else if (!strcmp(argv[i], OPTION_CONSTANT))             out->config.constant    = 1;
        else if (!strcmp(argv[i], OPTION_NO_CONSTANT))          out->config.constant    = 0;
        else if (!strcmp(argv[i], OPTION_COPYPROP))             out->config.copy_prop   = 1;
        else if (!strcmp(argv[i], OPTION_NO_COPYPROP))          out->config.copy_prop   = 0;
        else if (!strcmp(argv[i], OPTION_PEEPHOLE))             out->config.peephole    = 1;
        else if (!strcmp(argv[i], OPTION_NO_PEEPHOLE))          out->config.peephole    = 0;
        else if (!strcmp(argv[i], OPTION_ENABLE_AST_ANALYSIS))  out->flags.ast_analysis = 1;
        else if (!strcmp(argv[i], OPTION_ENABLE_IR_ANALYSIS))   out->flags.hir_analysis = 1;
        else if (!strcmp(argv[i], OPTION_EMIT_AST))             out->config.emit_ast    = 1;
        else if (!strcmp(argv[i], OPTION_AST_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.ast_output = argv[++i];
            out->config.emit_ast = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_IR))              out->config.emit_ir      = 1;
        else if (!strcmp(argv[i], OPTION_IR_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.ir_output = argv[++i];
            out->config.emit_ir = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_LIR))             out->config.emit_lir     = 1;
        else if (!strcmp(argv[i], OPTION_LIR_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.lir_output = argv[++i];
            out->config.emit_lir = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_ASM))             out->config.emit_asm     = 1;
        else if (!strcmp(argv[i], OPTION_ASM_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.asm_output = argv[++i];
            out->config.emit_asm = 1;
        }
        else if (!strcmp(argv[i], OPTION_DEBUG))                out->config.debug       = 1;
        else if (!strcmp(argv[i], OPTION_NO_DEBUG))             out->config.debug       = 0;
        else if (!strcmp(argv[i], OPTION_NO_OPTIMIZATION))      _set_optimization_profile(out, 0);
        else if (!strcmp(argv[i], OPTION_ROUGHT_OPTIMIZATION))  _set_optimization_profile(out, 1);
        else if (!strcmp(argv[i], OPTION_GOOD_OPTIMIZATION))    _set_optimization_profile(out, 2);
        else if (!strcmp(argv[i], OPTION_MAX_OPTIMIZATION))     _set_optimization_profile(out, 3);
        else if (argv[i][0] == '-') goto _fail;
        else out->locations.files[out->locations.files_count++] = argv[i];
    }

    if (out->flags.compile_only && (out->flags.no_compile || out->flags.no_object_build)) goto _fail;

    if (out->flags.no_compile || out->flags.no_object_build) {
        out->config.emit_asm = 1;
    }

    return 1;

_fail: {}
    list_free_force_op(&out->locations.defines, _unload_cli_define);
    mm_free((void*)out->locations.files);
    out->locations.files = NULL;
    return 0;
}

#define RELOAD_CFG                         \
    HIR_CFG_unload(&cfgctx);               \
    HIR_CFG_build(&hirctx, &cfgctx, &smt); \
    HIR_CG_unload(&callctx);               \
    HIR_CG_build(&cfgctx, &callctx, &smt); \
    HIR_CG_perform_dfe(&callctx, &smt);    \
    HIR_CG_apply_dfe(&cfgctx, &smt);

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

    if (options.flags.show_help) {
        _print_help_message();
        list_free_force_op(&options.locations.defines, _unload_cli_define);
        mm_free((void*)options.locations.files);
        return EXIT_SUCCESS;
    }

    if (options.flags.show_something) {
        _print_gem(stdout);
        list_free_force_op(&options.locations.defines, _unload_cli_define);
        mm_free((void*)options.locations.files);
        return EXIT_SUCCESS;
    }

    if (options.flags.show_version) {
        _print_version(stdout);
        list_free_force_op(&options.locations.defines, _unload_cli_define);
        mm_free((void*)options.locations.files);
        return EXIT_SUCCESS;
    }

    char stdlib_path[PATH_MAX] = { 0 };
    options.locations.stdlib = _find_stdlib(argv[0], stdlib_path, sizeof(stdlib_path));
    char runtime_path[PATH_MAX] = { 0 };
    options.locations.runtime = _find_runtime_library(argv[0], runtime_path, sizeof(runtime_path));
    if (options.flags.print_stdlib) {
        if (options.locations.stdlib) {
            puts(options.locations.stdlib);
            list_free_force_op(&options.locations.defines, _unload_cli_define);
            mm_free((void*)options.locations.files);
            return EXIT_SUCCESS;
        }
        fprintf(stderr, "CPL standard library isn't found\n");
        list_free_force_op(&options.locations.defines, _unload_cli_define);
        mm_free((void*)options.locations.files);
        return EXIT_FAILURE;
    }

    if (!options.locations.files_count) {
        fprintf(stderr, "No input files\n");
        list_free_force_op(&options.locations.defines, _unload_cli_define);
        mm_free((void*)options.locations.files);
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
    memset(object_files, 0, (size_t)options.locations.files_count * sizeof(*object_files));

    list_t* token_lists = mm_malloc((size_t)options.locations.files_count * sizeof(*token_lists));
    if (!token_lists && options.locations.files_count > 0) {
        fprintf(stderr, "Can't allocate token lists array\n");
        return EXIT_FAILURE;
    }
    memset(token_lists, 0, (size_t)options.locations.files_count * sizeof(*token_lists));
    int token_lists_count = 0;

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    for (int i = 0; i < options.locations.files_count; i++) {
        int fd = open(options.locations.files[i], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s isn't found!\n", options.locations.files[i]);
            return 1;
        }

        finder_ctx_t finctx = {
            .bpath = options.locations.include,
            .spath = options.locations.stdlib
        };

        pp_ctx_t ppctx;
        PP_init_pp_ctx(&ppctx);
        _apply_cli_defines(&ppctx, &options.locations.defines);

        fd = PP_perform(fd, &finctx, &ppctx);
        if (fd < 0) {
            fprintf(stderr, "Failed to preprocess %s\n", options.locations.files[i]);
            return 1;
        }

        if (options.flags.preprocess_only) {
            FILE* output = stdout;
            if (options.locations.output) {
                output = fopen(options.locations.output, "w");
                if (!output) {
                    fprintf(stderr, "Can't open output file %s: %s\n", options.locations.output, strerror(errno));
                    close(fd);
                    return 1;
                }
            }

            if (!_copy_fd_to_stream(fd, output)) {
                fprintf(stderr, "Can't write preprocessed output for %s\n", options.locations.files[i]);
                if (output != stdout) fclose(output);
                close(fd);
                return 1;
            }

            if (output != stdout) fclose(output);
            close(fd);
            continue;
        }

        list_t* tokens = &token_lists[i];
        list_init(tokens);
        token_lists_count = i + 1;
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

        if (i + 1 < options.locations.files_count) {
            close(fd);
            continue;
        }

        if (!AST_finalize_parse(&sctx, &smt)) {
            fprintf(stderr, "AST finalization error!\n");
            return 1;
        }

        if (options.config.emit_ast) {
            const char* ast_output = _output_path_or_default(options.locations.ast_output, "output.ast");
            FILE* ast_file = fopen(ast_output, "w");
            if (!ast_file) {
                fprintf(stderr, "Can't open AST output file %s: %s\n", ast_output, strerror(errno));
                return 1;
            }
            DUMP_format_astctx(&sctx, ast_file);
            fclose(ast_file);
        }

        if (options.flags.ast_analysis) {
            SEM_perform_ast_check(&sctx, &smt);
        }

        hir_ctx_t hirctx = { 0 };
        HIR_generate(&sctx, &hirctx, &smt);

        if (options.flags.without_compilation && options.config.emit_ir) {
            const char* ir_output = _output_path_or_default(options.locations.ir_output, "output.ir");
            FILE* ir_file = fopen(ir_output, "w");
            if (!ir_file) {
                fprintf(stderr, "Can't open HIR output file %s: %s\n", ir_output, strerror(errno));
                return 1;
            }
            DUMP_format_hirctx(&hirctx, &smt, 0, 0, ir_file);
            fclose(ir_file);
        }

        if (options.flags.without_compilation) {
            HIR_unload_blocks(hirctx.hot.h);
            for (int j = 0; j < token_lists_count; j++) {
                list_free_force_op(&token_lists[j], (int (*)(void *))TKN_unload_token);
            }
            AST_unload_ctx(&sctx);
            SMT_unload(&smt);
            close(fd);
            continue;
        }

        cfg_ctx_t cfgctx = { .cid = 0 };
        HIR_CFG_build(&hirctx, &cfgctx, &smt);
        
        call_graph_t callctx;
        HIR_CG_build(&cfgctx, &callctx, &smt);

        HIR_FUNC_set_unused_duplicated_functions(&cfgctx);
        HIR_FUNC_set_last_return(&cfgctx);

        if (options.config.tre) {
            HIR_FUNC_perform_tre(&cfgctx, &smt);
        }

        RELOAD_CFG;
        
        // HIR_CFG_finilize_before_dom(&cfgctx);
        HIR_CFG_create_domdata(&cfgctx);
        ltree_ctx_t lctx;
        map_init(&lctx.lmap, MAP_NO_CMP);
        HIR_LOOP_mark_loops(&cfgctx, &lctx);

        if (options.config.finline) {
            HIR_FUNC_perform_inline(&cfgctx, &lctx, &smt);
            HIR_LTREE_unload_ctx(&lctx);
            RELOAD_CFG;
            // HIR_CFG_finilize_before_dom(&cfgctx);
            HIR_CFG_create_domdata(&cfgctx);
            map_init(&lctx.lmap, MAP_NO_CMP);
            HIR_LOOP_mark_loops(&cfgctx, &lctx);
        }

        HIR_LTREE_canonicalization(&cfgctx, &lctx);
        HIR_LOOP_perform_dle(&lctx);

        HIR_CFG_unload_domdata(&cfgctx);
        HIR_CFG_finilize_before_dom(&cfgctx);
        HIR_CFG_create_domdata(&cfgctx);

        ssa_ctx_t ssactx;
        map_init(&ssactx.vers, MAP_NO_CMP);
        HIR_SSA_insert_phi(&cfgctx, &smt);
        HIR_SSA_rename(&cfgctx, &ssactx, &smt);
        map_free_force(&ssactx.vers);

        HIR_compute_homes(&hirctx);
        if (options.config.licm) {
            HIR_LTREE_licm(&cfgctx, &lctx, &smt);
        }

        HIR_CFG_make_allias(&cfgctx, &smt);
        dag_ctx_t dagctx = { .curr_id = 0 };
        HIR_DAG_init(&dagctx);
        if (options.config.constant) {
            HIR_DAG_generate(&cfgctx, &dagctx, &smt);
            HIR_DAG_CFG_rebuild(&cfgctx, &dagctx);
            int folded = 0;
            do {
                folded = 0;
                HIR_sparse_const_propagation(&dagctx, &smt);
                folded = HIR_sparse_const_funcall_propagation(&cfgctx, &smt) || folded;
                folded = HIR_sparce_const_fret_propagation(&cfgctx, &smt)    || folded;
            } while (folded);
        }

        HIR_CFG_squeeze_blocks(&cfgctx);

        if (options.flags.hir_analysis) {
            SEM_perform_hir_check(&cfgctx, &dagctx, &hirctx, &smt);
        }

        if (options.config.emit_ir) {
            const char* ir_output = _output_path_or_default(options.locations.ir_output, "output.ir");
            FILE* ir_file = fopen(ir_output, "w");
            if (!ir_file) {
                fprintf(stderr, "Can't open HIR output file %s: %s\n", ir_output, strerror(errno));
                return 1;
            }
            DUMP_format_hirctx(&hirctx, &smt, 0, 0, ir_file);
            fclose(ir_file);
        }

        lir_ctx_t lirctx = { 0 };
        LIR_generate(&cfgctx, &lirctx, &smt);

        if (options.config.copy_prop) {
            LIR_variable_copy_propagation(&cfgctx, &smt);
            LIR_drop_unused_variables(&cfgctx);
        }

        inst_selector_t  inst_sel;
        register_saver_t reg_save;
        mem_selector_t   mem_sel;
        peephole_t       pph;
        long (*precolor)(lir_registers_t);
        switch (CONF_get_system_type()) {
            default:
            case MACHO64: {
                inst_sel.select_instructions = x86_64_macho_nasm_instruction_selection;
                reg_save.save_registers      = x86_64_macho_nasm_caller_saving;
                mem_sel.select_memory        = x86_64_macho_nasm_memory_selection;
                mem_sel.validate_memory      = x86_64_macho_nasm_memory_validation;
                pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
                precolor                     = x86_64_gnu_precolored_reg_to_color;
                break;
            }
            case LINUX64: {
                inst_sel.select_instructions = x86_64_gnu_nasm_instruction_selection;
                reg_save.save_registers      = x86_64_gnu_nasm_caller_saving;
                mem_sel.select_memory        = x86_64_gnu_nasm_memory_selection;
                mem_sel.validate_memory      = x86_64_gnu_nasm_memory_validation;
                pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
                precolor                     = x86_64_gnu_precolored_reg_to_color;
                break;
            }
            case I386: {
                inst_sel.select_instructions = i386_gnu_nasm_instruction_selection;
                reg_save.save_registers      = i386_gnu_nasm_caller_saving;
                mem_sel.select_memory        = i386_gnu_nasm_memory_selection;
                mem_sel.validate_memory      = i386_gnu_nasm_memory_validation;
                pph.perform_peephole         = x86_64_gnu_nasm_peephole_optimization;
                precolor                     = i386_gnu_precolored_reg_to_color;
                break;
            }
        }

        LIR_select_instructions(&cfgctx, &smt, &inst_sel);

        LIR_DFG_compute_inout(&cfgctx);
        LIR_DFG_create_deall(&cfgctx, &smt);

        if (options.config.emit_lir) {
            const char* lir_output = _output_path_or_default(options.locations.lir_output, "output.lir");
            FILE* lir_file = fopen(lir_output, "w");
            if (!lir_file) {
                fprintf(stderr, "Can't open LIR output file %s: %s\n", lir_output, strerror(errno));
                return 1;
            }
            DUMP_format_lirctx(&lirctx, &smt, 0, 0, lir_file);
            fclose(lir_file);
        }

        map_t colors;
        map_init(&colors, MAP_NO_CMP);
        LIR_RA_init_colors(&colors, &smt, precolor);

        LIR_regalloc(&cfgctx, &smt, &colors);
        LIR_RA_sort_phi_movs(&cfgctx, &colors);
        LIR_select_memory(&cfgctx, &colors, &smt, &mem_sel);
        
        if (options.config.copy_prop) {
            LIR_register_copy_propagation(&cfgctx);
        }

        LIR_destroy_ssa(&cfgctx);
        LIR_save_registers(&cfgctx, &callctx, &smt, &reg_save);

        if (options.config.peephole) {
            LIR_peephole_optimization(&cfgctx, &pph);
        }
        
        LIR_validate_memory(&cfgctx, &smt, &mem_sel);

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

        asm_gen_t asmgen = { 0 };
        switch (CONF_get_system_type()) {
            case MACHO64: asmgen.generator = x86_64_macho_nasm_generate_asm; break;
            case LINUX64: asmgen.generator = x86_64_gnu_nasm_generate_asm;   break;
            case I386:    asmgen.generator = i386_gnu_nasm_generate_asm;     break;
            default: break;
        }

        ASM_generate(&cfgctx, &smt, &asmgen, asm_file); 
        if (options.config.emit_asm) {
            const char* asm_output = _output_path_or_default(options.locations.asm_output, "output.s");
            FILE* asm_emit = fopen(asm_output, "w");
            if (!asm_emit) {
                fprintf(stderr, "Can't open ASM output file %s: %s\n", asm_output, strerror(errno));
                return 1;
            }
            ASM_generate(&cfgctx, &smt, &asmgen, asm_emit);
            fclose(asm_emit);
        }

        fclose(asm_file);

        if (
            options.flags.no_compile || 
            options.flags.no_object_build
        ) {
            unlink(asm_path);
            mm_free(asm_path);
            mm_free(obj_path);
        }
        else {
            if (!_compile_asm_to_object(&options, asm_path, obj_path)) {
                fprintf(stderr, "ASM compilation failed for %s\n", options.locations.files[i]);
                return 1;
            }

            unlink(asm_path);
            mm_free(asm_path);
            if (options.flags.compile_only) {
                const char* object_output = _output_path_or_default(options.locations.output, "output.o");
                if (!_move_file_path(obj_path, object_output)) {
                    fprintf(stderr, "Can't write object file %s: %s\n", object_output, strerror(errno));
                    mm_free(obj_path);
                    return 1;
                }

                mm_free(obj_path);
            }
            else {
                object_files[0] = obj_path;
            }
        }

        map_free(&colors);
        LIR_unload_blocks(lirctx.h);
        HIR_LTREE_unload_ctx(&lctx);
        HIR_CG_unload(&callctx);
        HIR_CFG_unload(&cfgctx);
        HIR_unload_blocks(hirctx.hot.h);
        for (int j = 0; j < token_lists_count; j++) {
            list_free_force_op(&token_lists[j], (int (*)(void *))TKN_unload_token);
        }
        AST_unload_ctx(&sctx);

        SMT_unload(&smt);
        close(fd);
    }

    if (
        !options.flags.no_compile          &&
        !options.flags.no_object_build     &&
        !options.flags.compile_only        &&
        !options.flags.preprocess_only     &&
        !options.flags.without_compilation &&
        options.locations.files_count > 0
    ) {
        if (!_link_objects(&options, object_files, 1)) {
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
    mm_free(token_lists);
    list_free_force_op(&options.locations.defines, _unload_cli_define);
    mm_free((void*)options.locations.files);
    return EXIT_SUCCESS;
}
