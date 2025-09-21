#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <std/str.h>
#include <std/vars.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <ast/ast.h>
#include <ast/synctx.h>
#include <symtab/symtab.h>

/* cpl_block.c */
ast_node_t* cpl_parse_block(token_t** curr, syntax_ctx_t* ctx, token_type_t ex);

/* cpl_asm.c */
ast_node_t* cpl_parse_asm(token_t** curr, syntax_ctx_t* ctx);

/* cpl_cond.c */
ast_node_t* cpl_parse_switch(token_t** curr, syntax_ctx_t* ctx);
ast_node_t* cpl_parse_condop(token_t** curr, syntax_ctx_t* ctx);

/* cpl_decl.c */
ast_node_t* cpl_parse_array_declaration(token_t** curr, syntax_ctx_t* ctx);
ast_node_t* cpl_parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx);

/* cpl_func.c */
ast_node_t* cpl_parse_extern(token_t** curr, syntax_ctx_t* ctx);
ast_node_t* cpl_parse_rexit(token_t** curr, syntax_ctx_t* ctx);
ast_node_t* cpl_parse_funccall(token_t** curr, syntax_ctx_t* ctx);
ast_node_t* cpl_parse_function(token_t** curr, syntax_ctx_t* ctx);

/* cpl_import.c */
ast_node_t* cpl_parse_import(token_t** curr, syntax_ctx_t* ctx);

/* cpl_op.c */
ast_node_t* cpl_parse_expression(token_t** curr, syntax_ctx_t* ctx);

/* cpl_scope.c */
ast_node_t* cpl_parse_scope(token_t** curr, syntax_ctx_t* ctx);

/* cpl_start.c */
ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx);

/* cpl_syscall.c */
ast_node_t* cpl_parse_syscall(token_t** curr, syntax_ctx_t* ctx);

#endif