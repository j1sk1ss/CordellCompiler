#ifndef MARKUP_H_
#define MARKUP_H_

#include <std/map.h>
#include <std/str.h>
#include <std/hash.h>
#include <std/list.h>
#include <std/stack.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <prep/token_types.h>

/*
Preparing tokens list for parsing tree.
We mark every token by command / symbol ar value type.
Params:
    - `tkn` - Tokens list.

Return -1 if something goes wrong.
Returns 1 if markup succeeds.
*/
int MRKP_mnemonics(list_t* tkn);

/*
Iterate throught tokens and mark variables. 
Params:
    - `tkn` - Tokens list.

Return -1 if something goes wrong.
Returns 1 if markup succeeds.
*/
int MRKP_variables(list_t* tkn);

#endif