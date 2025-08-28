#ifndef GENCTX_H_
#define GENCTX_H_

#include "syntax.h"

typedef struct gen_ctx {
    short         label;
    syntax_ctx_t* synt;
    int           (*datagen)(ast_node_t*, FILE*, int, int, struct gen_ctx*);
    int           (*funcdef)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*funcret)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*funccall)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*function)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*blockgen)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*elemegen)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*operand)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*store)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*ptrload)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*load)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*assign)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*decl)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*start)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*exit)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*syscall)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*ifgen)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*whilegen)(ast_node_t*, FILE*, struct gen_ctx*);
    int           (*switchgen)(ast_node_t*, FILE*, struct gen_ctx*);
} gen_ctx_t;

/* Allocate memory for generator context */
gen_ctx_t* GEN_create_ctx();

/* Unload generator context */
int GEN_destroy_ctx(gen_ctx_t* ctx);

#endif