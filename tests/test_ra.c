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
#include <hir/opt/ra.h>
#include "ast_helper.h"
#include "hir_helper.h"

void igraph_dump_dot(igraph_t* g) {
    fprintf(stdout, "graph G {\n");
    fprintf(stdout, "  node [style=filled];\n");

    list_iter_t it;
    list_iter_hinit(&g->nodes, &it);
    igraph_node_t* n;
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        const char* colors[] = { "red", "green", "blue", "yellow", "cyan", "magenta", "orange", "grey", "brown", "pink" };
        int palette_size = sizeof(colors)/sizeof(colors[0]);
        const char* fill = (n->color >= 0 && n->color < palette_size) ? colors[n->color] : "white";
        fprintf(stdout, "  v%ld [label=\"v%ld\\ncolor=%d\", fillcolor=%s];\n", n->v_id, n->v_id, n->color, fill);
    }

    list_iter_hinit(&g->nodes, &it);
    while ((n = (igraph_node_t*)list_iter_next(&it))) {
        set_iter_t sit;
        set_iter_init(&n->v, &sit);
        long neighbor;
        while ((neighbor = set_iter_next_int(&sit)) >= 0) {
            if (n->v_id < neighbor) {
                fprintf(stdout, "  v%ld -- v%ld;\n", n->v_id, neighbor);
            }
        }
    }

    fprintf(stdout, "}\n");
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

    if (!list_isempty(&smt.m.lst)) printf("========== ALLIAS ==========\n");
    list_iter_hinit(&smt.m.lst, &it);
    allias_t* mi;
    while ((mi = (allias_t*)list_iter_next(&it))) {
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while ((own_id = set_iter_next_int(&sit)) >= 0) printf("%i ", own_id);
        printf("\n");
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
