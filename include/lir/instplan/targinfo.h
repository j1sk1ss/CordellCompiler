#ifndef TARGINFO_H_
#define TARGINFO_H_

#include <stdio.h>
#include <std/mm.h>
#include <std/map.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef struct {
    lir_operation_t op;
    char            reads_memory;
    char            writes_memory;
    char            sets_flags;
    char            uses_flags;
    int             latency;
    float           throughput;
    int             issue_cost;
    char            commutative;
} __attribute__((packed)) op_info_t;

typedef struct {
    char name[32];
    int op_count;
    op_info_t* ops;
} __attribute__((packed)) target_info_t;

int TRGINF_load(char* path, map_t* s);
int TRGINF_unload(map_t* s);

#endif