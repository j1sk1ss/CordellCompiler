#ifndef Z3_H_
#define Z3_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <std/set.h>
#include <symtab/symtab_id.h>

#define Z3_SCRIPT "/home/j1sk1ss/Desktop/CordellCompiler/src/sem/hir/z3_wrapper/z3_wrapper.py"
#define Z3_WRAPPER_MAX_ARGS 128

int Z3_can_vid_be_equal(symbol_id_t v_id, long long value, string_t* f, FILE* dump);
int Z3_can_reach_label(long l_id, string_t* f, FILE* dump);

#endif
