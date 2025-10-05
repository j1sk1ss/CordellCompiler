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
#include <hir/opt/dfg.h>
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

    list_t tokens;
    list_init(&tokens);
    if (!TKN_tokenize(fd, &tokens)) {
        fprintf(stderr, "ERROR! tkn==NULL!\n");
        return 1;
    }

    MRKP_mnemonics(&tokens);
    MRKP_variables(&tokens);

    sym_table_t smt;
    SMT_init(&smt);
    syntax_ctx_t sctx = { .r = NULL };

    STX_create(&tokens, &sctx, &smt);

    printf("\n\n========== AST ==========\n");
    print_ast(sctx.r, 0);

    hir_ctx_t irctx = {
        .h = NULL, .t = NULL 
    };

    HIR_generate(&sctx, &irctx, &smt);

    cfg_ctx_t cfgctx;
    HIR_CFG_build(&irctx, &cfgctx);
    
    ssa_ctx_t ssactx;
    HIR_SSA_insert_phi(&cfgctx, &smt);
    HIR_SSA_rename(&cfgctx, &ssactx, &smt);

    HIR_DFG_collect_defs(&cfgctx);
    HIR_DFG_collect_uses(&cfgctx);
    HIR_DFG_compute_inout(&cfgctx);
    HIR_DFG_make_allias(&cfgctx, &smt);

    cfg_print(&cfgctx);

    printf("\n\n========== HIR ==========\n");
    hir_block_t* h = irctx.h;
    while (h) {
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    map_iter_t it;

    if (!map_isempty(&smt.v.vartb)) printf("==========   VARS  ==========\n");
    map_iter_init(&smt.v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vi->v_id, vi->name, vi->type, vi->s_id);
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

    HIR_unload_blocks(irctx.h);
    list_free_force(&tokens);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    close(fd);
    return 0;
}
