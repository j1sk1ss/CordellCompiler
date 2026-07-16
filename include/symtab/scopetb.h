#ifndef SCOPETB_H_
#define SCOPETB_H_

#include <std/map.h>
#include <std/stack.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t curr_id;
    map_t       parents; /* scope_id -> parent scope_id */
} scopetab_ctx_t;

symbol_id_t SCPTB_push_scope(scopetab_ctx_t* ctx, sstack_t* stack);
symbol_id_t SCPTB_get_parent(symbol_id_t id, scopetab_ctx_t* ctx);
int SCPTB_is_visible(symbol_id_t target, symbol_id_t from, scopetab_ctx_t* ctx);
int SCPTB_unload(scopetab_ctx_t* ctx);

#endif
