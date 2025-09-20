#ifndef HIRGENS_H_
#define HIRGENS_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <ast/synctx.h>
#include <hir/hir.h>
#include <hir/hir_types.h>

hir_subject_t* HIR_generate_elem(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_operand(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_syscall(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_update_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx);
int HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx);

#endif