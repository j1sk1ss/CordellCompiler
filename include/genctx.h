#ifndef GENCTX_H_
#define GENCTX_H_

#include "mm.h"
#include "syntax.h"

typedef struct gen_ctx {
    short         label;
    syntax_ctx_t* synt;
} gen_ctx_t;

typedef struct gen {
    int  (*datagen)(ast_node_t*, FILE*, int, int, gen_ctx_t*, struct gen*);
    int  (*funcdef)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*funcret)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*funccall)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*function)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*blockgen)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*elemegen)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*operand)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*store)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*ptrload)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*load)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*assign)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*decl)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*start)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*exit)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*syscall)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*ifgen)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*whilegen)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
    int  (*switchgen)(ast_node_t*, FILE*, gen_ctx_t*, struct gen*);
} gen_t;

gen_ctx_t* GEN_create_ctx();
int GEN_destroy_ctx(gen_ctx_t* ctx);

#endif