#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <ast/ast.h>
#include <prep/token.h>

typedef struct func_info {
    long              id;
    char              name[TOKEN_MAX_SIZE];
    int               global;
    int               external;
    ast_node_t*       args;
    ast_node_t*       rtype;
    struct func_info* next;
} func_info_t;

typedef struct func_ctx {
    long         curr_id;
    func_info_t* h;
} functab_ctx_t;

int FNTB_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx);
int FNTB_add_info(const char* name, int global, int external, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNTB_unload(functab_ctx_t* ctx);

#endif