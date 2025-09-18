#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <ast/syntax.h>
#include <prep/markup.h>
#include <sem/semantic.h>
#include <ast/parsers/cpl_parser.h>
#include "ast_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[512] = { 0 };
    pread(fd, data, 512, 0);
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
    };

    STX_create(tkn, &sctx, &p);
    print_ast(sctx.r, 0);

    SMT_check(sctx.r, &sctx.symtb);

    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

