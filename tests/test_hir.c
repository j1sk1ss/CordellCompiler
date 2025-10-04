#include <stdio.h>
#include <prep/token.h>
#include <unistd.h>
#include <stdlib.h>
#include <prep/markup.h>
#include <ast/syntax.h>
#include <hir/hirgen.h>
#include <hir/hirgens/hirgens.h>
#include "ast_helper.h"
#include "hir_helper.h"

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

    sym_table_t smt;
    SMT_init(&smt);
    syntax_ctx_t sctx = { .r = NULL };

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
        print_hir_block(h, 1, &smt);
        h = h->next;
    }

    printf("\n\n========== SYMTABLES ==========\n");
    map_iter_t it;

    if (!map_isempty(&smt.v.vartb)) printf("==========   VARS  ==========\n");
    map_iter_init(&smt.v.vartb, &it);
    variable_info_t* vi;
    while ((vi = (variable_info_t*)map_iter_next(&it))) {
        printf("id: %i, %s, type: %i, s_id: %i\n", vi->v_id, vi->name, vi->type, vi->s_id);
    }

    if (!map_isempty(&smt.a.arrtb)) printf("==========   ARRS  ==========\n");
    map_iter_init(&smt.a.arrtb, &it);
    array_info_t* ai;
    while ((ai = (array_info_t*)map_iter_next(&it))) {
        printf("id: %i, eltype: %i%s\n", ai->v_id, ai->el_type, ai->heap ? ", heap" : "");
    }

    if (!map_isempty(&smt.f.functb)) printf("==========  FUNCS  ==========\n");
    map_iter_init(&smt.f.functb, &it);
    func_info_t* fi;
    while ((fi = (func_info_t*)map_iter_next(&it))) {
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (!map_isempty(&smt.s.strtb)) printf("========== STRINGS ==========\n");
    map_iter_init(&smt.s.strtb, &it);
    str_info_t* si;
    while ((si = (str_info_t*)map_iter_next(&it))) {
        printf("id: %i, val: %s\n", si->id, si->value);
    }

    if (!map_isempty(&smt.m.allias)) printf("========== ALLIAS ==========\n");
    map_iter_init(&smt.m.allias, &it);
    allias_t* mi;
    while ((mi = (allias_t*)map_iter_next(&it))) {
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

