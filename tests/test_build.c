#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <builder.h>

#include <ast/parsers/parser.h>
#include <lir/x86_64_gnu_nasm/x86_64_irgen.h>
#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

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
            .syscall    = cpl_parse_syscall,
            .extrn      = cpl_parse_extern,
            .asmer      = cpl_parse_asm
        },
        .ir = {
            .funcdef    = LIR_generate_funcdef_block,
            .funcret    = LIR_generate_return_block,
            .funccall   = LIR_generate_funccall_block,
            .function   = LIR_generate_function_block,
            .blockgen   = LIR_generate_block,
            .elemegen   = LIR_generate_elem_block,
            .operand    = LIR_generate_operand_block,
            .store      = LIR_generate_store_block,
            .ptrload    = LIR_generate_ptr_load_block,
            .load       = LIR_generate_load_block,
            .assign     = LIR_generate_assignment_block,
            .decl       = LIR_generate_declaration_block,
            .start      = LIR_generate_start_block,
            .exit       = LIR_generate_exit_block,
            .syscall    = LIR_generate_syscall_block,
            .ifgen      = LIR_generate_if_block,
            .whilegen   = LIR_generate_while_block,
            .switchgen  = LIR_generate_switch_block,
            .asmer      = LIR_generate_asmblock
        },
        .g = {
            .declarator = x86_64_generate_data,
            .generator  = x86_64_generate_asm
        },
        .prms = {
            .save_asm = 1, .syntax = 1, .ir = 1,
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
