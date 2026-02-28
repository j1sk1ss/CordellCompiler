#ifndef ALLIAS_H_
#define ALLIAS_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/logg.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t v_id;
    set_t       owners;
    set_t       delown;
} allias_t;

typedef struct {
    map_t allias;
} allias_ctx_t;

int ALLIAS_mark_owner(symbol_id_t v_id, symbol_id_t owner_id, allias_ctx_t* ctx);
int ALLIAS_add_owner(symbol_id_t v_id, symbol_id_t owner_id, allias_ctx_t* ctx);
int ALLIAS_get_owners(symbol_id_t v_id, set_t* out, allias_ctx_t* ctx);
int ALLIAS_unload(allias_ctx_t* ctx);

#endif