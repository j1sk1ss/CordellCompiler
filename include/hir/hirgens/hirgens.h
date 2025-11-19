#ifndef HIRGENS_H_
#define HIRGENS_H_

#include <std/qsort.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <hir/hir.h>
#include <hir/hir_types.h>

/*
Generate convertion from one type to another. 
Params:
- ctx - HIR ctx.
- t - Target type.
- src - Source HIR subject.
- smt - Symtable.

Return converted HIR subject.
*/
hir_subject_t* HIR_generate_conv(hir_ctx_t* ctx, hir_subject_type_t t, hir_subject_t* src, sym_table_t* smt);

/*
Convert AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
hir_subject_t* HIR_generate_elem(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert funccall AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.
- ret - Will return or not?

Return parsed from AST HIR subject.
*/
hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret);

/*
Convert load AST node (arr[i]) into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert oprand AST node (x + x) into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
hir_subject_t* HIR_generate_operand(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.
- ret - Will return or not?

Return parsed from AST HIR subject.
*/
hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret);

/*
Convert import AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.

Return parsed from AST HIR subject.
*/
int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx);

/*
Generate breakpoint AST node into HIR element. 
Params:
- ctx - HIR ctx.

Return parsed from AST HIR subject.
*/
int HIR_generate_breakpoint_block(hir_ctx_t* ctx);

/*
Convert extern AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.

Return parsed from AST HIR subject.
*/
int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx);

/*
Convert operation (+=, -=, *=, /=) AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_update_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert load AST node (arr[i] = 0) node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert asmblock AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert assignment AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert block AST (from SCOPE to SCOPE) node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert if AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert while AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert switch AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert declaration AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert return AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert function AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert start AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

/*
Convert exit AST node into HIR element. 
Params:
- node - AST node.
- ctx - HIR ctx.
- smt - Symtable.

Return parsed from AST HIR subject.
*/
int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt);

#endif