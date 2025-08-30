#ifndef X86_64_GNU_NASM_
#define X86_64_GNU_NASM_

#include "regs.h"
#include "vars.h"
#include "qsort.h"
#include "vartb.h"
#include "arrtb.h"
#include "genctx.h"
#include "syntax.h"

#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)

/* x86_64_blockgen.c */
int x86_64_generate_elem(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_opgen.c */
int x86_64_generate_operand(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_storegen.c */
int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_loadgen.c */
int x86_64_generate_ptr_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_assigngen.c */
int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_declgen.c */
int x86_64_generate_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_datagen.c */
#define BSS            1
#define NO_BSS         0
#define DATA_SECTION   1
#define RODATA_SECTION 2
int get_stack_size(ast_node_t* root, gen_ctx_t* ctx);
int x86_64_generate_data(ast_node_t* node, FILE* output, int section, int bss, gen_ctx_t* ctx);

/* x86_64_sysclgen.c */
int x86_64_generate_start(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_exit(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_syscall(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_funcgen.c */
int x86_64_generate_funcdef(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_return(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_funccall(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_function(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

/* x86_64_condgen.c */
int x86_64_generate_if(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_while(ast_node_t* node, FILE* output, gen_ctx_t* ctx);
int x86_64_generate_switch(ast_node_t* node, FILE* output, gen_ctx_t* ctx);

#endif