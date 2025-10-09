#ifndef HIR_H_
#define HIR_H_

#include <std/mm.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <std/stack.h>
#include <std/set.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/syntax.h>
#include <hir/hir_types.h>

typedef struct {
    int s_id; /* String ID from strings symtab */
} hir_string_t;

typedef struct {
    long value;
} hir_constant_t;

#define HIR_VAL_MSIZE 128
typedef struct {
     char value[HIR_VAL_MSIZE];
} hir_number_t;

typedef struct {
    long v_id; /* Variable ID from variables symtab */
} hir_variable_t;

typedef struct {
    set_t h;
} hir_set_t;

typedef struct {
    unsigned long      hash;
    long               id;
    hir_subject_type_t t;
    union {
        hir_string_t   str;
        hir_constant_t cnst;
        hir_number_t   num;
        hir_variable_t var;
        hir_set_t      set;
    } storage;
} hir_subject_t;

typedef struct hir_block {
    struct hir_block* prev;
    struct hir_block* next;
    hir_operation_t   op;
    hir_subject_t*    farg;
    hir_subject_t*    sarg;
    hir_subject_t*    targ;
    int               args;
} hir_block_t;

typedef struct {
    hir_block_t* h;
    hir_block_t* t;
} hir_ctx_t;

long HIR_hash_subject(hir_subject_t* s);
hir_ctx_t* HIR_create_ctx();
int HIR_destroy_ctx(hir_ctx_t* ctx);
hir_subject_t* HIR_create_subject(hir_subject_type_t t, int v_id, const char* strval, long intval, int s_id);
hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta);
int HIR_insert_block_before(hir_block_t* block, hir_block_t* pos);
int HIR_insert_block_after(hir_block_t* block, hir_block_t* pos);
int HIR_append_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_remove_block(hir_block_t* block);
int HIR_unload_blocks(hir_block_t* block);

#define HIR_SUBJ_CONST(val)               HIR_create_subject(HIR_CONSTVAL, 0, NULL, val, -1)
#define HIR_SUBJ_NUMBER(val)              HIR_create_subject(HIR_NUMBER, 0, val, 0, -1)
#define HIR_SUBJ_STKVAR(v_id, kind, s_id) HIR_create_subject(kind, v_id, NULL, 0, s_id)
#define HIR_SUBJ_ASTVAR(n)                HIR_SUBJ_STKVAR(n->sinfo.v_id, HIR_get_token_stktype(n->token), n->sinfo.s_id)
#define HIR_SUBJ_TMPVAR(kind, id)         HIR_create_subject(HIR_get_tmp_type(kind), id, NULL, 0, -1)
#define HIR_SUBJ_LABEL()                  HIR_create_subject(HIR_LABEL, 0, NULL, 0, -1)
#define HIR_SUBJ_RAWASM(n)                HIR_create_subject(HIR_RAWASM, n->sinfo.v_id, NULL, 0, -1)
#define HIR_SUBJ_STRING(n)                HIR_create_subject(HIR_STRING, n->sinfo.v_id, NULL, 0, -1)
#define HIR_SUBJ_STRTB(id)                HIR_create_subject(HIR_STRING, id, NULL, 0, -1)
#define HIR_SUBJ_FUNCNAME(n)              HIR_create_subject(HIR_FNAME, n->sinfo.v_id, NULL, 0, -1)
#define HIR_SUBJ_FNAMETB(id)              HIR_create_subject(HIR_FNAME, id, NULL, 0, -1)
#define HIR_SUBJ_SET()                    HIR_create_subject(HIR_SET, 0, NULL, 0, -1)

/* ctx, op */
#define HIR_BLOCK0(ctx, op) HIR_append_block(HIR_create_block((op), NULL, NULL, NULL), (ctx))

/* ctx, op, x */
#define HIR_BLOCK1(ctx, op, fa) HIR_append_block(HIR_create_block((op), (fa), NULL, NULL), (ctx))

/* ctx, op, x, y */
#define HIR_BLOCK2(ctx, op, fa, sa) HIR_append_block(HIR_create_block((op), (fa), (sa), NULL), (ctx))

/* ctx, op, x, y, z */
#define HIR_BLOCK3(ctx, op, fa, sa, ta) HIR_append_block(HIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif