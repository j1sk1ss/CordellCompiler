#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>
#include <syntax.h>
#include <strdecl.h>
#include <deadscope.h>
#include <offsetopt.h>
#include <generator.h>
#include <builder.h>
#include <cpl_parser.h>
#include <x86_64_gnu_nasm.h>
#include "ast_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();

    char output[256] = { 0 };
    sprintf(output, "%s.bin", argv[1]);

    builder_ctx_t bctx = { 
        .p = {
            .block      = cpl_parse_block,
            .switchstmt = cpl_parse_switch,
            .condop     = cpl_parse_condop,
            .arraydecl  = cpl_parse_array_declaration,
            .vardecl    = cpl_parse_variable_declaration,
            .rexit      = cpl_parse_rexit,
            .funccall   = cpl_parse_funccall,
            .function   = cpl_parse_function,
            .import     = cpl_parse_import,
            .expr       = cpl_parse_expression,
            .scope      = cpl_parse_scope,
            .start      = cpl_parse_start,
            .syscall    = cpl_parse_syscall
        },
        .g = {
            .datagen  = x86_64_generate_data,
            .funcdef  = x86_64_generate_funcdef,
            .funcret  = x86_64_generate_return,
            .funccall = x86_64_generate_funccall,
            .function = x86_64_generate_function,
            .blockgen = x86_64_generate_block,
            .elemegen = x86_64_generate_elem,
            .operand  = x86_64_generate_operand,
            .store    = x86_64_generate_store,
            .ptrload  = x86_64_generate_ptr_load,
            .load     = x86_64_generate_load,
            .assign   = x86_64_generate_assignment,
            .decl     = x86_64_generate_declaration,
            .start    = x86_64_generate_start,
            .exit     = x86_64_generate_exit,
            .syscall  = x86_64_generate_syscall,
            .ifgen    = x86_64_generate_if,
            .whilegen = x86_64_generate_while,
            .switchgen= x86_64_generate_switch
        },
        .prms = {
            .save_asm = 1, .syntax = 1, 
            .asm_compiler = DEFAULT_ASM_COMPILER, 
            .arch         = DEFAULT_ARCH,
            .linker       = DEFAULT_LINKER, 
            .linker_arch  = DEFAULT_LINKER_ARCH, 
            .linker_flags = LINKER_FLAGS, 
            .save_path    = output
        }
    };

    BLD_add_target(argv[1], &bctx);
    BLD_build(&bctx);
    return 0;
}
