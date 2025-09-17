#ifndef X86_64_GNU_NASM_
#define X86_64_GNU_NASM_

#include "regs.h"
#include "vars.h"
#include "qsort.h"
#include "vartb.h"
#include "arrtb.h"
#include "genctx.h"
#include "syntax.h"

static inline int deallocate_scope_heap(ast_node_t* t, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (scope_id_top(&ctx->heap) == t->sinfo.s_id) {
        scope_elem_t hinfo;
        scope_pop_top(&ctx->heap, &hinfo);
        print_debug("Heap deallocation for scope=%i, return to [rbp - %i]", t->sinfo.s_id, hinfo.offset);
        iprintf(output, "mov rdi, [rbp - %d]\n", hinfo.offset);
        iprintf(output, "mov rax, 12\n");
        iprintf(output, "syscall\n");
    }

    return 1;
}

/* x86_64_blockgen.c */
int x86_64_generate_elem(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_asmgen.c */
int x86_64_generate_asm(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_opgen.c */
int x86_64_generate_operand(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_storegen.c */
int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_loadgen.c */
int x86_64_generate_ptr_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_assigngen.c */
int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_declgen.c */
int x86_64_generate_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_datagen.c */
int get_stack_size(ast_node_t* root, gen_ctx_t* ctx);
int x86_64_generate_data(ast_node_t* node, FILE* output, int section, int bss, gen_ctx_t* ctx, gen_t* g);

/* x86_64_sysclgen.c */
int x86_64_generate_start(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_exit(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_syscall(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_funcgen.c */
int x86_64_generate_funcdef(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_return(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_funccall(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_function(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

/* x86_64_condgen.c */
int x86_64_generate_if(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_while(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);
int x86_64_generate_switch(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g);

#endif