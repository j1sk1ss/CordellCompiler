#ifndef BUILDER_H_
#define BUILDER_H_

#include <stdlib.h>

#include "logg.h"
#include "token.h"
#include "arrtb.h"
#include "vartb.h"
#include "syntax.h"
#include "markup.h"
#include "deadopt.h"
#include "strdecl.h"
#include "semantic.h"
#include "constopt.h"
#include "deadfunc.h"
#include "generator.h"
#include "deadscope.h"
#include "offsetopt.h"
#include "varinline.h"
#include "condunroll.h"

#define MAX_FILES            100
#define DEFAULT_ASM_COMPILER "nasm"
#define DEFAULT_LINKER       "ld"
#define DEFAULT_ARCH         "elf64"
#define DEFAULT_LINKER_ARCH  "elf_x86_64"
#define LINKER_FLAGS         "-z relro -z now"

typedef struct {
    token_t*      toks;
    gen_ctx_t*    gen;
    syntax_ctx_t* syntax;
    char*         path;
} object_t;

typedef struct {
    char  syntax;
    char  save_asm;
    char* arch;
    char* linker;
    char* linker_arch;
    char* linker_flags;
    char* asm_compiler;
    char* save_path;
} params_t;

typedef struct {
    params_t prms;
    object_t files[MAX_FILES];
    char     fcount;
    parser_t p;
    gen_t    g;
} builder_ctx_t;

int BLD_add_target(char* input, builder_ctx_t* ctx);
int BLD_build(builder_ctx_t* ctx);

#endif