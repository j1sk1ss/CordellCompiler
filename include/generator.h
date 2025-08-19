#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include "regs.h"
#include "vars.h"
#include "qsort.h"
#include "arrmem.h"
#include "syntax.h"

#define ARRAYS_MAX_TOKEN 100
#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)
#define iprint_line(out) fprintf(out, "; --------------- \n");

typedef struct {
    short         label;
    syntax_ctx_t* synt;
} gen_ctx_t;

gen_ctx_t* GEN_create_ctx();
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

#endif