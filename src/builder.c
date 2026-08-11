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
        { OPTION_INLUCDE, "<dir>", "Add include directory" },
        { OPTION_DEFINE, "<name=value>", "Define preprocessor variable" },
        { OPTION_PRINT_STDLIB, NULL, "Print the standard library directory" },
        { OPTION_OUTPUT, "<file>", "Set output file" },
        { OPTION_ENABLE_AST_ANALYSIS, NULL, "Enable AST analysis" },
        { OPTION_ENABLE_IR_ANALYSIS, NULL, "Enable IR analysis" },
        { OPTION_ANALYSIS_ONLY, NULL, "Run AST and HIR analysis, then stop before code generation" },
        { OPTION_DEBUG, NULL, "Enable debug mode" },
        { OPTION_NO_DEBUG, NULL, "Disable debug mode" },
        { OPTION_NO_STRICT, NULL, "Keep compiling after static analysis warnings" },
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
        { "-L<dir>, -l<name>, -Wl,<arg>", NULL, "Pass library search paths, libraries, and driver linker options to the linker" },
        { OPTION_LINKER_ARG_SHORT ", " OPTION_LINKER_ARG, "<arg>", "Pass one raw argument to the linker command" },
        { OPTION_COMPILE_ONLY_SHORT ", " OPTION_COMPILE_ONLY, NULL, "Build an object file and skip linking" },
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
        { OPTION_EMIT_HIR_CFG, "<name>", "Emit HIR CFG dump for function" },
        { OPTION_EMIT_LIR, NULL, "Emit LIR dump" },
        { OPTION_EMIT_LIR_CFG, "<name>", "Emit LIR CFG dump for function" },
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

static inline char* _find_stdlib(const char* argv0, char* out, size_t out_size) {
    char* env = getenv("CPL_INCLUDE_PATH");
    if (_readable_directory(env))                                            return env;
    if (_path_from_executable(argv0, "../share/cpl/include", out, out_size)) return out;
    if (_path_from_executable(argv0, "../../cpllib", out, out_size))         return out;
    if (_readable_directory(CPL_DEFAULT_INCLUDE_DIR))                        return CPL_DEFAULT_INCLUDE_DIR;
    if (realpath("cpllib", out) && _readable_directory(out))                 return out;
    return NULL;
}

static inline char* _find_runtime_library(const char* argv0, char* out, size_t out_size) {
    char* env = getenv("CPL_RUNTIME_LIB");
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

static int _push_cmd_arg(list_t* cmd, const char* arg) {
    return list_push_back(cmd, (void*)arg);
}

static char** _make_argv(list_t* args) {
    if (!args) return NULL;
    if (!list_push_back(args, NULL)) return NULL;
    char** argv = (char**)list_flatten(args);
    list_remove(args, NULL);
    return argv;
}

static int _run_tool(const char* tool, list_t* args) {
    char** argv = _make_argv(args);
    if (!argv) return 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        mm_free(argv);
        return 0;
    }

    if (!pid) {
        execvp(tool, argv);
        perror(tool);
        _exit(127);
    }

    int status = 0;
    pid_t waited = 0;
    do {
        waited = waitpid(pid, &status, 0);
    } while (waited < 0 && errno == EINTR);

    if (waited < 0) {
        perror("waitpid");
        mm_free(argv);
        return 0;
    }

    mm_free(argv);
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
    if (
        errno != EXDEV ||
        !_copy_file_path(src, dst)
    ) return 0;
    return unlink(src) == 0;
}

static inline int _compile_asm_to_object(const options_t* options, const char* asm_path, const char* obj_path) {
    list_t cmd;
    list_init(&cmd);

    int ok =
        _push_cmd_arg(&cmd, options->tools.asm_compiler) &&
        _push_cmd_arg(&cmd, "-f")                       &&
        _push_cmd_arg(&cmd, options->tools.asm_format)   &&
        _push_cmd_arg(&cmd, asm_path)                    &&
        _push_cmd_arg(&cmd, "-o")                       &&
        _push_cmd_arg(&cmd, obj_path);

    if (ok) ok = _run_tool(options->tools.asm_compiler, &cmd);
    list_free(&cmd);
    return ok;
}

