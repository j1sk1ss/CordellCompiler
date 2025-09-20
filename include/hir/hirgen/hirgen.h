#ifndef HIRGEN_H_
#define HIRGEN_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <ast/synctx.h>
#include <hir/hir.h>

static inline hir_subject_type_t HIR_get_tmptype(token_t* token) {
    if (!token) return TMPVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? TMPVARI8 : TMPVARU8;
            case 16: return issigned ? TMPVARI16 : TMPVARU16;
            case 32: return issigned ? TMPVARI32 : TMPVARU32;
            default: return issigned ? TMPVARI64 : TMPVARU64;
        }
    }

    switch (bitness) {
        case 32: return TMPVARF32;
        default: return TMPVARF64;
    }
}  

static inline hir_subject_type_t HIR_get_stktype(token_t* token) {
    if (!token) return TMPVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? STKVARI8 : STKVARU8;
            case 16: return issigned ? STKVARI16 : STKVARU16;
            case 32: return issigned ? STKVARI32 : STKVARU32;
            default: return issigned ? STKVARI64 : STKVARU64;
        }
    }

    switch (bitness) {
        case 32: return STKVARF32;
        default: return STKVARF64;
    }
}  

static inline hir_subject_type_t HIR_get_glbtype(token_t* token) {
    if (!token) return TMPVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? GLBVARI8 : GLBVARU8;
            case 16: return issigned ? GLBVARI16 : GLBVARU16;
            case 32: return issigned ? GLBVARI32 : GLBVARU32;
            default: return issigned ? GLBVARI64 : GLBVARU64;
        }
    }

    switch (bitness) {
        case 32: return GLBVARF32;
        default: return GLBVARF64;
    }
}

hir_subject_t* HIR_generate_asmblock(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_elem(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx);
int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_declaration_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_funccall_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_function_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_ptr_load_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_load_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_operand_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_store_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_start_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx);
hir_subject_t* HIR_generate_syscall_block(ast_node_t* node, hir_ctx_t* ctx);

#endif