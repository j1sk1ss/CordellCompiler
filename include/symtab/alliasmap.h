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

/*
Get owners' set for the provided variable ID.
An owner - is a variable, which contains a link to the variable.
```cpl
i32 a;
ptr i32 b = ref a; :/ 'b' is an owner of the 'a' /:
```

Params:
    - `v_id` - Variable which owners we want to collect.
    - `out` - Uninitilized set for the answer.
    - `ctx` - Allias context.

Returns 1 if succeeds.
*/
int ALLIAS_get_owners(symbol_id_t v_id, set_t* out, allias_ctx_t* ctx);

/*
Get slaves' set for the provided variable ID.
A slave - is a variable, which's link is owned by the variable.
```cpl
i32 a;
ptr i32 b = ref a; :/ 'a' if a slave for the 'b' /:
```

Params:
    - `v_id` - Variable which slaves we want to collect.
    - `out` - Uninitilized set for the answer.
    - `ctx` - Allias context.

Returns 1 if succeeds.
*/
int ALLIAS_get_slaves(symbol_id_t v_id, set_t* out, allias_ctx_t* ctx);
int ALLIAS_unload(allias_ctx_t* ctx);

#endif