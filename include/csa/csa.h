#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include <std/list.h>
#include <symtab/symtab.h>

/* Fast AST analysis */
#include <ast/ast.h>
#include <ast/astgen.h>
#include <csa/ast/ast_walker.h>
#include <csa/ast/ast_visitors.h>

/* HIR based analysis (deep static analysis) */
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/cfg.h>
#include <csa/hir/hir_walker.h>
#include <csa/hir/hir_visitors.h>

typedef struct {
    struct {
        char provide_lines : 1;
        char provide_rows  : 1;
        char provide_files : 1;
    } l_conf;
} csa_conf_t;

#define DEF_CONF (csa_conf_t) { .l_conf = { .provide_files = 1, .provide_lines = 1, .provide_rows = 1 } }

int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt, csa_conf_t conf);
int SEM_perform_hir_check(cfg_ctx_t* ctx, dag_ctx_t* dctx, sym_table_t* smt, csa_conf_t conf);

#endif