#ifndef PEEPHOLE_H_
#define PEEPHOLE_H_

#include <std/map.h>
#include <std/list.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lir_types.h>

typedef enum {
    PEEPHOLE_ANY_OBJECT,
    PEEPHOLE_ANY_REGISTER,
    PEEPHOLE_ANY_CONSTANT,
    PEEPHOLE_REGISTER,
    PEEPHOLE_CONSTANT,
    PEEPHOLE_MEMORY
} peephole_object_type_t;

typedef struct {
    long                   id;
    long                   val;
    peephole_object_type_t type;
} peephole_object_t;

typedef struct {
    list_t src;
    list_t trg;
} peephole_pattern_t;

typedef struct {
    map_t patterns;
    int   (*perform_peephole)(cfg_ctx_t*);
} peephole_t;

int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole);

#endif