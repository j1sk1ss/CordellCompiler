#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>
#include <syntax.h>

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open("tests/test_code/markup_test.txt", O_RDONLY);
    char data[512] = { 0 };
    pread(fd, data, 512, 0);
    printf("Source data: %s\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    printf("Tokenizer:\n");
    token_t* h = tkn;
    while (h) {
        printf(
            "glob=%i, line=%i, ptr=%i, ro=%i, type=%i, data=%s\n", 
            h->vinfo.glob, h->lnum, h->vinfo.ptr, h->vinfo.ro, h->t_type, h->value
        );
        h = h->next;
    }

    MRKP_mnemonics(tkn);
    printf("Mnemonic markup:\n");
    h = tkn;
    while (h) {
        printf(
            "glob=%i, line=%i, ptr=%i, ro=%i, type=%i, data=%s\n", 
            h->vinfo.glob, h->lnum, h->vinfo.ptr, h->vinfo.ro, h->t_type, h->value
        );
        h = h->next;
    }

    MRKP_variables(tkn);
    printf("Variables markup:\n");
    h = tkn;
    while (h) {
        printf(
            "glob=%i, line=%i, ptr=%i, ro=%i, type=%i, data=%s\n", 
            h->vinfo.glob, h->lnum, h->vinfo.ptr, h->vinfo.ro, h->t_type, h->value
        );
        h = h->next;
    }

    TKN_unload(tkn);
    close(fd);
    return 0;
}
