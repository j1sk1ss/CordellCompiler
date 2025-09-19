#ifndef IRCTX_H_
#define IRCTX_H_

#include <ir/ir.h>
#include <ast/ast.h>

typedef struct ir_gen {
    int (*funcdef)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*funcret)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*funccall)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*function)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*blockgen)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*elemegen)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*operand)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*store)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*ptrload)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*load)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*assign)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*decl)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*start)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*asmer)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*exit)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*syscall)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*ifgen)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*whilegen)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
    int (*switchgen)(ast_node_t*, struct ir_gen*, ir_ctx_t*);
} ir_gen_t;

#endif