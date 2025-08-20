/* gcc -Iinclude tests/test_ast.c src/markup.c src/syntax/*.c src/token.c std/*.c -g -O2 -o tests/test_ast */
#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>
#include <syntax.h>

static int _print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("\t");
    if (node->token) printf(
        "[%s] (t=%d, size=%i, is_ptr=%i, off=%i, ro=%i glob=%i)\n", 
        (char*)node->token->value, node->token->t_type, node->variable_size, node->token->ptr, 
        node->variable_offset, node->token->ro, node->token->glob
    );
    else printf("{ scope }\n");
    
    ast_node_t* child = node->first_child;
    while (child) {
        _print_ast(child, depth + 1);
        child = child->next_sibling;
    }
    
    return 1;
}

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open("tests/test_code/ast_test.txt", O_RDONLY);
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

    arrmem_ctx_t actx = { .h = NULL };
    varmem_ctx_t vctx = { .h = NULL, .offset = 0 };
    syntax_ctx_t sctx = { .arrs = &actx, .vars = &vctx };
    STX_create(tkn, &sctx);
    _print_ast(sctx.r, 0);

    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

