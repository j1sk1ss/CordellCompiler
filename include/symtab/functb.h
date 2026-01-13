#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <std/str.h>
#include <std/map.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t id;
    string_t*   name;
    ast_node_t* args;
    ast_node_t* rtype;

    struct {
        char    global   : 1;
        char    external : 1;
        char    entry    : 1;
        char    used     : 1;
    } flags;
} func_info_t;

typedef struct func_ctx {
    symbol_id_t curr_id;
    map_t       functb;
} functab_ctx_t;

int FNTB_get_info_id(symbol_id_t id, func_info_t* out, functab_ctx_t* ctx);
int FNTB_get_info(string_t* fname, func_info_t* out, functab_ctx_t* ctx);
symbol_id_t FNTB_add_info(string_t* name, int global, int external, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNTB_update_info(symbol_id_t id, int used, int entry, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);
int FNTB_unload(functab_ctx_t* ctx);

#endif