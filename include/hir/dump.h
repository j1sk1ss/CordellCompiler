#ifndef DUMP_H_
#define DUMP_H_

#include <unistd.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>

typedef struct {
    int fd;
} dump_ctx_t;

#endif
