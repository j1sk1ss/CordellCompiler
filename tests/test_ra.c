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

#include "ast_helper.h"
#include "hir_helper.h"
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
    map_free(&clrs);

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

    if (smt.a.arrtb.size > 0) printf("==========   ARRS  ==========\n");
    for (int i = 0; i < smt.a.arrtb.capacity; i++) {
        if (!smt.a.arrtb.entries[i].used) continue;
        array_info_t* ai = (array_info_t*)smt.a.arrtb.entries[i].value;
        printf("id: %i, eltype: %i%s\n", ai->v_id, ai->el_type, ai->heap ? ", heap" : "");
    }

    if (smt.f.functb.size > 0) printf("==========  FUNCS  ==========\n");
    for (int i = 0; i < smt.f.functb.capacity; i++) {
        if (!smt.f.functb.entries[i].used) continue;
        func_info_t* fi = (func_info_t*)smt.f.functb.entries[i].value;
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (smt.s.strtb.size > 0) printf("========== STRINGS ==========\n");
    for (int i = 0; i < smt.s.strtb.capacity; i++) {
        if (!smt.s.strtb.entries[i].used) continue;
        str_info_t* si = (str_info_t*)smt.s.strtb.entries[i].value;
        printf("id: %i, val: %s\n", si->id, si->value);
    }

    if (smt.m.allias.size > 0) printf("========== ALLIAS ==========\n");
    for (int i = 0; i < smt.m.allias.capacity; i++) {
        if (!smt.m.allias.entries[i].used) continue;
        allias_t* mi = (allias_t*)smt.m.allias.entries[i].value;
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while ((own_id = set_iter_next_int(&sit)) >= 0) printf("%i ", own_id);
    }

    HIR_RA_unload_igraph(&ig);
    HIR_CFG_unload(&cfgctx);
    HIR_unload_blocks(irctx.h);
    AST_unload(sctx.r);
    SMT_unload(&smt);
    TKN_unload(tkn);
    close(fd);
    return 0;
}
