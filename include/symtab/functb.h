#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <ast/ast.h>
#include <prep/token.h>

typedef struct func_info {
    char              name[TOKEN_MAX_SIZE];
    ast_node_t*       args;
    ast_node_t*       rtype;
    struct func_info* next;
} func_info_t;

typedef struct func_ctx {
    func_info_t* h;
} functab_ctx_t;

functab_ctx_t* FNT_create_ctx();
int FNT_destroy_ctx(functab_ctx_t* ctx);
int FNT_get_info(const char* fname, func_info_t* out, functab_ctx_t* ctx);
int FNT_add_info(const char* name, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNT_unload(functab_ctx_t* ctx);

#endif