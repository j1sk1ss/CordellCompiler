#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include "ast.h"
#include "str.h"
#include "dict.h"
#include "vars.h"
#include "stack.h"
#include "token.h"
#include "synctx.h"
#include "vartb.h"
#include "arrtb.h"

/* cpl_block.c */
ast_node_t* cpl_parse_block(token_t** curr, syntax_ctx_t* ctx, unsigned long long stop_mask, parser_t* p);

/* cpl_asm.c */
ast_node_t* cpl_parse_asm(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_cond.c */
ast_node_t* cpl_parse_switch(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
ast_node_t* cpl_parse_condop(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_decl.c */
ast_node_t* cpl_parse_array_declaration(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
ast_node_t* cpl_parse_variable_declaration(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_func.c */
ast_node_t* cpl_parse_extern(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
ast_node_t* cpl_parse_rexit(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
ast_node_t* cpl_parse_funccall(token_t** curr, syntax_ctx_t* ctx, parser_t* p);
ast_node_t* cpl_parse_function(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_import.c */
ast_node_t* cpl_parse_import(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_op.c */
ast_node_t* cpl_parse_expression(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_scope.c */
ast_node_t* cpl_parse_scope(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_start.c */
ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

/* cpl_syscall.c */
ast_node_t* cpl_parse_syscall(token_t** curr, syntax_ctx_t* ctx, parser_t* p);

#endif