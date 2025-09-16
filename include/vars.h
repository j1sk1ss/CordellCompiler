#ifndef VARS_H_
#define VARS_H_

#include "regs.h"
#include "token.h"

int VRS_variable_bitness(token_t* token, char ptr);
int VRS_isptr(token_t* token);
int VRS_one_slot(token_t* token);
int VRS_instack(token_t* token);
int VRS_isblock(token_t* token);
int VRS_isdecl(token_t* token);
int VRS_isclose(token_t* token);
int VRS_isoperand(token_t* token);
int VRS_token_priority(token_t* token);
int VRS_isnumeric(token_t* token);
int VRS_isvariable(token_t* token);
int VRS_instant_movable(token_t* token);
int VRS_issign(token_t* token);
int VRS_is_control_change(token_t* token);
int VRS_is_unpredicted(token_t* token);
int VRS_is_float(token_t* token);
int VRS_update_operator(token_t* token);

#endif