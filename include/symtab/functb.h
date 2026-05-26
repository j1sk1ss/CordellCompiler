#ifndef FUNC_TB_H_
#define FUNC_TB_H_

#include <std/str.h>
#include <std/map.h>
#include <std/list.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/dump.h>
#include <symtab/symtab_id.h>

typedef struct {
    string_t*    name;  /* Base function name    */
    string_t*    virt;  /* De-virtual name       */

    symbol_id_t  id;    /* String ID in symtable */
    list_t       local; /* Local functions       */

    ast_node_t*  args;  /* Input arguments       */
    ast_node_t*  rtype; /* Function return type  */

    symbol_id_t  s_id;

    struct {
        char     global   : 1;
        char     external : 1;
        char     entry    : 1;
        char     used     : 1;
        char     local    : 1;
        char     naked;
        char     vargs    : 1;
        char     generic  : 1;
        char     self     : 1;
        char     inln;
    } flags;

    struct {
        list_t   registered_types; /* List of registered symbol_id_t types     */
        map_t    generic;          /* Generic base types. TypeId <-> TokenType */
        list_t   resolutions;      /* Resolved copies (use the generic field)  */
    } template;
} func_info_t;

typedef struct func_ctx {
    symbol_id_t curr_id;
    map_t       functb;
} functab_ctx_t;

/*
Collect all functions with the same name.
Params:
    - `fname` - Target function name.
    - `s_id` - Function's scope.
    - `out` - The output list.
    - `ctx` - Function symbol table.

Returns 1 on success, otherwise 0.
*/
int FNTB_collect_info(string_t* fname, symbol_id_t s_id, list_t* out, functab_ctx_t* ctx);

/*
Get function from a table by the provided ID.
Params:
    - `id` - Function's ID.
    - `out` - Function output body.
    - `ctx` - Function symbol table.

Returns 1 on success, otherwise 0.
*/
int FNTB_get_info_id(symbol_id_t id, func_info_t* out, functab_ctx_t* ctx);

/*
Get function from a table by the provided name.
Params:
    - `fname` - Function's name.
    - `sid` - Scope ID.
    - `out` - Function output body.
    - `ctx` - Function symbol table.

Returns 1 on success, otherwise 0.
*/
int FNTB_get_info(string_t* fname, symbol_id_t s_id, func_info_t* out, functab_ctx_t* ctx);

/*
Add a new function to a function symbol table.
Params:
    - `name` - Function's name.
               Note: Will copy the provided name.
    - `vname` - Vartual function's name.
                Note: May be the 'NULL' value.
    - `global` - Is this function global?
    - `local` - Is this a local function?
    - `entry` - Is this an entry function?
    - `naked` - Is this a naked function?
    - `vargs` - Is this a vargs function?
    - `generic` - Is this is a generic function?
    - `inln` - Inline status.
    - `stat` - Is this is a static function?
    - `args` - Function's arguments from AST.
    - `rtype` - Function's return type from AST.
    - `ctx` - Function symbol table.

Returns -1 if fails or a new function's ID.
*/
symbol_id_t FNTB_add_info(
    string_t* name, string_t* vname,
    int global, int local, int entry, int naked, int vargs, int generic, int inln, int stat, /* flags */
    symbol_id_t s_id, ast_node_t* args, ast_node_t* rtype, functab_ctx_t* ctx
);

/*
Create a copy of a function. Will copy all fields except locals.
Params:
    - `src` - Function to copy.
    - `ctx` - Symtable context.

Returns the ID of a copied function.
*/
symbol_id_t FNTB_add_copy(func_info_t* src, functab_ctx_t* ctx);

// TODO: docs
int FNTB_has_generic_types(symbol_id_t f_id, functab_ctx_t* ctx);

/*
Reset registered types for a function. Can be useful in the situation
when you re-register a function from a prototype to an implementation.
Params:
    - `f_id` - Function ID.
    - `ctx` - Symtable context.

Returns 1 if succeeds.
*/
int FNTB_clear_generic_types(symbol_id_t f_id, functab_ctx_t* ctx);

/*
Add a generic type for a function. This function adds a type obly if it is
a generic type. Consider to add the type to a symtable first.
Params:
    - `f_id` - Function ID.
    - `t_id` - Type ID.
    - `ctx` - Symtable context.

Returns 1 if succeeds.
*/
int FNTB_register_generic_type(symbol_id_t f_id, symbol_id_t t_id, functab_ctx_t* ctx);

/*
Register an existed function as a local function.
Params:
    - `f_id` - Parent function.
    - `l_id` - Local function.
    - `ctx` - Function symbol table.

Returns 1 on success, otherwise 0.
*/
int FNTB_add_local(symbol_id_t f_id, symbol_id_t l_id, functab_ctx_t* ctx);

#define FNTB_SET_EXTERNAL NULL, FIELD_NO_CHANGE, 1, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, NULL, NULL
#define FNTB_SET_VARGS    NULL, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, 1, NULL, NULL
#define FNTB_SET_NAKED    NULL, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, FIELD_NO_CHANGE, 2, FIELD_NO_CHANGE, NULL, NULL
/*
Update an existed function.
Note: Will update the virtual name of a function.
Params:
    - `id` - Function ID.
    - `name` - New name.
    - `used` - Is this function used?
    - `external` - Is this function external?
    - `global` - Is this function global?
    - `local` - Is this a local function?
    - `entry` - Is this an entry function?
    - `naked` - Is this a naked function?
    - `vargs` - Is this function has the vargs arg?
    - `args` - Function's arguments from AST.
    - `rtype` - Function's return type from AST.
    - `ctx` - Function symtable context.

Returns 1 on success, otherwise 0.
*/
int FNTB_update_func(
    symbol_id_t id, 
    string_t* name, 
    int used, int external, int global, int local, int entry, int naked, int vargs, /* flags */
    ast_node_t* args, ast_node_t* rtype,
    functab_ctx_t* ctx
);

/*
Create a copy with resolved generic types. Will create a copy with a modified name. Consider
to check whether a function with the provided types already exists.
Params:
    - `id` - Source founction (template function).
    - `types` - List of provided token types.
    - `ctx` - Symtable context.

Returns the ID of a new function.
*/
symbol_id_t FNTB_create_resolved_copy(symbol_id_t id, list_t* types, functab_ctx_t* ctx);

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
