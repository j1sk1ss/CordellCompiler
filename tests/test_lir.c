#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <prep/token.h>
#include <prep/markup.h>

#include <ast/syntax.h>

#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include <hir/opt/cfg.h>
#include <hir/opt/ssa.h>
#include <hir/opt/dfg.h>
#include <hir/opt/ra.h>

#include <lir/lirgen.h>
#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

#include "ast_helper.h"
#include "hir_helper.h"
#include "lir_helper.h"

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

    hir_ctx_t hirctx = { .h = NULL, .t = NULL };
    HIR_generate(&sctx, &hirctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&hirctx, &cfgctx);
    
    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    HIR_DFG_collect_defs(&cfgctx);
    HIR_DFG_collect_uses(&cfgctx);
    HIR_DFG_compute_inout(&cfgctx);
    HIR_DFG_make_allias(&cfgctx, &smt);

    cfg_print(&cfgctx);

    igraph_t ig;
    HIR_RA_build_igraph(&cfgctx, &ig, &smt);

    map_t clrs;
    HIR_RA_color_igraph(&ig, &clrs);
    igraph_dump_dot(&ig);

    printf("\n\n========== SSA HIR ==========\n");
    hir_block_t* hh = hirctx.h;
    while (hh) {
        print_hir_block(hh, 1, &smt);
        hh = hh->next;
    }

    lir_ctx_t lirctx = { .h = NULL, .t = NULL, .vars = &clrs };
    lir_gen_t lirgen = {
        .generate = x86_64_generate_lir
    };

    LIR_generate(&hirctx, &lirgen, &lirctx, &smt);

    printf("\n\n========== x86_64 LIR ==========\n");
    lir_block_t* lh = lirctx.h;
    while (lh) {
        print_lir_block(lh);
        lh = lh->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    list_iter_t it;

    if (!list_isempty(&smt.v.lst)) printf("==========   VARS  ==========\n");
    list_iter_hinit(&smt.v.lst, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)list_iter_next(&it))) {
        printf("id: %i (%i), %s, type: %i, s_id: %i\n", vi->v_id, vi->p_id, vi->name, vi->type, vi->s_id);
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

    HIR_unload_blocks(hirctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
