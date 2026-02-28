#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <std/str.h>
#include <std/map.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <symtab/symtab_id.h>

typedef struct {
    string_t*   name;  /* Base function name    */
    string_t*   virt;  /* De-virtual name       */

    symbol_id_t id;    /* String ID in symtable */
    ast_node_t* args;  /* Input arguments       */
    ast_node_t* rtype; /* Function return type  */

    short       sid;

    struct {
        char    global   : 1;
        char    external : 1;
        char    entry    : 1;
        char    used     : 1;
        char    local    : 1;
        char    naked    : 1;
    } flags;
} func_info_t;

typedef struct func_ctx {
    symbol_id_t curr_id;
    map_t       functb;
} functab_ctx_t;

/*
Collect all functions with the same name.
Params:
    - `fname` - Target function name.
    - `out` - The output list.
    - `ctx` - Function symbol table.

Returns 1 if succeeds. Otherwise will return 0.
*/
int FNTB_collect_info(string_t* fname, list_t* out, functab_ctx_t* ctx);

/*
Get function from a table by the provided ID.
Params:
    - `id` - Function's ID.
    - `out` - Function output body.
    - `ctx` - Function symbol table.

Returns 1 if succeeds. Otherwise will return 0. 
*/
int FNTB_get_info_id(symbol_id_t id, func_info_t* out, functab_ctx_t* ctx);

/*
Get function from a table by the provided name.
Params:
    - `fname` - Function's name.
    - `sid` - Scope ID.
    - `out` - Function output body.
    - `ctx` - Function symbol table.

Returns 1 if succeeds. Otherwise will return 0.
*/
int FNTB_get_info(string_t* fname, short sid, func_info_t* out, functab_ctx_t* ctx);

/*
Add a new function to a function symbol table.
Params:
    - `name` - Function's name.
               Note: Will copy the provided name.
    - `global` - Is this function global?
    - `local` - Is this a local function?
    - `entry` - Is this an entry function?
    - `naked` - Is this a naked function?
    - `args` - Function's arguments from AST.
    - `rtype` - Function's return type from AST.
    - `ctx` - Function symbol table.

Returns -1 if fails or a new function's ID.
*/
symbol_id_t FNTB_add_info(
    string_t* name, 
    int global, int local, int entry, int naked, /* flags */
    short sid, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx
);

/*
Update the provided function in a symtable.
Params:
    - `id` - Function ID.
    - `entry` - Is this is an entry function?
                Node: -1 - Saves the previous value.
    - `used` - Is this function used?
               Node: -1 - Saves the previous value.
    - `ext` - Is this is an external function?
              Node: -1 - Saves the previous value.
    - `args` - Function's AST arguments.
               Node: NULL - Saves the previous value.
    - `rtype` - Function's AST return type node.
                Node: NULL - Saves the previous value.
    - `ctx` - Function symtable context.

Returns 1 if succeeds.
*/
int FNTB_update_info(symbol_id_t id, int used, int entry, int ext, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx);

/*
Rename an existed function.
Note: Will update the virtual name of a function.
Params:
    - `id` - Function ID.
    - `name` - New name.
    - `ctx` - Function symtable context.

Returns 1 if succeeds, otherwise will return 0.
*/
int FNTB_rename_func(symbol_id_t id, string_t* name, functab_ctx_t* ctx);

/*
Unload a function symtable context.
Params:
    - `ctx` - Function symtable context.

Returns 1 if succeeds.
*/
int FNTB_unload(functab_ctx_t* ctx);

#define fn_iterate_args(fn) \
    for (ast_node_t* arg = (fn)->args->c; arg && arg->t->t_type != SCOPE_TOKEN; arg = arg->siblings.n)

#endif