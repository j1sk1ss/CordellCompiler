#ifndef CPL_PARSER_H_
#define CPL_PARSER_H_

#include <std/str.h>
#include <std/stack.h>
#include <prep/token.h>
#include <prep/token_types.h>
#include <prep/dict.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define CURRENT_TOKEN ((token_t*)list_iter_current(it))
int var_lookup(ast_node_t* node, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl block with input tokens. Should be invoked on new block.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.
- ex - Exit token type, that will end block parsing.

Return ast node.
*/
ast_node_t* cpl_parse_block(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, token_type_t ex);

/*
Parse .cpl asm block with input tokens. Should be invoked on new ASM token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_asm(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl switch block with input tokens. Should be invoked on switch token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_switch(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl if block with input tokens. Should be invoked on if token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_condop(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl declaration array block. Should be invoked on array declaration block.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_array_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl declaration variable block. Should be invoked on variable declaration block.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_variable_declaration(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl extern block. Should be invoked on extern block.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_extern(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl exit and return block. Should be invoked on return or exit token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_rexit(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl function call. Should be invoked on funccall token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_funccall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl function with body and params. Should be invoked on function entry body.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_function(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl import block. Should be invoked on import token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_import(list_iter_t* it, sym_table_t* smt);

/*
Parse .cpl expression block (function, arithmetics, etc.). Can be invoked on any token type.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_expression(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl scope block. Should be invoked on scope token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl start block. Should be invoked on start token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_start(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl syscall block. Should be invoked on syscall token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_syscall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt);

/*
Parse .cpl breakpoint block. Should be invoked on breakpoint token.
Params:
- it - Current iterator on token list.
- ctx - AST ctx.
- smt - Symtable pointer.

Return ast node.
*/
ast_node_t* cpl_parse_breakpoint(list_iter_t* it);

#endif