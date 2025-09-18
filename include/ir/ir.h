#ifndef IR_GEN_H_
#define IR_GEN_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/regs.h>
#include <std/vars.h>
#include <std/stack.h>
#include <ast/ast.h>
#include <ast/syntax.h>

#define IR_VAL_MSIZE 128
typedef struct {
    char glob;
    long obj_id;

    char instack;
    union pos {
        int  offset;
        char value[IR_VAL_MSIZE];
    } pos;

    int cnstvl;
} ir_vinfo_t;

typedef struct {
    int reg_id;
    char dref;
} ir_rinfo_t;

typedef struct {
    char isreg;
    char size;
    union storage {
        ir_vinfo_t vinfo;
        ir_rinfo_t rinfo;
    } storage;
} ir_subject_t;

typedef enum {
    /* Operations */
        /* Commands */
        FCLL, // function call
        ECLL, // extern function call
        STRT, // start macro
        SYSC, // syscall
        FRET, // function ret
        TDBL, // convert to double
        TST,  // test
        XCHG, // xchg
        CDQ,  // cdq
        MKLB, // mk label
        FDCL, // declare function
        OEXT, // extern object

        SETL,
        SETG,
        STLE,
        STGE,
        SETE,
        STNE,
        SETB,
        SETA,
        STBE,
        STAE,

        /* Data commands */
        RESV,
        VDCL,

        /* Jump instructions */
        JMP,  // jmp
        JE,
        JNE,
        JL,
        JG,

    /* Register */
        /* Operations */
        iMOV, // integer move
        iMOVb, // mov byte
        iMOVw, // mov word
        iMOVd, // mov dword
        iMOVq, // mov qword
        iMVZX,

        fMOV, // float move
        fMVf, // float to float move
        LEA,  // lea move
        PUSH, // push
        POP,  // pop

    /* Integer */
        /* Binary operations */
        iADD, // addition
        iSUB, // substraction
        iMUL, // multiplication
        DIV,
        iDIV, // division
        iMOD, // module
        iLRG, // larger
        iLGE, // larger or equals
        iLWR, // lower
        iLRE, // lower or equals
        iCMP, // comprarision

        /* Logic */
        iAND, // and
        iOR,  // or

    /* Float */
        /* Binary operations */
        fADD, // addition
        fSUB, // substruction
        fMUL, // multiplication
        fDIV, // division
        fCMP, // cmp for double

    /* Bits */
        /* Binary operations */
        bAND, // bit and
        bOR,  // bit or
        bXOR, // bit xor
        bSHL, // bitleft
        bSHR, // bitright
        bSAR, // bitleft unsgn

    /* Other */
    RAW,
} ir_operation_t;

typedef struct ir_block {
    struct ir_block* next;
    ir_operation_t   op;
    ir_subject_t*    farg;
    ir_subject_t*    sarg;
    ir_subject_t*    targ;
    int              args;
} ir_block_t;

typedef struct {
    long          cid;
    int           lid;
    ir_block_t*   h;
    ir_block_t*   t;
    syntax_ctx_t* synt;
    scope_stack_t heap;
} ir_ctx_t;

ir_ctx_t* IR_create_ctx();
ir_subject_t* IR_create_subject(int reg, int dref, int offset, const char* val1, int val2, int size);
ir_block_t* IR_create_block(ir_operation_t op, ir_subject_t* fa, ir_subject_t* sa, ir_subject_t* ta);
int IR_insert_block(ir_block_t* block, ir_ctx_t* ctx);
int IR_unload_blocks(ir_block_t* block);
int IR_destroy_ctx(ir_ctx_t* ctx);

static inline const char* _tmp_str(const char* fmt, ...) {
    static char buf[256] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
}

#define IR_SUBJ_DRFSTR(s, ...) \
    IR_create_subject(-1, 1, -1, _tmp_str(__VA_ARGS__), -1, s)
#define IR_SUBJ_STR(s, ...) \
    IR_create_subject(-1, 0, -1, _tmp_str(__VA_ARGS__), -1, s)

#define IR_SUBJ_REG(r, s)    IR_create_subject((r), 0, 0, NULL, -1, s)
#define IR_SUBJ_DRFREG(r, s) IR_create_subject((r), 1, 0, NULL, -1, s)
#define IR_SUBJ_CNST(v)      IR_create_subject(-1, 0, 0, NULL, v, 8)
#define IR_SUBJ_NONE()       IR_create_subject(-1, 0, 0, NULL, -1, -1)
#define IR_SUBJ_OFF(o, s)    IR_create_subject(-1, 0, o, NULL, -1, s)
#define IR_SUBJ_NOFF(o, s)   IR_create_subject(-1, 0, -o, NULL, -1, s)
#define IR_SUBJ_VAR(var)     VRS_instack((var)->token) ? \
        /* Local variable */    IR_create_subject(-1, 0, (var)->sinfo.offset, NULL, -1, (var)->sinfo.size) : \
        /* Global variable */   IR_create_subject(-1, !VRS_isnumeric((var)->token), 0, (var)->token->value, -1, (var)->sinfo.size)

#define IR_BLOCK0(ctx, op) \
    IR_insert_block(IR_create_block((op), NULL, NULL, NULL), (ctx))

#define IR_BLOCK1(ctx, op, fa) \
    IR_insert_block(IR_create_block((op), (fa), NULL, NULL), (ctx))

#define IR_BLOCK2(ctx, op, fa, sa) \
    IR_insert_block(IR_create_block((op), (fa), (sa), NULL), (ctx))

#define IR_BLOCK3(ctx, op, fa, sa, ta) \
    IR_insert_block(IR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif