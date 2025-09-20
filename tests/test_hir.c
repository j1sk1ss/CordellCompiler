#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hir.h>
#include <hir/hirgen/hirgen.h>
#include "ast_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

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

    STX_create(tkn, &sctx);
    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .cid = 0, .h = NULL, .lid = 0, .synt = &sctx, .t = NULL 
    };

    LIR_generate(&irgen, &irctx);
    printf("\n\n========== LIR ==========\n");
    lir_block_t* h = irctx.h;
    while (h) {
        print_irblock(h);
        h = h->next;
    }

    LIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

