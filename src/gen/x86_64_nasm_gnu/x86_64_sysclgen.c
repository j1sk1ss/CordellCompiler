#include <generator.h>

int x86_64_generate_start(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    iprintf(output, "global _start\n");
    iprintf(output, "_start:\n");
    iprintf(output, "push rbp\n");
    iprintf(output, "mov rbp, rsp\n");
    iprintf(output, "sub rsp, %d\n", ALIGN(get_stack_size(node, ctx)));
    x86_64_generate_block(node->child, output, ctx);
    return 1;
}

int x86_64_generate_exit(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    x86_64_generate_block(node->child, output, ctx);
    iprintf(output, "mov rdi, rax\n");
    iprintf(output, "mov rax, 60\n");
    iprintf(output, "%s\n", SYSCALL);
    return 1;
}

/* https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/ */
/* https://gist.github.com/GabriOliv/a9411fa771a1e5d94105cb05cbaebd21 */
/* https://math.hws.edu/eck/cs220/f22/registers.html */
int x86_64_generate_syscall(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    static const int args_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };

    int arg_index = 0;
    ast_node_t* args = node->child;
    while (args) {
        x86_64_generate_block(args, output, ctx);

        regs_t reg;
        get_reg(&reg, BASE_BITNESS, args_regs[arg_index++], 0);
        iprintf(output, "mov %s%s, rax\n", reg.operation, reg.name);
        
        args = args->sibling;
    }

    iprintf(output, "%s\n", SYSCALL);
    iprint_line(output);

    return 1;
}