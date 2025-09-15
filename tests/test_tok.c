#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>

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

    printf("\nTokens:\n");
    token_t* h = tkn;
    while (h) {
        printf(
            "%sline=%i, type=%i, data=[%s], %s%s%s%s\n",
            h->vinfo.glob ? "glob " : "", 
            h->lnum, 
            h->t_type, 
            h->value,
            h->vinfo.ptr  ? "ptr "  : "", 
            h->vinfo.ro   ? "ro "   : "",
            h->vinfo.dref ? "dref " : "",
            h->vinfo.ref  ? "ref "  : ""
        );
        h = h->next;
    }

    TKN_unload(tkn);
    close(fd);
    return 0;
}
