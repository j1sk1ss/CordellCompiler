#ifndef CFG_H_
#define CFG_H_

#include <std/str.h>
#include <std/mm.h>
#include <hir/hir.h>

typedef struct cfg_block {
    long              id;
    hir_block_t*      entry;
    hir_block_t*      exit;
    struct cfg_block* l;
    struct cfg_block* jmp;
    struct cfg_block* next;
} cfg_block_t;

typedef struct {
    cfg_block_t* h;
} cfg_ctx_t;

#endif