static int _link_objects(const options_t* options, list_t* objects) {
    list_t cmd;
    list_init(&cmd);

    if (!_push_cmd_arg(&cmd, options->tools.linker)) goto _fail;
    if (options->tools.linker_use_c_driver) {
        if (options->tools.linker_no_pie && !_push_cmd_arg(&cmd, "-no-pie")) goto _fail;
        if (options->tools.linker_m32    && !_push_cmd_arg(&cmd, "-m32"))    goto _fail;
    }

    if (
        !_push_cmd_arg(&cmd, "-o") ||
        !_push_cmd_arg(&cmd, options->locations.output ? options->locations.output : "a.out")
    ) goto _fail;

    char* object = NULL;
    foreach (object, objects) {
        if (!_push_cmd_arg(&cmd, object)) goto _fail;
    }

    if (options->locations.runtime) {
        if (!_push_cmd_arg(&cmd, options->locations.runtime)) goto _fail;
    }

    char* linker_arg = NULL;
    foreach (linker_arg, (list_t*)&options->tools.linker_args) {
        if (!_push_cmd_arg(&cmd, linker_arg)) goto _fail;
    }

    int ok = _run_tool(options->tools.linker, &cmd);
    list_free(&cmd);
    return ok;

_fail:
    list_free(&cmd);
    return 0;
}

static inline const char* _output_path_or_default(const char* path, const char* fallback) {
    return path ? path : fallback;
}

static inline int _parse_long_arg(const char* s, long* out) {
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

    if (!strcmp(s, "macho64")) {
        out->config.sys_type     = MACHO64;
        out->config.ro_section   = "__TEXT,__const";
        out->config.glob_section = "__DATA,__data";
        out->config.code_section = "__TEXT,__text";
        return 1;
    }

    if (!strcmp(s, "linux64")) {
        out->config.sys_type     = LINUX64;
        out->config.ro_section   = ".rodata";
        out->config.glob_section = ".data";
        out->config.code_section = ".text";
        return 1;
    }

    if (!strcmp(s, "i386")) {
        out->config.sys_type     = I386;
        out->config.ro_section   = ".rodata";
        out->config.glob_section = ".data";
        out->config.code_section = ".text";
        return 1;
    }

    if (!strcmp(s, "windows64")) {
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

static inline int _set_build_mode(options_t* out, build_mode_t mode) {
    if (!out) return 0;
    if (out->build_mode != BUILD_MODE_EXECUTABLE && out->build_mode != mode) return 0;
    out->build_mode = mode;
    return 1;
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
            .debug          = options->config.debug  ? 1 : 0,
            .strict         = options->config.strict ? 1 : 0,
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
            c == '_'               ||
            (i > 0 && c >= '0' && c <= '9');
        if (!ok) return 0;
    }

    return 1;
}

static int _unload_cli_define(cli_define_t* define) {
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
    cli_define_t* define = (cli_define_t*)mm_malloc(sizeof(cli_define_t));
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

static int _add_linker_arg(options_t* out, const char* arg) {
    if (!out || !arg || !arg[0]) return 0;
    char* copy = _copy_slice(arg, strlen(arg));
    if (!copy) return 0;
    if (!list_push_back(&out->tools.linker_args, copy)) {
        mm_free(copy);
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

static void _unload_options(options_t* options) {
    if (!options) return;
    list_free(&options->locations.files);
    list_free_force_op(&options->locations.defines, (int (*)(void*))_unload_cli_define);
    list_free_force(&options->tools.linker_args);
}

static int _unload_token_list(void* data) {
    list_t* tokens = (list_t*)data;
    if (!tokens) return 1;
    list_free_force_op(tokens, (int (*)(void *))TKN_unload_token);
    mm_free(tokens);
    return 1;
}

static void _unload_token_lists(list_t* token_lists) {
    if (!token_lists) return;
    list_free_force_op(token_lists, _unload_token_list);
}

static void _set_default_options(options_t* out) {
    memset(out, 0, sizeof(options_t));
    list_init(&out->locations.files);
    list_init(&out->locations.defines);
    list_init(&out->tools.linker_args);
    out->build_mode                = BUILD_MODE_EXECUTABLE;
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
    out->config.strict             = 1;
    _set_optimization_profile(out, 0);
}

static int _parse_input_args(char* argv[], int argc, options_t* out) {
    if (!argv || argc <= 0 || !out) return 0;
    _set_default_options(out);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], OPTION_HELP_SHORT) || !strcmp(argv[i], OPTION_HELP))                out->flags.show_help           = 1;
        else if (!strcmp(argv[i], OPTION_VERSION_SHORT) || !strcmp(argv[i], OPTION_VERSION))     out->flags.show_version        = 1;
        else if (!strcmp(argv[i], OPTION_SOMETHING_SHORT) || !strcmp(argv[i], OPTION_SOMETHING)) out->flags.show_something      = 1;
        else if (!strcmp(argv[i], OPTION_PREPROCESS_ONLY))                                       out->flags.preprocess_only     = 1;
        else if (!strcmp(argv[i], OPTION_ANALYSIS_ONLY)) {
            if (!_set_build_mode(out, BUILD_MODE_ANALYSIS)) goto _fail;
            out->flags.ast_analysis = 1;
            out->flags.hir_analysis = 1;
        }
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
            if (!strcmp(mode, "c") || !strcmp(mode, "driver"))    out->tools.linker_use_c_driver = 1;
            else if (!strcmp(mode, "raw") || !strcmp(mode, "ld")) out->tools.linker_use_c_driver = 0;
            else goto _fail;
        }
        else if (!strcmp(argv[i], OPTION_LINKER_ARG_SHORT) || !strcmp(argv[i], OPTION_LINKER_ARG)) {
            if (i + 1 >= argc || !_add_linker_arg(out, argv[i + 1])) goto _fail;
            i++;
        }
        else if (
            !strncmp(argv[i], "-l", 2) ||
            !strncmp(argv[i], "-L", 2) ||
            !strncmp(argv[i], "-Wl,", 4)
        ) {
            if (!_add_linker_arg(out, argv[i])) goto _fail;
            if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "-L")) {
                if (i + 1 >= argc) goto _fail;
                if (!_add_linker_arg(out, argv[i + 1])) goto _fail;
                i++;
            }
        }
        else if (
            !strcmp(argv[i], OPTION_COMPILE_ONLY_SHORT) ||
            !strcmp(argv[i], OPTION_COMPILE_ONLY)
        ) {
            if (!_set_build_mode(out, BUILD_MODE_OBJECT)) goto _fail;
        }
        else if (!strcmp(argv[i], OPTION_LINKER_NO_PIE)) out->tools.linker_no_pie = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_PIE))    out->tools.linker_no_pie = 0;
        else if (!strcmp(argv[i], OPTION_LINKER_M32))    out->tools.linker_m32    = 1;
        else if (!strcmp(argv[i], OPTION_LINKER_NO_M32)) out->tools.linker_m32    = 0;
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
        else if (!strcmp(argv[i], OPTION_EMIT_IR))              out->config.emit_ir     = 1;
        else if (!strcmp(argv[i], OPTION_IR_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.ir_output = argv[++i];
            out->config.emit_ir = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_HIR_CFG)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.hir_cfg_name = argv[++i];
            out->config.emit_hir_cfg = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_LIR))             out->config.emit_lir    = 1;
        else if (!strcmp(argv[i], OPTION_EMIT_LIR_CFG)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.lir_cfg_name = argv[++i];
            out->config.emit_lir_cfg = 1;
        }
        else if (!strcmp(argv[i], OPTION_LIR_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.lir_output = argv[++i];
            out->config.emit_lir = 1;
        }
        else if (!strcmp(argv[i], OPTION_EMIT_ASM))             out->config.emit_asm    = 1;
        else if (!strcmp(argv[i], OPTION_ASM_OUTPUT)) {
            if (i + 1 >= argc) goto _fail;
            out->locations.asm_output = argv[++i];
            out->config.emit_asm = 1;
        }
        else if (!strcmp(argv[i], OPTION_DEBUG))                out->config.debug       = 1;
        else if (!strcmp(argv[i], OPTION_NO_DEBUG))             out->config.debug       = 0;
        else if (!strcmp(argv[i], OPTION_NO_STRICT))            out->config.strict      = 0;
        else if (!strcmp(argv[i], OPTION_NO_OPTIMIZATION))      _set_optimization_profile(out, 0);
        else if (!strcmp(argv[i], OPTION_ROUGHT_OPTIMIZATION))  _set_optimization_profile(out, 1);
        else if (!strcmp(argv[i], OPTION_GOOD_OPTIMIZATION))    _set_optimization_profile(out, 2);
        else if (!strcmp(argv[i], OPTION_MAX_OPTIMIZATION))     _set_optimization_profile(out, 3);
        else if (argv[i][0] == '-') goto _fail;
        else if (!list_push_back(&out->locations.files, argv[i])) goto _fail;
    }

    if (out->flags.preprocess_only && out->build_mode != BUILD_MODE_EXECUTABLE) goto _fail;
    if (
        out->build_mode == BUILD_MODE_ANALYSIS && 
        (out->config.emit_lir || out->config.emit_lir_cfg || out->config.emit_asm
    )) goto _fail;

    return 1;

_fail: {}
    _unload_options(out);
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
        _unload_options(&options);
        return EXIT_SUCCESS;
    }

    if (options.flags.show_something) {
        _print_gem(stdout);
        _unload_options(&options);
        return EXIT_SUCCESS;
    }

    if (options.flags.show_version) {
        _print_version(stdout);
        _unload_options(&options);
        return EXIT_SUCCESS;
    }

    char stdlib_path[PATH_MAX] = { 0 };
    options.locations.stdlib = _find_stdlib(argv[0], stdlib_path, sizeof(stdlib_path));
    char runtime_path[PATH_MAX] = { 0 };
    options.locations.runtime = _find_runtime_library(argv[0], runtime_path, sizeof(runtime_path));
    if (options.flags.print_stdlib) {
        if (options.locations.stdlib) {
            puts(options.locations.stdlib);
            _unload_options(&options);
            return EXIT_SUCCESS;
        }

        fprintf(stderr, "CPL standard library isn't found\n");
        _unload_options(&options);
        return EXIT_FAILURE;
    }

    int files_left = list_size(&options.locations.files);
    if (!files_left) {
        fprintf(stderr, "No input files\n");
        _unload_options(&options);
        return EXIT_FAILURE;
    }

    CONF_set_config(_make_config(&options));

    list_t object_files;
    list_init(&object_files);

    list_t token_lists;
    list_init(&token_lists);

    sym_table_t smt;
    SMT_init(&smt);

    ast_ctx_t sctx;
    AST_init_ctx(&sctx);

    char* input_file = NULL;
    foreach (input_file, &options.locations.files) {
        files_left--;

        int fd = open(input_file, O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "File %s isn't found!\n", input_file);
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
            fprintf(stderr, "Failed to preprocess %s\n", input_file);
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
                fprintf(stderr, "Can't write preprocessed output for %s\n", input_file);
                if (output != stdout) fclose(output);
                close(fd);
                return 1;
            }

            if (output != stdout) fclose(output);
            close(fd);
            continue;
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

        if (files_left > 0) {
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
            if (SEM_perform_ast_check(&sctx, &smt) <= 0 && CONF_is_strict_compilation()) {
                fprintf(stderr, "AST semantic analysis failed\n");
                return 1;
            }
        }

        hir_ctx_t hirctx = { 0 };
        HIR_generate(&sctx, &hirctx, &smt);

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
        int needs_hir_analysis = options.flags.hir_analysis || options.build_mode == BUILD_MODE_ANALYSIS;
        if (options.config.constant || needs_hir_analysis) {
            HIR_DAG_generate(&cfgctx, &dagctx, &smt);
        }

        if (options.config.constant) {
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

        if (needs_hir_analysis) {
            if (SEM_perform_hir_check(&cfgctx, &dagctx, &hirctx, &smt) <= 0 && CONF_is_strict_compilation()) {
                fprintf(stderr, "HIR semantic analysis failed\n");
                return 1;
            }
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

        if (options.config.emit_hir_cfg) {
            const char* hir_cfg_output = "output.dot";
            FILE* hir_cfg_file = fopen(hir_cfg_output, "w");
            if (!hir_cfg_file) {
                fprintf(stderr, "Can't open HIR CFG output file %s: %s\n", hir_cfg_output, strerror(errno));
                return 1;
            }

            DUMP_format_hir_cfg(&cfgctx, &smt, options.locations.hir_cfg_name, hir_cfg_file);
            fclose(hir_cfg_file);
        }

        if (options.build_mode == BUILD_MODE_ANALYSIS) {
            HIR_DAG_unload(&dagctx);
            HIR_LTREE_unload_ctx(&lctx);
            HIR_CG_unload(&callctx);
            HIR_CFG_unload(&cfgctx);
            HIR_unload_blocks(hirctx.hot.h);
            _unload_token_lists(&token_lists);
            AST_unload_ctx(&sctx);
            SMT_unload(&smt);
            close(fd);
            continue;
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

        if (options.config.emit_lir_cfg) {
            const char* lir_cfg_output = "output.dot";
            FILE* lir_cfg_file = fopen(lir_cfg_output, "w");
            if (!lir_cfg_file) {
                fprintf(stderr, "Can't open LIR CFG output file %s: %s\n", lir_cfg_output, strerror(errno));
                return 1;
            }

            DUMP_format_lir_cfg(&cfgctx, &smt, options.locations.lir_cfg_name, lir_cfg_file);
            fclose(lir_cfg_file);
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
        LIR_clear_global_variables(&cfgctx, &smt);

        char* asm_path = _make_temp_path();
        char* obj_path = _make_temp_path();
        if (!asm_path || !obj_path) {
            fprintf(stderr, "Can't create temporary files for %s\n", input_file);
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

        if (!_compile_asm_to_object(&options, asm_path, obj_path)) {
            fprintf(stderr, "ASM compilation failed for %s\n", input_file);
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
            const char* object_output = _output_path_or_default(options.locations.output, "output.o");
            if (!_move_file_path(obj_path, object_output)) {
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
        _unload_token_lists(&token_lists);
        AST_unload_ctx(&sctx);

        SMT_unload(&smt);
        close(fd);
    }

    if (
        options.build_mode == BUILD_MODE_EXECUTABLE &&
        !options.flags.preprocess_only              &&
        list_size(&object_files) > 0
    ) {
        if (!_link_objects(&options, &object_files)) {
            fprintf(stderr, "Linking failed\n");
            return 1;
        }
    }

    char* object_file = NULL;
    foreach (object_file, &object_files) {
        if (object_file) {
            unlink(object_file);
            mm_free(object_file);
        }
    }

    list_free(&object_files);
    _unload_token_lists(&token_lists);
    _unload_options(&options);
    return EXIT_SUCCESS;
}
