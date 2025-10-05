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
#include "ral_helper.h"

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
    SMT_init(&smt);
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
    HIR_RA_create_deall(&cfgctx, &ig, &smt, &clrs);

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
    map_iter_t it;

    if (!map_isempty(&smt.v.vartb)) printf("==========   VARS  ==========\n");
    map_iter_init(&smt.v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        printf("id: %i, %s, type: %i, s_id: %i", vi->v_id, vi->name, vi->type, vi->s_id);
        if (vi->vmi.reg >= 0)         printf(", reg=%s", register_to_string(vi->vmi.reg + R11));
        else if (vi->vmi.offset >= 0) printf(", mem=[rbp - %i]", vi->vmi.offset);
        printf("\n");
    }

    if (!map_isempty(&smt.a.arrtb)) printf("==========   ARRS  ==========\n");
    map_iter_init(&smt.a.arrtb, &it);
    array_info_t* ai;
    while (map_iter_next(&it, (void**)&ai)) {
        printf("id: %i, eltype: %i%s\n", ai->v_id, ai->el_type, ai->heap ? ", heap" : "");
    }

    if (!map_isempty(&smt.f.functb)) printf("==========  FUNCS  ==========\n");
    map_iter_init(&smt.f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (!map_isempty(&smt.s.strtb)) printf("========== STRINGS ==========\n");
    map_iter_init(&smt.s.strtb, &it);
    str_info_t* si;
    while (map_iter_next(&it, (void**)&si)) {
        printf("id: %i, val: %s\n", si->id, si->value);
    }

    if (!map_isempty(&smt.m.allias)) printf("========== ALLIAS ==========\n");
    map_iter_init(&smt.m.allias, &it);
    allias_t* mi;
    while (map_iter_next(&it, (void**)&mi)) {
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while (set_iter_next(&sit, (void**)&own_id)) printf("%i ", own_id);
    }

    HIR_unload_blocks(hirctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
