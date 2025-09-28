#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/opt/cfg.h>
#include <hir/opt/ssa.h>
#include "ast_helper.h"
#include "hir_helper.h"

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

    sym_table_t smt;
    syntax_ctx_t sctx = { .r = NULL };

    STX_create(tkn, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&irctx, &cfgctx);
    
    ssa_ctx_t ssactx = { .h = NULL };
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    cfg_print(&cfgctx);

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    list_iter_t it;

    if (!list_isempty(&smt.v.lst)) printf("==========   VARS  ==========\n");
    list_iter_hinit(&smt.v.lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vi->v_id, vi->name, vi->type, vi->s_id);
    }

    if (!list_isempty(&smt.a.lst)) printf("==========   ARRS  ==========\n");
    list_iter_hinit(&smt.a.lst, &it);
    array_info_t* ai;
    while ((ai = (array_info_t*)list_iter_next(&it))) {
        printf("id: %i, name: %s, scope: %i\n", ai->v_id, ai->name, ai->s_id);
    }

    if (!list_isempty(&smt.f.lst)) printf("==========  FUNCS  ==========\n");
    list_iter_hinit(&smt.f.lst, &it);
    func_info_t* fi;
    while ((fi = (func_info_t*)list_iter_next(&it))) {
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (!list_isempty(&smt.s.lst)) printf("========== STRINGS ==========\n");
    list_iter_hinit(&smt.s.lst, &it);
    str_info_t* si;
    while ((si = (str_info_t*)list_iter_next(&it))) {
        printf("id: %i, val: %s\n", si->id, si->value);
    }

    if (!list_isempty(&smt.m.lst)) printf("========== ALLIAS ==========\n");
    list_iter_hinit(&smt.m.lst, &it);
    allias_t* mi;
    while ((mi = (allias_t*)list_iter_next(&it))) {
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while ((own_id = set_iter_next_int(&sit)) >= 0) printf("%i ", own_id);
    }

    HIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}

