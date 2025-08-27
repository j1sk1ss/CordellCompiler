#ifndef DEADOPT_H_
#define DEADOPT_H_

#include "mm.h"
#include "syntax.h"

typedef struct code_node {
    ast_node_t*       start;
    ast_node_t*       end;
    struct code_node* parent;
    struct code_node* child;
    struct code_node* sibling;
} code_node_t;

typedef struct func_code_node {
    char                   name[TOKEN_MAX_SIZE];
    code_node_t*           head;
    struct func_code_node* next;
} func_code_node_t;

int OPT_deadcode(syntax_ctx_t* ctx);

#endif