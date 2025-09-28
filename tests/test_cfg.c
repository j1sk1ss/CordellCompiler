#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/opt/cfg.h>
#include "ast_helper.h"
#include "hir_helper.h"

static int _depth = 0;

void print_hir_block(const hir_block_t* block, int ud) {
    if (!block) return;
    if (ud) for (int i = 0; i < _depth; i++) printf("    ");
    printf("%s ", hir_op_to_string(block->op), block->args);
    print_hir_subject(block->farg); if (block->sarg) printf(", ");
    print_hir_subject(block->sarg); if (block->targ) printf(", ");
    print_hir_subject(block->targ); printf("\n");
}

/* https://dreampuf.github.io/GraphvizOnline/?engine=dot */
static int _export_dot_func(cfg_func_t* f) {
    printf("digraph CFG_func%d {\n", f->id);
    printf("  rankdir=TB;\n");
    cfg_block_t* b = f->cfg_head;
    while (b) {
        printf("  B%ld [label=\"B%ld:\\nentry=%s\\nexit=%s\"];\n",
               b->id, b->id,
               b->entry ? hir_op_to_string(b->entry->op) : "NULL",
               b->exit  ? hir_op_to_string(b->exit->op)  : "NULL");
        if (b->l)   printf("  B%ld -> B%ld [label=\"fall\"];\n", b->id, b->l->id);
        if (b->jmp) printf("  B%ld -> B%ld [label=\"jump\"];\n", b->id, b->jmp->id);
        b = b->next;
    }
    printf("}\n");
    return 1;
}

void cfg_print(cfg_ctx_t* ctx) {
    cfg_func_t* f = ctx->h;
    printf("==== CFG DUMP ====\n");

    while (f) {
        printf("==== CFG DOT ====\n");
        _export_dot_func(f);
        printf("\n\n==== CFG VIS. FUNC_ID [%i] ====\n", f->id);
        cfg_block_t* b = f->cfg_head;
        while (b) {
            printf("Block %ld [\nentry=", b->id);
            print_hir_block(b->entry, 0);
            printf("exit=");
            print_hir_block(b->exit, 0);
            printf("]\n");

            if (b->l)   printf("fallthrough -> Block %ld\n", b->l->id);
            if (b->jmp) printf("jump -> Block %ld\n", b->jmp->id);

            printf("\n");
            b = b->next;
        }

        f = f->next;
    }

    printf("==================\n");
}

int main(int argc, char* argv[]) {
    printf("RUNNING TEST %s...\n", argv[0]);
    mm_init();
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "File %s not found!\n", argv[1]);
        return 1;
    }

    char data[2048] = { 0 };
    pread(fd, data, 2048, 0);
    printf("Source data: %s\n\n", data);

    token_t* tkn = TKN_tokenize(fd);
    if (!tkn) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        close(fd);
        return 1;
    }

    MRKP_mnemonics(tkn);
    MRKP_variables(tkn);

    sym_table_t smt = {
        .a = { .h = NULL },
        .v = { .h = NULL },
        .f = { .h = NULL }
    };
    
    syntax_ctx_t sctx  = { .r = NULL };

    STX_create(tkn, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);
    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    if (smt.v.h) printf("==========   VARS  ==========\n");
    variable_info_t* vh = smt.v.h;
    while (vh) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vh->v_id, vh->name, vh->type, vh->s_id);
        vh = vh->next;
    }

    if (smt.a.h) printf("==========   ARRS  ==========\n");
    array_info_t* ah = smt.a.h;
    while (ah) {
        printf("id: %i, name: %s, scope: %i\n", ah->v_id, ah->name, ah->s_id);
        ah = ah->next;
    }

    if (smt.f.h) printf("==========  FUNCS  ==========\n");
    func_info_t* fh = smt.f.h;
    while (fh) {
        printf("id: %i, name: %s\n", fh->id, fh->name);
        fh = fh->next;
    }

    if (smt.s.h) printf("========== STRINGS ==========\n");
    str_info_t* sh = smt.s.h;
    while (sh) {
        printf("id: %i, val: %s\n", sh->id, sh->value);
        sh = sh->next;
    }

    cfg_ctx_t cfgctx = { .h = NULL };
    HIR_CFG_build(&irctx, &cfgctx);
    cfg_print(&cfgctx);

    HIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

