#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <ast/syntax.h>
#include <prep/markup.h>
#include <ast/opt/deadfunc.h>
#include <ast/parsers/parser.h>
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

    syntax_ctx_t sctx;
    sym_table_t smt;
    STX_create(tkn, &sctx, &smt);

    deadfunc_ctx_t dctx = { .ctx = { NULL }, .size = 0 };
    OPT_deadfunc_add(&sctx, &dctx);
    OPT_deadfunc_clear(&dctx);

    print_ast(sctx.r, 0);

    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

