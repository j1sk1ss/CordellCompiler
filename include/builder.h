#ifndef BUILDER_H_
#define BUILDER_H_

#include <stdlib.h>
#include <std/logg.h>

#include <prep/token.h>
#include <prep/markup.h>

#include <ast/syntax.h>
#include <ast/tables/vartb.h>
#include <ast/tables/arrtb.h>
#include <ast/opt/deadopt.h>
#include <ast/opt/strdecl.h>
#include <ast/opt/constopt.h>
#include <ast/opt/deadfunc.h>
#include <ast/opt/deadscope.h>
#include <ast/opt/offsetopt.h>
#include <ast/opt/varinline.h>
#include <ast/opt/condunroll.h>

#include <sem/semantic.h>

#include <ir/irgen.h>
#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>
#include <asm/asmgen.h>
#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

#define MAX_FILES            100
#define DEFAULT_ASM_COMPILER "nasm"
#define DEFAULT_LINKER       "ld"
#define DEFAULT_ARCH         "elf64"
#define DEFAULT_LINKER_ARCH  "elf_x86_64"
#define LINKER_FLAGS         "-z relro -z now"

typedef struct {
    token_t*      toks;
    asmgen_ctx_t* gen;
    syntax_ctx_t* syntax;
    char*         path;
} object_t;

typedef struct {
    char  syntax;
    char  ir;
    char  save_asm;
    char* arch;
    char* linker;
    char* linker_arch;
    char* linker_flags;
    char* asm_compiler;
    char* save_path;
} params_t;

typedef struct {
    params_t  prms;
    object_t  files[MAX_FILES];
    char      fcount;
    parser_t  p;
    ir_gen_t  ir;
    asm_gen_t g;
} builder_ctx_t;

int BLD_add_target(char* input, builder_ctx_t* ctx);
int BLD_build(builder_ctx_t* ctx);

#endif