#ifndef VARS_H_
#define VARS_H_

#include <prep/token.h>

int TKN_variable_bitness(token_t* token, char ptr);
int TKN_isptr(token_t* token);
int TKN_one_slot(token_t* token);
int TKN_instack(token_t* token);
int TKN_isblock(token_t* token);
int TKN_isdecl(token_t* token);
int TKN_isclose(token_t* token);
int TKN_isoperand(token_t* token);
int TKN_token_priority(token_t* token);
int TKN_isnumeric(token_t* token);
int TKN_isvariable(token_t* token);
int TKN_issign(token_t* token);
int TKN_is_float(token_t* token);
int TKN_update_operator(token_t* token);

#endif