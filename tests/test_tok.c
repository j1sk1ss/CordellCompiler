#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>

int main(__attribute__ ((unused)) int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[512] = { 0 };
    pread(fd, data, 512, 0);
    printf("Source data: %s\n", data);

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn == NULL!\n");
        return EXIT_FAILURE;
    }

    printf("\nTokens:\n");
    foreach (token_t* h, &tokens) {
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

    fprintf(stdout, "Allocated: %i\n", mm_get_allocated());
    return EXIT_SUCCESS;
}
