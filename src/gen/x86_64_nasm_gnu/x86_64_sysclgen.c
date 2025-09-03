#include <x86_64_gnu_nasm.h>

int x86_64_generate_start(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    iprintf(output, "global _start\n");
    iprintf(output, "_start:\n");
    iprintf(output, "push rbp\n");
    iprintf(output, "mov rbp, rsp\n");
    iprintf(output, "sub rsp, %d\n", ALIGN(get_stack_size(node, ctx)));

    int arg_count = 0;
    static char* args[] = { "mov rax, [rbp + 8]", "lea rax, [rbp + 16]" };;
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) g->blockgen(t, output, ctx, g);
        else if (VRS_isdecl(t->token) && arg_count < 2) {
            iprintf(output, "%s\n", args[arg_count++]);
            g->store(t->child, output, ctx, g);   
        }
    }

    return 1;
}

int x86_64_generate_exit(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    g->elemegen(node->child, output, ctx, g);
    iprintf(output, "mov rdi, rax\n");
    iprintf(output, "mov rax, 60\n");
    iprintf(output, "%s\n", SYSCALL);
    return 1;
}

/* https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/ */
/* https://gist.github.com/GabriOliv/a9411fa771a1e5d94105cb05cbaebd21 */
/* https://math.hws.edu/eck/cs220/f22/registers.html */
int x86_64_generate_syscall(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    static const char* args_regs[] = { "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9" };

    int arg_index = 1;
    ast_node_t* syscall = node->child;
    ast_node_t* args    = syscall->sibling;
    while (args && arg_index < 7) {
        g->elemegen(args, output, ctx, g);
        iprintf(output, "mov %s, rax\n", args_regs[arg_index++]);
        args = args->sibling;
    }

    g->elemegen(syscall, output, ctx, g);
    iprintf(output, "%s\n", SYSCALL);
    return 1;
}