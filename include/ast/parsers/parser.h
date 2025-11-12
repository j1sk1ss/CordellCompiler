#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <std/str.h>
#include <prep/token.h>
#include <prep/token_types.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <ast/ast.h>
#include <ast/synctx.h>
#include <symtab/symtab.h>

int var_lookup(ast_node_t* node, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_block.c */
ast_node_t* cpl_parse_block(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt, token_type_t ex);

/* cpl_asm.c */
ast_node_t* cpl_parse_asm(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_cond.c */
ast_node_t* cpl_parse_switch(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_condop(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_decl.c */
ast_node_t* cpl_parse_array_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_func.c */
ast_node_t* cpl_parse_extern(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_rexit(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_funccall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_function(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_import.c */
ast_node_t* cpl_parse_import(list_iter_t* it, sym_table_t* smt);

/* cpl_op.c */
ast_node_t* cpl_parse_expression(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_scope.c */
ast_node_t* cpl_parse_scope(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_start.c */
ast_node_t* cpl_parse_start(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);

/* cpl_syscall.c */
ast_node_t* cpl_parse_syscall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt);
ast_node_t* cpl_parse_breakpoint(list_iter_t* it);

#endif