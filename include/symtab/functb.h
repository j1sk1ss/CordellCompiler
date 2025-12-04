#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <std/str.h>
#include <std/map.h>
#include <prep/token_types.h>
#include <ast/ast.h>

typedef struct {
    long        id;
    string_t*   name;
    char        global;
    char        external;
    char        entry;
    char        used;
    ast_node_t* args;
    ast_node_t* rtype;
} func_info_t;

typedef struct func_ctx {
    long  curr_id;
    map_t functb;
} functab_ctx_t;

int FNTB_get_info_id(long id, func_info_t* out, functab_ctx_t* ctx);
int FNTB_get_info(string_t* fname, func_info_t* out, functab_ctx_t* ctx);
int FNTB_add_info(string_t* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNTB_update_info(long id, int used, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNTB_unload(functab_ctx_t* ctx);

#endif