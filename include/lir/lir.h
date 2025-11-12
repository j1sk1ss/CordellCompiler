#ifndef LIR_H_
#define LIR_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/map.h>
#include <std/list.h>
#include <std/stack.h>
#include <std/stackmap.h>
#include <lir/lir_types.h>

typedef struct {
    long value;
} lir_constant_t;

typedef struct {
    int  offset;
    long v_id;
} lir_variable_t;

typedef struct {
    lir_registers_t reg;
} lir_register_t;

#define LIR_VAL_MSIZE 128
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
    list_t h;
} lir_list_t;

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
        lir_list_t     list;
    } storage;
} lir_subject_t;

typedef struct lir_block {
    char              unused;
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

lir_ctx_t* LIR_create_ctx();
lir_block_t* LIR_create_block(lir_operation_t op, lir_subject_t* fa, lir_subject_t* sa, lir_subject_t* ta);
lir_subject_t* LIR_create_subject(int t, int reg, int v_id, long offset, const char* strval, long intval, int size);
int LIR_unlink_block(lir_block_t* block);
int LIR_insert_block_after(lir_block_t* block, lir_block_t* pos);
int LIR_insert_block_before(lir_block_t* block, lir_block_t* pos);
int LIR_append_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_subj_equals(lir_subject_t* a, lir_subject_t* b);
int LIR_unload_subject(lir_subject_t* s);
int LIR_unload_blocks(lir_block_t* block);
int LIR_destroy_ctx(lir_ctx_t* ctx);

#define LIR_SUBJ_REG(reg, sz) LIR_create_subject(LIR_REGISTER, reg, -1, 0, NULL, 0, sz)
#define LIR_SUBJ_CONST(val)   LIR_create_subject(LIR_CONSTVAL, -1, -1, 0, NULL, val, 0)
#define LIR_SUBJ_NUMBER(val)  LIR_create_subject(LIR_NUMBER, -1, -1, 0, val, 0, 0)
#define LIR_SUBJ_VAR(id, sz)  LIR_create_subject(LIR_VARIABLE, -1, id, -1, NULL, 0, sz)
#define LIR_SUBJ_GLVAR(id)    LIR_create_subject(LIR_GLVARIABLE, -1, id, 0, NULL, 0, 0)
#define LIR_SUBJ_OFF(off, sz) LIR_create_subject(LIR_MEMORY, -1, -1, off, NULL, 0, sz)
#define LIR_SUBJ_LABEL(id)    LIR_create_subject(LIR_LABEL, -1, id, 0, NULL, 0, 0)
#define LIR_SUBJ_RAWASM(l)    LIR_create_subject(LIR_RAWASM, -1, l, 0, NULL, 0, 0)
#define LIR_SUBJ_STRING(id)   LIR_create_subject(LIR_STRING, -1, id, 0, NULL, 0, 0)
#define LIR_SUBJ_FUNCNAME(n)  LIR_create_subject(LIR_FNAME, -1, n->storage.str.s_id, 0, NULL, 0, 0)
#define LIR_SUBJ_LIST()       LIR_create_subject(LIR_ARGLIST, -1, -1, 0, NULL, 0, 0)

#define LIR_BLOCK0(ctx, op) LIR_append_block(LIR_create_block((op), NULL, NULL, NULL), (ctx))
#define LIR_BLOCK1(ctx, op, fa) LIR_append_block(LIR_create_block((op), (fa), NULL, NULL), (ctx))
#define LIR_BLOCK2(ctx, op, fa, sa) LIR_append_block(LIR_create_block((op), (fa), (sa), NULL), (ctx))
#define LIR_BLOCK3(ctx, op, fa, sa, ta) LIR_append_block(LIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif