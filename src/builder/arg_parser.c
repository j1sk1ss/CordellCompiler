#include <builder.h>

static int _add_symtab_type_arg(options_t* out, const char* type) {
    return out && type && list_push_back(&out->locations.symtab_types, (void*)type);
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
    out->config.tco           = 0;
    out->config.tre           = 0;
    out->config.finline       = 0;
    out->config.licm          = 0;
    out->config.unroll        = 0;
    out->config.constant      = 0;
    out->config.peephole      = 0;
    out->config.copy_prop     = 0;
    out->config.z3opt         = 0;

    if (level >= 2) {
        out->config.licm      = 1;
        out->config.constant  = 1;
        out->config.peephole  = 1;
    }

    if (level >= 3) {
        out->config.z3opt     = 1;
        out->config.copy_prop = 1;
        out->config.tco       = 1;
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
    if (!define) return 0;
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

static void _set_default_options(options_t* out) {
    memset(out, 0, sizeof(options_t));
    list_init(&out->locations.files);
    list_init(&out->locations.defines);
    list_init(&out->locations.symtab_types);
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
    out->config.strict             = 0;
    _set_optimization_profile(out, 0);
}

void BUILD_unload_options(options_t* options) {
    if (!options) return;
    list_free(&options->locations.files);
    list_free_force_op(&options->locations.defines, (int (*)(void*))_unload_cli_define);
    list_free(&options->locations.symtab_types);
    list_free_force(&options->tools.linker_args);
}

int BUILD_parse_input_args(char* argv[], int argc, options_t* out) {
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
        else if (!strcmp(argv[i], OPTION_INСLUDE)) {
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
        else if (!strcmp(argv[i], OPTION_TEST_COMPILATION))       out->flags.test_compilation    = 1;
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
        else if (
            !strcmp(argv[i], OPTION_NO_OBJECT_SHORT) ||
            !strcmp(argv[i], OPTION_NO_OBJECT)
        ) {
            if (!_set_build_mode(out, BUILD_MODE_RAW)) goto _fail;
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
        else if (!strcmp(argv[i], OPTION_TCO))                  out->config.tco         = 1;
        else if (!strcmp(argv[i], OPTION_NO_TCO))               out->config.tco         = 0;
        else if (!strcmp(argv[i], OPTION_TRE))                  out->config.tre         = 1;
        else if (!strcmp(argv[i], OPTION_NO_TRE))               out->config.tre         = 0;
        else if (!strcmp(argv[i], OPTION_FINLINE))              out->config.finline     = 1;
        else if (!strcmp(argv[i], OPTION_NO_FINLINE))           out->config.finline     = 0;
        else if (!strcmp(argv[i], OPTION_LICM))                 out->config.licm        = 1;
        else if (!strcmp(argv[i], OPTION_NO_LICM))              out->config.licm        = 0;
        else if (!strcmp(argv[i], OPTION_UNROLL))               out->config.unroll      = 1;
        else if (!strcmp(argv[i], OPTION_NO_UNROLL))            out->config.unroll      = 0;
        else if (!strcmp(argv[i], OPTION_Z3OPT))                out->config.z3opt       = 1;
        else if (!strcmp(argv[i], OPTION_NO_Z3OPT))             out->config.z3opt       = 0;
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
        else if (!strcmp(argv[i], OPTION_EMIT_SYMTAB)) {
            if (i + 1 >= argc || !_add_symtab_type_arg(out, argv[i + 1])) goto _fail;
            i++;
        }
        else if (!strcmp(argv[i], OPTION_DEBUG))                out->config.debug       = 1;
        else if (!strcmp(argv[i], OPTION_NO_DEBUG))             out->config.debug       = 0;
        else if (!strcmp(argv[i], OPTION_NON_STRICT))           out->config.strict      = 0;
        else if (!strcmp(argv[i], OPTION_NO_OPTIMIZATION))      _set_optimization_profile(out, 0);
        else if (!strcmp(argv[i], OPTION_ROUGHT_OPTIMIZATION))  _set_optimization_profile(out, 1);
        else if (!strcmp(argv[i], OPTION_GOOD_OPTIMIZATION))    _set_optimization_profile(out, 2);
        else if (!strcmp(argv[i], OPTION_MAX_OPTIMIZATION))     _set_optimization_profile(out, 3);
        else if (!strcmp(argv[i], OPTION_STRICT)) {
            out->flags.ast_analysis = 1;
            out->flags.hir_analysis = 1;
            out->config.strict      = 1;
        }
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
    BUILD_unload_options(out);
    return 0;
}
