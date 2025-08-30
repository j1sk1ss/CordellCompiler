#ifndef MARKUP_H_
#define MARKUP_H_

#include "dict.h"
#include "token.h"
#include "stack.h"

/*
Preparing tokens list for parsing tree.
We mark every token by command / symbol ar value type.
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_mnemonics(token_t* head);

/*
Iterate throught tokens and mark variables. 
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_variables(token_t* head);

#endif