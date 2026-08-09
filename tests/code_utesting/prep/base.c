#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <preproc/pp.h>
#include <prep/token.h>
#include <prep/markup.h>

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
    pp_ctx_t ppctx;
    PP_init_pp_ctx(&ppctx);

    fd = PP_perform(fd, &finctx, &ppctx);
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
        if (h->t_type == EOF_TOKEN) break;
        printf(
            "%sline=%li, type=%i, data=[%s], %s%s\n",
            h->flags.glob ? "glob " : "", 
            h->finfo.line, 
            h->t_type, 
            h->body->body,
            h->flags.ptr  ? "ptr "  : "", 
            h->flags.ro   ? "ro "   : ""
        );
        fflush(stdout);
    }

    list_free_force_op(&tokens, (int (*)(void *))TKN_unload_token);
    close(fd);
    return EXIT_SUCCESS;
}
