#ifndef CONDUNROLL_H_
#define CONDUNROLL_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>
#include <prep/token.h>
#include <ast/syntax.h>
#include <symtab/symtab.h>

/*
Unroll [IF], [WHILE] and [SWITCH] statements. 
[IF] Example of unrolling with constant stmt:
if 1; {
    int a = b;
    int c = a + b;
}
->
int a = b;
int c = a + b;

[WHILE] Example of unrolling with predicted steps count:
int a = 2;
while a - 1 > 0; {
    b = b + 1;
}
->
b = b + 1;
b = b + 1;

[SWITCH] Example of removing redundant branches
switch 1; {
    default {
        int a = 0;
    }
    case 1; {
        int b = 1;
    }
}
->
int a = 0;
*/
int OPT_condunroll(syntax_ctx_t* ctx);

#endif