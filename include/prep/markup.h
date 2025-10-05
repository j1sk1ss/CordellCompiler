#ifndef MARKUP_H_
#define MARKUP_H_

#include <prep/dict.h>
#include <prep/token.h>
#include <std/list.h>
#include <std/stack.h>

/*
Preparing tokens list for parsing tree.
We mark every token by command / symbol ar value type.
Params:
- tkn - Tokens list.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_mnemonics(list_t* tkn);

/*
Iterate throught tokens and mark variables. 
Params:
- tkn - Tokens list.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_variables(list_t* tkn);

#endif