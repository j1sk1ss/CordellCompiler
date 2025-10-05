#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[512] = { 0 };
    pread(fd, data, 512, 0);
    printf("Source data: %s\n", data);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    printf("\nTokens:\n");

    list_iter_t it;
    list_iter_hinit(&tokens, &it);
    token_t* h;
    while ((h = (token_t*)list_iter_next(&it))) {
        printf(
            "%sline=%i, type=%i, data=[%s], %s%s%s%s\n",
            h->flags.glob ? "glob " : "", 
            h->lnum, 
            h->t_type, 
            h->value,
            h->flags.ptr  ? "ptr "  : "", 
            h->flags.ro   ? "ro "   : "",
            h->flags.dref ? "dref " : "",
            h->flags.ref  ? "ref "  : ""
        );
    }

    list_free_force(&tokens);
    close(fd);
    return 0;
}
