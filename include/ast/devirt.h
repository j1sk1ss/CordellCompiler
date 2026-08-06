#ifndef AST_DEVIRT_H_
#define AST_DEVIRT_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/queue.h>
#include <symtab/symtab.h>
#include <ast/ast.h>

typedef struct {
    ast_node_t* root; /* Copy of a template without resolved calls */
    symbol_id_t f_id;
} template_t;

typedef struct {
    map_t   templates; /* FuncId <-> Template                                 */
    queue_t to_impl;   /* The list which contains templates to implementation */
    set_t   impl;      /* FuncId that are implemented                         */
} devirt_ctx_t;

typedef struct {
    string_t*   prefix;
    ast_node_t* func;
} method_t;

int AST_DVRT_init_ctx(devirt_ctx_t* ctx);
int AST_DVRT_register_template(symbol_id_t f_id, ast_node_t* root, devirt_ctx_t* ctx);
int AST_DVRT_register_implementation(symbol_id_t f_id, symbol_id_t src_id, devirt_ctx_t* ctx);
int AST_DVRT_pop_implementation(sym_table_t* smt, devirt_ctx_t* ctx, ast_node_t** out);
int AST_DVRT_unload_ctx(devirt_ctx_t* ctx);
int AST_DVRT_find_templates(ast_node_t* root, sym_table_t* smt, devirt_ctx_t* ctx);
int AST_DVRT_resolve_calls(ast_node_t* root, sym_table_t* smt, devirt_ctx_t* ctx);
int AST_DVRT_move_container_functions(ast_node_t* root, queue_t* out);

#endif
