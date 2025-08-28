#include <x86_64_gnu_nasm.h>

int x86_64_generate_start(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    iprintf(output, "global _start\n");
    iprintf(output, "_start:\n");
    iprintf(output, "push rbp\n");
    iprintf(output, "mov rbp, rsp\n");
    iprintf(output, "sub rsp, %d\n", ALIGN(get_stack_size(node, ctx)));
    ctx->blockgen(node->child, output, ctx);
    return 1;
}

int x86_64_generate_exit(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ctx->elemegen(node->child, output, ctx);
    iprintf(output, "mov rdi, rax\n");
    iprintf(output, "mov rax, 60\n");
    iprintf(output, "%s\n", SYSCALL);
    return 1;
}

/* https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/ */
/* https://gist.github.com/GabriOliv/a9411fa771a1e5d94105cb05cbaebd21 */
/* https://math.hws.edu/eck/cs220/f22/registers.html */
int x86_64_generate_syscall(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    static const char* args_regs[] = { "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9" };

    int arg_index = 1;
    ast_node_t* syscall = node->child;
    ast_node_t* args    = syscall->sibling;
    while (args) {
        ctx->elemegen(args, output, ctx);
        iprintf(output, "mov %s, rax\n", args_regs[arg_index++]);
        args = args->sibling;
    }

    ctx->elemegen(syscall, output, ctx);
    iprintf(output, "%s\n", SYSCALL);
    return 1;
}