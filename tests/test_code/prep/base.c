#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <preproc/pp.h>
#include <prep/token.h>
#include <prep/markup.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <ast/astgen/astgen.h>
#include <sem/misc/restore.h>
#include "../../misc/ast_helper.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments! Expected 3, got %i!\n", argc);
        return 1;
    }

    mm_init();

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s isn't found!\n", argv[1]);
        return 1;
    }

    finder_ctx_t finctx = { .bpath = argv[2] };
    fd = PP_perform(fd, &finctx);
    if (fd < 0) {
        fprintf(stderr, "Processed file %s isn't found!\n", argv[1]);
        return 1;
    }

    char pdata[2048] = { 0 };
    pread(fd, pdata, 2048, 0);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens) || !list_size(&tokens)) {
        fprintf(stderr, "ERROR! tkn == NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    foreach (token_t* h, &tokens) {
        printf(
            "%sline=%li, type=%i, data=[%s], %s%s\n",
            h->flags.glob ? "glob " : "", 
            h->finfo.line, 
            h->t_type, 
            h->body->body,
            h->flags.ptr  ? "ptr "  : "", 
            h->flags.ro   ? "ro "   : ""
        );
    }

    list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
    close(fd);
    return EXIT_SUCCESS;
}
