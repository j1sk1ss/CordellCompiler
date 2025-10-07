#ifndef LIR_H_
#define LIR_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/map.h>
#include <std/stack.h>
#include <std/stackmap.h>
#include <lir/lir_types.h>

#define LIR_VAL_MSIZE 128
typedef struct {
    long value;
} lir_constant_t;

typedef struct {
    int offset;
    int v_id;
} lir_variable_t;

typedef struct {
    lir_registers_t reg;
} lir_register_t;

typedef struct {
    char value[LIR_VAL_MSIZE];
} lir_number_t;

typedef struct {
    int lb_id;
} lir_label_t;

typedef struct {
    int sid;
} lir_str_t;

typedef struct {
    long               id;
    char               size;
    lir_subject_type_t t;
    union {
        lir_constant_t cnst;
        lir_variable_t var;
        lir_register_t reg;
        lir_number_t   num;
        lir_label_t    lb;
        lir_str_t      str;
    } storage;
} lir_subject_t;

typedef struct lir_block {
    struct lir_block* prev;
    struct lir_block* next;
    lir_operation_t   op;
    lir_subject_t*    farg;
    lir_subject_t*    sarg;
    lir_subject_t*    targ;
    int               args;
} lir_block_t;

typedef struct {
    long          cid;
    int           lid;
    lir_block_t*  h;
    lir_block_t*  t;
    scope_stack_t heap;
    stack_map_t   stk;
    map_t*        vars;
} lir_ctx_t;

lir_subject_t* LIR_create_subject(
    int t, lir_registers_t r, int v_id, long offset, const char* strval, long intval, int size, int s_id
);

lir_ctx_t* LIR_create_ctx();
lir_block_t* LIR_create_block(lir_operation_t op, lir_subject_t* fa, lir_subject_t* sa, lir_subject_t* ta);
int LIR_append_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_remove_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_unload_blocks(lir_block_t* block);
int LIR_destroy_ctx(lir_ctx_t* ctx);

#define LIR_SUBJ_REG(r, sz) \
    LIR_create_subject(LIR_REGISTER, r, -1, 0, NULL, 0, sz, 0)

#define LIR_SUBJ_CONST(val) \
    LIR_create_subject(LIR_CONSTVAL, 0, -1, 0, NULL, val, 0, 0)

#define LIR_SUBJ_NUMBER(val) \
    LIR_create_subject(LIR_NUMBER, 0, -1, 0, val, 0, 0, 0)

#define LIR_SUBJ_VAR(kind, off, sz) \
    LIR_create_subject(kind, 0, -1, off, NULL, 0, sz, 0)

#define LIR_SUBJ_GLVAR(id) \
    LIR_create_subject(LIR_GLVARIABLE, 0, id, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_OFF(off, sz) \
    LIR_create_subject(LIR_MEMORY, 0, -1, off, NULL, 0, sz, 0)

#define LIR_SUBJ_LABEL(id) \
    LIR_create_subject(LIR_LABEL, 0, id, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_RAWASM(l) \
    LIR_create_subject(LIR_RAWASM, 0, l, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_STRING(id) \
    LIR_create_subject(LIR_STRING, 0, id, 0, NULL, 0, 0, -1)

#define LIR_SUBJ_FUNCNAME(n) \
    LIR_create_subject(LIR_FNAME, 0, n->storage.str.s_id, 0, NULL, 0, 0, -1)

#define LIR_BLOCK0(ctx, op) \
    LIR_append_block(LIR_create_block((op), NULL, NULL, NULL), (ctx))

#define LIR_BLOCK1(ctx, op, fa) \
    LIR_append_block(LIR_create_block((op), (fa), NULL, NULL), (ctx))

#define LIR_BLOCK2(ctx, op, fa, sa) \
    LIR_append_block(LIR_create_block((op), (fa), (sa), NULL), (ctx))

#endif