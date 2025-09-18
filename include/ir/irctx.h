#ifndef IRCTX_H_
#define IRCTX_H_

#include <ir/ir.h>
#include <ast/ast.h>

#define BSS            1
#define NO_BSS         0
#define DATA_SECTION   1
#define RODATA_SECTION 2
#define EXT_SECTION    3

typedef struct ir_get {
    int (*datagen)(ast_node_t*, int, int, struct ir_get*, ir_ctx_t*);
    int (*funcdef)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*funcret)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*funccall)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*function)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*blockgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*elemegen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*operand)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*store)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*ptrload)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*load)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*assign)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*decl)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*start)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*asmer)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*exit)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*syscall)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*ifgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*whilegen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
    int (*switchgen)(ast_node_t*, struct ir_get*, ir_ctx_t*);
} ir_gen_t;

#endif