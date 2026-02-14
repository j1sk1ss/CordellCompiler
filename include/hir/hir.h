#ifndef HIR_H_
#define HIR_H_

#include <std/mm.h>
#include <std/mem.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/stack.h>
#include <prep/token_types.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <hir/hir_types.h>

typedef struct {
    int s_id; /* String ID from strings symtab */
} hir_string_t;

typedef struct {
    long value;
} hir_constant_t;

typedef struct {
    string_t* value;
} hir_number_t;

typedef struct {
    long v_id; /* Variable ID from variables symtab */
} hir_variable_t;

typedef struct {
    set_t h;
} hir_set_t;

typedef struct {
    list_t h;
} hir_list_t;

typedef struct {
    int                users; /* Users count             */
    struct hir_block*  home;  /* Home HIR block          */
    unsigned long      hash;  /* Subject's hash          */
    long               id;    /* Subject's ID            */
    hir_subject_type_t t;     /* Subject's type          */
    int                ptr;   /* Subject reference level */
    union {
        hir_string_t   str;
        hir_constant_t cnst;
        hir_number_t   num;
        hir_variable_t var;
        hir_set_t      set;
        hir_list_t     list;
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
    char              unused;
} hir_block_t;

typedef struct {
    hir_block_t* h;     /* Current HIR head                        */
    hir_block_t* t;     /* Current HIR tail                        */
    struct {
        void*    ptr;   /* pointer to a break target               */
        long     val1;  /* function's argument number              */
        long     val2;  /* free                                    */
    } carry;            /* Additional carry for any specific data  */
} hir_ctx_t;

/*
Get hash (i64) from the provided subject.
Params:
    - `s` - Target hir subject.

Return `i64`:hash.
*/
long HIR_hash_subject(hir_subject_t* s);

hir_subject_t* HIR_create_subject(hir_subject_type_t t, int v_id, string_t* strval, long intval);
hir_subject_t* HIR_copy_subject(hir_subject_t* s);
hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta);
hir_block_t* HIR_copy_block(hir_block_t* b, int copy_labels);
int HIR_insert_block_before(hir_block_t* block, hir_block_t* pos);
int HIR_insert_block_after(hir_block_t* block, hir_block_t* pos);
int HIR_compute_homes(hir_ctx_t* ctx);
int HIR_append_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_unlink_block(hir_block_t* block);
int HIR_unload_subject(hir_subject_t* s);
int HIR_unload_blocks(hir_block_t* block);

#define HIR_SUBJ_CONST(val)              HIR_create_subject(HIR_CONSTVAL, 0, NULL, val)
#define HIR_SUBJ_FNUMBER(val)            HIR_create_subject(HIR_F64NUMBER, 0, val, 0)
#define HIR_SUBJ_NUMBER(val)             HIR_create_subject(HIR_NUMBER, 0, val, 0)
#define HIR_SUBJ_STKVAR(v_id, kind, ptr) HIR_create_subject(kind, v_id, NULL, ptr)
#define HIR_SUBJ_ASTVAR(n)               HIR_SUBJ_STKVAR(n->sinfo.v_id, HIR_get_token_stktype(n->t, 0), n->t->flags.ptr)
#define HIR_SUBJ_TMPVAR(kind, id)        HIR_create_subject(HIR_get_tmp_type(kind), id, NULL, 0)
#define HIR_SUBJ_LABEL()                 HIR_create_subject(HIR_LABEL, 0, NULL, 0)
#define HIR_SUBJ_RAWASM(n)               HIR_create_subject(HIR_RAWASM, n->sinfo.v_id, NULL, 0)
#define HIR_SUBJ_STRING(n)               HIR_create_subject(HIR_STRING, n->sinfo.v_id, NULL, 0)
#define HIR_SUBJ_STRTB(id)               HIR_create_subject(HIR_STRING, id, NULL, 0)
#define HIR_SUBJ_FUNCNAME(n)             HIR_create_subject(HIR_FNAME, n->sinfo.v_id, NULL, 0)
#define HIR_SUBJ_FNAMETB(id)             HIR_create_subject(HIR_FNAME, id, NULL, 0)
#define HIR_SUBJ_SET()                   HIR_create_subject(HIR_PHISET, 0, NULL, 0)
#define HIR_SUBJ_LIST()                  HIR_create_subject(HIR_ARGLIST, 0, NULL, 0)

/* ctx, op */
#define HIR_BLOCK0(ctx, op) HIR_append_block(HIR_create_block((op), NULL, NULL, NULL), (ctx))

/* ctx, op, x */
#define HIR_BLOCK1(ctx, op, fa) HIR_append_block(HIR_create_block((op), (fa), NULL, NULL), (ctx))

/* ctx, op, x, y */
#define HIR_BLOCK2(ctx, op, fa, sa) HIR_append_block(HIR_create_block((op), (fa), (sa), NULL), (ctx))

/* ctx, op, x, y, z */
#define HIR_BLOCK3(ctx, op, fa, sa, ta) HIR_append_block(HIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif