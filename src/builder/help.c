#include <builder.h>

void BUILD_print_version(FILE* stream) {
    fprintf(
        stream, "cplc %s%s%s\n", 
        CCPL_VERSION, 
        CCPL_SPLASH ? "\n"        : "", 
        CCPL_SPLASH ? CCPL_SPLASH : ""
    );
}

static void BUILD_print_help_row(FILE* stream, const cli_help_option_t* row) {
    char label[64] = { 0 };
    if (row->argument && row->argument[0]) snprintf(label, sizeof(label), "%s %s", row->option, row->argument);
    else                                   snprintf(label, sizeof(label), "%s", row->option);
    fprintf(stream, "  %-28s %s\n", label, row->description);
}

static void BUILD_print_help_section(FILE* stream, const char* title, const cli_help_option_t* rows, size_t count) {
    fprintf(stream, "\n%s:\n", title);
    for (size_t i = 0; i < count; i++) {
        BUILD_print_help_row(stream, &rows[i]);
    }
}

int BUILD_print_help_message() {
    static const cli_help_option_t _general_options[] = {
        { OPTION_HELP_SHORT ", " OPTION_HELP,                 NULL,           "Show this help message"                                                        },
        { OPTION_VERSION_SHORT ", " OPTION_VERSION,           NULL,           "Show compiler version"                                                         },
        { OPTION_SOMETHING_SHORT ", " OPTION_SOMETHING,       NULL,           "Show something"                                                                },
        { OPTION_PREPROCESS_ONLY,                             NULL,           "Run preprocessor only"                                                         },
        { OPTION_INLUCDE,                                     "<dir>",        "Add include directory"                                                         },
        { OPTION_DEFINE,                                      "<name=value>", "Define preprocessor variable"                                                  },
        { OPTION_PRINT_STDLIB,                                NULL,           "Print the standard library directory"                                          },
        { OPTION_OUTPUT,                                      "<file>",       "Set output file"                                                               },
        { OPTION_ENABLE_AST_ANALYSIS,                         NULL,           "Enable AST analysis"                                                           },
        { OPTION_ENABLE_IR_ANALYSIS,                          NULL,           "Enable IR analysis"                                                            },
        { OPTION_ANALYSIS_ONLY,                               NULL,           "Run Cordell Static Analyzer only"                                              },
        { OPTION_DEBUG,                                       NULL,           "Enable debug mode"                                                             },
        { OPTION_NO_DEBUG,                                    NULL,           "Disable debug mode"                                                            },
        { OPTION_STRICT,                                      NULL,           "Enable compiler errors, strict typing and static analysis"                     },
        { OPTION_NON_STRICT,                                  NULL,           "Disables compiler errors, strict typing and static analysis"                   },
        { OPTION_TEST_COMPILATION,                            NULL,           "Sets compilation to a test mode. Will compile test binary"                     }
    };
    static const cli_help_option_t _optimization_options[] = {
        { OPTION_NO_OPTIMIZATION,                             NULL,           "Disable optimizations"                                                         },
        { OPTION_ROUGHT_OPTIMIZATION,                         NULL,           "Rough optimization level"                                                      },
        { OPTION_GOOD_OPTIMIZATION,                           NULL,           "Good optimization level"                                                       },
        { OPTION_MAX_OPTIMIZATION,                            NULL,           "Maximum optimization level"                                                    },
        { OPTION_FINLINE,                                     NULL,           "Enable function inlining"                                                      },
        { OPTION_NO_FINLINE,                                  NULL,           "Disable function inlining"                                                     },
        { OPTION_LICM,                                        NULL,           "Enable LICM"                                                                   },
        { OPTION_NO_LICM,                                     NULL,           "Disable LICM"                                                                  },
        { OPTION_UNROLL,                                      NULL,           "Enable guarded loop unrolling by two"                                          },
        { OPTION_NO_UNROLL,                                   NULL,           "Disable loop unrolling"                                                        },
        { OPTION_CONSTANT,                                    NULL,           "Enable constant propagation/folding"                                           },
        { OPTION_NO_CONSTANT,                                 NULL,           "Disable constant propagation/folding"                                          },
        { OPTION_COPYPROP,                                    NULL,           "Enable LIR copy propagation"                                                   },
        { OPTION_NO_COPYPROP,                                 NULL,           "Disable LIR copy propagation"                                                  },
        { OPTION_PEEPHOLE,                                    NULL,           "Enable peephole optimization"                                                  },
        { OPTION_NO_PEEPHOLE,                                 NULL,           "Disable peephole optimization"                                                 },
        { OPTION_TCO,                                         NULL,           "Enable tail call optimization"                                                 },
        { OPTION_NO_TCO,                                      NULL,           "Disable tail call optimization"                                                },
        { OPTION_TRE,                                         NULL,           "Enable TRE"                                                                    },
        { OPTION_NO_TRE,                                      NULL,           "Disable TRE"                                                                   }
    };
    static const cli_help_option_t _target_options[] = {
        { OPTION_ARCH,                                        "<arch>",       "Set target architecture (x86_64, x86, i386)"                                   },
        { OPTION_FULL_BYTNESS,                                "<size>",       "Set full bytness"                                                              },
        { OPTION_HALF_BYTNESS,                                "<size>",       "Set half bytness"                                                              },
        { OPTION_QUART_BYTNESS,                               "<size>",       "Set quart bytness"                                                             },
        { OPTION_EIGHT_BYTNESS,                               "<size>",       "Set eight bytness"                                                             },
        { OPTION_SYS_TYPE,                                    "<type>",       "Set system type (macho64, linux64, i386, windows64)"                           }
    };
    static const cli_help_option_t _assembler_options[] = {
        { OPTION_ASM_COMPILER,                                "<compiler>",   "Set assembler compiler (nasm, ...)"                                            },
        { OPTION_ASM_FORMAT,                                  "<format>",     "Set assembler format (macho64, elf64, elf32, ...)"                             }
    };
    static const cli_help_option_t _linker_options[] = {
        { OPTION_LINKER,                                      "<linker>",     "Set linker (ld, gcc, clang, ...)"                                              },
        { OPTION_LINKER_MODE,                                 "<mode>",       "Set linker mode (c, driver, raw, ld)"                                          },
        { "-L<dir>, -l<name>, -Wl,<arg>",                     NULL,           "Pass library search paths, libraries, and driver linker options to the linker" },
        { OPTION_LINKER_ARG_SHORT ", " OPTION_LINKER_ARG,     "<arg>",        "Pass one raw argument to the linker command"                                   },
        { OPTION_COMPILE_ONLY_SHORT ", " OPTION_COMPILE_ONLY, NULL,           "Build an object file and skip linking"                                         },
        { OPTION_NO_OBJECT_SHORT ", " OPTION_NO_OBJECT,       NULL,           "Don't build anything, just compile what you've got"                            },
        { OPTION_LINKER_NO_PIE,                               NULL,           "Disable PIE"                                                                   },
        { OPTION_LINKER_PIE,                                  NULL,           "Enable PIE"                                                                    },
        { OPTION_LINKER_M32,                                  NULL,           "Enable m32 mode"                                                               },
        { OPTION_LINKER_NO_M32,                               NULL,           "Disable m32 mode"                                                              },
        { OPTION_ENTRY_NAME,                                 "<name>",        "Set entry symbol name"                                                         }
    };
    static const cli_help_option_t _section_options[] = {
        { OPTION_RO_SECTION,                                 "<name>",        "Set read-only section name"                                                    },
        { OPTION_GLOB_SECTION,                               "<name>",        "Set global section name"                                                       },
        { OPTION_CODE_SECTION,                               "<name>",        "Set code section name"                                                         }
    };
    static const cli_help_option_t _emit_options[] = {
        { OPTION_EMIT_AST,                                   NULL,            "Emit AST dump"                                                                 },
        { OPTION_AST_OUTPUT,                                 "<file>",        "Set AST dump output path"                                                      },
        { OPTION_EMIT_IR,                                    NULL,            "Emit HIR dump"                                                                 },
        { OPTION_IR_OUTPUT,                                  "<file>",        "Set HIR dump output path"                                                      },
        { OPTION_EMIT_HIR_CFG,                               "<name>",        "Emit HIR CFG dump for function"                                                },
        { OPTION_EMIT_LIR,                                   NULL,            "Emit LIR dump"                                                                 },
        { OPTION_EMIT_LIR_CFG,                               "<name>",        "Emit LIR CFG dump for function"                                                },
        { OPTION_LIR_OUTPUT,                                 "<file>",        "Set LIR dump output path"                                                      },
        { OPTION_EMIT_ASM,                                   NULL,            "Emit produced assembly code"                                                   },
        { OPTION_ASM_OUTPUT,                                 "<file>",        "Set assembly output path"                                                      },
        { OPTION_EMIT_SYMTAB,                                "<type>",        "Emit symtable dump (var, fn, sec)"                                             }
    };

    BUILD_print_version(stdout);
    fprintf(stdout, "Usage: cplc [options] <input files>\n");
    BUILD_print_help_section(stdout, "General options",      _general_options, sizeof(_general_options) / sizeof(_general_options[0]));
    BUILD_print_help_section(stdout, "Optimization options", _optimization_options, sizeof(_optimization_options) / sizeof(_optimization_options[0]));
    BUILD_print_help_section(stdout, "Target options",       _target_options, sizeof(_target_options) / sizeof(_target_options[0]));
    BUILD_print_help_section(stdout, "Assembler options",    _assembler_options, sizeof(_assembler_options) / sizeof(_assembler_options[0]));
    BUILD_print_help_section(stdout, "Linker options",       _linker_options, sizeof(_linker_options) / sizeof(_linker_options[0]));
    BUILD_print_help_section(stdout, "Section options",      _section_options, sizeof(_section_options) / sizeof(_section_options[0]));
    BUILD_print_help_section(stdout, "Emit options",         _emit_options, sizeof(_emit_options) / sizeof(_emit_options[0]));
    return 0;
}


