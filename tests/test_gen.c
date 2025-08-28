#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>
#include <syntax.h>
#include <strdecl.h>
#include <deadscope.h>
#include <offsetopt.h>
#include <generator.h>
#include <x86_64_gnu_nasm.h>
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

    arrmem_ctx_t actx = { .h = NULL };
    varmem_ctx_t vctx = { .h = NULL, .offset = 0 };
    syntax_ctx_t sctx = { .arrs = &actx, .vars = &vctx };
    STX_create(tkn, &sctx);
    
    OPT_strpack(&sctx);
    OPT_offrecalc(&sctx);

    print_ast(sctx.r, 0);
    
    gen_ctx_t gctx = {
        .label    = 0, .synt     = &sctx,
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
        .switchgen= x86_64_generate_switch,
    };
    
    fprintf(stdout, "Generated code:\n");
    GEN_generate(&gctx, stdout);

    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
