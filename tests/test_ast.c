/* gcc -Iinclude tests/test_ast.c src/markup.c src/syntax/*.c src/token.c std/*.c -g -O2 -o tests/test_ast */
#include <stdio.h>
#include <token.h>
#include <unistd.h>
#include <stdlib.h>
#include <syntax.h>

static int _print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("    ");
    if (node->token && node->token->t_type != SCOPE_TOKEN) {
        printf(
            "[%s] (t=%d, size=%i,%soff=%i, s_id=%i%s%s)\n", 
            node->token->value, node->token->t_type, node->info.size, 
            node->token->ptr ? " ptr, " : " ", 
            node->info.offset, node->info.s_id,
            node->token->ro ? ", ro" : "", node->token->glob ? ", glob" : ""
        );
    }
    else if (node->token && node->token->t_type == SCOPE_TOKEN) {
        printf("{ scope }\n");
    }
    else {
        printf("[ block ]\n");
    }
    
    ast_node_t* child = node->child;
    while (child) {
        _print_ast(child, depth + 1);
        child = child->sibling;
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

