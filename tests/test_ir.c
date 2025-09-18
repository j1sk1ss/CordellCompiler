#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <ast/parsers/parser.h>
#include <ir/irgen.h>
#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>
#include "ast_helper.h"
#include "ir_helper.h"

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(tkn);
    MRKP_variables(tkn);

    arrtab_ctx_t actx  = { .h = NULL };
    vartab_ctx_t vctx  = { .h = NULL, .offset = 0 };
    functab_ctx_t fctx = { .h = NULL };
    syntax_ctx_t sctx  = { 
        .symtb = {
            .arrs  = &actx,
            .vars  = &vctx,
            .funcs = &fctx
        }
    };

    parser_t p = {
        .block      = cpl_parse_block,
        .switchstmt = cpl_parse_switch,
        .condop     = cpl_parse_condop,
        .arraydecl  = cpl_parse_array_declaration,
        .vardecl    = cpl_parse_variable_declaration,
        .extrn      = cpl_parse_extern,
        .rexit      = cpl_parse_rexit,
        .funccall   = cpl_parse_funccall,
        .function   = cpl_parse_function,
        .import     = cpl_parse_import,
        .expr       = cpl_parse_expression,
        .scope      = cpl_parse_scope,
        .start      = cpl_parse_start,
        .syscall    = cpl_parse_syscall,
        .asmer      = cpl_parse_asm
    };

    STX_create(tkn, &sctx, &p);
    print_ast(sctx.r, 0);

    ir_gen_t irgen = {
        .funcdef   = IR_generate_funcdef_block,
        .funcret   = IR_generate_return_block,
        .funccall  = IR_generate_funccall_block,
        .function  = IR_generate_function_block,
        .blockgen  = IR_generate_block,
        .elemegen  = IR_generate_elem_block,
        .operand   = IR_generate_operand_block,
        .store     = IR_generate_store_block,
        .ptrload   = IR_generate_ptr_load_block,
        .load      = IR_generate_load_block,
        .assign    = IR_generate_assignment_block,
        .decl      = IR_generate_declaration_block,
        .start     = IR_generate_start_block,
        .exit      = IR_generate_exit_block,
        .syscall   = IR_generate_syscall_block,
        .ifgen     = IR_generate_if_block,
        .whilegen  = IR_generate_while_block,
        .switchgen = IR_generate_switch_block,
        .asmer     = IR_generate_asmblock
    };

    ir_ctx_t irctx = {
        .cid = 0, .h = NULL, .lid = 0, .synt = &sctx, .t = NULL 
    };

    IR_generate(&irgen, &irctx);
    ir_block_t* h = irctx.h;
    while (h) {
        print_irblock(h);
        h = h->next;
    }

    IR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

