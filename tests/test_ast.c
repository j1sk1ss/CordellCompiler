#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <prep/token.h>
#include <prep/token_types.h>
#include <prep/markup.h>
#include <ast/astgen.h>
#include <ast/astgens/astgens.h>
#include "ast_helper.h"

int main(__attribute__ ((unused)) int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    sym_table_t smt;
    SMT_init(&smt);
    ast_ctx_t sctx = { .r = NULL };

    AST_parse_tokens(&tokens, &sctx, &smt);
    print_ast(sctx.r, 0);

    AST_unload(sctx.r);
    list_free_force(&tokens);
    SMT_unload(&smt);
    close(fd);

    fprintf(stdout, "Allocated: %i\n", mm_get_allocated());
    return EXIT_SUCCESS;
}

