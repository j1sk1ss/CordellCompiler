#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "regs.h"
#include "vars.h"
#include "qsort.h"
#include "varmem.h"
#include "arrmem.h"
#include "syntax.h"

#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)

typedef struct {
    short         label;
    syntax_ctx_t* synt;
} gen_ctx_t;

/*
Allocate memory for generator context
*/
gen_ctx_t* GEN_create_ctx();

/*
Unload generator context
*/
int GEN_destroy_ctx(gen_ctx_t* ctx);

/*
GEN_generate function generates ASM code for target platform.
Params:
- root - AST tree root.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int GEN_generate(gen_ctx_t* ctx, FILE* output);

/* x86_64_blockgen.c */
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
int x86_64_generate_data(ast_node_t* node, FILE* output, int section, int bss);

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