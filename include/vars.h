#ifndef VARS_H_
#define VARS_H_

#include "regs.h"
#include "token.h"

int VRS_variable_bitness(token_t* token, char ptr);
int VRS_isptr(token_t* token);
int VRS_one_slot(token_t* token);
int VRS_intext(token_t* token);
int VRS_isblock(token_t* token);
int VRS_isdecl(token_t* token);
int VRS_isclose(token_t* token);
int VRS_isoperand(token_t* token);
int VRS_token_priority(token_t* token);

#endif