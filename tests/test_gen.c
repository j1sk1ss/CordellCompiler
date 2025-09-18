#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <ast/syntax.h>
#include <prep/markup.h>
#include <ast/opt/strdecl.h>
#include <ast/opt/deadscope.h>
#include <ast/opt/offsetopt.h>
#include <asm/asmgen.h>
#include <ast/parsers/cpl_parser.h>
#include <asm/x86_64_gnu_nasm/x86_64_gnu_nasm_asm.h>
#include "ast_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(tkn);
    MRKP_variables(tkn);

    arrtab_ctx_t actx  = { .h = NULL };
    vartab_ctx_t vctx  = { .h = NULL, .offset = 0 };
    functab_ctx_t fctx = { .h = NULL };
    syntax_ctx_t sctx  = { 
        .symtb = {
            .arrs  = &actx,
            .vars  = &vctx,
            .funcs = &fctx
        }
    };
    
    parser_t p = {
        .block      = cpl_parse_block,
        .switchstmt = cpl_parse_switch,
        .condop     = cpl_parse_condop,
        .arraydecl  = cpl_parse_array_declaration,
        .vardecl    = cpl_parse_variable_declaration,
        .extrn      = cpl_parse_extern,
        .rexit      = cpl_parse_rexit,
        .funccall   = cpl_parse_funccall,
        .function   = cpl_parse_function,
        .import     = cpl_parse_import,
        .expr       = cpl_parse_expression,
        .scope      = cpl_parse_scope,
        .start      = cpl_parse_start,
        .syscall    = cpl_parse_syscall,
        .asmer      = cpl_parse_asm
    };

    STX_create(tkn, &sctx, &p);
    
    OPT_strpack(&sctx);
    OPT_offrecalc(&sctx);

    print_ast(sctx.r, 0);
    
    gen_ctx_t gctx = { .label = 0, .synt = &sctx };
    gen_t g = {
        .datagen   = x86_64_generate_data,
        .funcdef   = x86_64_generate_funcdef,
        .funcret   = x86_64_generate_return,
        .funccall  = x86_64_generate_funccall,
        .function  = x86_64_generate_function,
        .blockgen  = x86_64_generate_block,
        .elemegen  = x86_64_generate_elem,
        .operand   = x86_64_generate_operand,
        .store     = x86_64_generate_store,
        .ptrload   = x86_64_generate_ptr_load,
        .load      = x86_64_generate_load,
        .assign    = x86_64_generate_assignment,
        .decl      = x86_64_generate_declaration,
        .start     = x86_64_generate_start,
        .exit      = x86_64_generate_exit,
        .syscall   = x86_64_generate_syscall,
        .ifgen     = x86_64_generate_if,
        .whilegen  = x86_64_generate_while,
        .switchgen = x86_64_generate_switch,
        .asmer     = x86_64_generate_asm
    };

    fprintf(stdout, "Generated code:\n");
    GEN_generate(&gctx, &g, stdout);

    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
