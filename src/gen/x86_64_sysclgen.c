#include <generator.h>

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